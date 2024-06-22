/**
 * @file courseinformation.cpp
 * @brief 该文件包含了 `courseInformation` 类的定义，用于处理课程信息相关的操作和界面交互
 */

#include "studentTerminal/headfile/courseinformation.h"
#include "ui_courseinformation.h"

/**
 * @brief `courseInformation` 类的构造函数
 *
 * 初始化课程信息界面，并进行相关数据和组件的设置
 *
 * @param parent 父窗口指针
 * @param GDB 数据库指针
 */
courseInformation::courseInformation(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::courseInformation)
{
    ui->setupUi(this);

    QSettings set;
    user = set.value("Username").toString();
    //创建数据模型，打开数据表
    tabModel=new QSqlQueryModel(this);	//数据模型

    //初始化复选框
    sql = "SELECT settingContent FROM setting WHERE settingName = 'addTerm' ORDER BY settingContent DESC ";
    QSqlQuery ser(sql,*__GDB);
    QStringList termList;       //实用字符串链表组织学期
    while(ser.next())termList<<ser.value("settingContent").toString();
    ui->termSelection->addItems(termList);      //获取可用学期加入复选框

    //SQL搜索语句
    sql = QString("SELECT term, course.name AS courseName, course.type AS courseType, course.credit AS credit, teacher.name AS teacherName, course.classTime AS classTime ,course.place AS place, school.name AS schoolName "
                  "FROM score "
                  "INNER JOIN course ON score.courseID = course.courseID "
                  "INNER JOIN teacher ON course.teacherID = teacher.teacherID "
                  "INNER JOIN school ON course.schoolID = school.schoolID "
                  "WHERE score.studentID = %1 " ).arg(user);        //保存基础语句用于刷新搜索
    QString str =sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());

    tabModel->setQuery(str,*__GDB);			//设置数据表

    if (tabModel->lastError().isValid())
    {//查询数据失败
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
        ui->exportBtn_2->setEnabled(false);
        return;     
    }

    //初始化CSV模块
    connect(ui->exportBtn_2, &QPushButton::clicked, this, &courseInformation::exportToCsv);

    //设置字段显示标题
    QSqlRecord rec = tabModel->record();        //获取一个空记录，用于获取字段序号
    tabModel->setHeaderData(rec.indexOf("courseName"), Qt::Horizontal,"课程");
    tabModel->setHeaderData(rec.indexOf("courseType"), Qt::Horizontal, "课程类型");
    tabModel->setHeaderData(rec.indexOf("teacherName"),   Qt::Horizontal, "授课教师");
    tabModel->setHeaderData(rec.indexOf("credit"),   Qt::Horizontal, "学分");
    tabModel->setHeaderData(rec.indexOf("classTime"), Qt::Horizontal, "上课时间");
    tabModel->setHeaderData(rec.indexOf("place"), Qt::Horizontal, "地点");
    tabModel->setHeaderData(rec.indexOf("gradePoint"), Qt::Horizontal, "绩点");
    tabModel->setHeaderData(rec.indexOf("term"), Qt::Horizontal, "学期");
    tabModel->setHeaderData(rec.indexOf("schoolName"), Qt::Horizontal, "开设学院");

    //Model/View结构
    ui->tableView->setModel(tabModel);	//设置数据模型

    //创建界面组件与数据模型的字段之间的数据映射
    dataMapper= new QDataWidgetMapper(this);
    dataMapper->setModel(tabModel);		//设置数据模型
    //界面组件与数据模型的具体字段之间的联系
    dataMapper->addMapping(ui->term,rec.indexOf("term"));
    dataMapper->addMapping(ui->courseName,rec.indexOf("courseName"));
    dataMapper->addMapping(ui->courseType,rec.indexOf("courseType"));
    dataMapper->addMapping(ui->teacherName,rec.indexOf("teacherName"));
    dataMapper->addMapping(ui->point,rec.indexOf("credit"));
    dataMapper->addMapping(ui->schoolName,rec.indexOf("schoolName"));
    dataMapper->addMapping(ui->place,rec.indexOf("place"));
    dataMapper->addMapping(ui->classTime,rec.indexOf("classTime"));
    dataMapper->toFirst();  //移动到首记录

    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,       //切换映射
            this, [this](const QModelIndex &current, const QModelIndex &previous) {         //Lamda函数
                Q_UNUSED(previous)
                if (current.isValid()) {
                    dataMapper->setCurrentModelIndex(current);
                }
            });

    ui->radioBtnAscend->setChecked(true);
    ui->radioBtnDescend->setChecked(false);
    ui->radioBtnAscend->setEnabled(false);
    ui->radioBtnDescend->setEnabled(false);
}

