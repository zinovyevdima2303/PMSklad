#include "mainusersworkersinventory.h"
#include "ui_mainusersworkersinventory.h"


MainUsersWorkersInventory::MainUsersWorkersInventory(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUsersWorkersInventory)
{
    ui->setupUi(this);
    on_btnRefresh_clicked();
}

MainUsersWorkersInventory::~MainUsersWorkersInventory()
{
    delete ui;
}

void MainUsersWorkersInventory::recieveID(QString ID)
{
    m_idusers = ID;
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT fullname_users, post_users from \"Users\" WHERE id_users = ?");
    query->bindValue(0,ID);
    query->exec();
    if (query->next()) {
        ui->lblUser->setText(query->value(0).toString());
        FIO = query->value(0).toString();

        ui->lblPost->setText(query->value(1).toString());
    }
    if(query->value(1).toString() == "Кладовщик"){
        ui->pbAdd->show();
        ui->pbEdit->hide();
    }
    if(query->value(1).toString() == "Ст. кладовщик"){
        ui->pbAdd->hide();
        ui->pbEdit->show();
    }
}

void MainUsersWorkersInventory::on_btnRefresh_clicked()
{
    TBSI = new QSqlQueryModel;
    TBSI -> setQuery("SELECT id_inventory ,(SELECT  product_receipt FROM \"Receipt_documents\" WHERE id_receipt = "
                   "(SELECT  product_sklad FROM \"Warehouse\" WHERE  id_sklad = product_inventory)),location_inventory , "
                   "data_inventory , count_inventory , newcount_inventory, (SELECT  fullname_users FROM \"Users\" WHERE responsible_inventory = id_users ),"
                   "(SELECT  fullname_users FROM \"Users\" WHERE changer_inventory = id_users ), explanations_inventory FROM \"Inventory\" ORDER BY id_inventory ;");

    TBSI ->setHeaderData(0,Qt::Horizontal,"Номер п/п");
    TBSI ->setHeaderData(1,Qt::Horizontal,"Название");
    TBSI ->setHeaderData(2,Qt::Horizontal,"Место хранения");
    TBSI ->setHeaderData(3,Qt::Horizontal,"Дата");
    TBSI ->setHeaderData(4,Qt::Horizontal,"Количество");
    TBSI ->setHeaderData(5,Qt::Horizontal,"Новое количество");
    TBSI ->setHeaderData(6,Qt::Horizontal,"Ответсветнный");
    TBSI ->setHeaderData(7,Qt::Horizontal,"Редактор");
    TBSI ->setHeaderData(8,Qt::Horizontal,"Причина");


    ui->tbSklad->setModel(TBSI);
    ui->tbSklad->resizeColumnsToContents();
    ui->tbSklad->show();
}

void MainUsersWorkersInventory::on_btnClose_clicked()
{
    this->close();
    emit refresh_table();
    emit MainWorkersInventory();
}

void MainUsersWorkersInventory::obr_ref_tab()
{
    on_btnRefresh_clicked();
}

