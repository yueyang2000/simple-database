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
struct colume
{
    string name;
    string type;
    bool not_null;
};
class Table
{
private:
    int cnum;
    int rnum;
    int primary;
    vector <colume> col;
    vector <string> record;
public:
    Table(){}
    Table(SQL &sql);
    void insert_into(SQL &sql);
    void delete_from(SQL &sql){}
    void update(SQL &sql){}
    void select(SQL &sql){}
};


#endif /* Table_h */
