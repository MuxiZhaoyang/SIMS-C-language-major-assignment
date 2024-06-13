#ifndef COURSESELECTION_H
#define COURSESELECTION_H

/***********************************************
 * 课程选择模块（已弃用）
 **********************************************/

#include <QWidget>

#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class courseSelection;
}
QT_END_NAMESPACE

class courseSelection : public QWidget
{
    Q_OBJECT

private:
    QSqlDatabase * __GDB;       //全局数据库连接

public:
    courseSelection(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~courseSelection();

private:
    Ui::courseSelection *ui;
};
#endif // COURSESELECTION_H