void MainUsersWorkersInventory::on_pbAdd_clicked()
{
    MUWIAdd = new MainUsersWorkersInventoryAdd;
    connect(this, SIGNAL(sendID(QString)), MUWIAdd, SLOT(recieveID(QString)));
    emit sendID(m_idusers);

    connect(MUWIAdd,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUWIAdd->show();
}


void MainUsersWorkersInventory::on_pbEdit_clicked()
{
    MUWIM = new MainUsersWorkersInventoryModify;
    connect(this, SIGNAL(sendID(QString)), MUWIM, SLOT(recieveID(QString)));
    emit sendID(m_idusers);

    QModelIndex current = ui->tbSklad->currentIndex();
    int idNakl = current.sibling(current.row(), 0).data ().toInt ();
    connect(this,SIGNAL(sendIDNakl(int)),MUWIM,SLOT(obr_sendIDNakl(int)));
    emit sendIDNakl(idNakl);

    disconnect(MUWIM,SIGNAL(sendIDNakl(int)),this,SLOT(obr_sendIDNakl()));
    connect(MUWIM,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUWIM->show();
}

void MainUsersWorkersInventory::on_pbPrint_clicked()
{
    QModelIndex current = ui->tbSklad->currentIndex();
    int id = current.sibling(current.row(), 0).data ().toInt ();
    QString id_inventory, product_inventory, location_inventory, data_inventory, count_inventory, newcount_inventory, responsible_inventory, changer_inventory, explanations_inventory;

    QSqlQuery query;
    query.prepare("SELECT id_inventory ,(SELECT  product_receipt FROM \"Receipt_documents\" WHERE id_receipt = "
                  "(SELECT  product_sklad FROM \"Warehouse\" WHERE  id_sklad = product_inventory)),location_inventory , "
                  "data_inventory , count_inventory , newcount_inventory, (SELECT  fullname_users FROM \"Users\" WHERE responsible_inventory = id_users ),"
                  "(SELECT  fullname_users FROM \"Users\" WHERE changer_inventory = id_users ), explanations_inventory FROM \"Inventory\" WHERE (id_inventory = "+QString::number(id)+")");
    if (query.exec()) {
        if (query.next()) {
            id_inventory = query.value(0).toString();
            product_inventory = query.value(1).toString();
            location_inventory = query.value(2).toString();
            data_inventory = query.value(3).toString();
            count_inventory = query.value(4).toString();
            newcount_inventory = query.value(5).toString();
            responsible_inventory = query.value(6).toString();
            changer_inventory = query.value(7).toString();
            explanations_inventory = query.value(8).toString();

        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных");
        qDebug() << query.lastError().text();

        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как", "Отчет об ивентаризации", "PDF Files (*.pdf)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::warning(this, "Ошибка", "Не удалось создать файл");
            return;
        }
        // Формируем приходную накладную с помощью QTextDocument
        QTextDocument doc;
        QFont font;
        font.setPointSize(200);
        font.setBold(true);  // Устанавливаем жирный стиль
        font.setFamily("Arial");  // Устанавливаем семейство шрифта
        doc.setDefaultFont(font);
        QString html = "<html><body>";
        html += "<h1 align='center' style='font-size: 400px;'>Накладная</h1>";
        html += "<p class='bold'>Мебельное предприятие</p>";
        html += "<p><span class='bold'>Адрес:</span> г. Балаково, ул. Транспортная, дом 123</p>";
        html += "<p><span class='bold'>Телефон:</span> +7 927-147-9606</p>";
        html += "<h2 align='center' style='font-size: 300px;'>Ивентаризация</h2>";
        html += "<table>";
        html += "<tr><td style='font-size: 250px;'>Номер п/п:</td><td style='font-size: 250px;'>" + id_inventory + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Название:</td><td style='font-size: 250px;'>" + product_inventory + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Место хранения:</td><td style='font-size: 250px;'>" + location_inventory + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Дата:</td><td style='font-size: 250px;'>" + data_inventory + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Количество:</td><td style='font-size: 250px;'>" + count_inventory + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Новое количество:</td><td style='font-size: 250px;'>" + newcount_inventory + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Ответственный:</td><td style='font-size: 250px;'>" + responsible_inventory + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Редактор:</td><td style='font-size: 250px;'>" + changer_inventory + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Причина:</td><td style='font-size: 250px;'>" + explanations_inventory + "</td></tr>";
        html += "</table>";
        html += "<br> ";
        html += "<br> ";
        html += "<table>";
        html += "<tr><td style='font-size: 250px;'>Подпись _________________:</td><td style='font-size: 250px;'>" + FIO + "</td></tr>";
        html += "</table>";
        html += "</body></html>";
        doc.setHtml(html);


        QPdfWriter writer(fileName);
        writer.setPageSize(QPageSize::A4);
        doc.setPageSize(QSizeF(writer.width(), writer.height()));
        doc.print(&writer);
    }
}

void MainUsersWorkersInventory::on_pbPrintFilter_clicked()
{
    MUWIP = new MainUsersWorkersInventoryPrint;
    connect(this, SIGNAL(sendID(QString)), MUWIP, SLOT(recieveID(QString)));
    emit sendID(m_idusers);
    MUWIP->show();
}

