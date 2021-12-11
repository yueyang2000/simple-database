
#ifndef Table_h
#define Table_h
#include "SQL.h"
struct col_info                                     //用来储存每列的信息
{
    int order;                                      //在表中的顺序
    string type;                                    //本列的数据类型
    bool not_null;                                  //是否是非空
};
class Table                                         //Table类
{
private:
    int cnum;                                       //表的列数
    int rnum;                                       //表的行数
    int primary;                                    //主键位置
    string pri_type;                                //主键类型
    vector <string> record;                         //所有数据
    vector <string> col_name;                       //所有列名
    map<string,col_info> columns;                   //列信息，列名和列信息对应
    vector<bool> pick;                              //WhereClause语句筛选
public:
    Table(){}                                       //默认构造函数
    Table(SQL &sql);                                //带参数的Table构造函数
    void insert_into(SQL &sql);                     //将SQL语句传入，执行insert into操作
    void show_columns();                            //将SQL语句传入，执行show columns操作
    void delete_from(SQL &sql);                     //将SQL语句传入，执行delete from操作
    void update(SQL &sql);                          //将SQL语句传入，执行update操作
    void select(SQL &sql);                          //将SQL语句传入，执行selete操作
    void where_clause(SQL &sql);                    //将SQL语句传入，实现多条件whereClause子句
    bool judge(string &str, int r);
};


#endif /* Table_h */
