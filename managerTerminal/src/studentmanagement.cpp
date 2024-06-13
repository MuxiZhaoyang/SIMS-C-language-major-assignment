#include "managerTerminal/headfile/studentmanagement.h"
#include "ui_studentmanagement.h"

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

//获取所有字段名称
void studentManagement::getFieldNames()
{
    QSqlRecord  emptyRec=tabModel->record();    //获取空记录，只有字段名
    for (int i=0;i<emptyRec.count();i++)
        ui->comboFields->addItem(emptyRec.fieldName(i));
}

studentManagement::~studentManagement()
{
    delete ui;
}

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

//数据发生修改，更新actPost和actCancel 的状态
void studentManagement::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    ui->actSubmit->setEnabled(tabModel->isDirty());		//有未保存修改时可用
    ui->actRevert->setEnabled(tabModel->isDirty());
}

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








//自定义代理
TComboBoxDelegate::TComboBoxDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}

void TComboBoxDelegate::setItems(QStringList items, bool editable)
{
    m_itemList=items;
    m_editable=editable;
}

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

void TComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentText(str);

}

void TComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                     const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);

    QString str = comboBox->currentText();

    model->setData(index, str, Qt::EditRole);
}

void TComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                             const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}




void studentManagement::on_actRecInsert_clicked()
{//插入一条记录
    //// 使用QSqlRecord
    QModelIndex curIndex=ui->tableView->currentIndex();
    QSqlRecord rec=tabModel->record();      //获取一个空记录，只有字段定义
    tabModel->insertRecord(curIndex.row(),rec);     //在当前行前面插入一条记录

    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);    //设置当前行
}


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


void studentManagement::on_actRevert_clicked()
{//取消修改
    tabModel->revertAll();
    ui->actSubmit->setEnabled(false);
    ui->actRevert->setEnabled(false);
}


void studentManagement::on_actRecDelete_clicked()
{//删除当前记录
    QModelIndex curIndex=selModel->currentIndex();  //获取当前选择单元格的模型索引
    tabModel->removeRow(curIndex.row()); //删除当前行
}


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


void studentManagement::on_actPhotoClear_clicked()
{//清除照片
    int curRecNo=selModel->currentIndex().row();
    QSqlRecord  curRec=tabModel->record(curRecNo);  //获取当前记录

    curRec.setNull("photo");        //设置为空值
    tabModel->setRecord(curRecNo,curRec);   //修改当前记录

    ui->dbLabPhoto->clear();        //清除QLabel组件的显示
}


void studentManagement::on_radioBtnAscend_clicked()
{//升序排序
    tabModel->setSort(ui->comboFields->currentIndex(),Qt::AscendingOrder);
    tabModel->select();     //setSort()之后需要执行select()才会刷新数据
}


void studentManagement::on_radioBtnDescend_clicked()
{//降序排序
    tabModel->sort(ui->comboFields->currentIndex(),Qt::DescendingOrder);
}


void studentManagement::on_radioBtnMan_clicked()
{//选男性
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setText("");
    tabModel->setFilter(" gender='男' ");
}


void studentManagement::on_radioBtnWoman_clicked()
{//选女性
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setText("");
    tabModel->setFilter(" gender='女' ");
}


void studentManagement::on_radioBtnBoth_clicked()
{//全显示
    ui->lineEdit->setEnabled(true);
    tabModel->setFilter("");
}


void studentManagement::on_comboFields_currentIndexChanged(int index)
{//选择字段进行排序
    if (ui->radioBtnAscend->isChecked())
        tabModel->setSort(index,Qt::AscendingOrder);
    else
        tabModel->setSort(index,Qt::DescendingOrder);

    tabModel->select();
}


void studentManagement::on_lineEdit_textChanged(const QString &arg1)
{//搜索学号
    tabModel->setFilter(QString(" studentID LIKE '%1%' ").arg(arg1));
}

