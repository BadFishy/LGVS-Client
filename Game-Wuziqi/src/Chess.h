#ifndef CHESS_H
#define CHESS_H

#include <QWidget>
#include <QVector>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QMainWindow>
#include <QTimer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QSound>
#include "ui_Chess.h"
#include "connect.h"
#include "create.h"

namespace Ui
{
class Chess;
}

class Chess : public QMainWindow
{
    Q_OBJECT

public:
    Chess(QWidget *parent = 0, QString arg = "0");
    ~Chess();

signals:
    void addChess(int row,int col);

private slots:
    void on_createButton_clicked();
    void on_connectButton_clicked();
    void on_quitButton_clicked();
    void createReturn(int num);
    void connectReturn(int num);
    void readData();
    void sendChessInfo(QString chess);
    void sendWin();


private:
    Ui::Chess *ui;

    CreateDialog *dialog;
    ConnectDialog *dialog1;

    QTcpServer *server;
    QTcpSocket *socket;

    int hintNum;
};


#endif // CHESS_H
