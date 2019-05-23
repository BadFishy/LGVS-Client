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
        str[0] = "0";
    }

    Chess c(0,str);

    if(str[0]=="0"){
        c.setWindowTitle("五子棋 —— 主场黑棋");
    }
    else if(str[0]=="1"){
        c.setWindowTitle("五子棋 —— 客场白棋");
    }

    c.show();

    return a.exec();
}
