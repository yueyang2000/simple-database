//
//  Table.cpp
//  database
//
//  Created by 乐阳 on 2019/4/25.
//  Copyright © 2019 乐阳. All rights reserved.
//

#include "Header.h"
#include "Table.h"
#include<stack>
#include<sstream>
#include < cstdlib > 

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
		else if (op == "+") {
			if (record[r*cnum + c] == value) return true;
			else return false;
		}
		else {
			if (record[r*cnum + c] > value) return true;
			else return false;
		}
	}
}
void Table::where_clause(SQL &sql, int n) {//where的位置
	string suff;//转后缀式
	map<string, int>p = { {"and",1},{"or",0} };
	stack<string> s;
	for (int i = n + 1; i < sql.get_size(); i++) {
		if (sql[i] == "and" || sql[i] == "or") {
			if (s.empty()) {
				s.push(sql[i]);
			}
			else {
				while (!s.empty&&p[s.top] > p[sql[i]]) {
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
			if (temp == "and" || temp == "or") {
				bool t2 = cal.top(); cal.pop();
				bool t1 = cal.top(); cal.pop();
				if (temp == "and") {
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
			}
		}
		bool res = cal.top(); cal.pop();
		pick.push_back(res);
	}
	for (int i = 0; i < rnum; i++) {
		if (pick[i]) cout << "yes" << " ";
		else cout << "no" << " ";
	}
	cout << endl;
}



