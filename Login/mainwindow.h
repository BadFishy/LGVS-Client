#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "regui.h"
const bool test = 1;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    int on_pushButton_clicked();

    void on_label_4_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
