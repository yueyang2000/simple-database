//
//  Database.h
//  database
//
//  Created by 乐阳 on 2019/4/23.
//  Copyright © 2019 乐阳. All rights reserved.
//

#ifndef Database_h
#define Database_h
#include "Table.h"
class Database
{
private:
    map<string,Table> tables;
public:
    void operate(SQL &sql);
    void show();
};
#endif /* Database_h */
