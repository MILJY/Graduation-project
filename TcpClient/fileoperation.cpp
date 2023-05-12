#include "fileoperation.h"
#include "ui_fileoperation.h"
#include "protocol.h"
#include "tcpclient.h"
#include "sharefile.h"
FileOperation::FileOperation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileOperation)
{
    ui->setupUi(this);
    QLabel *background = new QLabel(this);
        // 加载图片并设置背景
    background->setPixmap(QPixmap(":/config/icon/222.jpg"));
        // 设置背景图片的拉伸方式
    background->setScaledContents(true);
        // 将背景设置为窗口底层
    background->lower();
    m_time = new QTimer;
    is_download = false;
    m_file_size = 0;
    m_receive_size = 0;
    connect(ui->filename_list, SIGNAL(doubleClicked(QModelIndex)),this, SLOT(Entry_File(QModelIndex)));
    connect(m_time, SIGNAL(timeout()), this, SLOT(UploadFileData()));
    setWindowTitle(TcpClient::ins().GetLoginName());
}

FileOperation::~FileOperation()
{
    delete m_time;
    delete ui;
}

FileOperation &FileOperation::ins()
{
    static FileOperation instance;
    return instance;
}

void FileOperation::FlushFolder(PDU *pdu)
{
    if(pdu == NULL)
        return;
    ClearFileNameList();
    int file_cnt = pdu->msg_len / sizeof(FileInfo);
    for(int i = 0; i < file_cnt; i++)
    {
        FileInfo *fileinfo = (FileInfo *)(pdu->ca_msg) + i;
        QListWidgetItem *item = new QListWidgetItem;
        if(strcmp(fileinfo->file_name, ".") == 0 || strcmp(fileinfo->file_name, "..") == 0)
            continue;
        if(fileinfo->file_type == 0)
            item->setIcon(QIcon(QPixmap(":/config/icon/folder.png")));
        else
            item->setIcon(QIcon(QPixmap(":/config/icon/file.jpeg")));
        item->setText(fileinfo->file_name);
        ui->filename_list->addItem(item);
    }
}

void FileOperation::ClearFileNameList()
{
    int cnt = ui->filename_list->count();
    for(int i = cnt - 1; i >= 0; i--)
    {
        QListWidgetItem *tmp_item = ui->filename_list->item(i);
        ui->filename_list->removeItemWidget(tmp_item);
        delete(tmp_item);
    }
}

void FileOperation::closeEvent(QCloseEvent *event)
{
    ClearFileNameList();
}

void FileOperation::EntryFile()
{
    on_flush_floder_clicked();
}

void FileOperation::ReturnPath()
{
    on_flush_floder_clicked();
}

void FileOperation::StartTimer()
{
    m_time->start(1000);
}

void FileOperation::SetDownload(bool status)
{
    is_download = status;
}

bool FileOperation::GetDownload()
{
    return is_download;
}

void FileOperation::HandleDownloadFileRespond(PDU *pdu)
{
    char file_name[50] = {'\0'};
    sscanf(pdu->ca_data, "%s %lld", file_name, &m_file_size);
    qDebug() << file_name << m_file_size;
    if(strlen(file_name) > 0 && m_file_size > 0)
    {

        m_download_file.setFileName(m_download_file_path);
        if(m_download_file.open(QIODevice::WriteOnly))
        {
            is_download = true;
            m_receive_size = 0;
        }
        else
        {
            m_file_size = 0;
            QMessageBox::warning(this, "Download File", "Download File Path Fail");
        }
    }
}

void FileOperation::DownloadFileData(QByteArray buff)
{
    qDebug() << buff.size();
    m_download_file.write(buff);
    m_receive_size += buff.size();
    qDebug() << m_receive_size << m_file_size;
    if(m_receive_size == m_file_size)
    {
        QMessageBox::information(this, "Download File", "Download File Success");
        m_download_file.close();
        is_download = false;
        m_receive_size = 0;
        m_file_size = 0;
    }
    else
    {
        if(m_file_size < m_receive_size)
        {
            m_download_file.close();
            is_download = false;
            m_receive_size = 0;
            m_file_size = 0;
            QMessageBox::warning(this, "Error", "Download File Fail");
          }
     }


}



