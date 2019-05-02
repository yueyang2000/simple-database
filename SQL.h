
#ifndef SQL_h
#define SQL_h
#include "Header.h"
class SQL
{
private:
    void parser();
    string str;
    vector<string> words;
public:
    SQL(string &s):str(s){parser();}
    string operator[](int n) const;
    int get_size(){return (int)words.size();}
};

#endif /* SQL_h */
