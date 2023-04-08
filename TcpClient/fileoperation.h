#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QWidget>
#include <QListWidget>
#include <QInputDialog>
#include "protocol.h"
namespace Ui {
class FileOperation;
}

class FileOperation : public QWidget
{
    Q_OBJECT

public:
    explicit FileOperation(QWidget *parent = nullptr);
    ~FileOperation();
    static FileOperation &ins();
    void FlushFolder(PDU *pdu);
    void ClearFileNameList();
    void closeEvent(QCloseEvent *event);
private slots:
    void on_return_button_clicked();

    void on_create_folder_clicked();

    void on_delete_floder_clicked();

    void on_rename_folder_clicked();

    void on_flush_floder_clicked();

    void on_upload_file_clicked();

    void on_delete_file_clicked();

    void on_download_file_clicked();

    void on_share_file_clicked();

private:
    Ui::FileOperation *ui;
};

#endif // FILEOPERATION_H
