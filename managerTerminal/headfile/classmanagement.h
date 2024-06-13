#ifndef CLASSMANAGEMENT_H
#define CLASSMANAGEMENT_H

/***********************************************
 * 班级管理模块
 **********************************************/

#include <QWidget>

#include "smsdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class classManagement;
}
QT_END_NAMESPACE

class classManagement : public QWidget
{
    Q_OBJECT

private:
    QSqlDatabase * __GDB;       //全局数据库连接

    QSqlRelationalTableModel  *tabModel;          //数据模型
    QItemSelectionModel *selModel;	//选择模型
    QDataWidgetMapper   *dataMapper;	//数据映射

    void    getFieldNames();	//获取字段名称,填充“排序字段”下拉列表框

private slots:

    //SelectionModel的当前项的内容发生了变化，进行处理
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    // SelectionModel的当前行发生了变化，进行处理
    void do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_actRecAppend_clicked();

    void on_actRecInsert_clicked();

    void on_actSubmit_clicked();

    void on_actRevert_clicked();

    void on_actRecDelete_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_radioBtnDescend_clicked();

    void on_radioBtnAscend_clicked();

    void on_comboFields_currentIndexChanged(int index);

public:
    classManagement(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~classManagement();

private:
    Ui::classManagement *ui;
};
#endif // CLASSMANAGEMENT_H
