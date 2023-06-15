#include "mainusersworkersinventoryadd.h"
#include "ui_mainusersworkersinventoryadd.h"

MainUsersWorkersInventoryAdd::MainUsersWorkersInventoryAdd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersInventoryAdd)
{
    ui->setupUi(this);

    ui->dtDate->setDateTime(QDateTime::currentDateTime());

    QSqlQuery query("SELECT id_sklad, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = product_sklad) AS product FROM \"Warehouse\" ORDER BY id_sklad;");
    ui->cbProduct->clear();
    ui->cbProduct->setPlaceholderText("Выберите продукт");

    while (query.next()) {
        QString idSklad = query.value(0).toString();
        QString product = query.value(1).toString();
        ui->cbProduct->addItem(product);
        ui->cbProduct->setItemData(ui->cbProduct->count() - 1, idSklad);  // Сохраняем id_sklad в дополнительных данных элемента комбобокса
    }

    connect(ui->cbProduct, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
}

void MainUsersWorkersInventoryAdd::onComboBoxIndexChanged(int index)
{
    QString idSklad = ui->cbProduct->itemData(index).toString();  // Получаем id_sklad из дополнительных данных элемента комбобокса

    QSqlQuery query;
    query.prepare("SELECT count_sklad, location_sklad  FROM \"Warehouse\" WHERE id_sklad = :selectedValue");
    query.bindValue(":selectedValue", idSklad);
    idSkladProduct = ui->cbProduct->itemData(index).toString();
    if (query.exec() && query.next()) {
        ui->txtCount->setText(query.value(0).toString());
        ui->txtLocation->setText(query.value(1).toString());
    }
        else {
        qDebug() << "Ошибка выполнения запроса" << query.lastError().text();
            return;
    }
}

MainUsersWorkersInventoryAdd::~MainUsersWorkersInventoryAdd()
{
    delete ui;
}

void MainUsersWorkersInventoryAdd::recieveID(QString ID)
{
    m_idusers = ID;
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT fullname_users from \"Users\" WHERE id_users = ?");
    query->bindValue(0,ID);
    query->exec();
    if (query->next()) {
            ui->txtResponsible->setText(query->value(0).toString());
            ui->txtResponsible->setEnabled(0);
            ui->txtChanger->setText(query->value(0).toString());
            ui->txtChanger->setEnabled(0);
    }
}
void MainUsersWorkersInventoryAdd::on_pbAdd_clicked()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dtDate->setDateTime(currentDateTime);

    QSqlQuery* query = new QSqlQuery;
    query->prepare("INSERT INTO \"Inventory\"(product_inventory, location_inventory, data_inventory, count_inventory, newcount_inventory,"
                   " responsible_inventory, changer_inventory, explanations_inventory) "
                   "VALUES (:product,:location, :data, :count, :newcount, :responsible, :changer, :explanations)");
    query->bindValue(":product", idSkladProduct);
    query->bindValue(":location", ui->txtLocation->text());
    query->bindValue(":data", currentDateTime);
    query->bindValue(":count", ui->txtCount->text().toLongLong());
    query->bindValue(":newcount", ui->txtNewCount->text().toLongLong());
    query->bindValue(":responsible", m_idusers.toLongLong());
    query->bindValue(":changer", m_idusers.toLongLong());
    query->bindValue(":explanations", ui->txtExplanations->text());
    QMessageBox* mess = new QMessageBox;
    if (!query->exec())
    {
            qDebug() << query->lastError().text();
            mess->setText("Запрос составлен неверно!");
            mess->show();
            return;
    }
    query->prepare("UPDATE  \"Warehouse\" SET count_sklad=? WHERE id_sklad = ?");
    query->bindValue(0,ui->txtNewCount->text().toLongLong());
    query->bindValue(1,idSkladProduct);

    if (!query->exec())
    {
            qDebug() << query->lastError().text();
            mess->setText("Запрос UPDATE составлен неверно!");
            mess->show();
            return;
    }
    // Остальной код, который нужно выполнить после успешного выполнения запросов
    emit refresh_table();
    this->close();
}


void MainUsersWorkersInventoryAdd::on_pbClear_clicked()
{
    ui->cbProduct->setCurrentIndex(0);
    ui->txtNewCount->setText("");
    ui->txtExplanations->setText("");
}


void MainUsersWorkersInventoryAdd::on_pbClose_clicked()
{
    this->close();
}

