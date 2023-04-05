#include "onlineuser.h"
#include "ui_onlineuser.h"

OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

OnlineUser &OnlineUser::ins()
{
    static OnlineUser instance;
    return instance;
}

void OnlineUser::SHowOnlineUser(PDU *pdu)
{
    if(pdu == NULL)
        return;
    uint user_cnt = pdu->msg_len / 32;
    char user_name[32];
    ui->online_user->clear();
    for(uint i = 0; i < user_cnt; i++)
    {
        memcpy(user_name, (char *)(pdu->ca_msg) + i * 32, 32);

        ui->online_user->addItem(user_name);
    }
}
