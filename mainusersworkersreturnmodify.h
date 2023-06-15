#ifndef MAINUSERSWORKERSRETURNMODIFY_H
#define MAINUSERSWORKERSRETURNMODIFY_H

#include <QWidget>
#include "qsqlquery.h"

namespace Ui {
class MainUsersWorkersReturnModify;
}

class MainUsersWorkersReturnModify : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersReturnModify(QWidget *parent = nullptr);
    ~MainUsersWorkersReturnModify();
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
    Ui::MainUsersWorkersReturnModify *ui;
};

#endif // MAINUSERSWORKERSRETURNMODIFY_H
