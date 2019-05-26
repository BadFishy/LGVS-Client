#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox.h"
#include "QtWidgets"
#include "time.h"
#include "QDateTime"
#include "QHostInfo"
#include <windows.h>

MainWindow::MainWindow(QWidget *parent, char *arg[]) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);



    //this->setStyleSheet("background-color: rgb(255,255,255);");
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);//设置背景为白色
    //********************************************************************************

    ui->games->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置表格QTableWidget不可编辑
    //使表格QTableWidget选中选择整行
    ui->games->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->games->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->games->horizontalHeader()->setFocusPolicy(Qt::NoFocus); //设置表头不可选
    ui->games->setFocusPolicy(Qt::NoFocus);//无虚线
    //ui->games->horizontalHeader()->setStretchLastSection(true);//最后一栏自适应长度
    ui->games->setShowGrid(false); //设置不显示格子线
    ui->games->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->games->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    //ui->games->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    ui->games->horizontalHeader()->setHighlightSections(false);  //防止表头塌陷

    //设置水平、垂直滚动条样式
    ui->games->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::this->add-line{background:transparent;}");

    ui->games->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::this->add-line{background:transparent;}");

    // 使列宽不能拖动
    ui->games->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->games->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //ui->games->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );

    //********************************************************************************
//    QPixmap f5("C:\\Git\\LGVS-Client\\Lobby\\res\\f5_16.png");  //刷新图片路径
//    ui->F5->setPixmap(QPixmap(f5).scaled(16,16));
//    ui->F5->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);//图片居中

//    QPixmap bak("C:\\Git\\LGVS-Client\\Lobby\\res\\bak_16.png");  //返回图片路径
//    ui->bak->setPixmap(QPixmap(bak).scaled(16,16));
//    ui->bak->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);//图片居中

    //********************************************************************************
    //ui->userinfo->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置表格QTableWidget不可编辑
    ui->message->document()->setMaximumBlockCount(100);//设置信息栏最多100行

    //********************************************************************************
    char* ip;
    if(test == 0){
        QHostInfo info = QHostInfo::fromName("play.niconiconi.cc");
        QString host = info.addresses().first().toString();
        QByteArray ba = host.toLatin1(); // must
        ip = ba.data();
    }
    else ip = "127.0.0.1";


    socket = new QTcpSocket(this);
    socket->connectToHost(ip,10199);
    if(socket->waitForConnected(10000))
    {

        QString sendbuf = "lobby," + (QString)arg[1]+",end";
        userid = ((QString)arg[1]).toInt();
        //qDebug()<< sendbuf;
        socket->write(sendbuf.toStdString().data());//发送握手识别码



        connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    }


    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));//心跳包
    m_pTimer->start(3000);
    //********************************************************************************
    ui->ready->hide();

}

