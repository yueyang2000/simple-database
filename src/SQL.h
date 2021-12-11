
#ifndef SQL_h
#define SQL_h
#include "Header.h"
class SQL
{
private:
    void parser();                                          //将str处理为短字符串存入words
    string str;                                             //str初始化为当前SQL语句的指令
    vector<string> words;                                   //words存储将str处理后的短字符串
public:
    SQL(string &s):str(s){parser();}                        //构造函数在初始化SQL对象时，调用parser()函数将str处理为短字符串存入words
    string operator[](int n) const;                         //下标重载，返回words中的第n个单词
    int get_size(){return (int)words.size();}               //get_size函数返回words中的字符串数量
};

#endif /* SQL_h */
