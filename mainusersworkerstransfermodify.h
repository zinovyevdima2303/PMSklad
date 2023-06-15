#ifndef MAINUSERSWORKERSTRANSFERMODIFY_H
#define MAINUSERSWORKERSTRANSFERMODIFY_H

#include <QWidget>
#include "qsqlquery.h"

namespace Ui {
class MainUsersWorkersTransferModify;
}

class MainUsersWorkersTransferModify : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersTransferModify(QWidget *parent = nullptr);
    ~MainUsersWorkersTransferModify();
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
    Ui::MainUsersWorkersTransferModify *ui;
};

#endif // MAINUSERSWORKERSTRANSFERMODIFY_H
