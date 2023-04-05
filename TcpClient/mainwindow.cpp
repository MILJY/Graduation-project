#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "friendchat.h"
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    delete ui;
}

MainWindow &MainWindow::ins()
{
    static MainWindow instance;
    return instance;
}

void MainWindow::on_chat_op_clicked()
{
    FriendChat::ins().show();
    this->hide();
}

void MainWindow::on_file_op_clicked()
{

}
