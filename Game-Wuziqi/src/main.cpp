#include <QApplication>
#include "Chess.h"

int main (int argc, char *argv[])
{
    QApplication a(argc,argv);
    QString str[4];
    if(argc>1){
        for(int i=0;i<argc;i++){
            str[i] = QString(QLatin1String(argv[i+1]));
        }
    }
    else{
        str[0] = "err";
    }

    Chess c(0,str);
    if(str[0]=="err"){
//        auto quit = QMessageBox::critical(&c,"错误","请使用客户端启动",QMessageBox::Yes);
//        if(quit == QMessageBox::Yes)
//        {
//            return -1;
//        }
        QMessageBox box(QMessageBox::Critical,"错误","请使用客户端启动");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        box.exec ();
        return -1;
    }

    if(str[0]=="0"){
        c.setWindowTitle("五子棋 —— 主场黑棋");
    }
    else if(str[0]=="1"){
        c.setWindowTitle("五子棋 —— 客场白棋");
    }

    else{
        c.setWindowTitle("五子棋 —— 凉糕对战平台 房间号："+str[1]);
    }

    if(c.isconnect == false){
        return -1;
    }

    c.show();

    return a.exec();
}
