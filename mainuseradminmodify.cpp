#include "mainuseradminmodify.h"
#include "qsqlerror.h"
#include "ui_mainuseradminmodify.h"
#include <QCryptographicHash>

mainuseradminmodify::mainuseradminmodify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainuseradminmodify)
{
    ui->setupUi(this);

    ui->txtLogin->setPlaceholderText("Введите логин не меньше 6 символов ");
    ui->txtPass->setPlaceholderText("Введите пароль де меньше 8 символов и 1 латинской строчной буквы");
    ui->txtFullname->setPlaceholderText("Введите ФИО пример Иванов В.В.");
    ui->bxPost->setPlaceholderText("Выберите должность");
    ui->txtNumber->setPlaceholderText("Введите номер телефона в формате +7 (XXX) XXX-XXXX");
}

mainuseradminmodify::~mainuseradminmodify()
{
    delete ui;
}

void mainuseradminmodify::obr_sendID(int id)
{
    QSqlQuery* query = new QSqlQuery();
    query->prepare("SELECT login_users, pass_users, fullname_users, post_users, number_users from \"Users\" WHERE id_users = ?");
    query->bindValue(0,id);

    if(query->exec())
    {
        query->next();
        ui->txtpp->setText(QString::number(id));
        ui->txtLogin->setText(query->value(0).toString());
        ui->txtPass->setText(query->value(1).toString());
        ui->txtFullname->setText(query->value(2).toString());
        if(query->value(3).toString() == "Рабочий") {
            ui->bxPost->setCurrentIndex(0);
        }
        if(query->value(3).toString() == "Кладовщик") {
            ui->bxPost->setCurrentIndex(1);
        }

        if(query->value(3).toString() == "Ст. кладовщик") {
            ui->bxPost->setCurrentIndex(2);
        }
        if(query->value(3).toString() == "Администратор") {
            ui->bxPost->setCurrentIndex(3);
            ui->bxPost->setDisabled(1);
        }
        ui->txtNumber->setText(query->value(4).toString());
    }
}

void mainuseradminmodify::on_btnEdit_clicked()
{
    QString login, pass1, pass2, phone;

    login = ui->txtLogin->text();
    pass1 = ui->txtPass->text();
    phone = ui->txtNumber->text();

    // Определяем правила для валидации номера телефона
    QRegularExpression phoneRegex("^\\+7 \\(\\d{3}\\) \\d{3}-\\d{4}$|^8 \\(\\d{3}\\) \\d{3}-\\d{4}$");
        if (login.length() < 6)
        {
            // Если логин содержит менее 6 символов, то выводим сообщение об ошибке
            QMessageBox::critical(this, "Ошибка", "Логин должен содержать не менее 6 символов.");
            return;
        }
        else
        {
            if (pass1.length() < 8) {  // вывод сообщения об ошибке: "Пароль должен содержать не менее 8 символов"
                QMessageBox::critical(this, "Ошибка", "Пароль должен содержать не менее 8 символов.");
                return;
            } else if (!pass1.contains(QRegularExpression("[a-z]"))) { // вывод сообщения об ошибке: "Пароль должен содержать как минимум одну строчную букву"
                QMessageBox::critical(this, "Ошибка", "Пароль должен содержать как минимум одну строчную букву.");
                return;
            } else { // пароль прошел валидацию
                QString fullName = ui->txtFullname->text();
                QRegularExpression regex("^[А-ЯЁ][а-яё]+ [А-ЯЁ]\\.[А-ЯЁ]\\.( [А-ЯЁ]\\.[А-ЯЁ]\\.)?$");
                QRegularExpressionMatch match = regex.match(fullName);
                if (match.hasMatch()) {
                    if(ui->bxPost->currentText()=="Администратор"){
                        QMessageBox::critical(this, "Ошибка", "Администратор уже есть!");
                        return;
                    }
                    QSqlQuery* query = new QSqlQuery();
                    query->prepare("UPDATE  \"Users\" SET login_users=?, pass_users=?, fullname_users=?, post_users=?, number_users=? WHERE id_users = ?");
                    query->bindValue(0,ui->txtLogin->text());
                    query->bindValue(":pass",(QCryptographicHash::hash(pass1.toUtf8(), QCryptographicHash::Sha256))); //ui->txtPass->text())); //
                    query->bindValue(2,ui->txtFullname->text());
                    query->bindValue(3,ui->bxPost->currentText());
                    query->bindValue(4,ui->txtNumber->text());
                    query->bindValue(5,ui->txtpp->text().toInt());

                    if(query->exec())
                    {
                        emit refresh_table();
                        this->close();
                    }
                } else {
                    QMessageBox::critical(this, "Ошибка", "Некорректное поле ФИО.");
                    return;
                }
            }
        }
    }



void mainuseradminmodify::on_btnClose_clicked()
{
    this->close();
}


