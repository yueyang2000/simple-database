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
#define LOCAL

int main(int argc, const char * argv[]) {
#ifdef LOCAL
    freopen("/Users/yueyang/yiqunyang/大一下/面向对象的程序设计基础/database/database/input.txt","r",stdin);
    /*freopen("/Users/yueyang/yiqunyang/大一下/面向对象的程序设计基础/database/database/output.txt","w",stdout);*/
    
#endif
    sql_server mysql;
    mysql.start();
#ifdef LOCAL
    fclose(stdin);
    //fclose(stdout);
#endif
    return 0;
}
