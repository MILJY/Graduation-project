#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>
#include "protocol.h"
namespace Ui {
class ShareFile;
}

class ShareFile : public QWidget
{
    Q_OBJECT

public:
    explicit ShareFile(QWidget *parent = nullptr);
    ~ShareFile();
    static ShareFile &ins();
    void FlushOnlineFriend(PDU *pdu);
    void SetShareFilePath(QString path);

private slots:
    void on_confirm_clicked();

    void on_cancel_clicked();

private:
    Ui::ShareFile *ui;
    QString share_file_path;
};

#endif // SHAREFILE_H
