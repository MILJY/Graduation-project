#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDebug>
namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow& ins();

private slots:
    void on_chat_op_clicked();

    void on_file_op_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
