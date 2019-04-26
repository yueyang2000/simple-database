#  简易数据库
## 创建的类型
- SQL：负责接收一个sql语句并切割
- sql_server: 服务器对象，包含一个存数据库的容器负责执行数据库级别的语句，并将表级别语句传入正确的数据库
- Database: 数据库对象，负责执行表级别的语句，调用对应的表操作接口
- Table: 表对象，具体实现各类表操作的接口

## 已基本实现所有功能（还需调试）

## 初步debug完成的功能
- CREATE DATABASE
- DROP DATABASE
- USE
- SHOW DATABASES
- CREATE TABLE
- SHOW COLUMNS
## 革命尚未成功，同志仍需努力
