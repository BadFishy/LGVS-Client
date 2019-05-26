#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>

const bool test = 1;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent,char *arg[]);
    ~MainWindow();
   // bool addGame(QPixmap pix,QString gamename,int maxman,QString cid="NULL");
    bool addGame(QPixmap pix,QString gamename, QString info,QString cid="NULL");
    bool addRome(QString hid,QString homename, int man, int maxman,int state);
    bool addPlayer(QString uid,QString username, QString regtime,int money,int ready);

    void f5_games(QStringList gamesinfo);
    void f5_rooms(QString);
    void f5_home(QStringList gamesinfo);
    void f5_user(QStringList);
    void sendstr(QString in);
    void put(QString xx);
    void sendag(QString in);

public slots:
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
private slots:
    bool on_F5_clicked();
    bool on_games_doubleClicked(const QModelIndex &index);
    bool on_BAK_clicked();
    void on_ready_clicked();
    void readData();
    void handleTimeout();
    void sendTimeout(QString in);

private:
    Ui::MainWindow *ui;
     QTcpSocket *socket;

     int lobby_flag=0;
     /*  lobby_flag的取值
     *   0：游戏大厅
     *   1：房间大厅
     *   2：房间内
     */
     int nowcid=0;//当前所在游戏cid
     int nowhid=0;//当前所在房间hid
     int nowready = 0;

     QString gamelist[100];

     QTimer *m_pTimer;
     QTimer *send_pTimer;
     int userid=0;
    int gameCode;
    int gamestart=0;
    QProcess *process;
};

void f5_games(Ui::MainWindow ui);

#endif // MAINWINDOW_H
