#include "Header.h"
#include "Table.h"
#include <stack>
#include <sstream>
#include <set>
#include <iomanip>
Table::Table(SQL& sql)
{
    int i=3;                                    //
    int counter=0;                              //列数统计
    while(true)                                 //一直循环，语句结束跳出
    {
        if(sql[i]=="")                          //如果sql语句的每个单词都已遍历完毕，就跳出循环
        {
            break;
        }
        else if(sql[i]=="PRIMARY")              //如果是主键关键字
        {
            auto pri=columns.find(sql[i+2]);    //sql[i + 2]就是主键名，pri就是主键对应的pair
            primary=pri->second.order;          //primary是主键对应的列的列位置
            pri_type=pri->second.type;          //pri_type是主键对应的列的列的数据类型
            i=i+3;                              //跳过三个词，读取关于下一个列的信息
            continue;
        }
        col_info newc;                          //建立新的列信息newc
        newc.order=counter;                     //列的顺序是目前的counter
        counter++;                              //建立新列后，counter加1
        string cname=sql[i++];                  //列名是sql[i]，然后读取下一个词
        col_name.push_back(cname);              //记录列名顺序
        newc.type=sql[i++];                     //列的数据类型是sql[i]，然后读取下一个词
        if(sql[i]=="NOT"&&sql[i+1]=="NULL")     //如果下两个词是NOT NULL
        {
            newc.not_null=true;                 //该列就设为not null
            i+=2;                               //跳过这两个词
        }
        else                                    //反之
            newc.not_null=false;                //该列不是not null
        columns[cname]=newc;                    //将(cname, newc)加入列的map
    }
    cnum=(int)columns.size();                   //列的数目是columns的size
    rnum=0;                                     //行数为零
}

void Table::insert_into(SQL &sql)               //执行insert into操作
{
    int value_pos=0;                            //记录value的位置
    for(int i=3;i<sql.get_size();i++)           //遍历sql的每一个单词
    {
        if(sql[i]=="VALUES")                    //如果找到了VALUES
        {
            value_pos=i;                        //就把value_pos设为该VALUE的位置
            break;                              //跳出循环
        }
    }
    string* temp=new string[cnum];              //新建cnum(列数)个string
    for(int i=0;i<cnum;i++){                    //初始置为空
        temp[i]="NULL";
    }
    for(int i=3;i<value_pos;i++)                //从第三个词开始，遍历VALUE之前的所有词
    {
        int c_pos=columns[sql[i]].order;        //c_pos是名为sql[i]的列的位置
        temp[c_pos]=sql[i+value_pos-2];         //把temp[c_pos]赋值为value之后对应的插入数据
    }
    for(int i=0;i<cnum;i++)                     //遍历所有的temp<string>，将其插入所有数据记录中
    {
        record.push_back(temp[i]);
    }
    rnum++;                                     //行数加一
    delete []temp;                              //删除为temp申请的内存
}

bool Table::judge(string &str,int r) {          //r是第几行，计算每个表达式的正确性
    string op;                                  //用来读取运算符'>''<''='
    for (int i = 0; i < str.size(); i++) {      //遍历str，遇到'<'，'>'，'='后就停下
        if (str[i] == '<' || str[i] == '=' || str[i] == '>') {
            op = str[i];
            str[i] = ' ';
            break;
        }
    }
    stringstream ss(str);                       //用str初始化ss stringstream
    string name, value;                         //要比较的列名和数值
    ss >> name >> value;                        //输入要比较的列名和数值
    int c = columns[name].order;                //c是列名为name的列的列顺序
    if (columns[name].type =="INT" || columns[name].type == "DOUBLE") {                 //如果类型是int或double
        if (op =="<") {                         //如果是'<'
            if (atof(record[r*cnum + c].c_str()) < atof(value.c_str())) return true;    //小于为true，反之为false
            else return false;
        }
        else if (op == "=") {                   //如果是'='
            if (atof(record[r*cnum + c].c_str()) == atof(value.c_str())) return true;   //等于为true，反之为false
            else return false;
        }
        else {                                  //如果是'>'
            if (atof(record[r*cnum + c].c_str()) > atof(value.c_str())) return true;    //大于为true，反之为false
            else return false;
        }
    }
    else {                                      //类型是CHAR
        if (op == "<") {                        //如果是'<'
            if (record[r*cnum + c] < value) return true;    //小于为true，反之为false
            else return false;
        }
        else if (op == "=") {                   //如果是'='
            if (record[r*cnum + c] == value) return true;   //等于为true，反之为false
            else return false;
        }
        else {                                  //如果是'>'
            if (record[r*cnum + c] > value) return true;    //大于为true，反之为false
            else return false;
        }
    }
}

