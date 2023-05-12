#include "sharefile.h"
#include "ui_sharefile.h"
#include "tcpclient.h"
ShareFile::ShareFile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShareFile)
{
    ui->setupUi(this);
    QLabel *background = new QLabel(this);
    // 加载图片并设置背景
    background->setPixmap(QPixmap(":/config/icon/222.jpg"));
    // 设置背景图片的拉伸方式
    background->setScaledContents(true);
    // 将背景设置为窗口底层
    background->lower();
    setWindowTitle(TcpClient::ins().GetLoginName());
}

ShareFile::~ShareFile()
{
    delete ui;
}

ShareFile &ShareFile::ins()
{
    static ShareFile instance;
    return instance;
}

void ShareFile::FlushOnlineFriend(PDU *pdu)
{
    if(pdu == NULL)
        return;
    uint user_cnt = pdu->msg_len / 32;
    char friend_name[32];
    ui->online_frient_list->clear();
    for(uint i = 0; i < user_cnt; i++)
    {
        memcpy(friend_name, (char *)(pdu->ca_msg) + i * 32, 32);

        ui->online_frient_list->addItem(friend_name);
    }
}

void ShareFile::SetShareFilePath(QString path)
{
    share_file_path = path;
}

void ShareFile::on_confirm_clicked()
{
    QListWidgetItem *item = ui->online_frient_list->currentItem();
    if(item == NULL)
    {
        QMessageBox::warning(this, "Error", "Please Select A Friend To Share!");
    }
    else
    {
        QString friend_name = item->text();
        PDU *pdu = mkPDU(share_file_path.size() + 1);
        pdu->msg_type = MSG_TYPE_SHARE_FILE_REQUEST;
        memcpy((char *)(pdu->ca_msg), share_file_path.toStdString().c_str(), share_file_path.size());
        memcpy(pdu->ca_data, TcpClient::ins().GetLoginName().toStdString().c_str(), TcpClient::ins().GetLoginName().size());
        memcpy((pdu->ca_data) + 32, friend_name.toStdString().c_str(), friend_name.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}

void ShareFile::on_cancel_clicked()
{
    hide();
}
