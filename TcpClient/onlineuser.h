#ifndef ONLINEUSER_H
#define ONLINEUSER_H
#include <QWidget>
#include "protocol.h"
namespace Ui {
class OnlineUser;
}

class OnlineUser : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineUser(QWidget *parent = nullptr);
    ~OnlineUser();
    static OnlineUser &ins();
    void SHowOnlineUser(PDU *pdu);
private slots:
    void on_add_friend_clicked();

private:
    Ui::OnlineUser *ui;
};

#endif // ONLINEUSER_H
