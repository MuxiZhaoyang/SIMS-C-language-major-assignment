/**
 * @file courseselection.cpp
 * @brief 此文件包含了 `courseSelection` 类的定义（未使用）
 */

#include "studentTerminal/headfile/courseselection.h"
#include "ui_courseselection.h"

/**
 * @brief `courseSelection` 类的构造函数
 *
 * 用于初始化课程选择界面
 *
 * @param parent 父窗口指针
 * @param GDB 数据库指针
 */
courseSelection::courseSelection(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::courseSelection)
{
    ui->setupUi(this);
}

/**
 * @brief `courseSelection` 类的析构函数
 *
 * 用于释放与界面相关的资源
 */
courseSelection::~courseSelection()
{
    delete ui;
}
