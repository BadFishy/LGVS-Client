#include "create.h"

CreateDialog::CreateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDialog)
{
    ui->setupUi(this);

    QString ipAddress;

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for(int i = 0; i < ipAddressesList.size(); i++)
    {
        if(ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
        {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }

    if(ipAddress.isEmpty())
        ipAddress  = QHostAddress(QHostAddress::LocalHost).toString();

    ui->lineEdit->setText(ipAddress);

}

CreateDialog::~CreateDialog()
{
    delete ui;
}

QString CreateDialog::getHostAddress()
{
    return hostAddress;
}

void CreateDialog::on_okButton_clicked()
{
    //qDebug() << "hhh";
    hostAddress = ui->lineEdit->text();
    emit getButton(1);
    this->close();
}

void CreateDialog::on_cancelButton_clicked()
{
    this->close();
}
