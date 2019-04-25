#include "regui.h"
#include "ui_regui.h"
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
#include "mainwindow.h"

int zhuangtai = 0;
QString usr;
QString pas;

void Regui::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if(zhuangtai==4){
        MainWindow *w=new MainWindow(0,usr,pas);
        w->show();
    }
    else{
        MainWindow *w=new MainWindow();
        w->show();
    }

}


Regui::Regui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Regui)
{
    ui->setupUi(this);
    this->setWindowTitle("注册——凉糕对战平台 - LGVS");

    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit );
    ui->lineEdit->setValidator(validator);  //使用户名只能为英文数字
    ui->lineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);   //屏蔽中文输入法
    ui->lineEdit_4->setValidator(validator);  //使用户名只能为英文数字
    ui->lineEdit_4->setAttribute(Qt::WA_InputMethodEnabled, false);   //屏蔽中文输入法

}

Regui::~Regui()
{
    delete ui;

}

int Regui::on_pushButton_clicked()
{
    char* username = ui->lineEdit->text().toLatin1().data();
    if(strlen(username) < 4){
        QMessageBox::critical(NULL, "错误", "用户名太短，应该为4~16个字符！");
        return -1;
    }

    QString password_qs = ui->lineEdit_2->text();
    QString password_queren = ui->lineEdit_3->text();

    if(strlen(password_qs.toLatin1().data()) < 6){
        QMessageBox::critical(NULL, "错误", "密码太短，应该为6~32个字符！");
        return -1;
    }
    if(password_qs != password_queren){
        QMessageBox::critical(NULL, "错误", "两次输入的密码不相同，请检查后重新输入！");
        return -1;
    }
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
    char* password = password_qb.data();
//暂时不支持Email和昵称
    //QString email_qs = ui->lineEdit_4->text();
    char* email= "";


    //QString nickname_qs = ui->lineEdit_5->text();
    char* nickname= "";

    char sendBuf[256];
    sprintf(sendBuf, "reg,%s,%s,%s,%s", username, password,email,nickname);


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

        qDebug() << "已连接上服务器，请求注册中...";
        send(sock, sendBuf, 100, 0);    //发送注册信息


        char recvBuf2[100];
        recv(sock, recvBuf2, 100, 0);
        //qDebug() << recvBuf2;
        QString mi = QString(QLatin1String(recvBuf2));
        closesocket(sock);

        WSACleanup();

        zhuangtai = mi.toInt();

        /**
                     *	服务端返回的值
                     *	1. 登陆成功
                     *	2. 登陆失败用户名不存在
                     *	3. 登陆失败密码错误
                     *	4. 注册成功
                     *	5. 注册失败用户名已存在
                     *	6. 注册失败数据库插入失败
                     */

        if(zhuangtai == 4){
                qDebug() << "注册成功！";
                QMessageBox::information(NULL, "恭喜", "注册成功！");
                usr = ui->lineEdit->text();
                pas = ui->lineEdit_2->text();
                this->close();//关闭窗口
            }
        else if(zhuangtai == 5){
            QMessageBox::critical(NULL, "错误", "用户名已存在，请重新输入其他用户名！");
        }
        else{
            QMessageBox::critical(NULL, "错误", "注册失败！");
        }




    return 0;
}
