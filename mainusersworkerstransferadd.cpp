#include "mainusersworkerstransferadd.h"
#include "ui_mainusersworkerstransferadd.h"

MainUsersWorkersTransferAdd::MainUsersWorkersTransferAdd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersTransferAdd)
{
    ui->setupUi(this);
    ui->dtDate->setDateTime(QDateTime::currentDateTime());

    QSqlQuery query("SELECT id_sklad, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = product_sklad) AS product, location_sklad FROM \"Warehouse\" ORDER BY id_sklad;");
    ui->cbProduct->clear();
    ui->cbProduct->setPlaceholderText("Выберите товар");

    while (query.next()) {
        QString idSklad = query.value(0).toString();
        QString product = query.value(1).toString();
        QString location = query.value(2).toString();
        ui->cbProduct->addItem(product);
        ui->cbProduct->setItemData(ui->cbProduct->count() - 1, idSklad);  // Сохраняем id_sklad в дополнительных данных элемента комбобокса
    }

    connect(ui->cbProduct, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBoxIndexChanged(int)));
}

void MainUsersWorkersTransferAdd::onComboBoxIndexChanged(int index)
{
    QString idSklad = ui->cbProduct->itemData(index).toString();  // Получаем id_sklad из дополнительных данных элемента комбобокса

    QSqlQuery query;
    query.prepare("SELECT location_sklad FROM \"Warehouse\" WHERE id_sklad = :selectedValue");
    query.bindValue(":selectedValue", idSklad);
    idSkladProduct = ui->cbProduct->itemData(index).toString();
    if (query.exec() && query.next()) {
        QString productInfo = query.value(0).toString();
        ui->txtLocation->setText(productInfo);
    }
    else {
        qDebug() << "Ошибка выполнения запроса" << query.lastError().text();
            return;
    }
}

MainUsersWorkersTransferAdd::~MainUsersWorkersTransferAdd()
{
    delete ui;
}

void MainUsersWorkersTransferAdd::recieveID(QString ID)
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

void MainUsersWorkersTransferAdd::on_pbAdd_clicked()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dtDate->setDateTime(currentDateTime);

    QSqlQuery* query = new QSqlQuery;
    query->prepare("INSERT INTO \"Transfer_invoices\"(product_transfer, data_transfer, location_transfer, newlocation_transfer, responsible_transfer, changer_transfer ,explanations_transfer) "
                   "VALUES (:product, :data, :location, :newlocation, :responsible, :changer, :explanations)");
    query->bindValue(":product", idSkladProduct);
    query->bindValue(":data", ui->dtDate->dateTime());
    query->bindValue(":location", ui->txtLocation->text());
    query->bindValue(":newlocation", ui->txtNewLocation->text());
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
    query->prepare("UPDATE  \"Warehouse\" SET location_sklad=? WHERE id_sklad = ?");
    query->bindValue(0,ui->txtNewLocation->text());
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

void MainUsersWorkersTransferAdd::on_pbClear_clicked()
{
    ui->cbProduct->setPlaceholderText("Выберите продукт");
    ui->txtNewLocation->setText("");
    ui->txtExplanations->setText("");
}


void MainUsersWorkersTransferAdd::on_pbClose_clicked()
{
    this->close();
}

