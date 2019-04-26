//
//  main.cpp
//  database
//
//  Created by 乐阳 on 2019/4/23.
//  Copyright © 2019 乐阳. All rights reserved.
//

#include "Header.h"
#include "sql_server.h"
using namespace std;
int main(int argc, const char * argv[]) {
    sql_server mysql;
    mysql.start();
    return 0;
}
