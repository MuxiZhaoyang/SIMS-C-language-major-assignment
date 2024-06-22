/**
 * @file scorequery.cpp
 * @brief 这个文件包含了 `scoreQuery` 类的实现，用于处理学生成绩查询的相关功能
 */

#include "studentTerminal/headfile/scorequery.h"
#include "ui_scorequery.h"

/**
 * @brief 构造函数，初始化成绩查询界面
 *
 * @param parent 父窗口指针
 * @param GDB 数据库指针
 */
scoreQuery::scoreQuery(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::scoreQuery)
{
    ui->setupUi(this);

    QSettings set;
    QString user = set.value("Username").toString();
    //创建数据模型，打开数据表
    tabModel=new QSqlQueryModel(this);	//数据模型

    //SQL搜索语句
    sql = QString("SELECT course.name AS courseName, course.type AS courseType, teacher.name AS teacherName, course.credit AS credit, processEvaluation, finalExam, gradePoint, term "
                  "FROM score "
                  "INNER JOIN course ON score.courseID = course.courseID "
                  "INNER JOIN teacher ON course.teacherID = teacher.teacherID "
                  "WHERE score.studentID = %1 AND processEvaluation IS NOT NULL ").arg(user);

    tabModel->setQuery(sql,*__GDB);			//设置数据表

    if (tabModel->lastError().isValid())
    {//查询数据失败
        QMessageBox::critical(this, "错误信息",
                              "打开数据表错误,错误信息:\n"+tabModel->lastError().text());
        ui->exportBtn->setEnabled(false);
        return;
    }

    //初始化CSV模块
    connect(ui->exportBtn, &QPushButton::clicked, this, &scoreQuery::exportToCsv);

    //设置字段显示标题
    QSqlRecord rec = tabModel->record();        //获取一个空记录，用于获取字段序号
    tabModel->setHeaderData(rec.indexOf("courseName"), Qt::Horizontal,"课程");
    tabModel->setHeaderData(rec.indexOf("courseType"), Qt::Horizontal, "课程类型");
    tabModel->setHeaderData(rec.indexOf("teacherName"),   Qt::Horizontal, "授课教师");
    tabModel->setHeaderData(rec.indexOf("credit"),   Qt::Horizontal, "学分");
    tabModel->setHeaderData(rec.indexOf("processEvaluation"), Qt::Horizontal, "过程评价");
    tabModel->setHeaderData(rec.indexOf("finalExam"), Qt::Horizontal, "期末成绩");
    tabModel->setHeaderData(rec.indexOf("gradePoint"), Qt::Horizontal, "绩点");
    tabModel->setHeaderData(rec.indexOf("term"), Qt::Horizontal, "学期");


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
    dataMapper->addMapping(ui->processEvaluation,rec.indexOf("processEvaluation"));
    dataMapper->addMapping(ui->gradePoint,rec.indexOf("gradePoint"));
    dataMapper->addMapping(ui->finalExam,rec.indexOf("finalExam"));
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
 * @brief CSV模块输出函数
 *
 * 在指定位置生成CSV文件
 */
void scoreQuery::exportToCsv()
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
    out << "课程" << ',' << "课程类型" << ',' << "授课教师" << ',' << "学分" << ',' << "过程评价" << ',' << "期末成绩" << ',' << "绩点" << ',' << "学期" << '\n';

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
 * @brief 析构函数，释放资源
 */
scoreQuery::~scoreQuery()
{
    delete ui;
}

/**
 * @brief 当组合框字段的当前文本改变时的处理函数
 *
 * @param arg1 组合框的当前文本
 */
void scoreQuery::on_comboFields_currentTextChanged(const QString &arg1)
{//切换排序方式
    Q_UNUSED(arg1)
    QString str,now;
    now = ui->comboFields->currentText();
    if(now == "默认排序")
    {
        ui->radioBtnAscend->setChecked(true);
        ui->radioBtnDescend->setChecked(false);
        ui->radioBtnAscend->setEnabled(false);
        ui->radioBtnDescend->setEnabled(false);
        str = sql;
    }
    else
    {
        ui->radioBtnAscend->setEnabled(true);
        ui->radioBtnDescend->setEnabled(true);
        if(now == "学分")str = sql + "ORDER BY credit ";
        else if(now == "绩点")str = sql + "ORDER BY gradePoint ";
        else if(now == "过程评价")str = sql + "ORDER BY processEvaluation ";
        else if(now == "期末成绩")str = sql + "ORDER BY finalExam ";
    }

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);
}

/**
 * @brief 当升序单选按钮被点击时的处理函数
 */
void scoreQuery::on_radioBtnAscend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    if(now == "默认排序") str = sql;
    else if(now == "学分")str = sql + "ORDER BY credit ";
    else if(now == "绩点")str = sql + "ORDER BY gradePoint ";
    else if(now == "过程评价")str = sql + "ORDER BY processEvaluation ";
    else if(now == "期末成绩")str = sql + "ORDER BY finalExam ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);
}

/**
 * @brief 当降序单选按钮被点击时的处理函数
 */
void scoreQuery::on_radioBtnDescend_clicked()
{//切换排序方式
    QString str,now;
    now = ui->comboFields->currentText();
    if(now == "默认排序") str = sql;
    else if(now == "学分")str = sql + "ORDER BY credit ";
    else if(now == "绩点")str = sql + "ORDER BY gradePoint ";
    else if(now == "过程评价")str = sql + "ORDER BY processEvaluation ";
    else if(now == "期末成绩")str = sql + "ORDER BY finalExam ";

    if(ui->radioBtnAscend->isChecked() && now != "默认排序")str += "ASC ";
    else if(now != "默认排序")str += "DESC ";

    tabModel->setQuery(str,*__GDB);
}

