#ifndef MAINUSERSWORKERSINVENTORYMODIFY_H
#define MAINUSERSWORKERSINVENTORYMODIFY_H

#include <QWidget>
#include "qsqlquery.h"

namespace Ui {
class MainUsersWorkersInventoryModify;
}

class MainUsersWorkersInventoryModify : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersInventoryModify(QWidget *parent = nullptr);
    ~MainUsersWorkersInventoryModify();

private:
    QString m_idusers;

public slots:
    void recieveID(QString ID);

signals:
    void refresh_table();

private slots:
    void obr_sendIDNakl(int);

    void on_pbEdit_clicked();

    void on_pbClose_clicked();

private:
    Ui::MainUsersWorkersInventoryModify *ui;
};

#endif // MAINUSERSWORKERSINVENTORYMODIFY_H
