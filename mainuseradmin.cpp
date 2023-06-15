#include "mainuseradmin.h"
#include "qcryptographichash.h"
#include "ui_mainuseradmin.h"

MainUserAdmin::MainUserAdmin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUserAdmin)
{
    ui->setupUi(this);
    on_btnRefresh_clicked();
}

MainUserAdmin::~MainUserAdmin()
{
    delete ui;  
}

void MainUserAdmin::recieveID(QString ID)
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT fullname_users, post_users from \"Users\" WHERE id_users = ?");
    query->bindValue(0,ID);
    query->exec();
    if (query->next()) {
        ui->lblUser->setText(query->value(0).toString());
        ui->lblPost->setText(query->value(1).toString());
    }
}

void MainUserAdmin::on_btnClose_clicked()
{
    this->close();
    emit MainAdmin();
}

class EncryptedSqlQueryModel : public QSqlQueryModel
{
public:
    QVariant data(const QModelIndex &index, int role) const override
    {
        // Если запрашивается столбец паролей (например, 2), то зашифровываем данные
        if (index.column() == 2 && role == Qt::DisplayRole)
        {
            QString password = QSqlQueryModel::data(index, role).toString();
            QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
            return passwordHash.toHex();
        }
        // Иначе просто возвращаем данные из модели
        return QSqlQueryModel::data(index, role);
    }
};

void MainUserAdmin::on_btnRefresh_clicked()
{
    EncryptedSqlQueryModel *model = new EncryptedSqlQueryModel;
    model->setQuery("SELECT id_users, login_users, pass_users, fullname_users, post_users, number_users FROM \"Users\" ORDER BY id_users");

    model->setHeaderData(0, Qt::Horizontal, "п/п");
    model->setHeaderData(1, Qt::Horizontal, "Логин");
    model->setHeaderData(2, Qt::Horizontal, "Пароль");
    model->setHeaderData(3, Qt::Horizontal, "ФИО");
    model->setHeaderData(4, Qt::Horizontal, "Роль");
    model->setHeaderData(5, Qt::Horizontal, "Телефон");

    ui->tbWorks->setModel(model);
    ui->tbWorks->setColumnWidth(0, 10);
    ui->tbWorks->setColumnWidth(1, 150);
    ui->tbWorks->setColumnWidth(2, 150);
    ui->tbWorks->setColumnWidth(3, 150);
    ui->tbWorks->setColumnWidth(4, 150);
    ui->tbWorks->setColumnWidth(5, 150);
    ui->tbWorks->show();
}

void MainUserAdmin::on_btnAdd_clicked()
{
    MUAAdd = new mainuseradminadd;
    connect(MUAAdd,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUAAdd->show();
}

void MainUserAdmin::obr_ref_tab()
{
    on_btnRefresh_clicked();
}

void MainUserAdmin::on_btnEdit_clicked()
{

    MUAM = new mainuseradminmodify;

    QModelIndex current = ui->tbWorks->currentIndex();
    int id = current.sibling(current.row(), 0).data ().toInt ();
    connect(this,SIGNAL(sendID(int)),MUAM,SLOT(obr_sendID(int)));
    emit sendID(id);

    disconnect(MUAM,SIGNAL(sendID(int)),this,SLOT(obr_sendID()));
    connect(MUAM,SIGNAL(refresh_table()),this,SLOT(obr_ref_tab()));
    MUAM->show();
}


void MainUserAdmin::on_btnDelite_clicked()
{
    QModelIndex current = ui->tbWorks->currentIndex();
    int id = current.sibling(current.row(), 0).data().toInt();
    QString role = current.sibling(current.row(), 4).data().toString();

    if (role == "Администратор") {
        QMessageBox::critical(this, "Ошибка", "Невозможно удалить пользователя с ролью 'Администратор'.");
        return;
    }

    int g;
    QSqlQuery* query = new QSqlQuery;
    query->prepare("DELETE from \"Users\" WHERE (id_users = "+QString::number(id)+")");
    g = QMessageBox::warning(0,"Внимание!", "Подтвердите удаление пользователя?", QMessageBox::Yes | QMessageBox::No);
        if(g == QMessageBox::Yes)
        {
        query->exec();
        on_btnRefresh_clicked();
        }
}

