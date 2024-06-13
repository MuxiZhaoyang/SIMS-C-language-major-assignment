#include "studentTerminal/headfile/courseselection.h"
#include "ui_courseselection.h"

courseSelection::courseSelection(QWidget *parent,QSqlDatabase * GDB)
    : QWidget(parent)
    , __GDB(GDB)
    , ui(new Ui::courseSelection)
{
    ui->setupUi(this);
}

courseSelection::~courseSelection()
{
    delete ui;
}
