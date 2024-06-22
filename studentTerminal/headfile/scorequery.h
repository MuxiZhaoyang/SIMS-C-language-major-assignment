#ifndef SCOREQUERY_H
#define SCOREQUERY_H

/***********************************************
 * 成绩查询模块
 **********************************************/

#include <QWidget>

#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class scoreQuery;
}
QT_END_NAMESPACE

class scoreQuery : public QWidget
{
    Q_OBJECT

private:
    QSqlDatabase * __GDB;       //全局数据库连接
    QSqlQueryModel  * tabModel;        //关联数据库模型
    QDataWidgetMapper * dataMapper;             //关联字段模型
    QString sql;        //默认搜索语句

public:
    scoreQuery(QWidget *parent = nullptr, QSqlDatabase * GDB = nullptr);
    ~scoreQuery();

    void exportToCsv();
private slots:
    void on_comboFields_currentTextChanged(const QString &arg1);

    void on_radioBtnAscend_clicked();

    void on_radioBtnDescend_clicked();

private:
    Ui::scoreQuery *ui;
};
#endif // SCOREQUERY_H
