/**
 * @file studentmanagement.cpp
 * @brief 学生管理类的实现文件
 */

#include "managerTerminal/headfile/studentmanagement.h"
#include "ui_studentmanagement.h"

/**
 * @brief 学生管理类的构造函数
 *
 * 初始化学生管理界面的相关设置
 *
 * @param parent 父窗口指针
 * @param GDB 数据库指针
 */
studentManagement::studentManagement(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::studentManagement)
{
    ui->setupUi(this);

    //创建数据模型，打开数据表
    tabModel=new QSqlRelationalTableModel(this,*__GDB);	//数据模型
    tabModel->setTable("student");			//设置数据表
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);	//数据保存方式，OnManualSubmit , OnRowChange
    tabModel->setSort(tabModel->fieldIndex("studentID"),Qt::AscendingOrder); //排序
    if (!(tabModel->select()))	//查询数据失败
    {
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
        return;
    }

    //设置字段显示标题
    tabModel->setHeaderData(tabModel->fieldIndex("studentID"),  Qt::Horizontal, "学号");
    tabModel->setHeaderData(tabModel->fieldIndex("name"),   Qt::Horizontal, "姓名");
    tabModel->setHeaderData(tabModel->fieldIndex("gender"), Qt::Horizontal, "性别");
    tabModel->setHeaderData(tabModel->fieldIndex("birthday"),   Qt::Horizontal, "出生日期");
    tabModel->setHeaderData(tabModel->fieldIndex("province"),   Qt::Horizontal, "省份");
    tabModel->setHeaderData(tabModel->fieldIndex("password"),Qt::Horizontal,"密码");

    //不在tableView中显示
    tabModel->setHeaderData(tabModel->fieldIndex("photo"),Qt::Horizontal,"照片");

    tabModel->setRelation(tabModel->fieldIndex("classID"),
                          QSqlRelation("class","classID","name"));            //班级

    tabModel->setHeaderData(tabModel->fieldIndex("classID"),   Qt::Horizontal, "班级");

    //创建选择模型
    selModel=new QItemSelectionModel(tabModel,this);		//关联选择模型

    //当前行或列变化时发射currentChanged信号
    connect(selModel,&QItemSelectionModel::currentChanged,
            this,&studentManagement::do_currentChanged);
    //当前行变化时发射currentRowChanged()信号
    connect(selModel,&QItemSelectionModel::currentRowChanged,
            this,&studentManagement::do_currentRowChanged);

    //Model/View结构
    ui->tableView->setModel(tabModel);	//设置数据模型
    ui->tableView->setSelectionModel(selModel); //设置选择模型
    ui->tableView->setColumnHidden(tabModel->fieldIndex("photo"),true);	//隐藏列


    //设置自定义代理组件
    QStringList strList;
    strList<<"男"<<"女";
    bool isEditable=false;
    delegateSex.setItems(strList,isEditable);
    ui->tableView->setItemDelegateForColumn(
        tabModel->fieldIndex("gender"),&delegateSex);   //设置代理组件

    strList.clear();
    strList<<"河北"<<"山西"<<"辽宁"<<"吉林"<<"黑龙江"<<"江苏"<<"浙江"<<"安徽"
            <<"福建"<<"江西"<<"山东"<<"河南"<<"湖北"<<"湖南"<<"广东"<<"海南"
            <<"四川"<<"贵州"<<"云南"<<"陕西"<<"甘肃"<<"青海"<<"台湾"<<"内蒙古"
            <<"广西"<<"西藏"<<"宁夏"<<"新疆"<<"北京"<<"天津"<<"上海"<<"重庆"<<"香港"<<"澳门";
    isEditable=false;
    delegateProvince.setItems(strList,isEditable);
    ui->tableView->setItemDelegateForColumn(
        tabModel->fieldIndex("province"),&delegateProvince);   //设置代理组件

    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));  //为关系型字段设置默认代理组件

    //创建界面组件与数据模型的字段之间的数据映射
    dataMapper= new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel);		//设置数据模型
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit); //数据更新方式
    //界面组件与数据模型的具体字段之间的联系
    dataMapper->addMapping(ui->dbSpinEmpNo,tabModel->fieldIndex("studentID"));
    dataMapper->addMapping(ui->dbEditName,tabModel->fieldIndex("name"));
    dataMapper->addMapping(ui->dbComboSex,tabModel->fieldIndex("gender"));
    dataMapper->addMapping(ui->dbEditBirth,tabModel->fieldIndex("birthday"));
    dataMapper->addMapping(ui->dbComboProvince,tabModel->fieldIndex("province"));
    dataMapper->addMapping(ui->dbpswd,tabModel->fieldIndex("password"));
    //dataMapper->toFirst();  //移动到首记录

    //获取字段名称列表，填充groupBoxSort
    getFieldNames();

    //界面组件的使能状态
    ui->actRecAppend->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actRecDelete->setEnabled(true);

    ui->groupBoxSort->setEnabled(true);		//"排序"分租框
    ui->groupBoxFilter->setEnabled(true);	//"数据过滤"分组框

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);	//项选择
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);	//单项选择
    ui->tableView->setAlternatingRowColors(true);	//交错行底色
}

