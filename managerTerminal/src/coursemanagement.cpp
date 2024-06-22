/**
 * @file coursemanagement.cpp
 * @brief 课程管理类的实现文件
 */

#include "managerTerminal/headfile/coursemanagement.h"
#include "ui_coursemanagement.h"

/**
 * @brief 课程管理类的构造函数
 *
 * 初始化课程管理界面的相关设置
 *
 * @param parent 父窗口指针
 * @param GDB 数据库指针
 */
courseManagement::courseManagement(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::courseManagement)
{
    ui->setupUi(this);
    //创建数据模型，打开数据表
    tabModel=new QSqlRelationalTableModel(this,*__GDB);	//数据模型
    tabModel->setTable("course");			//设置数据表
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);	//数据保存方式，OnManualSubmit , OnRowChange
    tabModel->setSort(tabModel->fieldIndex("studentID"),Qt::AscendingOrder); //排序
    if (!(tabModel->select()))	//查询数据失败
    {
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
        return;
    }

    //设置字段显示标题
    tabModel->setHeaderData(tabModel->fieldIndex("courseID"),  Qt::Horizontal, "课程号");
    tabModel->setHeaderData(tabModel->fieldIndex("name"),  Qt::Horizontal, "课程");
    tabModel->setHeaderData(tabModel->fieldIndex("type"), Qt::Horizontal, "课程类型");
    tabModel->setHeaderData(tabModel->fieldIndex("credit"),   Qt::Horizontal, "学分");
    tabModel->setHeaderData(tabModel->fieldIndex("classTime"),   Qt::Horizontal, "课程时间");
    tabModel->setHeaderData(tabModel->fieldIndex("place"),Qt::Horizontal,"地点");

    //设置关联字段
    tabModel->setRelation(tabModel->fieldIndex("teacherID"),
                          QSqlRelation("teacher","teacherID","name"));            //教师
    tabModel->setRelation(tabModel->fieldIndex("schoolID"),
                          QSqlRelation("school","schoolID","name"));            //学院

    tabModel->setHeaderData(tabModel->fieldIndex("teacherID"),   Qt::Horizontal, "教师");
    tabModel->setHeaderData(tabModel->fieldIndex("schoolID"),   Qt::Horizontal, "学院");

    //创建选择模型
    selModel=new QItemSelectionModel(tabModel,this);		//关联选择模型

    //当前行或列变化时发射currentChanged信号
    connect(selModel,&QItemSelectionModel::currentChanged,
            this,&courseManagement::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel,&QItemSelectionModel::currentRowChanged,
            this,&courseManagement::do_currentRowChanged);

    //Model/View结构
    ui->tableView->setModel(tabModel);	//设置数据模型
    ui->tableView->setSelectionModel(selModel); //设置选择模型
    //ui->tableView->setColumnHidden(tabModel->fieldIndex("scoreID"),true);	//隐藏列

    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));  //为关系型字段设置默认代理组件

    //创建界面组件与数据模型的字段之间的数据映射
    dataMapper= new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel);		//设置数据模型
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit); //数据更新方式
    //界面组件与数据模型的具体字段之间的联系
    dataMapper->addMapping(ui->dbSpinEmpNo,tabModel->fieldIndex("courseID"));
    dataMapper->addMapping(ui->dbEditName,tabModel->fieldIndex("name"));
    dataMapper->addMapping(ui->dbComboSex,tabModel->fieldIndex("type"));
    dataMapper->addMapping(ui->dbEditBirth,tabModel->fieldIndex("credit"));
    dataMapper->addMapping(ui->dbComboProvince,tabModel->fieldIndex("classTime"));
    dataMapper->addMapping(ui->dbpswd,tabModel->fieldIndex("place"));
    dataMapper->toFirst();  //移动到首记录

    //获取字段名称列表，填充groupBoxSort
    getFieldNames();

    //界面组件的使能状态
    ui->actRecAppend->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actRecDelete->setEnabled(true);

    ui->groupBoxSort->setEnabled(true);		//"排序"分租框


    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);	//项选择
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);	//单项选择
    ui->tableView->setAlternatingRowColors(true);	//交错行底色

}

/**
 * @brief 课程管理类的析构函数
 */
courseManagement::~courseManagement()
{
    delete ui;
}

/**
 * @brief 获取所有字段名称
 */