void Table::where_clause(SQL &sql) {
    int n;
    for(n=0;n<sql.get_size();n++)               //找where的位置
    {
        if(sql[n]=="WHERE")
        {
            break;
        }
    }
    if(n==sql.get_size())                       //如果没有where
    {
        pick.clear();                           //那么清空pick
        for(int lp=0;lp<rnum;lp++)              //把所有行都置为ture
        {
            pick.push_back(true);
        }
        return;
    }
    string suff;                                    //suff为转成的后缀式
    map<string, int>p = { {"AND",1},{"OR",0} };     //map p中AND对应1，OR对应0
    stack<string> s;                                //转后缀式时用到的栈s
    for (int i = n + 1; i < sql.get_size(); i++) {  //从where后的第一个词开始遍历语句中的每一个词
        if (sql[i] == "AND" || sql[i] == "OR") {    //假如遇到了AND或OR
            if (s.empty()) {                        //若栈为空
                s.push(sql[i]);                     //把AND或OR放入栈中
            }
            else {                                  //若栈不为空
                while (!s.empty()&&p[s.top()] > p[sql[i]]) {    //当栈不为空且顶部的运算优先级高于当前的运算优先级
                    suff += s.top();                //把栈顶的词放入待完成的后缀式
                    suff += " ";                    //加一个空格
                    s.pop();                        //把栈顶词pop出来
                }
                s.push(sql[i]);                     //直到栈为空或者目前的运算符优先级比栈顶大，就把当前运算符放入栈顶
            }
        }
        else {                                      //如果不是运算符
            suff += sql[i];                         //就直接把该词放入待转后缀式
            suff += " ";                            //加空格
        }
    }
    while (!s.empty()) {                            //当栈不空时
        suff += s.top();                            //把所有的栈中的词放入待转后缀式中
        suff += " ";                                //加空格
        s.pop();
    }
    //cout << suff << endl;//后缀式
    pick.clear();                                   //清空原pick
    for (int i = 0; i < rnum; i++) {                //遍历所有的行
        stringstream ss(suff);                      //用转成的后缀式来初始化ss stringstream
        string temp;                                //用来读取当前的词
        stack<bool> cal;                            //
        while (ss >> temp) {                        //当ss没有结束的时候
            if (temp == "AND" || temp == "OR") {    //若temp是AND或OR
                bool t2 = cal.top(); cal.pop();     //t1是cal的栈顶
                bool t1 = cal.top(); cal.pop();     //t2是cal的顶部第二个
                if (temp == "AND") {                //若读入的是AND
                    if (t1&&t2) {                   //如果前两个都为true
                        cal.push(true);             //则在cal中放入true
                    }
                    else cal.push(false);           //否则放入false
                }
                else {                              //如果读入的是OR
                    if (t1 || t2) {                 //如果t1和t2有一为true
                        cal.push(true);             //在栈中放入true
                    }
                    else cal.push(false);           //否则放入false
                }
            }
            else {
                cal.push(judge(temp, i));           //第i行
                //cout<<"judge "<<(int)cal.top()<<endl;
            }
        }
        bool res = cal.top(); cal.pop();            //直到后缀式读完，读取cal栈顶的变量，弹出栈顶
        pick.push_back(res);                        //把cal栈顶放入pick中，决定是否选择这一行
        //cout<<"push_back "<<(int)res<<endl;
    }
    /*for (int i = 0; i < rnum; i++) {
        if (pick[i]) cout <<"yes" << " ";
        else cout << "no" << " ";
    }
    cout << endl;*/
}


