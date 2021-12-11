
#include "SQL.h"
#include <sstream>
#include <cctype>
#include <set>
void SQL::parser()                                                          //将str处理为短字符串存入words
{
    static set<string> keywords                                             //set容器：装有SQL语句的关键字(均为大写)
    {
        "CREATE","DATABASE","DATABASES","USE","WHERE","SHOW","TABLE",
        "TABLES","INT","DOUBLE","CHAR","NOT","NULL","INSERT","INTO",
        "VALUES","DELETE","FROM","UPDATE","SET","SELECT","COLUMNS",
        "DROP","AND","OR"
    };
    for(auto it=str.begin();it!=str.end();it++)                             //遍历str的每个字符，遇到',''('')'';'就转为空格
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
    stringstream ss(str);                                                   //用处理后的str初始化一个stringstream
    string temp;                                                            //暂存输入字符串
    while(ss>>temp)
    {
        if(isalpha(temp[0]))                                                //判断：如果第一位是字母
        {
            string copy=temp;                                               //copy是temp的复制
            for(auto it=temp.begin();it!=temp.end();it++)                   //把temp的每个字母都变成大写
            {
                *it=toupper(*it);
            }
            if(keywords.find(temp)!=keywords.end())                         //如果是关键字
            {
                words.push_back(temp);                                      //就把生成的全为大写的关键字放入vector<string>words中
            }
            else
            {
                words.push_back(copy);                                      //如果不是关键字，那么将原型放入vector<string>words中
            }

        }
        else
            words.push_back(temp);                                          //如果第一位不是字母，则肯定不是关键字，将temp放入vector<string>words中
    }
}

string SQL::operator[](int n)const                                          //运算符重载：返回words的第n个单词
{
    if(n>=(int)words.size())                                                //如果越界，则输出"operator[] RE!\n"，返回空字符串;
    {
        //cout<<"operator[] RE!\n";
        return "";
    }
    else
    {
        return words[n];
    }
}

