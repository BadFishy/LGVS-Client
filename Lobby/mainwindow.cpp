#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox.h"
#include "QtWidgets"


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
    ui->tableWidget->setColumnCount(4);//设置表格QTableWidget列
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);    //设置表格QTableWidget不可编辑
    //使表格QTableWidget选中选择整行
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget->horizontalHeader()->setFocusPolicy(Qt::NoFocus); //设置表头不可选
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);//无虚线
    //ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//最后一栏自适应长度
    ui->tableWidget->setShowGrid(false); //设置不显示格子线
    ui->tableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->tableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    //ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);  //防止表头塌陷

    //设置水平、垂直滚动条样式
    ui->tableWidget->horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");

    ui->tableWidget->verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
    "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
    "QScrollBar::handle:hover{background:gray;}"
    "QScrollBar::sub-line{background:transparent;}"
    "QScrollBar::add-line{background:transparent;}");

    // 使列宽不能拖动
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    //ui->tableWidget->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );

    //********************************************************************************
    QPixmap f5("C:\\Git\\LGVS-Client\\Lobby\\res\\f5.png");  //图片路径
    ui->F5->setPixmap(QPixmap(f5).scaled(32,32));
    ui->F5->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);//图片居中

    //********************************************************************************
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\1.png",(QString)"五子棋",20);
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\2.png",(QString)"中国象棋",20);
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\1.png",(QString)"星际争霸II",20);
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\2.png",(QString)"中国象棋",20);
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\1.png",(QString)"五子棋",20);
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\2.png",(QString)"中国象棋",20);
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\1.png",(QString)"五子棋",20);
    addGame((QString)"C:\\Git\\LGVS-Client\\Lobby\\gameicons\\2.png",(QString)"中国象棋",20);


}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::addGame(QPixmap pixpath, QString gamename, int maxman)
{
    int table_i = (ui->tableWidget->rowCount());//获取表目前的行数
    ui->tableWidget->setRowCount(table_i+1);//使表增加一行
    ui->tableWidget->setItem(table_i,0,new QTableWidgetItem(QString::number(table_i+1)));//自定编号
    ui->tableWidget->item(table_i, 0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    QPixmap pix(pixpath);
    QLabel *label = new QLabel;
    label->setPixmap(QPixmap(pix).scaled(72,72));
    label->setMinimumSize( 80, 80 );
    label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);//图片居中

    ui->tableWidget->setCellWidget(table_i,1,label);//显示图片label
//    int namelen = gamename.toLocal8Bit().length();
//    if( namelen <= 28){
//        for(int i = 0;i<(28-namelen)/2;i++){
//            gamename = " " +  gamename;
//        }
//        for(int i = (28-namelen)/2+namelen;i<28;i++){
//            gamename += " ";
//        }

//        ui->tableWidget->setItem(table_i,2,new QTableWidgetItem(gamename));
//    }
//    else {
//        ui->tableWidget->setItem(table_i,2,new QTableWidgetItem("游戏名过长（大于28字符）"));
//    }
    ui->tableWidget->setItem(table_i,2,new QTableWidgetItem(gamename));
    ui->tableWidget->item(table_i, 2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    ui->tableWidget->setItem(table_i,3,new QTableWidgetItem("0/"+QString::number(maxman)));
    ui->tableWidget->item(table_i, 3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

    //行和列的大小设为与内容相匹配
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();


}