void FileOperation::on_return_button_clicked()
{
    QString current_path = TcpClient::ins().GetCurrentPath();
    QString root_path = QString("./File/%1").arg(TcpClient::ins().GetLoginName());
    if(current_path == root_path)
    {
        QMessageBox::warning(this, "Error", "Already In The Root Directory, Unable To Return To The Previous Level");
    }
    else
    {
        int index = current_path.lastIndexOf("/");
        current_path = current_path.remove(index, current_path.size() - index);
        PDU *pdu = mkPDU(current_path.size() + 1);
        pdu->msg_type = MSG_TYPE_RETURN_PATH_REQUEST;
        memcpy((char *)(pdu->ca_msg), current_path.toStdString().c_str(), current_path.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }

}

void FileOperation::on_create_folder_clicked()
{
    QString folder_name = QInputDialog::getText(this, "Create Folder", "Folder Name");
    if(folder_name.isEmpty())
    {
        QMessageBox::critical(this, "Error", "Folder Name Cannot Be Empty!");
    }
    else
    {
        QString folder_path = TcpClient::ins().GetCurrentPath();
        PDU *pdu = mkPDU(folder_path.size() + 1);
        pdu->msg_type = MSG_TYPE_CREATE_FOLDER_REQUEST;
        memcpy(pdu->ca_data, folder_name.toStdString().c_str(), folder_name.size());
        memcpy((char *)(pdu->ca_msg), folder_path.toStdString().c_str(), folder_path.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}

void FileOperation::on_delete_floder_clicked()
{
    QListWidgetItem *item = ui->filename_list->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "Error", "Please Select A File To Delete!");
    }
    else
    {
        QString file_name = item->text();
        QString folder_path = TcpClient::ins().GetCurrentPath();
        PDU *pdu = mkPDU(folder_path.size() + 1);
        pdu->msg_type = MSG_TYPE_DELETE_FLODER_RESPOND;
        memcpy(pdu->ca_data, file_name.toStdString().c_str(), file_name.size());
        memcpy((char *)(pdu->ca_msg), folder_path.toStdString().c_str(), folder_path.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}

void FileOperation::on_rename_folder_clicked()
{
    QListWidgetItem *item = ui->filename_list->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "Error", "Please Select A File To Rename!");
    }
    else
    {
        QString new_file_name = QInputDialog::getText(this, "Rename", "New File Name");
        if(new_file_name.isEmpty())
        {
            QMessageBox::critical(this, "Error", "File Name Cannot Be Empty!");
        }
        else
        {
            QString file_name = item->text();
            QString folder_path = TcpClient::ins().GetCurrentPath();
            PDU *pdu = mkPDU(folder_path.size() + 1);
            pdu->msg_type = MSG_TYPE_RENAME_FILE_REQUEST;
            memcpy(pdu->ca_data, file_name.toStdString().c_str(), file_name.size());
            memcpy((pdu ->ca_data) + 32, new_file_name.toStdString().c_str(), new_file_name.size());
            memcpy((char *)(pdu->ca_msg), folder_path.toStdString().c_str(), folder_path.size());
            TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
            free(pdu);
            pdu = NULL;
        }

    }
}

void FileOperation::on_flush_floder_clicked()
{
    QString current_path = TcpClient::ins().GetCurrentPath();
    PDU *pdu = mkPDU(current_path.size() + 1);
    pdu->msg_type = MSG_TYPE_FLUSH_FOLDER_REQUEST;
    memcpy((char *)(pdu->ca_msg), current_path.toStdString().c_str(), current_path.size());
    TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
    free(pdu);
    pdu = NULL;
}

void FileOperation::on_upload_file_clicked()
{
    QString current_path = TcpClient::ins().GetCurrentPath();
    m_upload_file_path = QFileDialog::getOpenFileName();
    if(m_upload_file_path.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please Select The File To Upload");

    }
    else
    {
        int idx = m_upload_file_path.lastIndexOf('/');
        QString upload_file_name = m_upload_file_path.right(m_upload_file_path.size() - idx - 1);
        QFile m_file(m_upload_file_path);
        qint64 file_size = m_file.size();
        qDebug() << file_size;
        PDU *pdu = mkPDU(current_path.size() + 1);
        pdu->msg_type = MSG_TYPE_UPLOAD_FILE_REQUEST;
        memcpy((char *)(pdu->ca_msg), current_path.toStdString().c_str(), current_path.size());
        sprintf(pdu->ca_data, "%s %lld", upload_file_name.toStdString().c_str(), file_size);
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;

    }
}

void FileOperation::on_delete_file_clicked()
{
    on_delete_floder_clicked();
}

void FileOperation::on_download_file_clicked()
{
    QListWidgetItem *item = ui->filename_list->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "Error", "Please Select A File To Download!");
    }
    else
    {
        QString file_name = item->text();
        QString current_path = TcpClient::ins().GetCurrentPath();
        QString save_path = QFileDialog::getSaveFileName();
        if(save_path.isEmpty())
        {
            QMessageBox::warning(this, "Error", "Please Select A Path To Download");
        }
        else
        {
            m_download_file_path = save_path;
            PDU *pdu = mkPDU(current_path.size() + 1);
            pdu->msg_type = MSG_TYPE_DOWNLOAD_FILE_REQUEST;
            memcpy((char *)(pdu->ca_msg), current_path.toStdString().c_str(), current_path.size());
            memcpy(pdu->ca_data, file_name.toStdString().c_str(), file_name.size());
            TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
            free(pdu);
            pdu = NULL;
        }
    }
}

