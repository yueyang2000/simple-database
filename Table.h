//
//  Table.h
//  database
//
//  Created by 乐阳 on 2019/4/23.
//  Copyright © 2019 乐阳. All rights reserved.
//

#ifndef Table_h
#define Table_h
#include "SQL.h"
struct col_info
{
    int order;
    string type;
    bool not_null;
};
class Table
{
private:
    int cnum;//列数
    int rnum;//行数
    int primary;//主键位置
    vector <string> record;//所有数据
    vector <string> col_name;
    map<string,col_info> columns;//列信息
    vector<bool> pick;//WhereClause语句筛选
public:
    Table(){}
    Table(SQL &sql);
    void insert_into(SQL &sql);
    void show_table(SQL &sql);
    void show_columns();
    void delete_from(SQL &sql);
    void update(SQL &sql);
    void select(SQL &sql);
    void where_clause(SQL &sql);
        bool judge(string &str, int r);
};


#endif /* Table_h */