void Table::delete_from(SQL &sql)                   //执行delete from操作
{
    where_clause(sql);                              //先用where_clause函数根据sql语句计算出pick
    int pos=0;                                      //pos记录当前所处理的数据序号(全部数据)
    for(int i=0;i<rnum;i++)                         //遍历所有的行
    {
        if(pick[i])                                 //如果对应的bool变量为真
        {
            for(int j=0;j<cnum;j++)                 //遍历所有的列，把该行的所有列删去
            {
                record.erase(record.begin()+pos);
                /*for(int i=0;i<(int)record.size();i++)
                {
                    cout<<record[i]<<' ';
                }
                cout<<endl;*/
            }
        }
        else                                        //如果对应的bool变量为假，则移动到下一行的开头
            pos+=cnum;
    }
    rnum--;                                         //删除一行后，行数-1
}

void Table::update(SQL &sql)                        //执行update操作
{
    where_clause(sql);                              //先用where_clause函数计算出对应的pick
    int pos=3;                                      //从第三个词开始
    while(sql[pos]!="WHERE")                        //如果不是WHERE关键字
    {
        string cname;                               //列名
        int i=0;
        while(sql[pos][i]!='=')                     //如果词中不是'='
        {
            cname+=sql[pos][i];                     //把字符加入cname直到出现'='
            i++;                                    //
        }
        string value;                               //数据
        i++;                                        //跳过'='
        while(i!=sql[pos].size())                   //从'='后开始遍历所有的字母
        {
            value+=sql[pos][i];                     //读入value
            i++;
        }
        int c=columns[cname].order;                 //c是该列的序号
        for(int r=0;r<rnum;r++)                     //遍历所有的行
        {
            if(pick[r])                             //若该行的pick为true
            {
                record[r*cnum+c]=value;             //把改行对应的列改掉
            }
        }
        pos++;                                      //直到关键字WHERE
    }
}