/**
 * @brief 获取所有字段名称
 */
void studentManagement::getFieldNames()
{
    QSqlRecord  emptyRec=tabModel->record();    //获取空记录，只有字段名
    for (int i=0;i<emptyRec.count();i++)
        ui->comboFields->addItem(emptyRec.fieldName(i));
}

/**
 * @brief 学生管理类的析构函数
 */
studentManagement::~studentManagement()
{
    delete ui;
}

/**
 * @brief 处理添加记录的点击事件
 */
void studentManagement::on_actRecAppend_clicked()
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
 * @brief 当数据发生修改时，更新 actPost 和 actCancel 的状态
 *
 * @param current 当前的模型索引
 * @param previous 之前的模型索引
 */
void studentManagement::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    ui->actSubmit->setEnabled(tabModel->isDirty());		//有未保存修改时可用
    ui->actRevert->setEnabled(tabModel->isDirty());
}

/**
 * @brief 处理行切换时的状态控制
 *
 * 该函数用于在行切换时控制相关按钮的可用性，并根据当前行的记录情况处理图片显示
 *
 * @param current 当前的模型索引
 * @param previous 之前的模型索引（未使用）
 */
void studentManagement::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    // 行切换时的状态控制
    ui->actRecDelete->setEnabled(current.isValid());    //"删除"按钮
    ui->actPhoto->setEnabled(current.isValid());        //"设置照片"按钮
    ui->actPhotoClear->setEnabled(current.isValid());   //"清除照片"按钮

    if (!current.isValid())
    {
        ui->dbLabPhoto->clear();
        return;
    }

    dataMapper->setCurrentIndex(current.row());     //更细数据映射的行号

    int curRecNo=current.row();     //获取行号
    QSqlRecord  curRec=tabModel->record(curRecNo); //获取当前记录

    if (curRec.isNull("Photo"))     //图片字段内容为空
        ui->dbLabPhoto->clear();
    else
    {
        QByteArray data=curRec.value("Photo").toByteArray();
        QPixmap pic;
        pic.loadFromData(data);
        ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->size().width()));
    }
}

/**
 * @brief 自定义组合框代理的构造函数
 *
 * @param parent 父对象
 */
TComboBoxDelegate::TComboBoxDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

/**
 * @brief 设置组合框的项和可编辑状态
 *
 * @param items 项列表
 * @param editable 是否可编辑
 */
void TComboBoxDelegate::setItems(QStringList items, bool editable)
{
    m_itemList=items;
    m_editable=editable;
}

/**
 * @brief 创建编辑器
 *
 * 为给定的父部件、样式选项、模型索引创建组合框编辑器
 *
 * @param parent 父部件
 * @param option 样式选项
 * @param index 模型索引
 * @return QWidget* 创建的编辑器
 */
QWidget *TComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *editor = new QComboBox(parent);
    editor->setEditable(m_editable);	//是否可编辑
    for (int i=0;i<m_itemList.count();i++)   //从字符串列表初始下拉列表
        editor->addItem(m_itemList.at(i));
    return editor;
}

/**
 * @brief 设置编辑器数据
 *
 * 将模型数据设置到编辑器
 *
 * @param editor 编辑器
 * @param index 模型索引
 */
void TComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(str);

}

/**
 * @brief 设置模型数据
 *
 * 将编辑器的数据设置到模型
 *
 * @param editor 编辑器
 * @param model 模型
 * @param index 模型索引
 */
void TComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);

    QString str = comboBox->currentText();

    model->setData(index, str, Qt::EditRole);
}

/**
 * @brief 更新编辑器几何形状
 *
 * 设置编辑器的几何形状
 *
 * @param editor 编辑器
 * @param option 样式选项
 * @param index 模型索引
 */
void TComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

/**
 * @brief 处理插入记录的点击事件
 *
 * 在当前行前面插入一条空记录
 */
void studentManagement::on_actRecInsert_clicked()
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
 *
 * 尝试提交所有修改，如果失败显示错误信息，成功则更新按钮状态
 */
