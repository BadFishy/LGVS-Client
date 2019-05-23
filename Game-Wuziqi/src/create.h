#ifndef CREATE_H
#define CREATE_H

#include <QDialog>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QList>
#include "ui_create.h"

namespace Ui
{
class CreateDialog;
}

class CreateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDialog(QWidget *parent = 0);
    ~CreateDialog();

    QString getHostAddress();

signals:
    void getButton(int num);

private slots:

    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::CreateDialog *ui;
    QString hostAddress;
};


#endif // CREATE_H
