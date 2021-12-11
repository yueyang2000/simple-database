
#include "Header.h"
#include "sql_server.h"
#include "SQL.h"
#include "Database.h"

void sql_server::start()
{
    while(true)
    {
        //cout<<"mysql>";
        string temp;
        getline(cin,temp);                          //读入一行指令
        if(temp=="quit"||temp=="")                  //若为quit
        {
            //cout<<"BYE!\n";
            return;                                 //程序结束
        }
        else                                        //否则
        {
            SQL sql(temp);                          //创建sql对象，对指令进行处理
            operate(sql);                           //对处理后的sql指令进行相应操作
        }
    }
}

void sql_server::operate(SQL& sql)
{
    if(sql[0]=="USE")                               //如果是use操作，将迭代器指向数据库名称为sql[1]的pair
    {
        current=db.find(sql[1]);
    }
    else if(sql[1]=="DATABASE")                     //如果是database操作
    {
        if(sql[0]=="CREATE")                        //如果是create操作
        {
            Database temp;                          //建立新的数据库
            db[sql[2]]=temp;                        //将pair:(sql[2], new database)加入db map
            current=db.find(sql[2]);                //将迭代器current指向new database
            //cout<<"create database "<<sql[2]<<endl;
        }
        else if(sql[0]=="DROP")                     //如果是drop操作
        {
            //cout<<"drop database "<<sql[2]<<endl;
            db.erase(sql[2]);                       //从map中删除名为sql[2]的database
        }
    }
    else if(sql[0]=="SHOW"&&sql[1]=="DATABASES")    //如果是show databases的操作
    {
        cout<<"Database\n";
        for(auto it=db.begin();it!=db.end();it++)   //遍历db
        {
            cout<<it->first<<endl;                  //输出所有的databases的name
        }
    }
    else if(sql[0]=="SHOW"&&sql[1]=="TABLES")       //如果是show table的操作
    {
        cout<<"Tables_in_"<<current->first<<endl;
        current->second.show_tables();              //调用show_tables()输出current所指的database的所有tables
    }
    else
    {
        current->second.operate(sql);               //否则，不是database级操作，将sql语句传入current所指的pair中的database类的operate(SQL&)函数中
    }
}
