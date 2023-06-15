#ifndef MAINUSERSWORKERSINVENTORYPRINT_H
#define MAINUSERSWORKERSINVENTORYPRINT_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QDate>
#include "qsqlerror.h"
#include <QSqlQueryModel>
#include <QMainWindow>
#include <QPainter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QFileDialog>
#include <QTableWidget>
#include "qsqlquery.h"

namespace Ui {
class MainUsersWorkersInventoryPrint;
}

class MainUsersWorkersInventoryPrint : public QWidget
{
    Q_OBJECT

public:
    explicit MainUsersWorkersInventoryPrint(QWidget *parent = nullptr);
    ~MainUsersWorkersInventoryPrint();

private:
    QString m_idusers;
    QString FIO, POST;
    QString Location, Status;
private:
    QString selectedParm;

public slots:
    void recieveID(QString ID);
    void onFilterChanged(int index);

private slots:
    void on_pbPrint_clicked();

    void on_pbClose_clicked();

private:
    Ui::MainUsersWorkersInventoryPrint *ui;
    QMap<int, int> indexToNumberMap; // Добавлено объявление indexToNumberMap как член класса

};

#endif // MAINUSERSWORKERSINVENTORYPRINT_H
