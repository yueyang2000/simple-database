

#ifndef Database_h
#define Database_h
#include "Table.h"
class Database
{
private:
    map<string,Table> tables;                       //database中的table name与table组成map<string, Table> tables
public:
    void operate(SQL &sql);                         //接受对应的table级的SQL语句，执行相应操作
    void show_tables();                             //输出该database的所有table
};
#endif /* Database_h */
