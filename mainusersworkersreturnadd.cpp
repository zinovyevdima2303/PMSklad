#include "mainusersworkersreturnadd.h"
#include "ui_mainusersworkersreturnadd.h"

MainUsersWorkersReturnAdd::MainUsersWorkersReturnAdd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersReturnAdd)
{
    ui->setupUi(this);
    ui->dtDate->setDateTime(QDateTime::currentDateTime());

    QSqlQuery query("SELECT id_sklad, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = product_sklad) AS product, location_sklad FROM \"Warehouse\" ORDER BY id_sklad;");
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
void MainUsersWorkersReturnAdd::onComboBoxIndexChanged(int index)
{
    QString idSklad = ui->cbProduct->itemData(index).toString();  // Получаем id_sklad из дополнительных данных элемента комбобокса

    QSqlQuery query;
    query.prepare("SELECT location_sklad, count_sklad FROM \"Warehouse\" WHERE id_sklad = :selectedValue");
    query.bindValue(":selectedValue", idSklad);
    idSkladProduct = ui->cbProduct->itemData(index).toString();
    if (query.exec() && query.next()) {
        QString locationInfo = query.value(0).toString();
        ui->txtLocation->setText(locationInfo);
        QString countInfo = query.value(1).toString();
        ui->txtCount->setText(countInfo);
    }
    else {
        qDebug() << "Ошибка выполнения запроса" << query.lastError().text();
            return;
    }
}

void MainUsersWorkersReturnAdd::recieveID(QString ID)
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

MainUsersWorkersReturnAdd::~MainUsersWorkersReturnAdd()
{
    delete ui;
}

void MainUsersWorkersReturnAdd::on_pbAdd_clicked()
{
    QSqlQuery* query = new QSqlQuery;
    QMessageBox* mess = new QMessageBox;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    ui->dtDate->setDateTime(currentDateTime);

    QString countString = ui->txtCount->text();
    QString countReturnString = ui->txtCountReturn->text();

    bool conversionOk;
    qint64 countReturn = countReturnString.toLongLong(&conversionOk);
    if (!conversionOk) {
        QMessageBox::critical(this, "Ошибка", "Значение \"Количество на возврат\" пустое!");
        return;
    }

    qint64 count = countString.toLongLong(&conversionOk);
    qint64 CR = count - countReturn;
    if (countReturn > 0 && count > 0 && countReturn <= count) {

        query->prepare("INSERT INTO \"Return_documents\"(product_return, data_return, count_return, location_return, responsible_return, changer_return ,explanations_return) "
                       "VALUES (:product, :data, :count, :location, :responsible, :changer, :explanations)");
        query->bindValue(":product", idSkladProduct);
        query->bindValue(":data", ui->dtDate->dateTime());
        query->bindValue(":count", ui->txtCountReturn->text().toLongLong());
        query->bindValue(":location", ui->txtLocation->text());
        query->bindValue(":responsible", m_idusers.toLongLong());
        query->bindValue(":changer", m_idusers.toLongLong());
        query->bindValue(":explanations", ui->txtExplanations->text());
        if (!query->exec())
        {
            qDebug() << query->lastError().text();
            mess->setText("Запрос составлен неверно!");
            mess->show();
            return;
        }
    } else {
        QMessageBox::critical(this, "Ошибка", "Значение строки больше указанных значений или отрицательное.");
        return;
    }
    query->prepare("UPDATE  \"Warehouse\" SET count_sklad=? WHERE id_sklad = ?");
    query->bindValue(0,CR);
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


void MainUsersWorkersReturnAdd::on_pbClear_clicked()
{
    ui->cbProduct->setPlaceholderText("Выберите продукт");
    ui->txtCountReturn->setText("");
    ui->txtExplanations->setText("");
}


void MainUsersWorkersReturnAdd::on_pbClose_clicked()
{
    this->close();
}

