#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));


    //QString str[4];
    if(argc<1){
        QMessageBox box(QMessageBox::Critical,"错误","请使用客户端启动");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        box.exec ();
        return -1;
    }


    MainWindow w(0,argv);
    w.setWindowTitle("凉糕对战平台");
    w.show();
    return a.exec();
}
