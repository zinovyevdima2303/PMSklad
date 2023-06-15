#include "mainusersworkersreceipt_documents.h"
#include "ui_mainusersworkersreceipt_documents.h"
#include <QTableWidget>
#include "qsqlerror.h"
#include "qsqlquery.h"

MainUsersWorkersReceipt_documents::MainUsersWorkersReceipt_documents(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUsersWorkersReceipt_documents)
{
    ui->setupUi(this);
    on_btnRefresh_clicked();
}

MainUsersWorkersReceipt_documents::~MainUsersWorkersReceipt_documents()
{
    delete ui;
}

void MainUsersWorkersReceipt_documents::recieveID(QString ID)
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

void MainUsersWorkersReceipt_documents::on_btnRefresh_clicked()
{
    TBSRd = new QSqlQueryModel;
    TBSRd -> setQuery("SELECT id_receipt , product_receipt, provaider_receipt, data_receipt, count_receipt, location_receipt, "
                    "(SELECT  fullname_users FROM \"Users\" WHERE responsible_receipt = id_users ) FROM \"Receipt_documents\" ORDER BY id_receipt;");

    TBSRd ->setHeaderData(0,Qt::Horizontal,"Номер п/п");
    TBSRd ->setHeaderData(1,Qt::Horizontal,"Название");
    TBSRd ->setHeaderData(2,Qt::Horizontal,"Поставщик");
    TBSRd ->setHeaderData(3,Qt::Horizontal,"Дата");
    TBSRd ->setHeaderData(4,Qt::Horizontal,"Колличество");
    TBSRd ->setHeaderData(5,Qt::Horizontal,"Место хранения");
    TBSRd ->setHeaderData(6,Qt::Horizontal,"Ответственный");

    ui->tbSklad->setModel(TBSRd);
    ui->tbSklad->resizeColumnsToContents();
    ui->tbSklad->show();
}


void MainUsersWorkersReceipt_documents::on_btnClose_clicked()
{
    this->close();
    emit refresh_table();
    emit MainWorkersReceipt_documents();
}

void MainUsersWorkersReceipt_documents::obr_ref_tab()
{
    on_btnRefresh_clicked();
}

void MainUsersWorkersReceipt_documents::on_pbAdd_clicked()
{
    MUWRdAdd = new MainUsersWorkersReceipt_documentsAdd;
    connect(this, SIGNAL(sendID(QString)), MUWRdAdd, SLOT(recieveID(QString)));
    emit sendID(m_idusers);

    connect(MUWRdAdd,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUWRdAdd->show();
}


void MainUsersWorkersReceipt_documents::on_pbEdit_clicked()
{
    MUWRdM = new MainUsersWorkersReceipt_documentsModify;

    QModelIndex current = ui->tbSklad->currentIndex();
    int idNakl = current.sibling(current.row(), 0).data ().toInt ();
    connect(this,SIGNAL(sendIDNakl(int)),MUWRdM,SLOT(obr_sendIDNakl(int)));
    emit sendIDNakl(idNakl);

    disconnect(MUWRdM,SIGNAL(sendIDNakl(int)),this,SLOT(obr_sendIDNakl()));
    connect(MUWRdM,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUWRdM->show();
}



void MainUsersWorkersReceipt_documents::on_pbPrint_clicked()
{
    QModelIndex current = ui->tbSklad->currentIndex();
    int id = current.sibling(current.row(), 0).data ().toInt ();
    QString nomer, naimenovanie, postavshchik, data, kolichestvo, mesto, otvetstvennyi;

    QSqlQuery query;
    query.prepare("SELECT id_receipt, product_receipt, provaider_receipt, data_receipt, count_receipt, location_receipt, "
                  "(SELECT fullname_users FROM \"Users\" WHERE id_users = responsible_receipt) "
                  "FROM \"Receipt_documents\" WHERE (id_receipt = "+QString::number(id)+")");
    if (query.exec()) {
        if (query.next()) {
            nomer = query.value(0).toString();
            naimenovanie = query.value(1).toString();
            postavshchik = query.value(2).toString();
            data = query.value(3).toString();
            kolichestvo = query.value(4).toString();
            mesto = query.value(5).toString();
            otvetstvennyi = query.value(6).toString();
        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных");
        qDebug() << query.lastError().text();

        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как", "Отчет о приходных накладных", "PDF Files (*.pdf)");
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
        html += "<h2 align='center' style='font-size: 300px;'>Приходная накладная</h2>";
        html += "<table>";
        html += "<tr><td style='font-size: 250px;'>Номер п/п:</td><td style='font-size: 250px;'>" + nomer + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Название:</td><td style='font-size: 250px;'>" + naimenovanie + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Поставщик:</td><td style='font-size: 250px;'>" + postavshchik + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Дата:</td><td style='font-size: 250px;'>" + data + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Количество:</td><td style='font-size: 250px;'>" + kolichestvo + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Место хранения:</td><td style='font-size: 250px;'>" + mesto + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Ответственный:</td><td style='font-size: 250px;'>" + otvetstvennyi + "</td></tr>";
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



