#ifndef SMSDATABASE_H
#define SMSDATABASE_H

/***********************************************
 * 数据库模块，包含一个全局数据库接入点
 **********************************************/

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlQueryModel>
#include <QSettings>
#include <QSqlRecord>
#include <QSettings>
#include <QMessageBox>
#include <QByteArray>
#include <QCryptographicHash>
#include <QSqlError>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDataWidgetMapper>
#include <QStyledItemDelegate>
#include <QItemSelectionModel>
#include  <QComboBox>
#include <QSqlRelationalDelegate>
#include <QPushButton>

class smsDatabase
{//数据库类，用于实现项目里的各种数据库查询

private:
    QSqlDatabase * DB;        //数据库连接

public:
    QSqlDatabase * getConnection();      //获取数据库连接

    smsDatabase();

    smsDatabase(const smsDatabase & db);

    ~smsDatabase();

};

#endif // SMSDATABASE_H
