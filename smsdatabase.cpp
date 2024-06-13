#include "smsdatabase.h"

QSqlDatabase * smsDatabase::getConnection()
{//获取数据库连接
    return DB;
}

smsDatabase::smsDatabase()
{//初始化数据库连接
    DB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    DB->setDatabaseName("./students.db3");
    DB->open();
}

smsDatabase::smsDatabase(const smsDatabase & db)
{
    DB = db.DB;
}
smsDatabase::~smsDatabase()
{//数据库断开连接
    if(DB->isOpen())
        DB->close();
    delete DB;
}
