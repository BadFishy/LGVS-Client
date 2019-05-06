#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool addGame(QPixmap pix,QString gamename,int maxman,QString cid="NULL");
    bool addRome(QString hid,QString homename, int man, int maxman,int state);
    bool addPlayer(QString uid,QString username, QString regtime,int money);

    void f5_games();
    void f5_rooms();
    void f5_home();

    void put(QString xx);

private slots:
    bool on_F5_clicked();

    bool on_games_doubleClicked(const QModelIndex &index);

    bool on_BAK_clicked();

    void on_ready_clicked();

private:
    Ui::MainWindow *ui;
};

void f5_games(Ui::MainWindow ui);

#endif // MAINWINDOW_H
