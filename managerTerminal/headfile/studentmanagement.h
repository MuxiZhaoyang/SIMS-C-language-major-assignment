#ifndef STUDENTMANAGEMENT_H
#define STUDENTMANAGEMENT_H

/***********************************************
 * 学生管理模块
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
class studentManagement;
}
QT_END_NAMESPACE

class TComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

private:
    QStringList m_itemList; //选择列表
    bool    m_editable;     //是否可编辑

public:
    explicit TComboBoxDelegate(QObject *parent = nullptr);

    //自定义函数，初始化设置列表内容，是否可编辑
    void    setItems(QStringList items, bool editable);

    //自定义代理必须重新实现以下4个函数

    //创建编辑组件
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index)const;

    //从数据模型获取数据，显示到代理组件中
    void setEditorData(QWidget *editor, const QModelIndex &index)const;

    //将代理组件的数据，保存到数据模型中
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index)const;

    //更新代理编辑组件的大小
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index)const;
};

class studentManagement : public QWidget
{
    Q_OBJECT

private:
    QSqlDatabase * __GDB;       //全局数据库连接

    QSqlRelationalTableModel  *tabModel;          //数据模型
    QItemSelectionModel *selModel;	//选择模型
    QDataWidgetMapper   *dataMapper;	//数据映射

    TComboBoxDelegate   delegateSex;	//自定义数据代理，性别
    TComboBoxDelegate   delegateProvince; //自定义数据代理，省份

    void    getFieldNames();	//获取字段名称,填充“排序字段”下拉列表框

public:
    studentManagement(QWidget *parent = nullptr,QSqlDatabase * GDB = nullptr);
    ~studentManagement();

private slots:
    void on_actRecAppend_clicked();

    //SelectionModel的当前项的内容发生了变化，进行处理
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    // SelectionModel的当前行发生了变化，进行处理
    void do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);


    void on_actRecInsert_clicked();

    void on_actSubmit_clicked();

    void on_actRevert_clicked();

    void on_actRecDelete_clicked();

    void on_actPhoto_clicked();

    void on_actPhotoClear_clicked();

    void on_radioBtnAscend_clicked();

    void on_radioBtnDescend_clicked();

    void on_radioBtnMan_clicked();

    void on_radioBtnWoman_clicked();

    void on_radioBtnBoth_clicked();

    void on_comboFields_currentIndexChanged(int index);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::studentManagement *ui;
};



#endif // STUDENTMANAGEMENT_H
