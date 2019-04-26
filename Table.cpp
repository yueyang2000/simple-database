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
    while(true)
    {
        if(sql[i]=="PRIMARY")
        {
            for(auto it=col.begin();it!=col.end();it++)
            {
                if(it->name==sql[i+2])
                {
                    primary=(int)(it-col.begin());
                    cout<<"primary key:"<<primary<<"th colume\n";
                    break;
                }
            }
            break;
        }
        else if(sql[i]=="")
        {
            break;
        }
        colume newc;
        newc.name=sql[i++];
        newc.type=sql[i++];
        if(sql[i]=="NOT"&&sql[i+1]=="NULL")
        {
            newc.not_null=true;
            i+=2;
        }
        else
            newc.not_null=false;
        col.push_back(newc);
    }
    cnum=(int)col.size();
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
        for(c_pos=0;c_pos<cnum;c_pos++)
        {
            if(col[c_pos].name==sql[i])
                break;
        }
        temp[c_pos]=sql[i+value_pos-2];
    }
    for(int i=0;i<cnum;i++)
    {
        record.push_back(temp[i]);
    }
}



