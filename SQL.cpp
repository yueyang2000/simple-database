//
//  SQL.cpp
//  database
//
//  Created by 乐阳 on 2019/4/24.
//  Copyright © 2019 乐阳. All rights reserved.
//
#include "SQL.h"
#include <sstream>
#include <cctype>
#include <set>
void SQL::parser()
{
    static set<string> keywords
    {
        "CREATE","DATABASE","DATABASES","USE","WHERE","SHOW","TABLE",
        "TABLES","INT","DOUBLE","CHAR","NOT","NULL","INSERT","INTO",
        "VALUES","DELETE","FROM","UPDATE","SET","SELECT","COLUMNS",
        "DROP","AND","OR"
    };
    for(auto it=str.begin();it!=str.end();it++)
    {
        if(*it==','||*it=='('||*it==')'||*it==';')
        {
            *it=' ';
        }
        else if(*it=='\'')
        {
            *it='\"';
        }
    }
    stringstream ss(str);
    string temp;
    while(ss>>temp)
    {
        if(isalpha(temp[0]))//大小写相关
        {
            string copy=temp;
            for(auto it=temp.begin();it!=temp.end();it++)
            {
                *it=toupper(*it);
            }
            if(keywords.find(temp)!=keywords.end())
            {
                words.push_back(temp);
            }
            else
            {
                words.push_back(copy);
            }

        }
        else
            words.push_back(temp);
    }
    /*for(int lp=0;lp<words.size();lp++)
        cout<<words[lp]<<' ';
    cout<<endl;*/
}

string SQL::operator[](int n)const
{
    if(n>=(int)words.size())
    {
        //cout<<"operator[] RE!\n";
        return "";
    }
    else
    {
        return words[n];
    }
}

