

#include "Header.h"
#include "Database.h"

void Database::operate(SQL &sql)                            //接受对应的SQL语句并执行相应操作
{
    if(sql[0]=="CREATE"&&sql[1]=="TABLE")                   //如果是create table操作
    {
        Table newt(sql);                                    //新建table newt
        tables[sql[2]]=newt;                                //在map里加入pair:(sql[2], new table)
    }
    else if(sql[0]=="INSERT"&&sql[1]=="INTO")               //如果是insert info操作
    {
        auto it=tables.find(sql[2]);                        //找到名为sql[2]的表
        it->second.insert_into(sql);                        //调用该table对象的成员函数insert_into，执行相应插入操作
    }
    else if(sql[0]=="DELETE"&&sql[1]=="FROM")               //如果是delete from的操作
    {
        auto it=tables.find(sql[2]);                        //找到名为sql[2]的表
        it->second.delete_from(sql);                        //调用该table对象的成员函数delete_from，执行相应操作
    }
    else if(sql[0]=="UPDATE")                               //如果是update操作
    {
        auto it=tables.find(sql[1]);                        //找到名为sql[1]的表
        it->second.update(sql);                             //调用该table对象的成员函数update，执行相应操作
    }
    else if(sql[0]=="SELECT")                               //如果是selete操作
    {
        int i;
        for(i=0;i!=sql.get_size();i++)                      //找到from关键字，之后的就是表名
        {
            if(sql[i]=="FROM")
            {
                break;
            }
        }
        auto it=tables.find(sql[i+1]);                      //找到名为sql[i + 1]的表
        it->second.select(sql);                             //调用该table对象的成员函数selete，执行相应操作
    }
    else if(sql[0]=="DROP"&&sql[1]=="TABLE")                //如果是drop table操作
    {
        tables.erase(sql[2]);                               //从<table name, table>map中将主键为sql[2]的pair删除
    }
    else if(sql[0]=="SHOW"&&sql[1]=="COLUMNS")              //如果是show columns操作
    {
        tables[sql[3]].show_columns();                      //调用名为sql[3]的表对象的成员函数show_columns，执行相应操作
    }
}
void Database::show_tables()                                //输出该database中的所有表
{
    
    for(auto it=tables.begin();it!=tables.end();it++)       //依次输出各个表名
    {
        cout<<it->first<<endl;
    }
}