/**
 * @brief `courseInformation` 类的析构函数
 *
 * 释放分配的界面资源
 */
courseInformation::~courseInformation()
{
    delete ui;
}

/**
 * @brief CSV模块输出函数
 *
 * 在指定位置生成CSV文件
 */
void courseInformation::exportToCsv()
{//CSV模块
    QString filePath = QFileDialog::getSaveFileName(this, "导出 CSV 文件", "", "CSV 文件 (*.csv)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件进行写入");
        return;
    }

    QTextStream out(&file);

    // 写入表头
    out << "学期" << ',' << "课程" << ',' << "课程类型" << ',' << "学分" << ',' << "授课教师" << ',' << "上课时间" << ',' << "地点" << ',' << "开设学院" << '\n';

    int rowCount = tabModel->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < tabModel->columnCount(); ++col) {
            QModelIndex index = tabModel->index(row, col);
            QString data = tabModel->data(index).toString();
            if(col == tabModel->columnCount()-1)out << data;
            else out << data << ',';
        }
        out << '\n';
    }

    file.close();
}

/**
 * @brief 当组合框字段的当前文本改变时的处理函数
 *
 * 根据新的选择切换排序方式，并更新相关显示
 *
 * @param arg1 组合框的当前文本
 */
void courseInformation::on_comboFields_currentTextChanged(const QString &arg1)
{//切换排序方式
    Q_UNUSED(arg1)
    QString str,now;
    str = sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());
    now = ui->comboFields->currentText();
    if(now == "默认排序")
    {
        ui->radioBtnAscend->setChecked(true);
        ui->radioBtnDescend->setChecked(false);
        ui->radioBtnAscend->setEnabled(false);
        ui->radioBtnDescend->setEnabled(false);
    }
    else
    {
        ui->radioBtnAscend->setEnabled(true);
        ui->radioBtnDescend->setEnabled(true);
        if(now == "学分")str += "ORDER BY credit ";
    }

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询


    if(tabModel->columnCount() > 0)dataMapper->toFirst();   //刷新当前选框
    else
    {
        ui->courseName->setText("");
        ui->courseType->setText("");
        ui->teacherName->setText("");
        ui->point->setText("");
        ui->schoolName->setText("");
        ui->place->setText("");
        ui->classTime->setText("");
    }
}

/**
 * @brief 当升序单选按钮被点击时的处理函数
 *
 * 切换排序方式并更新显示
 */
void courseInformation::on_radioBtnAscend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    str = sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());
    if(now == "默认排序");
    else if(now == "学分")str += "ORDER BY credit ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询

    if(tabModel->columnCount() > 0)dataMapper->toFirst();   //刷新当前选框
    else
    {
        ui->courseName->setText("");
        ui->courseType->setText("");
        ui->teacherName->setText("");
        ui->point->setText("");
        ui->schoolName->setText("");
        ui->place->setText("");
        ui->classTime->setText("");
    }
}

/**
 * @brief 当降序单选按钮被点击时的处理函数
 *
 * 切换排序方式并更新显示
 */
void courseInformation::on_radioBtnDescend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    str = sql + QString("AND score.term = '%1' ").arg(ui->termSelection->currentText());
    if(now == "默认排序");
    else if(now == "学分")str += "ORDER BY credit ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询

    if(tabModel->columnCount() > 0)dataMapper->toFirst();   //刷新当前选框
    else
    {
        ui->courseName->setText("");
        ui->courseType->setText("");
        ui->teacherName->setText("");
        ui->point->setText("");
        ui->schoolName->setText("");
        ui->place->setText("");
        ui->classTime->setText("");
    }
}

/**
 * @brief 当学期选择组合框的当前文本改变时的处理函数
 *
 * 切换排序方式，执行查询，并更新显示
 *
 * @param arg1 学期选择组合框的当前文本
 */
void courseInformation::on_termSelection_currentTextChanged(const QString &arg1)
{//切换排序方式
    ui->term->setText(ui->termSelection->currentText());    //刷新学期
    QString str,now;
    now = ui->comboFields->currentText();
    str = sql + QString("AND score.term = '%1' ").arg(arg1);
    if(now == "默认排序");
    else if(now == "学分")str += "ORDER BY credit ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);     //二次查询

    QSqlQuery ser(str,*__GDB);
    ser.exec();
    if(ser.first())
    {//刷新当前选框
        dataMapper->toFirst();
        ui->comboFields->setEnabled(true);
    }
    else
    {
        ui->courseName->setText("");
        ui->courseType->setText("");
        ui->teacherName->setText("");
        ui->point->setText("");
        ui->schoolName->setText("");
        ui->place->setText("");
        ui->classTime->setText("");
    }
}

