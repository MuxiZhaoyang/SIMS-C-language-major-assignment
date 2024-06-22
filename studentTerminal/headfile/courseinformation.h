#ifndef COURSEINFORMATION_H
#define COURSEINFORMATION_H

/***********************************************
 * 课程信息模块
 **********************************************/

#include <QWidget>

#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class courseInformation;
}
QT_END_NAMESPACE

class courseInformation : public QWidget
{
    Q_OBJECT

private:
    QSqlDatabase * __GDB;       //全局数据库连接
    QSqlQueryModel  * tabModel;        //关联数据库模型
    QString sql;        //默认搜索语句
    QString user;       //用户
    QDataWidgetMapper * dataMapper;             //关联字段模型

public:
    courseInformation(QWidget *parent = nullptr, QSqlDatabase * GDB = nullptr);
    ~courseInformation();

    void exportToCsv();
private slots:
    void on_comboFields_currentTextChanged(const QString &arg1);

    void on_radioBtnAscend_clicked();

    void on_radioBtnDescend_clicked();

    void on_termSelection_currentTextChanged(const QString &arg1);

private:
    Ui::courseInformation *ui;
};
#endif // COURSEINFORMATION_H
