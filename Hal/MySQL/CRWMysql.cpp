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
//	// strlocal:��������ַ������ֱ������IP��
//	// strroot:�˺ţ�
//	// strpass:���룻
//	// strdia:���ݿ�����
//	// 3306:����˿�
//BOOL CRWMysql::MySQL_InitDataBase(CString strlocal, CString strroot, CString strpass, CString strdia, int nport)
//{
//	USES_CONVERSION;
//	mysql_init(&m_sqlCon);
//	if (NULL == mysql_real_connect(&m_sqlCon, T2A(strlocal), T2A(strroot), T2A(strpass), T2A(strdia), nport, NULL, 0))
//	{
//		AfxMessageBox(_T("���ݿ�����ʧ�ܣ�"));
//		mysql_close(&m_sqlCon);
//		return FALSE;
//	}
//	else // MySQL���ӳɹ�������������ݿ�
//	{
//		AfxMessageBox(_T("���ݿ����ӳɹ���"));
//
//		mysql_set_character_set(&m_sqlCon, "gb2312"); //�����ֽڷ�����ֹ��������.
//		return TRUE;
//	}
//}
//BOOL CRWMysql::MySQL_CreateTable(CString sql_create_table)
//{
//	//�������UserInfo������������
//	/*CString sql_create_table = _T("CREATE TABLE IF NOT EXISTS UserInfo(\
//		id       INT PRIMARY KEY NOT NULL,\
//		Account  VARCHAR(20) NOT NULL UNIQUE,\
//		Pass     VARCHAR(20)    NOT NULL\
//		);\
//		");*/
//	char buffer[512];
//	wcstombs(buffer, sql_create_table.GetBuffer(), sizeof(buffer));
//	int result = mysql_query(&m_sqlCon, buffer); // ִ�в�ѯ���
//	sql_create_table.ReleaseBuffer();
//	if (result != 0)
//	{
//		AfxMessageBox(_T("����ʧ�ܣ�"));
//		mysql_close(&m_sqlCon);
//		return FALSE;
//	}
//	//AfxMessageBox(_T("���ݿ����б�"));
//	return TRUE;
//}
//
//BOOL CRWMysql::MySQL_SelectData(char* sql_select)
//{
//	// ��ѯ����֮ǰ�����list control�е���������
//	//m_List.DeleteAllItems(); //������б���
//		MYSQL_RES* res; // �������ݲ�ѯ���
//		MYSQL_ROW row; // ȡÿ�н���Ķ���
//		mysql_real_query(&m_sqlCon, sql_select, strlen(sql_select)); // ��ѯ������������ַ�����ʾ
//		res = mysql_store_result(&m_sqlCon); // �õ���ѯ���
//
//		//�ظ���ȡ�У������ݷ����б��У�ֱ��rowΪNULL
//		//
//		int selectCount = 0; // ��¼��ѯ����ĸ���������ѯ���Ϊ�գ�Ҳ����0
//		while (row = mysql_fetch_row(res))
//		{
//			CString strId, strAccount, strPass;
//			strId = row[0];
//			strAccount = row[1];
//			strPass = row[2];
//
//			// ��list control�в�������
//			/*m_List.InsertItem(selectCount, strId);
//			m_List.SetItemText(selectCount, 1, strAccount);
//			m_List.SetItemText(selectCount, 2, strPass);*/
//
//			selectCount++;
//		}
//		mysql_close(&m_sqlCon);
//
//		if (selectCount == 0) return FALSE; // û��ѯ������
//		else return TRUE;
//}
//// MySQL��������
//BOOL CRWMysql::MySQL_InsertData(char* ctable, char* cfield, char* cvalue)
//{
//	CString strSQL;
//	strSQL.Format(_T("insert into \'%s\'(\'%s\')' values(/'%s/')"), ctable, cfield, cvalue);
//	if (mysql_query(&m_sqlCon, (char*)(LPCTSTR)strSQL) == 0)
//	{
//		return TRUE;// ����ɹ�
//	}
//	return FALSE; // ����ʧ��
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
