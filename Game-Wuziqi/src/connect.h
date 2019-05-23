#ifndef CONNECT_H
#define CONNECT_H

#include <QDialog>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QSignalMapper>
#include <QPushButton>
#include "ui_connect.h"

namespace Ui
{
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

    QString getHostIp();

signals:
    void getButton(int num);

private slots:
    void addNumber(QString str);

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ConnectDialog *ui;

    QString hostIp;
    QPushButton *button[12];
};



#endif // CONNECT_H
