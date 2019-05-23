#include <QApplication>
#include "Chess.h"

int main (int argc, char *argv[])
{
    QApplication a(argc,argv);
    QString str;



    if(argc>1){
        str = QString(QLatin1String(argv[1]));
    }
    else{
        str = "0";
    }

    Chess c(0,str);

    if(str=="0"){
        c.setWindowTitle("五子棋 —— 主场黑棋");
    }
    else if(str=="1"){
        c.setWindowTitle("五子棋 —— 客场白棋");
    }

    c.show();

    return a.exec();
}
