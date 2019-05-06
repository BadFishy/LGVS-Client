#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox.h"
#include "QtWidgets"
#include "time.h"
#include "QDateTime"


MainWindow::MainWindow(QWidget *parent) :
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
    ui->ready->hide();
    f5_games();


}

MainWindow::~MainWindow()
{
    delete ui;
}

int lobby_flag=0;
/*  lobby_flag的取值
*   0：游戏大厅
*   1：房间大厅
*   2：房间内
*/
int nowcid=0;//当前所在游戏cid

void MainWindow::f5_games(){
    nowcid=0;
    ui->title->setText("游戏大厅");
    ui->BAK->setText("退出");
    ui->games->QTableWidget::clear();
    ui->games->setColumnCount(4);//设置表格列数
    ui->games->setRowCount(0);
    QStringList headers;
    headers << QStringLiteral(" 编号(cid)") << QStringLiteral("游戏图标") << QStringLiteral("游戏名(game_name)")<< QStringLiteral("玩家数/最大玩家数");
    ui->games->setHorizontalHeaderLabels(headers);
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\1.png",(QString)"五子棋",20,"10086");
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\2.png",(QString)"中国象棋",20,"10010");
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\1.png",(QString)"星际争霸II",20);

    //行和列的大小设为与内容相匹配
    ui->games->resizeColumnsToContents();
    ui->games->resizeRowsToContents();
}

void MainWindow::f5_rooms(){
    QString title;
    switch ( nowcid )
    {
    case 10086 :
        title="五子棋";
        break;
    case 10010 :
        title="中国象棋";
        break;
    case 1 :
        title="星际争霸II";
        break;
    default:
        title="选择房间";
        break;
    }
    ui->title->setText(title);
    ui->ready->hide();
    ui->BAK->setText("返回");
    ui->games->QTableWidget::clear();
    ui->games->setColumnCount(4);//设置表格列数
    ui->games->setRowCount(0);
    QStringList headers;
    headers << QStringLiteral(" 编号(hid)") << QStringLiteral("房间名(home_class)") << QStringLiteral("玩家数/最大玩家数")<< QStringLiteral("房间状态");
    ui->games->setHorizontalHeaderLabels(headers);

    addRome("1","五子棋房间", 1, 2,1);
    addRome("12","五子棋房间", 1, 2,0);
    addRome("123","五子棋房间", 0, 2,2);

    //行和列的大小设为与内容相匹配
    ui->games->resizeColumnsToContents();
    ui->games->resizeRowsToContents();

}

void MainWindow::f5_home(){
    ui->ready->show();
    ui->title->setText("房间");
    ui->games->QTableWidget::clear();
    ui->games->setColumnCount(4);//设置表格列数
    ui->games->setRowCount(0);
    QStringList headers;
    headers << QStringLiteral(" 用户编号(uid)") << QStringLiteral("用户名") << QStringLiteral("注册时间")<< QStringLiteral("积分");
    ui->games->setHorizontalHeaderLabels(headers);

    addPlayer("1","huai", "123", 200);
    addPlayer("12","lulu", "123", 2000);
    addPlayer("123","shuchong", "123", 20);

    //行和列的大小设为与内容相匹配
    ui->games->resizeColumnsToContents();
    ui->games->resizeRowsToContents();

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
        on_F5_clicked();
        return true;
    }
    return false;
}

bool MainWindow::on_BAK_clicked()
{
    qDebug() << "返回";
    if(lobby_flag==0){
        this->close();
    }
    else{
        put("用户操作：返回");
        lobby_flag--;
        on_F5_clicked();
    }

}

time_t f5time = time(0);
int timer =0;
bool MainWindow::on_F5_clicked()
{
    ui->F5->setEnabled(false);

    if(time(0) - f5time<2){
        timer++;

        if(timer>=3){
            qDebug() << "刷新太快";
            put("系统提示：操作太快，喝杯茶休息会儿");
            //Sleep(5000);
            ui->F5->setEnabled(true);
            return false;
        }
        else{
            f5time = time(0);
        }
    }
    else{
        timer=0;
    }


    qDebug() << "刷新";
    //put("用户操作：刷新");
    if(lobby_flag==0){
        f5_games();
    }
    else if(lobby_flag==1){
        f5_rooms();
    }
    else if(lobby_flag==2){
        f5_home();
    }
    f5time = time(0);
    ui->F5->setEnabled(true);
    return true;
}


bool MainWindow::addGame(QPixmap pixpath, QString gamename, int maxman,QString cid)
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

    ui->games->setItem(table_i,3,new QTableWidgetItem("0/"+QString::number(maxman)));
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


bool MainWindow::addPlayer(QString uid,QString username, QString regtime,int money)
{
    int table_i = (ui->games->rowCount());//获取表目前的行数
    ui->games->setRowCount(table_i+1);//使表增加一行
    ui->games->setItem(table_i,0,new QTableWidgetItem(uid));//房间编号
    ui->games->item(table_i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->games->setItem(table_i,1,new QTableWidgetItem(username));
    ui->games->item(table_i, 1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->games->setItem(table_i,2,new QTableWidgetItem(regtime));
    ui->games->item(table_i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);


    ui->games->setItem(table_i,3,new QTableWidgetItem(QString::number(money)));
    ui->games->item(table_i, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

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

}
