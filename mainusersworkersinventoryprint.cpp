#include "mainusersworkersinventoryprint.h"
#include "ui_mainusersworkersinventoryprint.h"

MainUsersWorkersInventoryPrint::MainUsersWorkersInventoryPrint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUsersWorkersInventoryPrint)
{
    ui->setupUi(this);
    ui->cbFilter->setPlaceholderText("Выберите фильтр");
    ui->cbParm->setDisabled(1);
    connect(ui->cbFilter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainUsersWorkersInventoryPrint::onFilterChanged);

}

void MainUsersWorkersInventoryPrint::onFilterChanged(int index)
{
    ui->cbParm->setEnabled(1);
    if (index == 0)
    {
        QMap<int, int> indexToNumberMap;
        int currentIndex = 0;

        QSqlQuery query("SELECT DISTINCT (SELECT  product_receipt FROM \"Receipt_documents\" WHERE id_receipt = "
                        "(SELECT  product_sklad FROM \"Warehouse\" WHERE  id_sklad = product_inventory)),product_inventory FROM \"Inventory\";");
        ui->cbParm->clear();
        ui->cbParm->setPlaceholderText("Выберите параметр");

        while (query.next()) {
            QString product = query.value(0).toString();
            int productID = query.value(1).toInt();
            ui->cbParm->addItem(product);
            indexToNumberMap[currentIndex] = productID;
            currentIndex++;
        }
        this->indexToNumberMap = indexToNumberMap;

    }
    else if (index == 1)
    {
        QSqlQuery query("SELECT DISTINCT location_inventory FROM \"Inventory\";");
        ui->cbParm->clear();
        ui->cbParm->setPlaceholderText("Выберите параметр");

        while (query.next()) {
            QString product = query.value(0).toString();
            ui->cbParm->addItem(product);
        }
    }
    else if (index == 2)
    {
        QMap<int, int> indexToNumberMap;
        int currentIndex = 0;

        QSqlQuery query("SELECT DISTINCT (SELECT  fullname_users FROM \"Users\" WHERE responsible_inventory = id_users ),responsible_inventory  FROM \"Inventory\";");
        ui->cbParm->clear();
        ui->cbParm->setPlaceholderText("Выберите параметр");

        while (query.next()) {
            QString product = query.value(0).toString();
            int productID = query.value(1).toInt();
            ui->cbParm->addItem(product);
            indexToNumberMap[currentIndex] = productID;
            currentIndex++;
        }
        this->indexToNumberMap = indexToNumberMap;
    }
    else if (index == 3)
    {
        QMap<int, int> indexToNumberMap;
        int currentIndex = 0;

        QSqlQuery query("SELECT DISTINCT (SELECT  fullname_users FROM \"Users\" WHERE changer_inventory = id_users ),changer_inventory FROM \"Inventory\";");
        ui->cbParm->clear();
        ui->cbParm->setPlaceholderText("Выберите параметр");

        while (query.next()) {
            QString product = query.value(0).toString();
            int productID = query.value(1).toInt();
            ui->cbParm->addItem(product);
            indexToNumberMap[currentIndex] = productID;
            currentIndex++;
        }
        this->indexToNumberMap = indexToNumberMap;
    }
}

MainUsersWorkersInventoryPrint::~MainUsersWorkersInventoryPrint()
{
    delete ui;
}

void MainUsersWorkersInventoryPrint::recieveID(QString ID)
{
    m_idusers = ID;
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT fullname_users, post_users from \"Users\" WHERE id_users = ?");
    query->bindValue(0,ID);
    query->exec();
    if (query->next()) {
        FIO = query->value(0).toString();
        POST = query->value(1).toString();
    }
}

