

#ifndef sql_server_h
#define sql_server_h
#include "Database.h"
class sql_server
{
private:
    map<string,Database> db;                        //建立map<string, Database> db，对应于数据库的名字和数据库
    map<string,Database>::iterator current;         //迭代器指向当前的数据库所对应的pair
public:
    void start();                                   //开始的入口函数
    void operate(SQL& sql);                         //对传入的sql语句进行相应操作
};

#endif /* sql_server_h */
