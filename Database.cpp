//
//  Database.cpp
//  database
//
//  Created by 乐阳 on 2019/4/24.
//  Copyright © 2019 乐阳. All rights reserved.
//

#include "Header.h"
#include "Database.h"

void Database::operate(SQL &sql)
{
    if(sql[0]=="CREATE"&&sql[1]=="TABLE")
    {
        Table newt(sql);
        tables[sql[2]]=newt;
    }
    else if(sql[0]=="INSERT"&&sql[1]=="INTO")
    {
        auto it=tables.find(sql[2]);
        it->second.insert_into(sql);
    }
    else if(sql[0]=="DELETE"&&sql[1]=="FROM")
    {
        auto it=tables.find(sql[2]);
        it->second.delete_from(sql);
    }
    else if(sql[0]=="UPDATE")
    {
        auto it=tables.find(sql[1]);
        it->second.update(sql);
    }
    else if(sql[0]=="SELECT")
    {
        auto it=tables.find(sql[sql.get_size()-1]);
        it->second.select(sql);
    }
    else if(sql[0]=="DROP"&&sql[1]=="TABLE")
    {
        tables.erase(sql[2]);
    }
}
void Database::show_tables()
{
    cout<<"has tables: ";
    for(auto it=tables.begin();it!=tables.end();it++)
    {
        cout<<it->first<<' ';
    }
    cout<<endl;
}
