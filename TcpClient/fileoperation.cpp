#include "fileoperation.h"
#include "ui_fileoperation.h"
#include "protocol.h"
#include "tcpclient.h"
FileOperation::FileOperation(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileOperation)
{
    ui->setupUi(this);
}

FileOperation::~FileOperation()
{
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
    int file_cnt = pdu->msg_len / sizeof(FileInfo);
    for(int i = 0; i < file_cnt; i++)
    {
        FileInfo *fileinfo = (FileInfo *)(pdu->ca_msg) + i;
        QListWidgetItem *item = new QListWidgetItem;
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


void FileOperation::on_return_button_clicked()
{

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

}

void FileOperation::on_delete_file_clicked()
{

}

void FileOperation::on_download_file_clicked()
{

}

void FileOperation::on_share_file_clicked()
{

}