void MainWindow::readData()
{

    QByteArray ba = socket->readAll();
    //std::string str = ba.toStdString();
    std::string str(ba.constData(), ba.length());
    //qDebug()<<str;
    //char* recv = socket->readAll().data();
    //QString msg = QString::fromGBK(ba);


    QString QS = QString::fromLocal8Bit(str.data());
    qDebug()<<QS;
    QStringList messages = QS.split(",");
    //const char* recv = QS.toStdString().c_str();
    //qDebug()<<QS;
    //qDebug()<<recv;

    //const char *sep = ","; //分割接收的数据
    //char *p;

//    p = strtok((char*)recv, sep);
//    messages[0] = p;
    //qDebug()<<p;
    bool finished = false;
    while(!finished)
    {       

        if(messages[0] == "f5")
        {

            if(lobby_flag == 0){
//                for (int i = 0; i < 2; i++) {
//                            messages[i] = p;
//                            //qDebug()<<p<<i;
//                            p = strtok(NULL, sep);
//                        }
//                int l=messages[1].toInt()*4+2;
//                for (int i = 2; i < l; i++) {
//                            messages[i] = p;
//                            //qDebug()<<p<<i;
//                            p = strtok(NULL, sep);
//                        }
                f5_games(messages);
            }

            else if(lobby_flag == 1){
                f5_rooms(QS);
            }
            else if(lobby_flag == 2){
                if(messages[1]=="manle"){
                    //lobby_flag--;
                    on_BAK_clicked();
                        QMessageBox box(QMessageBox::Critical,"错误","房间满员");
                        box.setStandardButtons (QMessageBox::Ok);
                        box.setButtonText (QMessageBox::Ok,QString("确 定"));
                        box.exec ();
                   // f5_rooms(QS);
                }
                else if(nowready == 1){
                ui->ready->setText("取消准备");
                ui->ready->setDisabled(false);
                f5_home(messages);
                }
                else{
                    gamestart = 0;
                    f5_home(messages);
                }
            }
        }

        else if(messages[0] == "lobbyok")
        {
            f5_user(messages);
            on_F5_clicked();
        }
        else if(messages[0] == "user")
        {
            f5_user(messages);
        }
        else if(messages[0] == "allready")
        {
            if(gamestart==1){
                qDebug()<<"游戏已经开始";
            }
            else{
                gamestart=1;
                ui->ready->setText("游戏中");
                ui->ready->setDisabled(true);
                ui->BAK->setDisabled(true);
                nowready = 2;

                process = new QProcess(this);

                QString qidong =".\\apps\\"+QString::number(nowcid)+".exe " +QString::number(userid+1)+" " + QString::number(nowhid);
                put("系统消息：启动"+qidong);
                process->start(qidong);
                //process.startDetached(qidong);
                process->waitForStarted();
                connect(process,SIGNAL(finished(int,QProcess::ExitStatus)),SLOT(finished(int,QProcess::ExitStatus)));
                //process->waitForFinished();
                //QString strResult = QString::fromLocal8Bit(process.readAllStandardOutput());

            }

        }



        ui->F5->setDisabled(false);
        ui->BAK->setDisabled(false);
        finished = true;
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::f5_games(QStringList info){
    //qDebug()<< info[1];
    nowcid=0;
    ui->title->setText("游戏大厅");
    ui->BAK->setText("退出");
    ui->games->QTableWidget::clear();
    ui->games->setColumnCount(4);//设置表格列数
    ui->games->setRowCount(0);
    QStringList headers;
    //headers << QStringLiteral(" 编号(cid)") << QStringLiteral("游戏图标") << QStringLiteral("游戏名(game_name)")<< QStringLiteral("玩家数/最大玩家数");
     headers << QStringLiteral(" 编号(cid)") << QStringLiteral("游戏图标") << QStringLiteral("游戏名(game_name)")<< QStringLiteral("游戏介绍（game_int）");
    ui->games->setHorizontalHeaderLabels(headers);
    for(int i=0,j=2;i<info[1].toInt();i++){
        QString picname = "apps\\" + info[j] + ".png";
       addGame(picname,info[j+1],info[j+3],info[j]);
       gamelist[info[j].toInt()]=info[j+1];
       j+=4;
    }
//    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\1.png",(QString)"五子棋",20,"1");

    //行和列的大小设为与内容相匹配
    ui->games->resizeColumnsToContents();
    ui->games->resizeRowsToContents();
}

void MainWindow::f5_rooms(QString info){

    QStringList list = info.split(",");

//    const char* recv = info.toStdString().c_str();
//    qDebug()<<recv;

//    const char *sep = ","; //分割接收的数据
//    char *p;
//    QString messages[10];
//    p = strtok((char*)recv, sep);
//    messages[0] = p;
//    //qDebug()<<p;
//    for (int i = 0; i < 2; i++) {
//            messages[i] = p;
//            qDebug()<<p<<i;
//            p = strtok(NULL, sep);
//     }


    QString title=gamelist[nowcid];
//    switch ( nowcid )
//    {
//    case 1 :
//        title="五子棋";
//        break;
//    case 2 :
//        title="中国象棋";
//        break;
//    case 3 :
//        title="星际争霸II";
//        break;
//    default:
//        title="选择房间";
//        break;
//    }



    ui->title->setText(title);
    ui->ready->hide();
    ui->BAK->setText("返回");
    ui->games->QTableWidget::clear();
    ui->games->setColumnCount(4);//设置表格列数
    ui->games->setRowCount(0);
    QStringList headers;
    headers << QStringLiteral(" 编号(hid)") << QStringLiteral("房间名(home_class)") << QStringLiteral("玩家数/最大玩家数")<< QStringLiteral("房间状态");
    ui->games->setHorizontalHeaderLabels(headers);

        for(int i=0,j=0;i<list[1].toInt();i++){
           addRome(list[j+2],title+"房间",list[j+5].toInt(),list[j+4].toInt(),list[j+6].toInt());
           ui->games->resizeColumnsToContents();
           ui->games->resizeRowsToContents();
            j+=5;
        }


    //int l = messages[1].toInt()*5+2;
//    for(int i=0;i<messages[1].toInt();i++){
//        for (int j = 2; j < 7; j++) {
//                    messages[i] = p;
//                    qDebug()<< p<< i<< j;
//                    p = strtok(NULL, sep);
//                }
//       addRome(messages[2],title+"房间",messages[3].toInt(),messages[4].toInt(),messages[6].toInt());
//       ui->games->resizeColumnsToContents();
//       ui->games->resizeRowsToContents();

//    }

//    addRome("1","五子棋房间", 1, 2,1);
//    addRome("12","五子棋房间", 1, 2,0);
//    addRome("123","五子棋房间", 0, 2,2);

    //行和列的大小设为与内容相匹配
//    ui->games->resizeColumnsToContents();
//    ui->games->resizeRowsToContents();

}

void MainWindow::f5_home(QStringList info){
    ui->ready->show();
    ui->title->setText(gamelist[nowcid]+"房间"+QString::number(nowhid));
    ui->games->QTableWidget::clear();
    ui->games->setColumnCount(5);//设置表格列数
    ui->games->setRowCount(0);
    QStringList headers;
    headers << QStringLiteral(" UID") << QStringLiteral("用户名")<< QStringLiteral("积分（money）") << QStringLiteral("注册时间")<< QStringLiteral("状态");
    ui->games->setHorizontalHeaderLabels(headers);
    for(int i=0,j=2;i<info[1].toInt();i++){
       addPlayer(info[j],info[j+1], info[j+2], info[j+3].toInt(),info[j+4].toInt());
       j+=5;
    }

//    addPlayer("1","huai", "123", 200,1);
//    addPlayer("12","lulu", "123", 2000,2);
//    addPlayer("123","shuchong", "123", 20,3);

    //行和列的大小设为与内容相匹配
    ui->games->resizeColumnsToContents();
    ui->games->resizeRowsToContents();

}

void MainWindow::f5_user(QStringList info){
        //ui->userinfo->count();
    ui->userinfo->QListWidget::clear();
    ui->userinfo->addItem("用户名："+info[3]);
    ui->userinfo->addItem("UID："+info[2]);
    ui->userinfo->addItem("积分："+info[6]);
    ui->userinfo->addItem("注册时间：");
    ui->userinfo->addItem(info[4]);
    ui->userinfo->addItem("最近活动时间：");
    ui->userinfo->addItem(info[5]);


//            int table_i = (ui->games->rowCount());//获取表目前的行数
//            ui->games->setRowCount(table_i+1);//使表增加一行
//            ui->games->setItem(table_i,0,new QTableWidgetItem(hid));//房间编号
//            ui->games->item(table_i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}




bool MainWindow::on_games_doubleClicked(const QModelIndex &index)
{
    if(lobby_flag==0){//在游戏大厅
        QList<QTableWidgetItem*> items = ui->games->selectedItems();
        QTableWidgetItem *gameid = items.at(0);
        QString cid = gameid->text(); //获取第1列内容
        QTableWidgetItem *gamename = items.at(1);
        QString gname = gamename->text(); //获取第3列内容
        put("用户操作：打开游戏["+cid+gname+"]房间大厅");
        lobby_flag ++;
        nowcid = cid.toInt();
        on_F5_clicked();
        return true;
    }
    else if(lobby_flag==1){//在房间大厅
        QList<QTableWidgetItem*> items = ui->games->selectedItems();
        QTableWidgetItem *gameid = items.at(0);
        QString hid = gameid->text(); //获取第1列内容
        put("用户操作：打开房间["+hid+"]");
        lobby_flag ++;
        nowhid = hid.toInt();
        on_F5_clicked();
        return true;
    }
    return false;
}

bool MainWindow::on_BAK_clicked()
{
    //m_pTimer->stop();
    ui->BAK->setDisabled(true);
    qDebug() << "返回";
    if(lobby_flag==0){
        this->close();
    }
    else if(lobby_flag==2){
        put("用户操作：返回");
        lobby_flag--;
        //sendstr("heart");
        //Sleep(100);
        nowready=0;
        on_F5_clicked();

    }
    else{
        put("用户操作：返回");
        lobby_flag--;
        on_F5_clicked();
    }
   // m_pTimer->start(3000);

}

void MainWindow::finished(int exitCode,QProcess::ExitStatus exitStatus)
{
//    qDebug()<<"finished";
    gameCode=exitCode;
    nowready = 0;
    ui->ready->setText("准备");
    ui->ready->setDisabled(false);
    ui->BAK->setDisabled(false);
    //gamestart = 0;
    if(gameCode<1){
        sendstr("gameover");
        put("系统提示：游戏战败");
    }
    else{
        sendstr("win,"+QString::number(gameCode));
        put("系统提示：游戏胜利");
    }

    //put("系统提示：游戏返回值（"+ QString::number(gameCode)+"）");



//    qDebug()<<exitCode;// 被调用程序的main返回的int
    //qDebug()<<exitStatus;// QProcess::ExitStatus(NormalExit)
//    qDebug() <<"finished-output-readAll:";
//    qDebug()<<QString::fromLocal8Bit(process->readAll());
//    qDebug()<<"finished-output-readAllStandardOutput:";
//    qDebug()<<QString::fromLocal8Bit(process->readAllStandardOutput());

}


bool MainWindow::on_F5_clicked()
{


    ui->F5->setDisabled(true);
    qDebug() << "刷新";
    //put("用户操作：刷新");
    if(lobby_flag==0){
        socket->write("class");
        //f5_games();
    }
    else if(lobby_flag==1){
        QString fa = "rooms,"+QString::number(nowcid);
        //socket->write(fa.toLatin1().data());
        sendstr(fa);
        //sendag(fa);
        //f5_rooms();
    }
    else if(lobby_flag==2){
        QString fa = "home,"+QString::number(nowhid);
        //socket->write(fa.toLatin1().data());
        sendstr(fa.toLatin1().data());
        //f5_home();
    }

    return true;
}


//bool MainWindow::addGame(QPixmap pixpath, QString gamename, int maxman,QString cid)
//{
//    int table_i = (ui->games->rowCount());//获取表目前的行数
//    ui->games->setRowCount(table_i+1);//使表增加一行
//    if(cid=="NULL"){
//            ui->games->setItem(table_i,0,new QTableWidgetItem(QString::number(table_i+1)));//自定编号
//    }
//    else{
//        ui->games->setItem(table_i,0,new QTableWidgetItem(cid));//自定编号
//    }

//    ui->games->item(table_i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

//    QPixmap pix(pixpath);
//    QLabel *label = new QLabel;
//    label->setPixmap(QPixmap(pix).scaled(72,72));
//    label->setMinimumSize( 80, 80 );
//    label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);//图片居中

//    ui->games->setCellWidget(table_i,1,label);//显示图片label

//    ui->games->setItem(table_i,2,new QTableWidgetItem(gamename));
//    ui->games->item(table_i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

////    ui->games->setItem(table_i,3,new QTableWidgetItem("0/"+QString::number(maxman)));
////    ui->games->item(table_i, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


//    //行和列的大小设为与内容相匹配
//    ui->games->resizeColumnsToContents();
//    ui->games->resizeRowsToContents();
//}

bool MainWindow::addGame(QPixmap pixpath, QString gamename, QString info,QString cid)
{
    int table_i = (ui->games->rowCount());//获取表目前的行数
    ui->games->setRowCount(table_i+1);//使表增加一行
    if(cid=="NULL"){
            ui->games->setItem(table_i,0,new QTableWidgetItem(QString::number(table_i+1)));//自定编号
    }
    else{
        ui->games->setItem(table_i,0,new QTableWidgetItem(cid));//自定编号
    }

    ui->games->item(table_i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    QPixmap pix(pixpath);
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap(pix).scaled(72,72));
    label->setMinimumSize( 80, 80 );
    label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);//图片居中

    ui->games->setCellWidget(table_i,1,label);//显示图片label

    ui->games->setItem(table_i,2,new QTableWidgetItem(gamename));
    ui->games->item(table_i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->games->setItem(table_i,3,new QTableWidgetItem(info));
    ui->games->item(table_i, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    //行和列的大小设为与内容相匹配
    ui->games->resizeColumnsToContents();
    ui->games->resizeRowsToContents();
}



bool MainWindow::addRome(QString hid,QString homename, int man, int maxman,int state)
{
    int table_i = (ui->games->rowCount());//获取表目前的行数
    ui->games->setRowCount(table_i+1);//使表增加一行
    ui->games->setItem(table_i,0,new QTableWidgetItem(hid));//房间编号
    ui->games->item(table_i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->games->setItem(table_i,1,new QTableWidgetItem(homename));
    ui->games->item(table_i, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->games->setItem(table_i,2,new QTableWidgetItem(QString::number(man)+"/"+QString::number(maxman)));
    ui->games->item(table_i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    QString statestr;
    switch ( state )
    {
    case 0 :
        statestr="关闭";
        break;
    case 1 :
        statestr="等待中";
        break;
    case 2 :
        statestr="游戏中";
        break;
    }

    ui->games->setItem(table_i,3,new QTableWidgetItem(statestr));
    ui->games->item(table_i, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    //行和列的大小设为与内容相匹配
    ui->games->resizeColumnsToContents();
    ui->games->resizeRowsToContents();
}


bool MainWindow::addPlayer(QString uid,QString username, QString regtime,int money,int ready)
{
    int table_i = (ui->games->rowCount());//获取表目前的行数
    ui->games->setRowCount(table_i+1);//使表增加一行
    ui->games->setItem(table_i,0,new QTableWidgetItem(uid));//房间编号
    ui->games->item(table_i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->games->setItem(table_i,1,new QTableWidgetItem(username));
    ui->games->item(table_i, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->games->setItem(table_i,2,new QTableWidgetItem(QString::number(money)));
    ui->games->item(table_i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->games->setItem(table_i,3,new QTableWidgetItem(regtime));
    ui->games->item(table_i, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    QString readystr;
    switch ( ready )
    {
    case 1 :
        readystr="已准备";
        break;
    case 0 :
        readystr="等待中";
        break;
    case 2 :
        readystr="游戏中";
        break;
    }

    ui->games->setItem(table_i,4,new QTableWidgetItem(readystr));
    ui->games->item(table_i, 4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    //行和列的大小设为与内容相匹配
    ui->games->resizeColumnsToContents();
    ui->games->resizeRowsToContents();
}


void MainWindow::put(QString xx){//向底部信息栏发送消息
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString nowtime =current_date_time.toString("hh:mm:ss");
    ui->message->append("["+nowtime+"]"+xx);
}


void MainWindow::on_ready_clicked()
{
    if(nowready == 0){
        nowready = 1;
        ui->ready->setText("已准备");
        ui->ready->setDisabled(true);
    }
    else if(nowready == 1){
        nowready = 0;
        ui->ready->setText("准备");
        //ui->ready->setDisabled(false);
    }

}

void MainWindow::handleTimeout()
{
    if(lobby_flag == 2){
        sendstr("ready,"+QString::number(nowready)+","+QString::number(nowhid));
    }
    //socket->write(("user,"+QString::number(userid)).toLatin1().data());
    else if(lobby_flag==1){
        QString fa = "rooms,"+QString::number(nowcid);
        //socket->write(fa.toLatin1().data());
        sendstr(fa);
        //sendag(fa);
        //f5_rooms();
    }
    else{
        sendstr("heart");
    }
    //socket->write("heart");
}

bool sendlock = false;
void MainWindow::sendstr(QString in){
    while(sendlock){
        Sleep(100);
    }
    sendlock = true;
    socket->write(in.toLatin1().data());
    Sleep(100);
    sendlock = false;
}

void MainWindow::sendag(QString in){
    while(sendlock){
        Sleep(100);
    }
    sendlock = true;
    send_pTimer= new QTimer(this);
    connect(send_pTimer, SIGNAL(timeout()), this, SLOT(sendTimeout(QString in)));
    send_pTimer->start(3000);
    socket->write(in.toLatin1().data());
    Sleep(100);
    sendlock = false;
}


void MainWindow::sendTimeout(QString in)
{
    sendstr(in);
    send_pTimer->stop();
}
