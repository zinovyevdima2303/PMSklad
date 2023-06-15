#include "mainusersworkerstransfer.h"
#include "ui_mainusersworkerstransfer.h"

MainUsersWorkersTransfer::MainUsersWorkersTransfer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUsersWorkersTransfer)
{
    ui->setupUi(this);
    on_btnRefresh_clicked();

}

MainUsersWorkersTransfer::~MainUsersWorkersTransfer()
{
    delete ui;
}

void MainUsersWorkersTransfer::recieveID(QString ID)
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

void MainUsersWorkersTransfer::on_btnRefresh_clicked()
{
    TBST = new QSqlQueryModel;
    TBST -> setQuery("SELECT id_transfer, (SELECT  product_receipt FROM \"Receipt_documents\" WHERE  id_receipt = "
                     "(SELECT  product_sklad FROM \"Warehouse\" WHERE  id_sklad = product_transfer)),data_transfer,"
"location_transfer , newlocation_transfer , (SELECT  fullname_users FROM \"Users\" WHERE responsible_transfer = id_users ),"
"(SELECT  fullname_users FROM \"Users\" WHERE changer_transfer = id_users ), explanations_transfer FROM \"Transfer_invoices\" ORDER BY id_transfer ;");

    TBST ->setHeaderData(0,Qt::Horizontal,"Номер п/п");
    TBST ->setHeaderData(1,Qt::Horizontal,"Название");
    TBST ->setHeaderData(2,Qt::Horizontal,"Дата");
    TBST ->setHeaderData(3,Qt::Horizontal,"Место хранения");
    TBST ->setHeaderData(4,Qt::Horizontal,"Новое место хранения");
    TBST ->setHeaderData(5,Qt::Horizontal,"Ответственный");
    TBST ->setHeaderData(6,Qt::Horizontal,"Редактор");
    TBST ->setHeaderData(7,Qt::Horizontal,"Причина");


    ui->tbSklad->setModel(TBST);
    ui->tbSklad->resizeColumnsToContents();
    ui->tbSklad->show();
}

void MainUsersWorkersTransfer::on_btnClose_clicked()
{
    this->close();
    emit refresh_table();
    emit MainWorkersTransfer();
}

void MainUsersWorkersTransfer::obr_ref_tab()
{
    on_btnRefresh_clicked();
}

void MainUsersWorkersTransfer::on_pbAdd_clicked()
{
    MUWTAdd = new MainUsersWorkersTransferAdd;
    connect(this, SIGNAL(sendID(QString)), MUWTAdd, SLOT(recieveID(QString)));
    emit sendID(m_idusers);

    connect(MUWTAdd,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUWTAdd->show();
}


void MainUsersWorkersTransfer::on_pbEdit_clicked()
{
    MUWTM = new MainUsersWorkersTransferModify;
    connect(this, SIGNAL(sendID(QString)), MUWTM, SLOT(recieveID(QString)));
    emit sendID(m_idusers);

    QModelIndex current = ui->tbSklad->currentIndex();
    int idNakl = current.sibling(current.row(), 0).data ().toInt ();
    connect(this,SIGNAL(sendIDNakl(int)),MUWTM,SLOT(obr_sendIDNakl(int)));
    emit sendIDNakl(idNakl);
    disconnect(MUWTM,SIGNAL(sendIDNakl(int)),this,SLOT(obr_sendIDNakl()));

    connect(MUWTM,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUWTM->show();
}

void MainUsersWorkersTransfer::on_pbPrint_clicked()
{
    QModelIndex current = ui->tbSklad->currentIndex();
    int id = current.sibling(current.row(), 0).data ().toInt ();
    QString id_transfer, product_transfer, data_transfer, location_transfer, newlocation_transfer, responsible_transfer, changer_transfer, explanations_transfer;

    QSqlQuery query;
    query.prepare("SELECT id_transfer, (SELECT  product_receipt FROM \"Receipt_documents\" WHERE  id_receipt = "
                  "(SELECT  product_sklad FROM \"Warehouse\" WHERE  id_sklad = product_transfer)),data_transfer ,"
                  "location_transfer , newlocation_transfer , (SELECT  fullname_users FROM \"Users\" WHERE responsible_transfer = id_users ),"
                  "(SELECT  fullname_users FROM \"Users\" WHERE changer_transfer = id_users ), explanations_transfer FROM \"Transfer_invoices\" WHERE (id_transfer = "+QString::number(id)+")");
    if (query.exec()) {
        if (query.next()) {
            id_transfer = query.value(0).toString();
            product_transfer = query.value(1).toString();
            data_transfer = query.value(2).toString();
            location_transfer = query.value(3).toString();
            newlocation_transfer = query.value(4).toString();
            responsible_transfer = query.value(5).toString();
            changer_transfer = query.value(6).toString();
            explanations_transfer = query.value(7).toString();
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных");
        qDebug() << query.lastError().text();

        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как", "Отчет о перемещении", "PDF Files (*.pdf)");
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
        html += "<h2 align='center' style='font-size: 300px;'>Накладная о перемещении</h2>";
        html += "<table>";
        html += "<tr><td style='font-size: 250px;'>Номер п/п:</td><td style='font-size: 250px;'>" + id_transfer + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Название:</td><td style='font-size: 250px;'>" + product_transfer + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Дата:</td><td style='font-size: 250px;'>" + data_transfer + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Место хранения::</td><td style='font-size: 250px;'>" + location_transfer + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Новое место хранения::</td><td style='font-size: 250px;'>" + newlocation_transfer + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Ответственный:</td><td style='font-size: 250px;'>" + responsible_transfer + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Редактор:</td><td style='font-size: 250px;'>" + changer_transfer + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Причина:</td><td style='font-size: 250px;'>" + explanations_transfer + "</td></tr>";
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
