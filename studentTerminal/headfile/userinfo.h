#ifndef USERINFO_H
#define USERINFO_H

/***********************************************
 * 个人信息模块
 **********************************************/

#include <QWidget>
#include <smsdatabase.h>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui {
class userInfo;
}
QT_END_NAMESPACE

class userInfo : public QWidget
{
    Q_OBJECT

private:
    QString __student_ID;          //学生学号
    QSqlDatabase * __GDB;            //全局数据库

public:
    userInfo(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~userInfo();

private:
    Ui::userInfo *ui;
};
#endif // USERINFO_H
