//#pragma once
//#ifndef _CRWMYSQL_H
//#define _CRWMYSQL_H
//#include <mysql.h>
//#include "winsock.h"
//
////��¼��mysql - u root - p ����
////�������ݿ⣺ create database test;
////�鿴���������ݿ⣺show databases;
////ʹ�ô��������ݿ⣺use test;
////�鿴��ǰʹ�õ����ݿ⣺select database();
////�������ݱ�create table students(
////	id int(4) primary key auto_increment, //��Ϊ���� �Զ�����  
////	name varchar(40) not null,
////	age int(4) not null,
////	class varchar(40) not null);
////�鿴���������ݱ�desc students;
////������ݣ�insert into student(name, age, class) values('����', '����', '�༶');
////��ѯ���ݣ�select* from students;
////ɾ�����ݣ�delete from student where name = '����'
////�������ݣ�update student set age = '������', class = '�°༶' where name = '����';
////ɾ�����ݿ⣺drop database test;
//class CRWMysql
//{
//public:
//	CRWMysql();
//	~CRWMysql();
//public:
//	BOOL MySQL_InitDataBase(CString strlocal, CString strroot, CString strpass, CString strdia, int nport); // ��ʼ��MySQL���ݿ�
//	BOOL MySQL_CreateTable(CString sql_create_table);//������
//	
//	BOOL MySQL_SelectData(char* sql_select);//��ѯ����
//
//	BOOL MySQL_InsertData(char* ctable, char* cfield, char* cvalue);//��������
//	BOOL MySQL_DeleteData(char* ctable, char* cfield, char* cvalue);//ɾ������
//
//	void CloseMySQL();
//
//private:
//	MYSQL m_sqlCon;
//};
//
//#endif // !_CRWMYSQL_H
//
