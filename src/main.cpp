
#include "Header.h"
#include "sql_server.h"
using namespace std;
//#define LOCAL

int main(int argc, const char * argv[]) {
#ifdef LOCAL
    freopen("input.txt","r",stdin);
#endif
    sql_server mysql;                           //mysql中储存若干database
    mysql.start();                              //start
#ifdef LOCAL
    fclose(stdin);
#endif
    return 0;
}
