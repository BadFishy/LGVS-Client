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

    void testSlot(int row,int column);


private slots:
    void on_F5_clicked();

    void on_games_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
