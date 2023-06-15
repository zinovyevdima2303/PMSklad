#include "mainuseradminadd.h"
#include "qsqlerror.h"
#include "ui_mainuseradminadd.h"
#include <QCryptographicHash>

mainuseradminadd::mainuseradminadd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainuseradminadd)
{
    ui->setupUi(this);

    ui->txtLogin->setPlaceholderText("Введите логин не меньше 6 символов ");
    ui->txtPass->setPlaceholderText("Введите пароль де меньше 8 символов и 1 латинской строчной буквы");
    ui->txtFullname->setPlaceholderText("Введите ФИО пример Иванов В.В.");
    ui->bxPost->setPlaceholderText("Выберите должность");
    ui->txtNumber->setPlaceholderText("Введите номер телефона в формате +7 (XXX) XXX-XXXX");
}

mainuseradminadd::~mainuseradminadd()
{
    delete ui;
}

// Реализация метода eventFilter для блокировки копирования и вставки текста
bool mainuseradminadd::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->txtPass || object == ui->txtPass_2)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->matches(QKeySequence::Copy) || keyEvent->matches(QKeySequence::Paste))
            {
                return true;
            }
        }
    }
    return false;
}

void mainuseradminadd::on_btnAdd_clicked()
{

    QString login, pass1, pass2, phone;

    login = ui->txtLogin->text();
    pass1 = ui->txtPass->text();
    pass2 = ui->txtPass_2->text();
    phone = ui->txtNumber->text();

    // Определяем правила для валидации номера телефона
    QRegularExpression phoneRegex("^\\+7 \\(\\d{3}\\) \\d{3}-\\d{4}$|^8 \\(\\d{3}\\) \\d{3}-\\d{4}$");

    QSqlQuery checkLoginQuery;
    checkLoginQuery.prepare("SELECT COUNT(*) FROM \"Users\" WHERE login_users = :login");
    checkLoginQuery.bindValue(":login", login);
    if (!checkLoginQuery.exec()) {
        qDebug() << "Ошибка выполнения запроса" << checkLoginQuery.lastError().text();
            return;
    }
    checkLoginQuery.next();
    int count = checkLoginQuery.value(0).toInt();
    if (count > 0) {
        // Логин уже занят, выполнение регистрации невозможно
        // Вывести сообщение об ошибке и остановить регистрацию
        QMessageBox::critical(this, "Ошибка", "Логин уже занят");
        return;
    } else {
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
                if (pass1.compare(pass2) == 0) {     // Пароли совпадают
                    // Проверяем, соответствует ли номер телефона заданным правилам
                    if (!phoneRegex.match(phone).hasMatch())
                    {
                        // Если номер телефона не соответствует правилам, то выводим сообщение об ошибке
                        QMessageBox::critical(this, "Ошибка", "Некорректный номер телефона.");
                        return;
                    }
                    else
                    {
                        QSqlQuery checkNomberQuery;
                        checkNomberQuery.prepare("SELECT COUNT(*) FROM \"Users\" WHERE number_users = :Nomber");
                        checkNomberQuery.bindValue(":Nomber", phone);
                        if (!checkNomberQuery.exec()) {
                            qDebug() << "Ошибка выполнения запроса" << checkNomberQuery.lastError().text();
                                return;
                        }
                        checkNomberQuery.next();
                        int count2 = checkNomberQuery.value(0).toInt();
                        if (count2 > 0) {
                            // Номер уже занят, выполнение регистрации невозможно
                            // Вывести сообщение об ошибке и остановить регистрацию
                            QMessageBox::critical(this, "Ошибка", "Номер уже занят");
                            return;
                        } else {    // Номер свободен, можно продолжить регистрацию
                            QString fullName = ui->txtFullname->text();
                            QRegularExpression regex("^[А-ЯЁ][а-яё]+ [А-ЯЁ]\\.[А-ЯЁ]\\.( [А-ЯЁ]\\.[А-ЯЁ]\\.)?$");
                            QRegularExpressionMatch match = regex.match(fullName);
                            if (match.hasMatch()) {
                            QSqlQuery* query = new QSqlQuery;
                            query->prepare("INSERT INTO \"Users\"(login_users, pass_users, fullname_users, post_users, number_users) VALUES (:login,:pass,:fullname,:post,:number)");
                            query->bindValue(":login",ui->txtLogin->text());
                            query->bindValue(":pass",(QCryptographicHash::hash(pass1.toUtf8(), QCryptographicHash::Sha256))); //ui->txtPass->text())); //
                            query->bindValue(":fullname",ui->txtFullname->text());
                            query->bindValue(":post",ui->bxPost->currentText());
                            query->bindValue(":number",ui->txtNumber->text());

                            QMessageBox* mess= new QMessageBox;
                            if(!query->exec())
                            {
                                mess->setText("Запрос составлен не верно!");
                                mess->show();
                            }
                            emit refresh_table();
                            this->close();
                            } else {
                            QMessageBox::critical(this, "Ошибка", "Некорректное поле ФИО.");
                            return;
                            }
                        }
                    }
                } else { // Пароли не совпадают
                    QMessageBox::critical(this, "Ошибка", "Пароли не совпадают.");
                    return;
                }
            }
        }
    }
}


void mainuseradminadd::on_btnClose_clicked()
{
    this->close();
}


void mainuseradminadd::on_btnRefresh_clicked()
{
    ui->txtLogin->setText("");
    ui->txtPass->setText("");
    ui->txtPass_2->setText("");
    ui->txtFullname->setText("");
    ui->bxPost->currentText();
    ui->txtNumber->setText("");
}


void mainuseradminadd::on_pbLookpassword_clicked()
{
    // подключаем сигнал pressed() к слоту, который устанавливает режим отображения символов на обычный
    connect(ui->pbLookpassword, &QPushButton::pressed, [=]() {
        ui->txtPass->setEchoMode(QLineEdit::Normal);
        ui->txtPass_2->setEchoMode(QLineEdit::Normal);
    });
    // подключаем сигнал released() к слоту, который устанавливает режим отображения символов на пароль
    connect(ui->pbLookpassword, &QPushButton::released, [=]() {
        ui->txtPass->setEchoMode(QLineEdit::Password);
        ui->txtPass_2->setEchoMode(QLineEdit::Password);
    });
}

