#include "mainusersworkersinventorymodify.h"
#include "ui_mainusersworkersinventorymodify.h"

MainUsersWorkersInventoryModify::MainUsersWorkersInventoryModify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersInventoryModify)
{
    ui->setupUi(this);
}

MainUsersWorkersInventoryModify::~MainUsersWorkersInventoryModify()
{
    delete ui;
}

void MainUsersWorkersInventoryModify::recieveID(QString ID)
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

void MainUsersWorkersInventoryModify::obr_sendIDNakl(int idNakl)
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT id_inventory ,(SELECT  product_receipt FROM \"Receipt_documents\" WHERE id_receipt = "
                   "(SELECT  product_sklad FROM \"Warehouse\" WHERE  id_sklad = product_inventory)),location_inventory , "
                   "data_inventory , count_inventory , newcount_inventory, (SELECT  fullname_users FROM \"Users\" WHERE responsible_inventory = id_users ),"
                   "explanations_inventory FROM \"Inventory\" WHERE id_inventory = ?");
    query->bindValue(0,idNakl);
    if(query->exec())
    {
        query->next();
        ui->txtID->setText(QString::number(idNakl));
        ui->txtProduct->setText(query->value(1).toString());
        ui->txtLocation->setText(query->value(2).toString());
        ui->dtDate->setDateTime(query->value(3).toDateTime());
        ui->txtCount->setText(query->value(4).toString());
        ui->txtNewCount->setText(query->value(5).toString());
        ui->txtResponsible->setText(query->value(6).toString());
        ui->txtExplanations->setText(query->value(7).toString());

    }
 }

void MainUsersWorkersInventoryModify::on_pbEdit_clicked()
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("UPDATE  \"Inventory\" SET changer_inventory=?, explanations_inventory=?  WHERE id_inventory = ?");
    query->bindValue(0,m_idusers.toLongLong());
    query->bindValue(1,ui->txtExplanations->text());
    query->bindValue(2,ui->txtID->text().toInt());

    if(query->exec())
    {
        emit refresh_table();
        this->close();
    }
}


void MainUsersWorkersInventoryModify::on_pbClose_clicked()
{
    this->close();

}

