#include "mainusersworkersreceipt_documentsadd.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_mainusersworkersreceipt_documentsadd.h"

MainUsersWorkersReceipt_documentsAdd::MainUsersWorkersReceipt_documentsAdd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersReceipt_documentsAdd)
{
    ui->setupUi(this);
    ui->dtData->setDateTime(QDateTime::currentDateTime());

    QSqlQuery query("SELECT id_sklad, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = product_sklad), product_sklad FROM \"Warehouse\" ORDER BY id_sklad;");
    ui->cbProduct->clear();
    ui->cbProduct->setPlaceholderText("Выберите товар");
    ui->cbProduct->setEnabled(false);

    while (query.next()) {
        QString idSklad = query.value(0).toString();
        QString product = query.value(1).toString();
        idProduct = query.value(2).toString();
        ui->cbProduct->addItem(product);
        ui->cbProduct->setItemData(ui->cbProduct->count() - 1, idSklad);  // Сохраняем id_sklad в дополнительных данных элемента комбобокса
    }

    connect(ui->checkBox, &QCheckBox::clicked, this, &MainUsersWorkersReceipt_documentsAdd::toggleComboBox);
    connect(ui->cbProduct, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));

}

void MainUsersWorkersReceipt_documentsAdd::onComboBoxIndexChanged(int index)
{
    QString idSklad = ui->cbProduct->itemData(index).toString();  // Получаем id_sklad из дополнительных данных элемента комбобокса
    QSqlQuery query;
    query.prepare("SELECT product_receipt, provaider_receipt FROM \"Receipt_documents\" WHERE id_receipt = :selectedValue");
    query.bindValue(":selectedValue", idProduct);
    idSkladProduct = ui->cbProduct->itemData(index).toString();
    if (query.exec() && query.next()){
        ui->txtProduct->setText(query.value(0).toString());
        ui->txtProvaider->setText(query.value(1).toString());
    }
    else {
        qDebug() << "Ошибка выполнения запроса" << query.lastError().text();
            return;
    }
    query.prepare("SELECT  count_sklad, location_sklad FROM \"Warehouse\" WHERE id_sklad = :selectedValue");
    query.bindValue(":selectedValue", idSklad);
    if (query.exec() && query.next()){
            idCount = query.value(0).toString();
            ui->txtLocation->setText(query.value(1).toString());
    }
    else {
            qDebug() << "Ошибка выполнения запроса" << query.lastError().text();
                return;
    }
}

void MainUsersWorkersReceipt_documentsAdd::toggleComboBox()
{
    if (ui->checkBox->isChecked()) {
            ui->cbProduct->setEnabled(true);
            ui->txtProduct->setEnabled(false);
            ui->txtProvaider->setEnabled(false);
            ui->txtLocation->setEnabled(false);
    } else {
            ui->cbProduct->setEnabled(false);
            ui->txtProduct->setText("");
            ui->txtProvaider->setText("");
            ui->txtLocation->setText("");
            ui->txtProduct->setEnabled(true);
            ui->txtProvaider->setEnabled(true);
            ui->txtLocation->setEnabled(true);
    }
}
MainUsersWorkersReceipt_documentsAdd::~MainUsersWorkersReceipt_documentsAdd()
{
    delete ui;
}
void MainUsersWorkersReceipt_documentsAdd::recieveID(QString ID)
{
    m_idusers = ID;
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT fullname_users from \"Users\" WHERE id_users = ?");
    query->bindValue(0,ID);
    query->exec();
    if (query->next()) {
        ui->txtResp->setText(query->value(0).toString());
        ui->txtResp->setEnabled(0);
    }
}

void MainUsersWorkersReceipt_documentsAdd::on_pbAdd_clicked()
{

    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dtData->setDateTime(currentDateTime);

    QSqlQuery* query = new QSqlQuery;
    query->prepare("INSERT INTO \"Receipt_documents\"(product_receipt, provaider_receipt, data_receipt, count_receipt, location_receipt, responsible_receipt) "
                   "VALUES (:product, :provaider, :data, :count, :location, :responsible)");
    query->bindValue(":product", ui->txtProduct->text());
    query->bindValue(":provaider", ui->txtProvaider->text());
    query->bindValue(":data", currentDateTime);
    query->bindValue(":count", ui->txtCount->text().toLongLong());
    query->bindValue(":location", ui->txtLocation->text());
    query->bindValue(":responsible", m_idusers.toLongLong());

    QMessageBox* mess = new QMessageBox;
    if (!query->exec())
    {
        qDebug() << query->lastError().text();
        mess->setText("Запрос составлен неверно!");
        mess->show();
        return;
    }

    if (ui->checkBox->isChecked()) {
        int updatedCount = idCount.toInt() + ui->txtCount->text().toInt();  // Прибавление значения из txtCount к idCount

        query->prepare("UPDATE  \"Warehouse\" SET count_sklad=? WHERE id_sklad = ?");

        query->bindValue(0,updatedCount);
        query->bindValue(1,idSkladProduct);

        if (query->exec()) {
            qDebug() << "Данные успешно обновлены";
            // Дополнительные действия после успешного обновления данных
        } else {
            qDebug() << "Ошибка выполнения запроса на обновление данных" << query->lastError().text();
            // Обработка ошибки
        }
    }
    else {

    // Выполнение запроса SELECT
    query->prepare("SELECT id_receipt FROM \"Receipt_documents\" WHERE product_receipt = :product AND data_receipt = :data AND count_receipt = :count AND location_receipt = :location");
    query->bindValue(":product", ui->txtProduct->text());
    query->bindValue(":data", ui->dtData->dateTime());
    query->bindValue(":count", ui->txtCount->text().toLongLong());
    query->bindValue(":location", ui->txtLocation->text());

    if (!query->exec())
    {
        qDebug() << query->lastError().text();
        mess->setText("Запрос SELECT составлен неверно!");
        mess->show();
        return;
    }

    // Получение значения из запроса SELECT
    if (query->next())
    {
        QString receiptId = query->value(0).toString();

        // Выполнение запроса INSERT в таблицу "Warehouse"
        query->prepare("INSERT INTO \"Warehouse\"(product_sklad, count_sklad, location_sklad) "
                       "VALUES (:product, :count, :location)");
        query->bindValue(":product", receiptId);
        query->bindValue(":count", ui->txtCount->text().toLongLong());
        query->bindValue(":location", ui->txtLocation->text());

        if (!query->exec())
        {
            qDebug() << query->lastError().text();
            mess->setText("Запрос INSERT в таблицу Warehouse составлен неверно!");
            mess->show();
            return;
        }
    }
    else
    {
        // Если запрос SELECT не вернул результаты, выводите сообщение об ошибке или выполняйте другие необходимые действия.
        qDebug() << "Запрос SELECT не вернул результаты!";
        mess->setText("Запрос SELECT не вернул результаты!");
        mess->show();
        return;
    }
    }

    // Остальной код, который нужно выполнить после успешного выполнения запросов
    emit refresh_table();
    this->close();
}


void MainUsersWorkersReceipt_documentsAdd::on_pbClaer_clicked()
{
    ui->txtProduct->setText("");
    ui->txtProvaider->setText("");
    ui->txtCount->setText("");
    ui->txtLocation->setText("");
}


void MainUsersWorkersReceipt_documentsAdd::on_pbClose_clicked()
{
    this->close();
}



