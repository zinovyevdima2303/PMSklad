#include "login.h"
#include "ui_login.h"
#include <cstdlib>
#include <QTime>
#include <QDebug>
#include <QCryptographicHash>
#include <QByteArray>

Login::Login(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->txtUserLogin->setPlaceholderText("Введите логин для входа в систему");
    ui->txtUserPassword->setPlaceholderText("Введите пароль eдля входа в систему");

    myDB = QSqlDatabase::addDatabase("QPSQL");
    myDB.setHostName("127.0.0.1");
    myDB.setDatabaseName("pmsklad");
    myDB.setUserName("postgres");
    myDB.setPassword("root");
    if (!myDB.open()){
        ui->lblStatus->setText("[+] Ошибка, проверьте БД!");
    }
    else
    {
        ui->lblStatus->setText("[+] Соединение с БД установлено!");
    }
    MUAD=NULL;
    MUW=NULL;

    generateCaptcha(); // вызываем функцию для генерации и отображения капчи


}

Login::~Login()
{
    delete ui;
    qDebug()<<"БД закрывается";
    myDB.close();
}

void Login::generateCaptcha()
{
    QTime time = QTime::currentTime();
    srand((uint)time.msec());
    int num1 = rand() % 10;
    int num2 = rand() % 10;
    int num3 = rand() % 10;

    captchaResult = num1 + num2 + num3;

    QString captchaText = QString::number(num1) + "+" + QString::number(num2) + "+" + QString::number(num3);
    ui->labelCaptcha->setText(captchaText);
}

void Login::on_btnClear_clicked()
{
    ui->txtUserLogin->setText("");
    ui->txtUserPassword->setText("");
    ui->lineEditCaptcha->setText("");
}


void Login::on_btnLogin_clicked()
{
    QSqlQuery qry;
    QString username = ui->txtUserLogin->text();
    QString password = ui->txtUserPassword->text();
    QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
    QString passwordHashWithPrefix = "\\x" + passwordHash;


    if(!myDB.isOpen())
    {
        qDebug()<<"Нет соединения с БД";
        return;
    }

    QString userAnswer = ui->lineEditCaptcha->text();

    // Проверяем, правильно ли пользователь ввел сумму
    if (userAnswer.toInt()  == captchaResult) {
        // Код для продолжения выполнения программы

       if(qry.exec("SELECT id_users, login_users, pass_users, post_users FROM \"Users\" WHERE login_users=\'" + username + "\'AND pass_users=\'" + passwordHashWithPrefix + "\'"))
        {
            if(qry.next())
            {
                ui->lblStatus->setText("Данные подтверждены");
                QString msg = "Должность = " + qry.value(3).toString();
                QMessageBox::warning (this, "Логин верный",msg);
                if (qry.value(3).toString() == "Рабочий" || qry.value(3).toString() == "Кладовщик" || qry.value(3).toString() == "Ст. кладовщик") {
                    if (MUW==NULL) {
                        MUW = new MainUsersWorkers();
                        connect(MUW,&MainUsersWorkers::MainWorkers,this,&Login::show);
                        connect(this, SIGNAL(sendID(QString)), MUW, SLOT(recieveID(QString)));
                    }
                    emit sendID(qry.value(0).toString());
                    MUW->show();
                    this->close();
                    on_btnClear_clicked();
                    generateCaptcha();
                    if (!myDB.open()){
                        ui->lblStatus->setText("[+] Ошибка, проверьте БД!");
                    }
                    else
                    {
                        ui->lblStatus->setText("[+] Соединение с БД установлено!");
                    }
                }
                if(qry.value(3).toString() == "Администратор") {
                    if (MUAD == NULL){
                        MUAD = new MainUserAdmin();
                        connect(MUAD,&MainUserAdmin::MainAdmin,this,&Login::show);
                        connect(this, SIGNAL(sendID(QString)), MUAD, SLOT(recieveID(QString)));
                    }
                    emit sendID(qry.value(0).toString());
                    MUAD->show();
                    this->close();
                    on_btnClear_clicked();
                    generateCaptcha();
                    if (!myDB.open()){
                        ui->lblStatus->setText("[+] Ошибка, проверьте БД!");
                    }
                    else
                    {
                        ui->lblStatus->setText("[+] Соединение с БД установлено!");
                    }
                }
            }
            else
            {
                ui->lblStatus->setText("Данные не подтверждены!");
                on_btnClear_clicked();
            }
        }
    } else {
        // Вывод сообщения об ошибке и повтор попытки
        QMessageBox::warning(this, "Ошибка", "Неверная сумма чисел. Попробуйте еще раз.");
        ui->lineEditCaptcha->clear();
    }
}
