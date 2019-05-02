

#ifndef Database_h
#define Database_h
#include "Table.h"
class Database
{
private:
    map<string,Table> tables;
public:
    void operate(SQL &sql);
    void show_tables();
};
#endif /* Database_h */
