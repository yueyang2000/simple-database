#  简易数据库
##创建的类型
- SQL：负责接收一个sql语句并切割
- sql_server: 服务器对象，包含一个存数据库的容器负责执行数据库级别的语句，并将表级别语句传入正确的数据库
- Database: 数据库对象，负责执行表级别的语句，调用对应的表操作接口
- Table: 表对象，具体实现各类表操作的接口

##已经实现功能（还需调试）
- CREATE DATABASE
- DROP DATABASE
- USE
- SHOW DATABASES
- CREATE TABLE
- DROP TABLE
- INSERT INTO

##未实现的功能见作业文档以及Table类未实现的方法

