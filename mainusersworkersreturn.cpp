#include "mainusersworkersreturn.h"
#include "ui_mainusersworkersreturn.h"

MainUsersWorkersReturn::MainUsersWorkersReturn(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUsersWorkersReturn)
{
    ui->setupUi(this);
    on_btnRefresh_clicked();
}

MainUsersWorkersReturn::~MainUsersWorkersReturn()
{
    delete ui;
}

void MainUsersWorkersReturn::recieveID(QString ID)
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

void MainUsersWorkersReturn::on_btnRefresh_clicked()
{
    TBSR = new QSqlQueryModel;
    TBSR -> setQuery("SELECT id_return,(SELECT  product_receipt FROM \"Receipt_documents\" WHERE id_receipt = (SELECT product_sklad FROM \"Warehouse\" WHERE id_sklad = product_return)),"
    "data_return, count_return, location_return, (SELECT  fullname_users FROM \"Users\" WHERE responsible_return = id_users ),"
    "(SELECT  fullname_users FROM \"Users\" WHERE changer_return = id_users ), explanations_return FROM \"Return_documents\" ORDER BY id_return;");
    TBSR ->setHeaderData(0,Qt::Horizontal,"Номер п/п");
    TBSR ->setHeaderData(1,Qt::Horizontal,"Название");
    TBSR ->setHeaderData(2,Qt::Horizontal,"Дата");
    TBSR ->setHeaderData(3,Qt::Horizontal,"Колличесвто");
    TBSR ->setHeaderData(4,Qt::Horizontal,"Место хранения");
    TBSR ->setHeaderData(5,Qt::Horizontal,"Ответсветнный");
    TBSR ->setHeaderData(6,Qt::Horizontal,"Редактор");
    TBSR ->setHeaderData(7,Qt::Horizontal,"Причина");


    ui->tbSklad->setModel(TBSR);
    ui->tbSklad->resizeColumnsToContents();
    ui->tbSklad->show();
}

void MainUsersWorkersReturn::on_btnClose_clicked()
{
    this->close();
    emit refresh_table();
    emit MainWorkersReturn();
}

void MainUsersWorkersReturn::obr_ref_tab()
{
    on_btnRefresh_clicked();
}

void MainUsersWorkersReturn::on_pbAdd_clicked()
{
    MUWRAdd = new MainUsersWorkersReturnAdd;
    connect(this, SIGNAL(sendID(QString)), MUWRAdd, SLOT(recieveID(QString)));
    emit sendID(m_idusers);

    connect(MUWRAdd,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUWRAdd->show();
}


void MainUsersWorkersReturn::on_pbEdit_clicked()
{
    MUWRM = new MainUsersWorkersReturnModify;
    connect(this, SIGNAL(sendID(QString)), MUWRM, SLOT(recieveID(QString)));
    emit sendID(m_idusers);

    QModelIndex current = ui->tbSklad->currentIndex();
    int idNakl = current.sibling(current.row(), 0).data ().toInt ();
    connect(this,SIGNAL(sendIDNakl(int)),MUWRM,SLOT(obr_sendIDNakl(int)));
    emit sendIDNakl(idNakl);

    disconnect(MUWRM,SIGNAL(sendIDNakl(int)),this,SLOT(obr_sendIDNakl()));
    connect(MUWRM,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUWRM->show();
}

void MainUsersWorkersReturn::on_pbPrint_clicked()
{
    QModelIndex current = ui->tbSklad->currentIndex();
    int id = current.sibling(current.row(), 0).data ().toInt ();
    QString id_return, product_return, data_return, count_return, location_return, responsible_return, changer_return, explanations_return;

    QSqlQuery query;
    query.prepare("SELECT id_return,(SELECT  product_receipt FROM \"Receipt_documents\" WHERE id_receipt = (SELECT product_sklad FROM \"Warehouse\" WHERE id_sklad = product_return)),"
                  "data_return, count_return, location_return, (SELECT  fullname_users FROM \"Users\" WHERE responsible_return = id_users ),"
                  "(SELECT  fullname_users FROM \"Users\" WHERE changer_return = id_users ), explanations_return FROM \"Return_documents\" WHERE (id_return = "+QString::number(id)+")");
    if (query.exec()) {
        if (query.next()) {
            id_return = query.value(0).toString();
            product_return = query.value(1).toString();
            data_return = query.value(2).toString();
            count_return = query.value(3).toString();
            location_return = query.value(4).toString();
            responsible_return = query.value(5).toString();
            changer_return = query.value(6).toString();
            explanations_return = query.value(7).toString();

        }
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных");
        qDebug() << query.lastError().text();

        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как", "Отчет выходных документах", "PDF Files (*.pdf)");
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
        html += "<h2 align='center' style='font-size: 300px;'>Выходные документы</h2>";
        html += "<table>";
        html += "<tr><td style='font-size: 250px;'>Номер п/п:</td><td style='font-size: 250px;'>" + id_return + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Название:</td><td style='font-size: 250px;'>" + product_return + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Дата:</td><td style='font-size: 250px;'>" + data_return + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Колличество:</td><td style='font-size: 250px;'>" + count_return + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Место хранения::</td><td style='font-size: 250px;'>" + location_return + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Ответственный:</td><td style='font-size: 250px;'>" + responsible_return + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Редактор:</td><td style='font-size: 250px;'>" + changer_return + "</td></tr>";
        html += "<tr><td style='font-size: 250px;'>Причина:</td><td style='font-size: 250px;'>" + explanations_return + "</td></tr>";
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
