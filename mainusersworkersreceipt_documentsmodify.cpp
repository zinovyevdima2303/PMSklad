#include "mainusersworkersreceipt_documentsmodify.h"
#include "ui_mainusersworkersreceipt_documentsmodify.h"

MainUsersWorkersReceipt_documentsModify::MainUsersWorkersReceipt_documentsModify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersReceipt_documentsModify)
{
    ui->setupUi(this);
}

MainUsersWorkersReceipt_documentsModify::~MainUsersWorkersReceipt_documentsModify()
{
    delete ui;
}

void MainUsersWorkersReceipt_documentsModify::obr_sendIDNakl(int idNakl)
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT product_receipt, provaider_receipt, data_receipt, count_receipt, location_receipt, (SELECT  fullname_users FROM \"Users\" WHERE responsible_receipt = id_users ) FROM \"Receipt_documents\" WHERE id_receipt = ?");
    query->bindValue(0,idNakl);
    if(query->exec())
    {
        query->next();
        ui->txtID->setText(QString::number(idNakl));
        ui->txtProduct->setText(query->value(0).toString());
        ui->txtProvaider->setText(query->value(1).toString());
        ui->dtData->setDateTime(query->value(2).toDateTime());
        ui->txtCount->setText(query->value(3).toString());
        ui->txtLocation->setText(query->value(4).toString());
        ui->txtResp->setText(query->value(5).toString());
    }
}

void MainUsersWorkersReceipt_documentsModify::on_pbEdit_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("UPDATE  \"Receipt_documents\" SET product_receipt=?, provaider_receipt=?  WHERE id_receipt = ?");
    query->bindValue(0,ui->txtProduct->text());
    query->bindValue(1,ui->txtProvaider->text());
    query->bindValue(2,ui->txtID->text().toInt());

    if(query->exec())
    {
        emit refresh_table();
        this->close();
    }

}


void MainUsersWorkersReceipt_documentsModify::on_pbClose_clicked()
{
      this->close();
}

