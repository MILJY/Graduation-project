#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "friendchat.h"
#include "tcpclient.h"
#include "fileoperation.h"
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
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
    FileOperation::ins().show();
    this->hide();
}