void FileOperation::on_share_file_clicked()
{
    QListWidgetItem *item = ui->filename_list->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "Error", "Please Select A File To Share!");
    }
    else
    {
        PDU *pdu = mkPDU(0);
        pdu->msg_type =  MSG_TYPE_ONLINE_FRIEND_REQUEST;
        memcpy(pdu->ca_data, TcpClient::ins().GetLoginName().toStdString().c_str(), TcpClient::ins().GetLoginName().size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
        if(ShareFile::ins().isMinimized())
            ShareFile::ins().showNormal();
        else
            ShareFile::ins().show();
        QString path = TcpClient::ins().GetCurrentPath();
        path = path + "/" + (item->text());
        ShareFile::ins().SetShareFilePath(path);
    }

}

void FileOperation::UploadFileData()
{
    m_time->stop();
    QFile file(m_upload_file_path);
    if(file.open(QIODevice::ReadOnly))
    {
        char *m_buffer = new char[4096]; // 4096个字节读写效率更高
        qint64 actual_size = 0;
        while(true)
        {
            actual_size = file.read(m_buffer, 4096);
            if(actual_size > 0 && actual_size <= 4096)
            {
                TcpClient::ins().GetTcpSocket().write(m_buffer, actual_size);
            }
            else
            {
                if(actual_size == 0)
                    break;
                else
                {
                    QMessageBox::warning(this, "Upload File", "Upload File Fail");
                    break;
                }
            }
        }
        file.close();
        delete []m_buffer;
        m_buffer = NULL;

    }
    else
    {
        QMessageBox::warning(this, "Error", "Open File Fail");
    }
    m_upload_file_path.clear();
}

void FileOperation::Entry_File(const QModelIndex &index)
{
    QString file_name = index.data().toString();
    QString entry_file_path = QString("%1/%2").arg(TcpClient::ins().GetCurrentPath()).arg(file_name);
    PDU *pdu = mkPDU(entry_file_path.size() + 1);
    pdu->msg_type = MSG_TYPE_ENTRY_FILE_REQUEST;
    memcpy((char *)(pdu->ca_msg), entry_file_path.toStdString().c_str(), entry_file_path.size());
    TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
    free(pdu);
    pdu = NULL;
}
