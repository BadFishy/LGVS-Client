#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include "QDebug"
#include "QString"
#include "QByteArray"
#include "QTcpSocket"
#include "QHostInfo"
#include <time.h>
#include <QCryptographicHash>
#include "jiami.h"
#include <QMessageBox>
#include <QProcess>


using namespace std;

MainWindow::MainWindow(QWidget *parent, QString usr, QString pas) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    this->setFixedSize( this->width (),this->height ());

    this->setWindowTitle("凉糕对战平台 - LGVS");

    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit );
    ui->lineEdit->setValidator(validator);  //使用户名只能为英文数字
    ui->lineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);   //屏蔽中文输入法
    ui->lineEdit->setText(usr);
    ui->lineEdit_2->setText(pas);

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    this->setFixedSize( this->width (),this->height ());

    this->setWindowTitle("凉糕对战平台 - LGVS");

    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit );
    ui->lineEdit->setValidator(validator);  //使用户名只能为英文数字
    ui->lineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);   //屏蔽中文输入法

}

MainWindow::~MainWindow()
{
    delete ui;
}



int MainWindow::on_pushButton_clicked()
{
    char* usename;
    QString usename_qs = ui->lineEdit->text();
    QByteArray usename_qb =  usename_qs.toLatin1();
    usename = usename_qb.data();
    char* password;
    QString password_qs = ui->lineEdit_2->text();
    QString md5;
    {//MD5加密密码
        QByteArray ba,bb;
        QCryptographicHash md(QCryptographicHash::Md5);
        ba.append(password_qs);
        md.addData(ba);
        bb = md.result();
        md5.append(bb.toHex());
    }

    QByteArray password_qb =  md5.toLatin1();
    password = password_qb.data();



    char sendBuf[64];
    time_t timer = time( 0 );
    int mm = (int)timer / 10000;
    //jiami(password, mm);
    sprintf(sendBuf, "login,%s,%s", usename, password);

    //char jie[64];
    //jiemi(password, mm);
    //sprintf(jie, "%s,%s,%d", usename, password, mm);
    //qDebug() << password;


    char* ip;
    if(test == 0){
        QHostInfo info = QHostInfo::fromName("play.niconiconi.cc");
        QString host = info.addresses().first().toString();
        QByteArray ba = host.toLatin1(); // must
        ip = ba.data();
    }
    else ip = "127.0.0.1";



    //1.启动SOCKET库，版本为2.0
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 0);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        //cout << "Socket2.0初始化失败，Exit!";
        return 0;
    }
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
    {
        WSACleanup();
        return 0;
    }


    //2.创建socket，
    SOCKET sock;                   //socket
    sock = socket(
        AF_INET,           //internetwork: UDP, TCP, etc
        SOCK_STREAM,        //TCP
        0                  //protocol
    );

    if (sock == INVALID_SOCKET) {
        //cout << "Socket 创建失败，Exit!";
        return 0;
    }

    //3.设置发往的地址
    SOCKADDR_IN addrto;            //发往的地址
    memset(&addrto, 0, sizeof(addrto));
    addrto.sin_family = AF_INET;

    // https://jingyan.baidu.com/article/1709ad8041dca64634c4f006.html
    //去掉SDL检查。设置项目属性，将c/c++->SDL检查设置为“否”即可跳过检查，使用旧的函数可编译成功。
    addrto.sin_addr.S_un.S_addr = inet_addr(ip); //旧函数，不设置sdl检查会报错

    //端口号必须和服务器绑定的端口号一致
    addrto.sin_port = htons(10099);
    //申请链接服务器
    ::connect(sock, (SOCKADDR*)&addrto, sizeof(SOCKADDR));

    //while (true) {
        char recvBuf[100];
        recv(sock, recvBuf, 100, 0);
        //qDebug() << recvBuf;
        QString hand = QString(QLatin1String(recvBuf));
        if(hand != "LGVS_Login"){
            qDebug() << "服务器连接错误！";
            QMessageBox::critical(NULL, "错误", "服务器连接错误！");
            return 0;
        }

        qDebug() << "已连接上服务器，请求登陆中...";
        send(sock, sendBuf, 100, 0);    //发送登录信息


        char recvBuf2[100];
        recv(sock, recvBuf2, 100, 0);
        //qDebug() << recvBuf2;
        QString mi = QString(QLatin1String(recvBuf2));
        QStringList messages = mi.split(",");
        closesocket(sock);

        WSACleanup();

        if(messages[0] == "1"){
                qDebug() << "登陆成功！";
                QProcess process(this);
                process.startDetached(".\\bin\\lobby.exe " + messages[1]);
                qApp->exit(0);
            }
        else if(mi == "2"){
            QMessageBox::critical(NULL, "错误", "用户名不存在！");
        }
        else if(mi == "3"){
            QMessageBox::critical(NULL, "错误", "密码错误！");
        }
        else if(mi == "7"){
            QMessageBox::critical(NULL, "错误", "用户已在线！如果这不是你自己请联系管理员");
        }
        else{
            QMessageBox::critical(NULL, "错误", "登陆失败！");
        }

        //break;
    //}
        return 0;

}



void MainWindow::on_label_4_clicked()
{
    this->close();
    Regui *regg=new Regui();
    regg->show();

}
