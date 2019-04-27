//
//  Table.cpp
//  database
//
//  Created by 乐阳 on 2019/4/25.
//  Copyright © 2019 乐阳. All rights reserved.
//

#include "Header.h"
#include "Table.h"
#include <stack>
#include <sstream>
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
            break;
        }
        else if(sql[i]=="")
        {
            break;
        }
        col_info newc;
        newc.order=counter;
        counter++;
        string cname=sql[i++];
        col_name.push_back(cname);//记录列名顺序
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
    for(int lp=0;lp<cnum;lp++)
    {
        cout<<col_name[lp]<<' ';
    }
    cout<<endl;
}

void Table::insert_into(SQL &sql)
{
    int value_pos=0;
    for(int i=3;i<sql.get_size();i++)
    {
        if(sql[i]=="VALUES")
        {
            value_pos=i;
            break;
        }
    }
    string* temp=new string[cnum];
    for(int i=3;i<value_pos;i++)
    {
        int c_pos=columns[sql[i]].order;
        temp[c_pos]=sql[i+value_pos-2];
    }
    for(int i=0;i<cnum;i++)
    {
        //cout<<"insert "<<temp[i]<<endl;
        record.push_back(temp[i]);
    }
    rnum++;
    delete []temp;

}

bool Table::judge(string &str,int r) {//r是第几行 //计算每个表达式的正确性
    string op;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '<' || str[i] == '=' || str[i] == '>') {
            op = str[i];
            str[i] = ' ';
            break;
        }
    }
    stringstream ss(str);
    string name, value;
    ss >> name >> value;//要比较的列名和数值
    /*cout<<name<<' '<<value<<endl;
    for(auto it=columns.begin();it!=columns.end();it++)
    {
        cout<<it->first<<it->second.order<<' '<<it->second.type<<endl;
    }*/
    int c = columns[name].order;
    if (columns[name].type =="INT" || columns[name].type == "DOUBLE") {//如果类型是int或double
        if (op =="<") {
            if (atof(record[r*cnum + c].c_str()) < atof(value.c_str())) return true;
            else return false;
        }
        else if (op == "=") {
            if (atof(record[r*cnum + c].c_str()) == atof(value.c_str())) return true;
            else return false;
        }
        else {
            if (atof(record[r*cnum + c].c_str()) > atof(value.c_str())) return true;
            else return false;
        }
    }
    else {//类型是CHAR
        if (op == "<") {
            if (record[r*cnum + c] < value) return true;
            else return false;
        }
        else if (op == "=") {
            if (record[r*cnum + c] == value) return true;
            else return false;
        }
        else {
            if (record[r*cnum + c] > value) return true;
            else return false;
        }
    }
}
void Table::where_clause(SQL &sql) {//where的位置
    int n;
    for(n=0;n<sql.get_size();n++)
    {
        if(sql[n]=="WHERE")
        {
            break;
        }
    }
    if(n==sql.get_size())
    {
        pick.clear();
        for(int lp=0;lp<rnum;lp++)
        {
            pick.push_back(true);
        }
        return;
    }
    string suff;//转后缀式
    map<string, int>p = { {"AND",1},{"OR",0} };
    stack<string> s;
    for (int i = n + 1; i < sql.get_size(); i++) {
        if (sql[i] == "AND" || sql[i] == "OR") {
            if (s.empty()) {
                s.push(sql[i]);
            }
            else {
                while (!s.empty()&&p[s.top()] > p[sql[i]]) {
                    suff += s.top();
                    suff += " ";
                    s.pop();
                }
                s.push(sql[i]);
            }
        }
        else {//不是运算符
            suff += sql[i];
            suff += " ";
        }
    }
    while (!s.empty()) {
        suff += s.top();
        suff += " ";
        s.pop();
    }
    cout << suff << endl;//后缀式
    pick.clear();//清空原pick
    for (int i = 0; i < rnum; i++) {//每行循环
        stringstream ss(suff);
        string temp;
        stack<bool> cal;
        while (ss >> temp) {
            //cout<<temp<<endl;
            if (temp == "AND" || temp == "OR") {
                bool t2 = cal.top(); cal.pop();
                bool t1 = cal.top(); cal.pop();
                if (temp == "AND") {
                    if (t1&&t2) {
                        cal.push(true);
                    }
                    else cal.push(false);
                }
                else {
                    if (t1 || t2) {
                        cal.push(true);
                    }
                    else cal.push(false);
                }
            }
            else {
                cal.push(judge(temp, i));//第i行
                //cout<<"judge "<<(int)cal.top()<<endl;
            }
        }
        bool res = cal.top(); cal.pop();
        pick.push_back(res);
        //cout<<"push_back "<<(int)res<<endl;
    }
    for (int i = 0; i < rnum; i++) {
        if (pick[i]) cout << i<<"yes" << " ";
        else cout << "no" << " ";
    }
    cout << endl;
}


void Table::delete_from(SQL &sql)
{
    where_clause(sql);
    int pos=0;
    for(int i=0;i<rnum;i++)
    {
        if(pick[i])
        {
            for(int j=0;j<cnum;j++)
            {
                record.erase(record.begin()+pos);
                /*for(int i=0;i<(int)record.size();i++)
                {
                    cout<<record[i]<<' ';
                }
                cout<<endl;*/
            }
        }
        else
            pos+=cnum;
    }
    rnum--;
}
void Table::update(SQL &sql)
{
    where_clause(sql);
    int pos=3;
    while(sql[pos]!="WHERE")
    {
        string cname;
        int i=0;
        while(sql[pos][i]!='=')
        {
            cname+=sql[pos][i];
            i++;
        }
        string value;
        i++;
        while(i!=sql[pos].size())
        {
            value+=sql[pos][i];
            i++;
        }
        cout<<cname<<' '<<value<<endl;
        int c=columns[cname].order;
        for(int r=0;r<rnum;r++)
        {
            if(pick[r])
            {
                record[r*cnum+c]=value;
            }
        }
        pos++;
    }
}
void Table::select(SQL &sql)
{
    where_clause(sql);
    int pos=1;
    vector<bool> output;
    for(int lp=0;lp<cnum;lp++)
    {
        output.push_back(false);
    }
    while(sql[pos]!="FROM")
    {
        if(sql[pos]=="*")
        {
            for(int lp=0;lp<cnum;lp++)
            {
                output[lp]=true;
            }
            break;
        }
        else
        {
            output[columns[sql[pos]].order]=true;
        }
        pos++;
    }
    for(int i=0;i<cnum;i++)
    {
        if(output[i])
            cout<<col_name[i]<<'\t';
    }
    cout<<endl;
    for(int r=0;r<rnum;r++)
    {
        if(pick[r])
        {
            for(int c=0;c<cnum;c++)
            {
                if(output[c])
                {
                    cout<<record[r*cnum+c]<<'\t';
                }
            }
            cout<<endl;
        }
    }
}

void Table::show_columns()
{
    cout<<"Field    Type    Null    Key    Default    Extra\n";
    for(int c=0;c<cnum;c++)
    {
        cout<<col_name[c]<<'\t'<<columns[col_name[c]].type<<'\t';
        if(columns[col_name[c]].not_null)
        {
            cout<<"NO"<<'\t';
        }
        else
            cout<<"YES"<<'\t';
        if(c==primary)
        {
            cout<<"PRI"<<'\t';
        }
        else
            cout<<'\t';
        cout<<"NULL"<<endl;
    }
}
