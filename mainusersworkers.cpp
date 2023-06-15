#include "mainusersworkers.h"
#include "qsqlquery.h"
#include "ui_mainusersworkers.h"

MainUsersWorkers::MainUsersWorkers(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUsersWorkers)
{
    ui->setupUi(this);
    on_btnRefresh_2_clicked();
    MUWRd=NULL;
    MUWT=NULL;
    MUWR=NULL;
    MUWI=NULL;
}

MainUsersWorkers::~MainUsersWorkers()
{
    delete ui;
}

void MainUsersWorkers::recieveID(QString ID)
{
    m_idusers = ID;
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT fullname_users, post_users from \"Users\" WHERE id_users = ?");
    query->bindValue(0,ID);
    query->exec();
    if (query->next()) {
        ui->lblUser_2->setText(query->value(0).toString());
        FIO = query->value(0).toString();
        ui->lblPost_2->setText(query->value(1).toString());
    }
    if(query->value(1).toString() == "Рабочий"){
        ui->pbReceipt_documents->hide();
        ui->pbReturn_documents->hide();
        ui->pbTransfer_invoices->hide();
        ui->pbInventory->hide();
    }
    if(query->value(1).toString() == "Кладовщик"){
        ui->pbReceipt_documents->show();
        ui->pbReturn_documents->show();
        ui->pbTransfer_invoices->show();
        ui->pbInventory->show();
    }
    if(query->value(1).toString() == "Ст. кладовщик"){
        ui->pbReceipt_documents->show();
        ui->pbReturn_documents->show();
        ui->pbTransfer_invoices->show();
        ui->pbInventory->show();
    }
}

void MainUsersWorkers::obr_ref_tab()
{
    on_btnRefresh_2_clicked();
}

void MainUsersWorkers::on_btnRefresh_2_clicked()
{
    ui->txtLocation->setText("");
    ui->txtProduct->setText("");
    TBS = new QSqlQueryModel;
    TBS -> setQuery("SELECT id_sklad, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = product_sklad), count_sklad, location_sklad FROM \"Warehouse\" ORDER BY id_sklad;");

    TBS ->setHeaderData(0,Qt::Horizontal,"Номер п/п");
    TBS ->setHeaderData(1,Qt::Horizontal,"Название");
    TBS ->setHeaderData(2,Qt::Horizontal,"Колличество");
    TBS ->setHeaderData(3,Qt::Horizontal,"Место хранения");

    ui->tbSklad->setModel(TBS);
    ui->tbSklad->resizeColumnsToContents();
    ui->tbSklad->show();
}


void MainUsersWorkers::on_btnClose_2_clicked()
{
    this->close();
    emit MainWorkers();
}

void MainUsersWorkers::on_pbReceipt_documents_clicked()
{

    if (MUWRd==NULL) {
        MUWRd = new MainUsersWorkersReceipt_documents();
        connect(MUWRd,&MainUsersWorkersReceipt_documents::MainWorkersReceipt_documents,this,&MainUsersWorkers::show);
        connect(this, SIGNAL(sendID(QString)), MUWRd, SLOT(recieveID(QString)));
        connect(MUWRd,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));

    }
    emit sendID (m_idusers);
    MUWRd->show();
    this->close();
}

void MainUsersWorkers::on_pbReturn_documents_clicked()
{
    if (MUWR==NULL) {
        MUWR = new MainUsersWorkersReturn();
        connect(MUWR,&MainUsersWorkersReturn::MainWorkersReturn,this,&MainUsersWorkers::show);
        connect(this, SIGNAL(sendID(QString)), MUWR, SLOT(recieveID(QString)));
        connect(MUWR,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));

    }
    emit sendID (m_idusers);
    MUWR->show();
    this->close();
}

void MainUsersWorkers::on_pbTransfer_invoices_clicked()
{
    if (MUWT==NULL) {
        MUWT = new MainUsersWorkersTransfer();
        connect(MUWT,&MainUsersWorkersTransfer::MainWorkersTransfer,this,&MainUsersWorkers::show);
        connect(this, SIGNAL(sendID(QString)), MUWT, SLOT(recieveID(QString)));
        connect(MUWT,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));

    }
    emit sendID (m_idusers);
    MUWT->show();
    this->close();
}

void MainUsersWorkers::on_pbInventory_clicked()
{
    if (MUWI==NULL) {
        MUWI = new MainUsersWorkersInventory();
        connect(MUWI,&MainUsersWorkersInventory::MainWorkersInventory,this,&MainUsersWorkers::show);
        connect(this, SIGNAL(sendID(QString)), MUWI, SLOT(recieveID(QString)));
        connect(MUWI,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));

    }
    emit sendID (m_idusers);
    MUWI->show();
    this->close();
}

void MainUsersWorkers::on_pbPrint_clicked()
{
    QModelIndex current = ui->tbSklad->currentIndex();
    int id = current.sibling(current.row(), 0).data ().toInt ();
    QString id_sklad, product_sklad, count_sklad, location_sklad;

    QSqlQuery query;
    query.prepare("SELECT id_sklad, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = product_sklad),"
                  " count_sklad, location_sklad FROM \"Warehouse\" WHERE (id_sklad = "+QString::number(id)+")");
    if (query.exec()) {
        if (query.next()) {
            id_sklad = query.value(0).toString();
            product_sklad = query.value(1).toString();
            count_sklad = query.value(2).toString();
            location_sklad = query.value(3).toString();
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных");
        qDebug() << query.lastError().text();

        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как", "Отчет о товаре", "PDF Files (*.pdf)");
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
        html += "<h2 align='center' style='font-size: 300px;'>Сведения о товаре</h2>";
        html += "<table>";
        html += "<tr><td style='font-size: 250px;'>Номер п/п:</td><td style='font-size: 250px;'>" + id_sklad + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Название:</td><td style='font-size: 250px;'>" + product_sklad + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Колличество:</td><td style='font-size: 250px;'>" + count_sklad + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Место хранения:</td><td style='font-size: 250px;'>" + location_sklad + "</td></tr>";
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



void MainUsersWorkers::on_pbSearchLocation_clicked()
{
    ui->txtProduct->setText("");
    QString Loc = ui->txtLocation->text();
    if (Loc.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле места хранения пустое");
    } else {
    TBS = new QSqlQueryModel;
    TBS->setQuery("SELECT id_sklad, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = product_sklad), count_sklad, location_sklad FROM \"Warehouse\" WHERE (location_sklad = '" + Loc + "')");

    TBS->setHeaderData(0, Qt::Horizontal, "Номер п/п");
    TBS->setHeaderData(1, Qt::Horizontal, "Название");
    TBS->setHeaderData(2, Qt::Horizontal, "Количество");
    TBS->setHeaderData(3, Qt::Horizontal, "Место хранения");

    ui->tbSklad->setModel(TBS);
    ui->tbSklad->resizeColumnsToContents();
    ui->tbSklad->show();

}
}


void MainUsersWorkers::on_pbSearchProduct_clicked()
{
    ui->txtLocation->setText("");
    QString productName = ui->txtProduct->text();
    if (productName.isEmpty()) {
    QMessageBox::warning(this, "Ошибка", "Поле места хранения пустое");
    } else {

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(TBS);

    // Установка фильтра по колонке "Название"
    proxyModel->setFilterKeyColumn(1); // Индекс колонки "Название"
    proxyModel->setFilterFixedString(productName);

    ui->tbSklad->setModel(proxyModel);
    ui->tbSklad->resizeColumnsToContents();
    ui->tbSklad->show();

}
}

