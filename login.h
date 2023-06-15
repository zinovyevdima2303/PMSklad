#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QRandomGenerator>
#include <mainuseradmin.h>
#include <mainusersworkers.h>
#include <QtGlobal>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow

{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();
    void generateCaptcha();

signals:
    void sendID(QString ID);

private slots:
    void on_btnClear_clicked();

    void on_btnLogin_clicked();

private:
    Ui::Login *ui;
    QSqlDatabase myDB;
    MainUserAdmin *MUAD;
    MainUsersWorkers *MUW;
    int captchaResult;
};

#endif // LOGIN_H
