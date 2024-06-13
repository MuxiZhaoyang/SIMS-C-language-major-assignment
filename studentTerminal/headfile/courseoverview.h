#ifndef COURSEOVERVIEW_H
#define COURSEOVERVIEW_H

/***********************************************
 * 选课浏览模块
 **********************************************/

#include <QWidget>

#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class courseOverview;
}
QT_END_NAMESPACE

class courseOverview : public QWidget
{
    Q_OBJECT

private:
    QSqlDatabase * __GDB;       //全局数据库连接
    QSqlQueryModel  * tabModel;        //关联数据库模型
    QString sql;        //默认搜索语句
    QString user;       //用户

public:
    courseOverview(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~courseOverview();

    void setClassSchedule();        //加载课程表
    void initClassSchedule();       //初始化课程表

private slots:
    void on_comboFields_currentTextChanged(const QString &arg1);

    void on_radioBtnAscend_clicked();

    void on_radioBtnDescend_clicked();

    void on_termSelection_currentTextChanged(const QString &arg1);

private:
    Ui::courseOverview *ui;
};
#endif // COURSEOVERVIEW_H
