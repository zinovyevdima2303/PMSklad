#ifndef MAINUSERSWORKERSRETURNADD_H
#define MAINUSERSWORKERSRETURNADD_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
#include "qsqlerror.h"

namespace Ui {
class MainUsersWorkersReturnAdd;
}

class MainUsersWorkersReturnAdd : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersReturnAdd(QWidget *parent = nullptr);
    ~MainUsersWorkersReturnAdd();

private:
    QString m_idusers, idSkladProduct;


signals:
    void refresh_table();

public slots:
    void recieveID(QString ID);
    void onComboBoxIndexChanged(int index);

private slots:
    void on_pbAdd_clicked();

    void on_pbClear_clicked();

    void on_pbClose_clicked();

private:
    Ui::MainUsersWorkersReturnAdd *ui;
};

#endif // MAINUSERSWORKERSRETURNADD_H
