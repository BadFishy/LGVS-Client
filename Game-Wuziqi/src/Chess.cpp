#include "Chess.h"
#include "QHostInfo"
#include <windows.h>

Chess::Chess(QWidget *parent,QString arg[]):
    QMainWindow(parent),
    ui(new Ui::Chess)
{
    ui->setupUi(this);

    hintNum = 0;
    qDebug() << arg[0]<< arg[1];
    exit=0;
    char* ip;
    if(test == 0){
        QHostInfo info = QHostInfo::fromName("play.niconiconi.cc");
        QString host = info.addresses().first().toString();
        QByteArray ba = host.toLatin1(); // must
        ip = ba.data();
    }
    else ip = "127.0.0.1";

    if(arg[0]=="1"){//是客场

        socket = new QTcpSocket(this);
        socket->connectToHost(ip,10299);

        if(socket->waitForConnected(10000))
        {

            QString sendbuf = "wuziqi," + arg[1] +"," + arg[0];
            qDebug()<< sendbuf;
            socket->write(sendbuf.toStdString().data());//发送场次握手识别码

            ui->gameboard->playerColor = Qt::white;
            ui->gameboard->enemyColor = Qt::black;
            ui->turnLabel->setText(tr("等待对方连接"));

            connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
            connect(ui->gameboard, SIGNAL(addChess(QString)), this, SLOT(sendChessInfo(QString)));
            connect(ui->gameboard, SIGNAL(win()), this, SLOT(sendWin()));
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("连接失败！"));
        }
    }
    else if(arg[0]=="0"){//是主场
        ui->gameboard->setDisabled(true);
        socket = new QTcpSocket(this);
        socket->connectToHost(ip,10299);
        bool isconnect = false;
        if(socket->waitForConnected(10000))
        {
            QString sendbuf = "wuziqi," + arg[1] +"," + arg[0];
            qDebug()<< sendbuf;
            socket->write(sendbuf.toStdString().data());//发送场次握手识别码


            ui->gameboard->playerColor = Qt::black;
            ui->gameboard->enemyColor = Qt::white;
            ui->gameboard->inRound = true;
            ui->turnLabel->setText(tr("等待对方连接"));

            connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
            connect(ui->gameboard, SIGNAL(addChess(QString)), this, SLOT(sendChessInfo(QString)));
            connect(ui->gameboard, SIGNAL(win()), this, SLOT(sendWin()));


            isconnect = true;
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("连接失败！"));
        }
    }


    connect(this, SIGNAL(addChess(int,int)), ui->gameboard, SLOT(addEnemysChess(int,int)));

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));//心跳包
    m_pTimer->start(3000);


}

Chess::~Chess()
{
    delete ui;
}



void Chess::on_quitButton_clicked()
{

    //ui->quitButton->setEnabled(false);
    //ui->quitButton->setText(tr("等待"));
    if(exit==0){
        auto quit = QMessageBox::question(this,tr("退出"),tr("你将投降认输"),QMessageBox::Yes|QMessageBox::No);

        if(quit == QMessageBox::Yes)
        {
            //socket->write("agree quit ");
            //socket->waitForBytesWritten();
            //sendWin();
            socket->write("quit");
            qApp->quit();
        }
    }
    else{
        qApp->quit();
    }



}



void Chess::readData()
{

    char* recv = socket->readAll().data();
    qDebug()<<recv;
    const char *sep = ","; //分割接收的数据
    char *p;
    QString messages[5];
    p = strtok((char*)recv, sep);
    messages[0] = p;
    qDebug()<<p;
    bool finished = false;
    while(!finished)
    {
        if(messages[0] == "add")
        {
            for (int i = 0; i < 3; i++) {
                        messages[i] = p;
                        p = strtok(NULL, sep);
                    }
            emit addChess(messages[1].toInt(), messages[2].toInt());
            ui->turnLabel->setText(tr("您的回合，请点击棋盘下棋"));
                finished = true;
        }

        else if(messages[0] == "starthei")
        {
           ui->turnLabel->setText(tr("对方已连接！ 您的回合，请点击棋盘下棋"));
           ui->gameboard->setDisabled(false);
        }

        else if(messages[0] == "startbai")
        {
            ui->turnLabel->setText(tr("对方已连接！ 请等待他思考"));
        }


        else if(messages[0] == "win")
        {
            QMessageBox *mmm;
            mmm = new QMessageBox(QMessageBox::NoIcon,tr("失败"),tr("<strong>失败乃成功之母，很遗憾您输了！</strong>"),QMessageBox::Ok,this);
            mmm->show();
            ui->gameboard->setDisabled(true);
            ui->quitButton->setText(tr("退出游戏"));
            exit=1;
        }


        else if(messages[0] == "quit")
        {
            auto quit = QMessageBox::question(this,tr("退出"),tr("对方投降了"),QMessageBox::Yes|QMessageBox::No);

            if(quit == QMessageBox::Yes)
            {
                qApp->quit();
            }
        }
        finished = true;
    }
}

void Chess::sendChessInfo(QString chess)
{
    ui->turnLabel->setText(tr("对方正在思考，请等待"));
    socket->write((QString("add,") + chess).toStdString().c_str());//读取的时候会多一个空格..
}
QTimer *mTimer;
void Chess::sendWin()
{
//    QSound::play(":/...");
    QMessageBox *message;
    message = new QMessageBox(QMessageBox::NoIcon, tr("胜利"), tr("<strong>恭喜您，获得胜利！</strong>"),QMessageBox::Ok,this);

    message->show();
    //qDebug() << "send win";
    connect(this, SIGNAL(addChess(int,int)), ui->gameboard, SLOT(addEnemysChess(int,int)));

    ui->gameboard->setDisabled(true);
    ui->quitButton->setText(tr("退出游戏"));
    exit=1;

    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(writewin()));
    mTimer->start(1000);

}


void Chess::handleTimeout()
{
    qDebug() << "send heart";
    socket->write("heart");
}

void Chess::writewin()
{
    qDebug() << "send win";
    socket->write("win");
    if(mTimer->isActive()){
            mTimer->stop();
    }

}









