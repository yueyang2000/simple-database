//
//  sql_server.h
//  database
//
//  Created by 乐阳 on 2019/4/24.
//  Copyright © 2019 乐阳. All rights reserved.
//

#ifndef sql_server_h
#define sql_server_h
#include "Database.h"
class sql_server
{
private:
    map<string,Database> db;
    map<string,Database>::iterator current;
public:
    void start();
    void operate(SQL& sql);
};

#endif /* sql_server_h */
