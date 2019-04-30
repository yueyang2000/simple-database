//
//  sql_server.cpp
//  database
//
//  Created by 乐阳 on 2019/4/24.
//  Copyright © 2019 乐阳. All rights reserved.
//
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
        getline(cin,temp);
        //cout<<temp<<endl;
        if(temp=="quit"||temp=="")
        {
            //cout<<"BYE!\n";
            return;
        }
        else
        {
            SQL sql(temp);
            operate(sql);
        }
    }
}

void sql_server::operate(SQL& sql)
{
    if(sql[0]=="USE")
    {
        current=db.find(sql[1]);
    }
    else if(sql[1]=="DATABASE")//判断是否为database操作
    {
        if(sql[0]=="CREATE")
        {
            Database temp;
            db[sql[2]]=temp;
            current=db.find(sql[2]);
            //cout<<"create database "<<sql[2]<<endl;
        }
        else if(sql[0]=="DROP")
        {
            //cout<<"drop database "<<sql[2]<<endl;
            db.erase(sql[2]);
        }
    }
    else if(sql[0]=="SHOW"&&sql[1]=="DATABASES")
    {
        cout<<"Database\n";
        for(auto it=db.begin();it!=db.end();it++)
        {
            cout<<it->first<<endl;
        }
    }
    else if(sql[0]=="SHOW"&&sql[1]=="TABLES")
    {
        current->second.show_tables();
    }
    else
    {
        current->second.operate(sql);
    }
}
