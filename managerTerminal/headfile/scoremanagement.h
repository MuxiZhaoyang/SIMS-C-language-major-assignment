#ifndef SCOREMANAGEMENT_H
#define SCOREMANAGEMENT_H

/***********************************************
 * 成绩管理模块
 **********************************************/

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDataWidgetMapper>
#include <QStyledItemDelegate>
#include <QItemSelectionModel>
#include    <QComboBox>
#include <QSqlRelationalDelegate>

#include "smsdatabase.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class scoreManagement;
}
QT_END_NAMESPACE

class scoreManagement : public QWidget
{
    Q_OBJECT

private:
    QSqlDatabase * __GDB;       //全局数据库连接

    QSqlRelationalTableModel  *tabModel;          //数据模型
    QItemSelectionModel *selModel;	//选择模型
    QDataWidgetMapper   *dataMapper;	//数据映射

    void    getFieldNames();	//获取字段名称,填充“排序字段”下拉列表框

public:
    scoreManagement(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~scoreManagement();

    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
private slots:
    void on_actRecAppend_clicked();

    void on_actRecInsert_clicked();

    void on_actSubmit_clicked();

    void on_actRevert_clicked();

    void on_actRecDelete_clicked();

    void on_radioBtnDescend_clicked();

    void on_radioBtnAscend_clicked();

    void on_comboFields_currentIndexChanged(int index);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::scoreManagement *ui;
};
#endif // SCOREMANAGEMENT_H