void MainUsersWorkersInventoryPrint::on_pbPrint_clicked()
{
    QSqlQuery query;

    if (ui->cbFilter->currentIndex() == 0)
    {
        Status = "Продукт " + ui->cbParm->currentText();
        int selectedIdx = ui->cbParm->currentIndex();
        int selectedNumber = indexToNumberMap.value(selectedIdx);

        query.prepare("SELECT id_inventory, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = "
                      "(SELECT product_sklad FROM \"Warehouse\" WHERE id_sklad = product_inventory)), location_inventory, "
                      "data_inventory, count_inventory, newcount_inventory, (SELECT fullname_users FROM \"Users\" WHERE responsible_inventory = id_users), "
                      "(SELECT fullname_users FROM \"Users\" WHERE changer_inventory = id_users), explanations_inventory FROM \"Inventory\" WHERE product_inventory = :product");
        query.bindValue(":product", selectedNumber);
    }
    else if (ui->cbFilter->currentIndex() == 1)
    {
        Status = "Место хранения "+ ui->cbParm->currentText();

        Location = ui->cbParm->currentText();  // Обновляем значение Location
        query.prepare("SELECT id_inventory, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = "
                      "(SELECT product_sklad FROM \"Warehouse\" WHERE id_sklad = product_inventory)), location_inventory, "
                      "data_inventory, count_inventory, newcount_inventory, (SELECT fullname_users FROM \"Users\" WHERE responsible_inventory = id_users), "
                      "(SELECT fullname_users FROM \"Users\" WHERE changer_inventory = id_users), explanations_inventory FROM \"Inventory\" WHERE location_inventory = :location");
        query.bindValue(":location", Location);
    }
    else if (ui->cbFilter->currentIndex() == 2)
    {
        Status = "Ответственный " + ui->cbParm->currentText();
        int selectedIdx = ui->cbParm->currentIndex();
        int selectedNumber = indexToNumberMap.value(selectedIdx);

        query.prepare("SELECT id_inventory, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = "
                      "(SELECT product_sklad FROM \"Warehouse\" WHERE id_sklad = product_inventory)), location_inventory, "
                      "data_inventory, count_inventory, newcount_inventory, (SELECT fullname_users FROM \"Users\" WHERE responsible_inventory = id_users), "
                      "(SELECT fullname_users FROM \"Users\" WHERE changer_inventory = id_users), explanations_inventory FROM \"Inventory\" WHERE responsible_inventory = :responsible");
        query.bindValue(":responsible", selectedNumber);
    }
    else if (ui->cbFilter->currentIndex() == 3)
    {
        Status = "Редактор " + ui->cbParm->currentText();

        int selectedIdx = ui->cbParm->currentIndex();
        int selectedNumber = indexToNumberMap.value(selectedIdx);

        query.prepare("SELECT id_inventory, (SELECT product_receipt FROM \"Receipt_documents\" WHERE id_receipt = "
                      "(SELECT product_sklad FROM \"Warehouse\" WHERE id_sklad = product_inventory)), location_inventory, "
                      "data_inventory, count_inventory, newcount_inventory, (SELECT fullname_users FROM \"Users\" WHERE responsible_inventory = id_users), "
                      "(SELECT fullname_users FROM \"Users\" WHERE changer_inventory = id_users), explanations_inventory FROM \"Inventory\" WHERE changer_inventory = :changer");
        query.bindValue(":changer", selectedNumber);
    }
    if (query.exec())
    {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как", "Отчет об инвентаризации", "PDF Files (*.pdf)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly))
        {
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
        html += "<h2 align='center' style='font-size: 300px;'>Инвентаризация с фильтром:" + Status + "</h2>";
        html += "<br> ";
        html += "<br> ";
        html += "<br> ";
        html += "<br> ";
        html += "<table>";
        html += "<tr>";
        html += "<td style='font-size: 150px;'>Номер п/п:</td>";
        html += "<td style='font-size: 150px;'>Название:</td>";
        html += "<td style='font-size: 150px;'>Место хранения:</td>";
        html += "<td style='font-size: 150px;'>Дата:</td>";
        html += "<td style='font-size: 150px;'>Количество:</td>";
        html += "<td style='font-size: 150px;'>Новое количество:</td>";
        html += "<td style='font-size: 150px;'>Ответственный:</td>";
        html += "<td style='font-size: 150px;'>Редактор:</td>";
        html += "<td style='font-size: 150px;'>Причина:</td>";
        html += "</tr>";
        while (query.next())
        {
            QString  id_inventory = query.value(0).toString();
            QString  product_inventory = query.value(1).toString();
            QString  location_inventory = query.value(2).toString();
            QString  data_inventory = query.value(3).toString();
            QString  count_inventory = query.value(4).toString();
            QString  newcount_inventory = query.value(5).toString();
            QString   responsible_inventory = query.value(6).toString();
            QString  changer_inventory = query.value(7).toString();
            QString  explanations_inventory = query.value(8).toString();

            html += "<tr>";
            html += "<td style='font-size: 150px;'>" + id_inventory + "</td>";
            html += "<td style='font-size: 150px;'>" + product_inventory + "</td>";
            html += "<td style='font-size: 150px;'>" + location_inventory + "</td>";
            html += "<td style='font-size: 150px;'>" + data_inventory + "</td>";
            html += "<td style='font-size: 150px;'>" + count_inventory + "</td>";
            html += "<td style='font-size: 150px;'>" + newcount_inventory + "</td>";
            html += "<td style='font-size: 150px;'>" + responsible_inventory + "</td>";
            html += "<td style='font-size: 150px;'>" + changer_inventory + "</td>";
            html += "<td style='font-size: 150px;'>" + explanations_inventory + "</td>";
            html += "</tr>";
        }
        html += "</table>";
        html += "<br> ";
        html += "<br> ";
        html += "<table>";
        html += "<tr><td style='font-size: 250px;'>Подпись _________________:</td><td style='font-size: 250px;'>" + POST +"  "+ FIO + "</td></tr>";
        html += "</table>";
        html += "</body></html>";
        doc.setHtml(html);


        QPdfWriter writer(fileName);
        writer.setPageSize(QPageSize::A4);
        doc.setPageSize(QSizeF(writer.width(), writer.height()));
        doc.print(&writer);
    }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось выполнить запрос к базе данных");
            qDebug() << query.lastError().text();
            return;
        }
    }
}


void MainUsersWorkersInventoryPrint::on_pbClose_clicked()
{
    this->close();
}

