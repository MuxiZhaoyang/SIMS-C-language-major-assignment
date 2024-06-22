/**
 * @file smsdatabase.cpp
 * @brief 这个文件包含了 `smsDatabase` 类的实现，作为全局共用的数据库接口
 */

#include "smsdatabase.h"

/**
 * @brief 获取数据库连接
 * 
 * @return QSqlDatabase* 全局数据库接口
 */
QSqlDatabase * smsDatabase::getConnection()
{//获取数据库连接
    return DB;
}

/**
 * @brief 构造函数，初始化数据库连接
 * 
 */
smsDatabase::smsDatabase()
{//初始化数据库连接
    DB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    DB->setDatabaseName("./students.db3");
    DB->open();
}

/**
 * @brief 构造函数，初始化数据库连接
 * 
 * @param db 现有的数据库接口
 */
smsDatabase::smsDatabase(const smsDatabase & db)
{
    DB = db.DB;
}

/**
 * @brief 析构函数，断开数据库连接
 * 
 */
smsDatabase::~smsDatabase()
{//数据库断开连接
    if(DB->isOpen())
        DB->close();
    delete DB;
}
