//
//  Table.cpp
//  database
//
//  Created by 乐阳 on 2019/4/25.
//  Copyright © 2019 乐阳. All rights reserved.
//

#include "Header.h"
#include "Table.h"

Table::Table(SQL& sql)
{
    int i=3;
    int counter=0;//列数统计
    while(true)
    {
        if(sql[i]=="PRIMARY")
        {
            auto pri=columns.find(sql[i+2]);
            primary=pri->second.order;
        }
        else if(sql[i]=="")
        {
            break;
        }
        col_info newc;
        newc.order=counter;
        counter++;
        string cname=sql[i++];
        newc.type=sql[i++];
        if(sql[i]=="NOT"&&sql[i+1]=="NULL")
        {
            newc.not_null=true;
            i+=2;
        }
        else
            newc.not_null=false;
        columns[cname]=newc;
    }
    cnum=(int)columns.size();
    rnum=0;
}

void Table::insert_into(SQL &sql)
{
    int value_pos=0;
    for(int i=3;i<sql.get_size();i++)
    {
        if(sql[i]=="VALUE")
        {
            value_pos=i;
            break;
        }
    }
    string temp[cnum];
    for(int i=3;i<value_pos;i++)
    {
        int c_pos=-1;
        for(auto it=columns.begin();it!=columns.end();it++)
        {
            if(it->first==sql[i])
            {
                c_pos=it->second.order;
            }
        }
        temp[c_pos]=sql[i+value_pos-2];
    }
    for(int i=0;i<cnum;i++)
    {
        record.push_back(temp[i]);
    }
}



