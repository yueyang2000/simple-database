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
        cout<<"mysql>";
        string temp;
        getline(cin,temp);
        if(temp=="quit")
        {
            cout<<"BYE!\n";
            return;
        }
        else
        {
            SQL sql(temp);
            //operate(sql);
        }
    }
}

void sql_server::operate(SQL& sql)
{
    if(sql[1]=="DATABASE")//判断是否为database操作
    {
        if(sql[0]=="CREATE")
        {
            Database temp;
            db[sql[2]]=temp;
            current=db.find(sql[2]);
        }
        else if(sql[0]=="DROP")
        {
            db.erase(sql[2]);
        }
        else if(sql[0]=="USE")
        {
            current=db.find(sql[2]);
        }
        else if(sql[0]=="SHOW")
        {
            auto it=db.find(sql[2]);
            it->second.show();
        }
    }
    else
    {
        current->second.operate(sql);
    }
}
