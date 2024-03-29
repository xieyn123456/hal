//#include "pch.h"
//#include "CRWMysql.h"
//
//CRWMysql::CRWMysql()
//{
//}
//
//CRWMysql::~CRWMysql()
//{
//}
//
//	// strlocal:服务器地址，可以直接填入IP；
//	// strroot:账号；
//	// strpass:密码；
//	// strdia:数据库名；
//	// 3306:网络端口
//BOOL CRWMysql::MySQL_InitDataBase(CString strlocal, CString strroot, CString strpass, CString strdia, int nport)
//{
//	USES_CONVERSION;
//	mysql_init(&m_sqlCon);
//	if (NULL == mysql_real_connect(&m_sqlCon, T2A(strlocal), T2A(strroot), T2A(strpass), T2A(strdia), nport, NULL, 0))
//	{
//		AfxMessageBox(_T("数据库连接失败！"));
//		mysql_close(&m_sqlCon);
//		return FALSE;
//	}
//	else // MySQL连接成功则继续访问数据库
//	{
//		AfxMessageBox(_T("数据库连接成功！"));
//
//		mysql_set_character_set(&m_sqlCon, "gb2312"); //设置字节符，防止中文乱码.
//		return TRUE;
//	}
//}
//BOOL CRWMysql::MySQL_CreateTable(CString sql_create_table)
//{
//	//表格名称UserInfo，表格参数定义
//	/*CString sql_create_table = _T("CREATE TABLE IF NOT EXISTS UserInfo(\
//		id       INT PRIMARY KEY NOT NULL,\
//		Account  VARCHAR(20) NOT NULL UNIQUE,\
//		Pass     VARCHAR(20)    NOT NULL\
//		);\
//		");*/
//	char buffer[512];
//	wcstombs(buffer, sql_create_table.GetBuffer(), sizeof(buffer));
//	int result = mysql_query(&m_sqlCon, buffer); // 执行查询语句
//	sql_create_table.ReleaseBuffer();
//	if (result != 0)
//	{
//		AfxMessageBox(_T("建表失败！"));
//		mysql_close(&m_sqlCon);
//		return FALSE;
//	}
//	//AfxMessageBox(_T("数据库中有表"));
//	return TRUE;
//}
//
//BOOL CRWMysql::MySQL_SelectData(char* sql_select)
//{
//	// 查询操作之前先清空list control中的所有内容
//	//m_List.DeleteAllItems(); //清空所有表项
//		MYSQL_RES* res; // 声明数据查询结果
//		MYSQL_ROW row; // 取每行结果的对象
//		mysql_real_query(&m_sqlCon, sql_select, strlen(sql_select)); // 查询函数，语句用字符串表示
//		res = mysql_store_result(&m_sqlCon); // 得到查询结果
//
//		//重复读取行，把数据放入列表中，直到row为NULL
//		//
//		int selectCount = 0; // 记录查询结果的个数，若查询结果为空，也返回0
//		while (row = mysql_fetch_row(res))
//		{
//			CString strId, strAccount, strPass;
//			strId = row[0];
//			strAccount = row[1];
//			strPass = row[2];
//
//			// 在list control中插入数据
//			/*m_List.InsertItem(selectCount, strId);
//			m_List.SetItemText(selectCount, 1, strAccount);
//			m_List.SetItemText(selectCount, 2, strPass);*/
//
//			selectCount++;
//		}
//		mysql_close(&m_sqlCon);
//
//		if (selectCount == 0) return FALSE; // 没查询到数据
//		else return TRUE;
//}
//// MySQL插入数据
//BOOL CRWMysql::MySQL_InsertData(char* ctable, char* cfield, char* cvalue)
//{
//	CString strSQL;
//	strSQL.Format(_T("insert into \'%s\'(\'%s\')' values(/'%s/')"), ctable, cfield, cvalue);
//	if (mysql_query(&m_sqlCon, (char*)(LPCTSTR)strSQL) == 0)
//	{
//		return TRUE;// 插入成功
//	}
//	return FALSE; // 插入失败
//
//}
//
//BOOL CRWMysql::MySQL_DeleteData(char* ctable, char* cfield, char* cvalue)
//{
//	CString strSQL;
//	strSQL.Format(_T("delete from \'%s\' where \'%s\'=\'%s\'"), ctable, cfield, cvalue);
//	if (mysql_real_query(&m_sqlCon, (char*)(LPCTSTR)strSQL, (UINT)strSQL.GetLength()) != 0)
//	{
//		return FALSE;
//	}
//	return TRUE;
//}
//
//void CRWMysql::CloseMySQL()
//{
//	mysql_close(&m_sqlCon);
//}
//
