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


const bool test = 1;
namespace Ui
{
class Chess;
}

class Chess : public QMainWindow
{
    Q_OBJECT

public:
    Chess(QWidget *parent, QString arg[]);
    ~Chess();

signals:
    void addChess(int row,int col);

private slots:
    void on_quitButton_clicked();
    void readData();
    void sendChessInfo(QString chess);
    void sendWin();
    void handleTimeout();
    void writewin();

private:
    Ui::Chess *ui;
    int exit;
    QTcpServer *server;
    QTcpSocket *socket;
    QTimer *m_pTimer;
    int hintNum;
};


#endif // CHESS_H
