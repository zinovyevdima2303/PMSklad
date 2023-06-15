#ifndef MAINUSERADMIN_H
#define MAINUSERADMIN_H

#include <QMainWindow>
#include <QSqlQueryModel>

#include <mainuseradminadd.h>
#include <ui_mainuseradminadd.h>

#include <mainuseradminmodify.h>
#include <ui_mainuseradminmodify.h>

namespace Ui {
class MainUserAdmin;
}

class MainUserAdmin : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUserAdmin(QWidget *parent = nullptr);
    ~MainUserAdmin();

signals:
    void MainAdmin();
    void sendID(int);

public slots:
    void recieveID(QString ID);
    void obr_ref_tab();


private slots:
    void on_btnClose_clicked();

    void on_btnRefresh_clicked();

    void on_btnAdd_clicked();

    void on_btnEdit_clicked();

    void on_btnDelite_clicked();

private:
    Ui::MainUserAdmin *ui;
    QSqlQueryModel *TBW;
    mainuseradminadd *MUAAdd;
    mainuseradminmodify *MUAM;
};

#endif // MAINUSERADMIN_H