void courseManagement::getFieldNames()
{
    QSqlRecord  emptyRec=tabModel->record();    //获取空记录，只有字段名
    for (int i=0;i<emptyRec.count();i++)
        ui->comboFields->addItem(emptyRec.fieldName(i));
}

/**
 * @brief 当数据发生修改时，更新 actPost 和 actCancel 的状态
 *
 * @param current 当前的模型索引
 * @param previous 之前的模型索引
 */
void courseManagement::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    ui->actSubmit->setEnabled(tabModel->isDirty());		//有未保存修改时可用
    ui->actRevert->setEnabled(tabModel->isDirty());
}

/**
 * @brief 处理当前行变化事件
 *
 * @param current 当前的模型索引
 * @param previous 之前的模型索引
 */
void courseManagement::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    // 行切换时的状态控制
    ui->actRecDelete->setEnabled(current.isValid());    //"删除"按钮

    dataMapper->setCurrentIndex(current.row());     //更细数据映射的行号

    int curRecNo=current.row();     //获取行号
    QSqlRecord  curRec=tabModel->record(curRecNo); //获取当前记录
}

/**
 * @brief 处理添加记录的点击事件
 */
void courseManagement::on_actRecAppend_clicked()
{//添加一条记录
    //// 使用QSqlRecord
    QSqlRecord rec=tabModel->record();      //获取一个空记录，只有字段定义
    rec.setValue(tabModel->fieldIndex("empNo"),2000+tabModel->rowCount());
    rec.setValue(tabModel->fieldIndex("Gender"),"男");  //设置数据
    tabModel->insertRecord(tabModel->rowCount(),rec);   //插入到数据模型的最后

    selModel->clearSelection();
    QModelIndex curIndex=tabModel->index(tabModel->rowCount()-1,1);     //创建最后一行的ModelIndex
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}

/**
 * @brief 处理插入记录的点击事件
 */
void courseManagement::on_actRecInsert_clicked()
{//插入一条记录
    //// 使用QSqlRecord
    QModelIndex curIndex=ui->tableView->currentIndex();
    QSqlRecord rec=tabModel->record();      //获取一个空记录，只有字段定义
    tabModel->insertRecord(curIndex.row(),rec);     //在当前行前面插入一条记录

    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);    //设置当前行
}

/**
 * @brief 处理保存修改的点击事件
 */
void courseManagement::on_actSubmit_clicked()
{//保存修改
    bool res=tabModel->submitAll();
    if (!res)
        QMessageBox::information(this, "消息", "数据保存错误,错误信息\n"+tabModel->lastError().text());
    else
    {
        ui->actSubmit->setEnabled(false);
        ui->actRevert->setEnabled(false);
    }
}

/**
 * @brief 处理取消修改的点击事件
 */
void courseManagement::on_actRevert_clicked()
{//取消修改
    tabModel->revertAll();
    ui->actSubmit->setEnabled(false);
    ui->actRevert->setEnabled(false);
}

/**
 * @brief 处理删除当前记录的点击事件
 */
void courseManagement::on_actRecDelete_clicked()
{//删除当前记录
    QModelIndex curIndex=selModel->currentIndex();  //获取当前选择单元格的模型索引
    tabModel->removeRow(curIndex.row()); //删除当前行
}

/**
 * @brief 处理升序排序的点击事件
 */
void courseManagement::on_radioBtnAscend_clicked()
{//升序排序
    tabModel->setSort(ui->comboFields->currentIndex(),Qt::AscendingOrder);
    tabModel->select();     //setSort()之后需要执行select()才会刷新数据
}

/**
 * @brief 处理降序排序的点击事件
 */
void courseManagement::on_radioBtnDescend_clicked()
{//降序排序
    tabModel->sort(ui->comboFields->currentIndex(),Qt::DescendingOrder);
}

/**
 * @brief 处理选择字段排序的组合框索引变化事件
 *
 * @param index 组合框的当前索引
 */
void courseManagement::on_comboFields_currentIndexChanged(int index)
{//选择字段进行排序
    if (ui->radioBtnAscend->isChecked())
        tabModel->setSort(index,Qt::AscendingOrder);
    else
        tabModel->setSort(index,Qt::DescendingOrder);

    tabModel->select();
}

/**
 * @brief 处理搜索课程文本框内容变化事件
 *
 * @param arg1 文本框的新内容
 */
void courseManagement::on_lineEdit_textChanged(const QString &arg1)
{//搜索课程
    tabModel->setFilter(QString(" courseID LIKE '%1%' ").arg(arg1));
}
