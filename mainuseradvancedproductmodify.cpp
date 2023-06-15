#include "mainuseradvancedproductmodify.h"
#include "qsqlerror.h"
#include "ui_mainuseradvancedproductmodify.h"

MainUserAdvancedProductModify::MainUserAdvancedProductModify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUserAdvancedProductModify)
{
    ui->setupUi(this);
    QSqlQuery query("Select  name_provider from \"Provider\"");
    ui->cbProvider->clear();
    while (query.next()) {
        QString name = query.value(0).toString();
        ui->cbProvider->addItem(name);
    }
}

MainUserAdvancedProductModify::~MainUserAdvancedProductModify()
{
    delete ui;
}

void MainUserAdvancedProductModify::obr_sendID(int id)
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT name_product, (SELECT name_provider FROM \"Provider\" WHERE id_provider = provider_product), count_product FROM \"Product\" WHERE id_product = ?");
    query->bindValue(0,id);

    if(query->exec())
    {
        query->next();
        ui->txtpp->setText(QString::number(id));
        ui->txtProduct->setText(query->value(0).toString());
        int index = ui->cbProvider->findText(query->value(1).toString());
        ui->cbProvider->setCurrentIndex(index);
        ui->txtCount->setText(query->value(2).toString());
    }
}

void MainUserAdvancedProductModify::on_btnEdit_clicked()
{
    QString providerName = ui->cbProvider->currentText();

    // Первый запрос для получения идентификатора поставщика по его имени
    QSqlQuery getProviderIdQuery;
    getProviderIdQuery.prepare("SELECT id_provider FROM \"Provider\" WHERE name_provider = :name");
    getProviderIdQuery.bindValue(":name", providerName);
    if (!getProviderIdQuery.exec() || !getProviderIdQuery.next()) {
        qDebug() << "Ошибка выполнения запроса" << getProviderIdQuery.lastError().text();
            return;
    }

    // Получаем id_provider из первого запроса
    int providerId = getProviderIdQuery.value(0).toInt();

    QSqlQuery* query = new QSqlQuery();
    query->prepare("UPDATE  \"Product\" SET name_product=?, provider_product=?, count_product=? WHERE id_product = ?");
    query->bindValue(0,ui->txtProduct->text());
    query->bindValue(1,providerId);
    query->bindValue(2,ui->txtCount->text().toInt());
    query->bindValue(3,ui->txtpp->text().toInt());

    if(query->exec())
    {
        emit refresh_table();
        this->close();
    }
    if (!query->exec() || !query->next()) {
        qDebug() << "Ошибка выполнения запроса" << query->lastError().text();
            return;
    }

}


void MainUserAdvancedProductModify::on_btnClose_clicked()
{
     this->close();
}

