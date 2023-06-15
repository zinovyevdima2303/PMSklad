#include "mainusersworkerstransfermodify.h"
#include "ui_mainusersworkerstransfermodify.h"

MainUsersWorkersTransferModify::MainUsersWorkersTransferModify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersTransferModify)
{
    ui->setupUi(this);
}

MainUsersWorkersTransferModify::~MainUsersWorkersTransferModify()
{
    delete ui;
}

void MainUsersWorkersTransferModify::recieveID(QString ID)
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

void MainUsersWorkersTransferModify::obr_sendIDNakl(int idNakl)
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT id_transfer, (SELECT  product_receipt FROM \"Receipt_documents\" WHERE  id_receipt = "
                   "(SELECT  product_sklad FROM \"Warehouse\" WHERE  id_sklad = product_transfer)),data_transfer,"
                   "location_transfer , newlocation_transfer , (SELECT  fullname_users FROM \"Users\" WHERE responsible_transfer = id_users ),"
                   "explanations_transfer FROM \"Transfer_invoices\" WHERE id_transfer = ?");
    query->bindValue(0,idNakl);
    if(query->exec())
    {
        query->next();
        ui->txtID->setText(QString::number(idNakl));
        ui->txtProduct->setText(query->value(1).toString());
        ui->dtDate->setDateTime(query->value(2).toDateTime());
        ui->txtLocation->setText(query->value(3).toString());
        ui->txtNewLocation->setText(query->value(4).toString());
        ui->txtResponsible->setText(query->value(5).toString());
        ui->txtExplanations->setText(query->value(6).toString());

    }
}


void MainUsersWorkersTransferModify::on_pbEdit_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("UPDATE  \"Transfer_invoices\" SET changer_transfer=?, explanations_transfer=?  WHERE id_transfer = ?");
    query->bindValue(0,m_idusers.toLongLong());
    query->bindValue(1,ui->txtExplanations->text());
    query->bindValue(2,ui->txtID->text().toInt());

    if(query->exec())
    {
        emit refresh_table();
        this->close();
    }
}


void MainUsersWorkersTransferModify::on_pbClose_clicked()
{
    this->close();

}

