#include "mainusersworkersreturnmodify.h"
#include "ui_mainusersworkersreturnmodify.h"

MainUsersWorkersReturnModify::MainUsersWorkersReturnModify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersReturnModify)
{
    ui->setupUi(this);
}

MainUsersWorkersReturnModify::~MainUsersWorkersReturnModify()
{
    delete ui;
}

void MainUsersWorkersReturnModify::recieveID(QString ID)
{
    m_idusers = ID;
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT fullname_users from \"Users\" WHERE id_users = ?");
    query->bindValue(0,ID);
    query->exec();
    if (query->next()) {
        ui->txtChanger->setText(query->value(0).toString());
        ui->txtChanger->setEnabled(0);
    }
}

void MainUsersWorkersReturnModify::obr_sendIDNakl(int idNakl)
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT id_return,(SELECT  product_receipt FROM \"Receipt_documents\" WHERE id_receipt = (SELECT product_sklad FROM \"Warehouse\" WHERE id_sklad = product_return)),"
                   "data_return, count_return, location_return, (SELECT  fullname_users FROM \"Users\" WHERE responsible_return = id_users ),"
                   " explanations_return FROM \"Return_documents\" WHERE id_return = ?");
    query->bindValue(0,idNakl);
    if(query->exec())
    {
        query->next();
        ui->txtID->setText(QString::number(idNakl));
        ui->txtProduct->setText(query->value(1).toString());
        ui->dtDate->setDateTime(query->value(2).toDateTime());
        ui->txtCountReturn->setText(query->value(3).toString());
        ui->txtLocation->setText(query->value(4).toString());
        ui->txtResponsible->setText(query->value(5).toString());
        ui->txtExplanations->setText(query->value(6).toString());
    }
}

void MainUsersWorkersReturnModify::on_pbEdit_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("UPDATE  \"Return_documents\" SET changer_return=?, explanations_return=?  WHERE id_return = ?");
    query->bindValue(0,m_idusers.toLongLong());
    query->bindValue(1,ui->txtExplanations->text());
    query->bindValue(2,ui->txtID->text().toInt());

    if(query->exec())
    {
        emit refresh_table();
        this->close();
    }
}


void MainUsersWorkersReturnModify::on_pbClose_clicked()
{
    this->close();

}
