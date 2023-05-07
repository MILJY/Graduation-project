#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QWidget>
#include <QListWidget>
#include <QInputDialog>
#include <QFileDialog>
#include <QTimer>
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
    void EntryFile();
    void ReturnPath();
    void StartTimer();
    void SetDownload(bool status);
    bool GetDownload();
    void HandleDownloadFileRespond(PDU *pdu);
    void DownloadFileData(QByteArray buff);
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
    void UploadFileData();
    void Entry_File(const QModelIndex &index);

private:
    Ui::FileOperation *ui;
    QString m_upload_file_path;
    QTimer *m_time;
    QString m_download_file_path;

    QFile m_download_file;
    bool is_download;
    qint64 m_file_size;
    qint64 m_receive_size;
};

#endif // FILEOPERATION_H
