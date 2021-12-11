# 简易数据库实现——第一阶段
## 一、功能简介  
在大作业第一阶段，我们实现了简单的关系数据库。程序能够识别简单的sql语言，完成数据库信息的显示、删改、插入以及单表查询。
## 二、数据结构与封装
### 建立的类型
类型|含义
-|-
`class SQL`|每条SQL语句作为一个对象
`class sql_server`|sql服务器对象
`class Database`|数据库对象
`class Table`|数据表对象
### 数据封装层次
- `SQL`对象由一条sql语句字符串初始化，在类内去除标点后按空格分割成语句段存储在数组`words`中，通过`operator[]`让外界访问`words`数组中的内容。
- `sql_server`对象中包含存储数据库对象的map容器，以及记录当前数据库的标记`current`。
- `Database`对象中包含存储数据表对象的map容器
- `Table`对象中用一维数组`record`存储该表的全部数据，另有用于存储顺序列名信息的数组`col_name`以及列的类型信息的map容器`columns`。还有存储主键位置、行数、列数的成员`primary`、`rnum`、`cnum`。
- sql语句在执行的过程中，由`sql_server`、`Database`、`Table`对象逐层识别由`SQL`对象提供的语句信息，并逐层传入sql语句的作用对象。(数据库、数据表)
## 三、接口与实现
### SQL
1. 构造函数：用一条sql语句初始化`str`字符串，并调用`parser()`进一步处理。
2. `parser()`：将所有标点换成空格，单引号转换成双引号，将所有sql关键词转化位大写。(预处理)接着将字符串逐段切割存入数组`words`
3. `operator[]（int n）`：返回`words[n]`
4. `get_size()`：汇报`words`的字段数目。
### sql_server
1. `start()`：服务器运行的主程序，接受sql语句的输入。
2. `operate(SQL& sql)`：执行USE以及含DATABASE的指令，其余指令传入`current`标记的数据库中。
### Database
1. `operate(SQL& sql)`：识别表级别指令的关键字，调用相应表的正确接口将SQL对象传入。执行创建、删除表的指令。
2. `show_tables()`：输出现有表名，由`Database::operate`调用执行SHOW TABLES指令。
### Table
1. `Table(SQL &sql)`：执行CREATE TABLE语句，初始化`columns`,`col_name`,`cnum`,`primary`等成员。
2. `insert_into(SQL &sql)`：执行INERST INTO语句，将一行信息加入`record`的后面。注意`record`用一维数组顺序存储一张表的数据，其中列的顺序为CREATE TABLE时所指定的列顺序，行的顺序为INSERT的先后顺序。
3. `show_columns(SQL &sql)`：执行SHOW COLUMNS语句，输出顺序为列名字母序。
4. `delete_from(SQL &sql)`：执行DELETE语句。
5. `where_clause（SQL &sql）judge(string &str, int r)`：识别WHERE之后的条件语句。`where_clasue`内先将条件语句转化为后缀式，接着计算后缀式的结果。这个过程需要调用judge计算比较关系式的值(str为关系式，r为待判断的行的标号),再处理逻辑运算AND和OR。where_clause的选择结果存储在布尔数组`pick`中。
6. `update(SQL &sql)`：先调用`where_clause`得出选择结果，再更新对应行的数据。
7. `select(SQL &sql)`：先调用`where_clause`得出行选择结果，再识别FROM之前的语句得出列的选择结果。将被选出行的主键信息放入set中进行排序（这里需要识别主键的类型)，接着按主键大小顺序进行输出。
## 四、注意事项
- 本地读文件调试请define LOCAL
- 在存储字符串的时候并没有将两侧的双引号去掉，只是在select进行输出时将双引号隐藏。
- where_clause语句中，比较运算式里不能有空格，如`1+1=2`不能写作`1 + 1 = 2`
- 请一定注意`Table`对象中数据的存储顺序。
## 五、程序的运行环境
- macOS Mojave10.14.3  
Apple LLVM version 10.0.0 (clang-1000.10.44.4)  
Target: x86_64-apple-darwin18.2.0