void studentManagement::on_actSubmit_clicked()
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
 *
 * 撤销所有修改，并更新按钮状态
 */
void studentManagement::on_actRevert_clicked()
{//取消修改
    tabModel->revertAll();
    ui->actSubmit->setEnabled(false);
    ui->actRevert->setEnabled(false);
}

/**
 * @brief 处理删除当前记录的点击事件
 *
 * 删除当前选中的行
 */
void studentManagement::on_actRecDelete_clicked()
{//删除当前记录
    QModelIndex curIndex=selModel->currentIndex();  //获取当前选择单元格的模型索引
    tabModel->removeRow(curIndex.row()); //删除当前行
}

/**
 * @brief 处理设置照片的点击事件
 *
 * 选择照片文件，将其数据保存到当前记录的"photo"字段，并在界面显示
 */
void studentManagement::on_actPhoto_clicked()
{//设置照片
    QString aFile=QFileDialog::getOpenFileName(this,"选择图片文件","","照片(*.jpg)");
    if (aFile.isEmpty())
        return;

    QByteArray data;
    QFile* file=new QFile(aFile);
    file->open(QIODevice::ReadOnly);    //打开文件
    data = file->readAll();     //读取图片数据
    file->close();
    delete file;

    int curRecNo=selModel->currentIndex().row();    //当前记录的行号
    QSqlRecord  curRec=tabModel->record(curRecNo);      //获取当前记录
    curRec.setValue("photo",data);  //设置字段数据
    tabModel->setRecord(curRecNo,curRec);   //修改记录

    QPixmap pic;
    pic.load(aFile);  //在界面上显示
    ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->width()));
}

/**
 * @brief 处理清除照片的点击事件
 *
 * 将当前记录的"photo"字段设置为空，并清除界面显示
 */
void studentManagement::on_actPhotoClear_clicked()
{//清除照片
    int curRecNo=selModel->currentIndex().row();
    QSqlRecord  curRec=tabModel->record(curRecNo);  //获取当前记录

    curRec.setNull("photo");        //设置为空值
    tabModel->setRecord(curRecNo,curRec);   //修改当前记录

    ui->dbLabPhoto->clear();        //清除QLabel组件的显示
}

/**
 * @brief 处理升序排序的点击事件
 *
 * 根据选择的字段进行升序排序，并刷新数据
 */
void studentManagement::on_radioBtnAscend_clicked()
{//升序排序
    tabModel->setSort(ui->comboFields->currentIndex(),Qt::AscendingOrder);
    tabModel->select();     //setSort()之后需要执行select()才会刷新数据
}

/**
 * @brief 处理降序排序的点击事件
 *
 * 根据选择的字段进行降序排序
 */
void studentManagement::on_radioBtnDescend_clicked()
{//降序排序
    tabModel->sort(ui->comboFields->currentIndex(),Qt::DescendingOrder);
}

/**
 * @brief 处理选择男性的点击事件
 *
 * 禁用文本编辑框，清空文本，并设置筛选条件
 */
void studentManagement::on_radioBtnMan_clicked()
{//选男性
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setText("");
    tabModel->setFilter(" gender='男' ");
}

/**
 * @brief 处理选择女性的点击事件
 *
 * 禁用文本编辑框，清空文本，并设置筛选条件
 */
void studentManagement::on_radioBtnWoman_clicked()
{//选女性
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setText("");
    tabModel->setFilter(" gender='女' ");
}

/**
 * @brief 处理全显示的点击事件
 *
 * 启用文本编辑框，并清空筛选条件
 */
void studentManagement::on_radioBtnBoth_clicked()
{//全显示
    ui->lineEdit->setEnabled(true);
    tabModel->setFilter("");
}

/**
 * @brief 处理选择字段排序的组合框索引变化事件
 *
 * 根据排序按钮状态和选择的字段进行排序，并刷新数据
 */
void studentManagement::on_comboFields_currentIndexChanged(int index)
{//选择字段进行排序
    if (ui->radioBtnAscend->isChecked())
        tabModel->setSort(index,Qt::AscendingOrder);
    else
        tabModel->setSort(index,Qt::DescendingOrder);

    tabModel->select();
}

/**
 * @brief 处理搜索学号文本框内容变化事件
 *
 * 根据输入内容设置筛选条件
 *
 * @param arg1 文本框的新内容
 */
void studentManagement::on_lineEdit_textChanged(const QString &arg1)
{//搜索学号
    tabModel->setFilter(QString(" studentID LIKE '%1%' ").arg(arg1));
}