void Table::select(SQL &sql)                        //执行select操作
{
    where_clause(sql);                              //用where_clause函数处理sql语句，计算pick的值
    bool has_result=false;                          //判断是否有查询结果，初始为false
    for(int i=0;i<rnum;i++)                         //遍历所有的行，如果有结果，则将has_result赋为true
    {
        if(pick[i])
            has_result=true;
    }
    if(!has_result)                                 //如果没有结果，直接返回
        return;
    
    int pos=1;                                      //
    vector<bool> output;                            //判断对应的列是否输出
    for(int lp=0;lp<cnum;lp++)                      //遍历所有列
    {
        output.push_back(false);                    //初始化为所有都为false
    }
    while(sql[pos]!="FROM")                         //遍历sql语句中FROM之前的所有词
    {
        if(sql[pos]=="*")                           //如果是'*'，所有都为true，不再查找，直接跳出
        {
            for(int lp=0;lp<cnum;lp++)
            {
                output[lp]=true;
            }
            break;
        }
        else                                        //否则把对应的列设为true
        {
            output[columns[sql[pos]].order]=true;
        }
        pos++;                                      //跳到下一个词
    }
    for(int c=0;c<cnum;c++)                         //遍历所有的列，如果该列要输出，就输出列名和'\t'
    {
        if(output[c])
            cout<<col_name[c]<<'\t';
    }
    cout<<endl;                                     //换行后输出列内容
    
    if(pri_type=="CHAR"){                           //如果主键类型是char
    set<string> pri;                                //存主键
    for(int r=0;r<rnum;r++)                         //遍历所有行，如果该行对应为true，存入要输出的主键
    {
        if(pick[r])
        {
            pri.insert(record[r*cnum+primary]);
        }//把要输出的主键放入
    }
    for(auto it=pri.begin();it!=pri.end();it++){    //遍历所有的主键
        for(int r=0;r<rnum;r++){                    //遍历所有的行
            if(*it==record[r*cnum+primary]){        //如果要输出第r行
                for(int c=0;c<cnum;c++){            //遍历所有的列
                    if(output[c]){                  //如果该列要输出
                        if(columns[col_name[c]].type=="CHAR"){          //如果是char型
                            if(record[r*cnum+c]=="NULL"){               //如果是NULL，输出"NULL"
                                cout<<"NULL\t";
                            }
                            else{string temp;                           //否则
                            for(int i=1;i<record[r*cnum+c].size()-1;i++)//将要输出的内容读到temp，输出temp
                            {
                                temp+=record[r*cnum+c][i];
                            }
                            cout<<temp<<'\t';
                            }
                        }
                        else if(columns[col_name[c]].type=="DOUBLE"){   //如果是double型
                            if(record[r*cnum+c]=="NULL"){               //如果是NULL，输出"NULL"
                                cout<<"NULL\t";
                            }
                            else{cout<<fixed<<setprecision(4);          //否则输出x，x是对应列下的数据
                            double x=atof(record[r*cnum+c].c_str());
                            cout<<x<<'\t';
                            //std::cout << std::defaultfloat;
                            }
                        }
                         else cout<<record[r*cnum+c]<<'\t';             //否则是int型，直接输出
                        }
                }
            cout<<endl;                             //换行
            break;
            }
        }
    }
    }
    else{                                           //主键类型是int或double
    set<double> pri;                                //pri存放主键
    for(int r=0;r<rnum;r++)                         //遍历所有行，把要输出的主键放入pri
    {
        if(pick[r])
        {
            pri.insert(atof(record[r*cnum+primary].c_str()));
        }//把要输出的主键放入
    }
    for(auto it=pri.begin();it!=pri.end();it++){                        //遍历所有主键
        for(int r=0;r<rnum;r++){                    //遍历所有的行，找到要输出的行
            if(*it==atof(record[r*cnum+primary].c_str())){              //如果要输出第r行
                for(int c=0;c<cnum;c++){            //遍历所有的列
                    if(output[c]){                  //若该列要输出
                        if(columns[col_name[c]].type=="CHAR"){          //如果是char型
                            if(record[r*cnum+c]=="NULL"){               //如果是NULL，输出"NULL"
                                cout<<"NULL\t";
                            }
                            else{string temp;       //反之，将内容输入temp中，将temp输出
                            for(int i=1;i<record[r*cnum+c].size()-1;i++)
                            {
                                temp+=record[r*cnum+c][i];
                            }
                            cout<<temp<<'\t';
                            }
                        }
                        else if(columns[col_name[c]].type=="DOUBLE"){   //如果是double型
                            if(record[r*cnum+c]=="NULL"){               //如果是NULL，输出"NULL"
                                cout<<"NULL\t";
                            }
                            else{cout<<fixed<<setprecision(4);          //反之，输出数(转换成浮点数)
                            double x=atof(record[r*cnum+c].c_str());
                            cout<<x<<'\t';
                            //std::cout << std::defaultfloat;
                            }
                        }
                         else cout<<record[r*cnum+c]<<'\t';             //如果是int型，直接输出这个数
                        }
                }
            cout<<endl;                             //换行
            break;
            }
        }
    }
    }
}

void Table::show_columns()                      //输出列
{
    cout<<"Field\tType\tNull\tKey\tDefault\tExtra\n";
    for(int i=0;i<cnum;i++)                     //遍历所有列
    {
        auto it=columns.find(col_name[i]);      //找到对应列名所对应的列
        cout<<it->first<<'\t';                  //输出列名
        if(it->second.type=="INT")              //如果数据类型是int型
        {
            cout<<"int(11)\t";                  //输出"int(11)\t"
        }
        else if(it->second.type=="CHAR")        //如果数据类型是char型
        {
            cout<<"char(1)\t";                  //输出"char(1)\t"
        }
        else                                    //如果数据类型是double型
            cout<<"double\t";                   //输出"double\t"
        if(it->second.not_null)                 //如果数据不为NULL
        {
            cout<<"NO"<<'\t';                   //输出"NO"
        }
        else                                    //反之输出"YES"
            cout<<"YES"<<'\t';
        if(it->second.order==primary)           //如果是主键，就输出"PRI"
        {
            cout<<"PRI"<<'\t';
        }
        else                                    //若不是主键，输出'\t'
            cout<<'\t';
        cout<<"NULL"<<endl;                     //输出"NULL"，换行
    }
}
