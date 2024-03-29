#include "pch.h"
#include "CHttpConnect.h"
#include <afxinet.h>

#define  BUFFER_SIZE       1024

#define  NORMAL_CONNECT    INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT    NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST    INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE
#define  SECURE_REQUEST    NORMAL_REQUEST|INTERNET_FLAG_SECURE|INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID

//初始类
CHttpClient::CHttpClient(const CString& szReqCode, LPCTSTR strAgent)
{
	m_pSession = new CInternetSession(strAgent);//
	m_szReqCode = szReqCode;
	m_pConnection = NULL;
	m_pFile = NULL;
}
//结束时关闭
CHttpClient::~CHttpClient(void)
{
	Clear();
	if (NULL != m_pSession)
	{
		m_pSession->Close();
		delete m_pSession;
		m_pSession = NULL;
	}
}

//Get请求方式
int CHttpClient::HttpGet(LPCTSTR strUrl, CString& strResponse, CString& strStatus)
{
	return ExecuteRequest(_T("GET"), strUrl, NULL, strResponse, strStatus);
}

//POST请求方式
int CHttpClient::HttpPost(LPCTSTR strUrl, LPCTSTR strPostData, CString& strResponse, CString& strStatus)
{
	return ExecuteRequest(_T("POST"), strUrl, strPostData, strResponse, strStatus);
}

//执行请求
int CHttpClient::ExecuteRequest(LPCTSTR strMethod, LPCTSTR strUrl, LPCTSTR strPostData, CString& strResponse, CString& strStatus)
{
	DWORD dwFlags;
	CString strServer;
	CString strObject;
	DWORD dwServiceType;
	INTERNET_PORT nPort;
	strResponse = _T("");

	AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);					  //解析URL字符串

	if (AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)//服务类型正确3 3
	{
		return -1;//FAILURE;
	}
	try
	{
		m_pConnection = m_pSession->GetHttpConnection(strServer,
			dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT, nPort);
		m_pFile = m_pConnection->OpenRequest(strMethod, strObject, NULL, 1, NULL, NULL,
			(dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));
		if (AFX_INET_SERVICE_HTTPS == dwServiceType)
		{
			m_pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
			dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
			m_pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);
		}
		BOOL rst = false;
		if (strMethod == _T("GET"))
		{
			m_pFile->AddRequestHeaders(_T("Content-Type:application/json;charset=utf-8"));
		}
		else
		{
			m_pFile->AddRequestHeaders(L"Accept-Charset:UTF8");
			m_pFile->AddRequestHeaders(_T("Connection:close"));
			m_pFile->AddRequestHeaders(_T("Authorization:0579fadb1821653edec4c23fb7d1a66847cf318e261c4585bbea773800db2f927b7cc96d568f8f35b16d014b552de5e6"));
			m_pFile->AddRequestHeaders(_T("Content-Type:application/json"));
		}
		//rst=m_pFile->AddRequestHeaders(L"Accept-Charset:UTF8");
		//m_pFile->AddRequestHeaders(_T("Accept: *,*/*"));//  m_pFile->AddRequestHeaders(L"Accept-Charset:UTF8");
		//m_pFile->AddRequestHeaders(_T("Accept-Language: zh-cn"));
		//m_pFile->AddRequestHeaders(_T("Content-Type: application/x-www-form-urlencoded"));
		//m_pFile->AddRequestHeaders(_T("Accept-Encoding: deflate"));

		if (strPostData != NULL)										//POST方式请求数据
		{
			CStringA strData = GetReqData(strPostData);
			m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCSTR)strData, strlen(strData));
		}
		else															//GET方式请求数据
		{
			m_pFile->SendRequest();
		}
		//查询执行状态
		DWORD dwRet;
		m_pFile->QueryInfoStatusCode(dwRet);//查询执行状态
		strStatus.Format(_T("%d"), dwRet);
		if (dwRet != HTTP_STATUS_OK)
		{
			CString strErr = _T("http请求执行失败");
			strErr.Format(_T("err:%d;%s"), dwRet, strErr);
			Clear();
			return  -1;//SUCCESS http请求执行失败
		}
		//保存http响应
		char szChars[BUFFER_SIZE + 1] = { 0 };
		CStringA strRawResponse = "";
		UINT nReaded = 0;
		while ((nReaded = m_pFile->Read((void*)szChars, BUFFER_SIZE)) > 0)
		{
			szChars[nReaded] = '\0';
			strRawResponse += szChars;
			memset(szChars, 0, BUFFER_SIZE + 1);
		}
		strResponse = GetResData(strRawResponse);
		Clear();
	}
	catch (CInternetException* e)
	{
		TCHAR sz[1024] = { 0 };
		CString strErr = _T("");
		Clear();
		DWORD dwError = GetLastError();
		DWORD dwErrorCode = e->m_dwError;
		e->GetErrorMessage(sz, 1024);
		e->Delete();
		strErr.Format(_T("err:%d;%s"), dwErrorCode, CString(sz));
		strResponse = strErr;

		if (ERROR_INTERNET_TIMEOUT == dwErrorCode)
		{
			return -1;//OUTTIME;
		}
		else
		{
			return -1;//FAILURE;
		}
	}
	return SUCCESS;
}

//转换POST请求信息
CStringA CHttpClient::GetReqData(const CString& szReqData)
{
	// 预算-缓冲区中多字节的长度    
	int ansiiLen = WideCharToMultiByte(CP_UTF8, 0, szReqData, -1, NULL, 0, NULL, NULL);
	char* pAssii = (char*)malloc(sizeof(char) * ansiiLen);
	// 开始向缓冲区转换字节    
	WideCharToMultiByte(CP_UTF8, 0, szReqData, -1, pAssii, ansiiLen, NULL, NULL);
	CStringA szRet(pAssii);
	free(pAssii);
	return szRet;
}

//转换Http返回信息
CString CHttpClient::GetResData(const CStringA& szResData)
{
	if (m_szReqCode == REQ_CODE_GBK)		return CString(szResData);
	// 预算-缓冲区中宽字节的长度    
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, szResData, -1, NULL, 0);
	wchar_t* pUnicode = (wchar_t*)malloc(sizeof(wchar_t) * unicodeLen);
	// 开始向缓冲区转换字节    
	MultiByteToWideChar(CP_UTF8, 0, szResData, -1, pUnicode, unicodeLen);
	CString szRet(pUnicode);
	free(pUnicode);
	return szRet;
}

//关闭并清除连接
void CHttpClient::Clear()
{
	if (NULL != m_pFile)
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}

	if (NULL != m_pConnection)
	{
		m_pConnection->Close();
		delete m_pConnection;
		m_pConnection = NULL;
	}
}

BOOL CHttpClient::UploadFile(LPCTSTR szRemoteURI, LPCTSTR szLocalPath)
{
	BOOL bResult = FALSE;
	DWORD dwType = 0;
	CString sServer = _T("");
	CString sObject = _T("");

	INTERNET_PORT wPort = 0;
	DWORD dwPostSize = 0;
	const int nTimeOut = 5000;

	CHttpConnection* pHttpConn = NULL;
	CHttpFile* pHttpFile = NULL;
	CInternetSession cis;

	bResult = AfxParseURL(szRemoteURI, dwType, sServer, sObject, wPort);
	if (!bResult)
		return FALSE;

	bResult = FALSE;

	try
	{
		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

		pHttpConn = cis.GetHttpConnection(sServer, wPort, NULL, NULL);
		//VERIFY_EX(NULL != pHttpConn);
		pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_PUT, sObject);
		//VERIFY_EX(NULL != pHttpFile);

		if (UseHttpSendReqEx(pHttpFile, szLocalPath))
		{
			DWORD dwStateCode = 0;
			pHttpFile->QueryInfoStatusCode(dwStateCode);

			if (dwStateCode == HTTP_STATUS_CREATED || dwStateCode == HTTP_STATUS_OK)
				bResult = TRUE;
		}
	}
	catch (CFileException* e)
	{
		e->Delete();
		//REPORT_CACHED_EXCEPTION(_T("CFileException"));
	}
	catch (CInternetException* e)
	{
		e->Delete();

		CString sError;
		sError.Format(_T("Inernet connection error : %d"), e->m_dwError);
		//REPORT_CACHED_EXCEPTION(sError);
	}

	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConn)
	{
		pHttpConn->Close();
		delete pHttpConn;
	}
	cis.Close();

	return bResult;
}

BOOL CHttpClient::DownLoadFile(LPCTSTR szRemoteURI, LPCTSTR szLocalPath)
{
	CInternetSession session;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	CString strServer = _T(""), strObject = _T("");
	INTERNET_PORT wPort = 0;
	DWORD dwType = 0;

	HANDLE hFile = NULL;
	TCHAR pszBuffer[__BUFFER_SIZE];
	DWORD  dwFileSize = 0;

	const int nTimeOut = 2000;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

	BOOL bResult = FALSE;

	try
	{
		AfxParseURL(szRemoteURI, dwType, strServer, strObject, wPort);
		pHttpConnection = session.GetHttpConnection(strServer, wPort, NULL, NULL);
		//VERIFY_EX(NULL != pHttpConnection);
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
		//VERIFY_EX(NULL != pHttpFile);
		if (!pHttpFile->SendRequest())
			goto _err_handler;

		DWORD dwStateCode;
		pHttpFile->QueryInfoStatusCode(dwStateCode);
		if (dwStateCode != HTTP_STATUS_OK)
			goto _err_handler;

		hFile = CreateFile(szLocalPath, GENERIC_WRITE,
			FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			goto _err_handler;

		BOOL bRet = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, dwFileSize);
		if (!bRet)
			goto _err_handler;


		DWORD dwWrite = 0, dwTotalWrite = 0;
		UINT nRead = 0;

		do
		{
			nRead = pHttpFile->Read(pszBuffer, __BUFFER_SIZE);
			if (nRead <= 0) break;
			WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL);
			//VERIFY_EX(nRead == dwWrite);
			dwTotalWrite += dwWrite;
		} while (TRUE);

		if (dwTotalWrite != dwFileSize)
			goto _err_handler;

		bResult = TRUE;
	}
	catch (CFileException* e)
	{
		e->Delete();
		//REPORT_CACHED_EXCEPTION(_T("CFileException"));
	}
	catch (CInternetException* e)
	{
		CString sError;
		sError.Format(_T("Inernet connection error : %d"), e->m_dwError);
		e->Delete();
		//REPORT_CACHED_EXCEPTION(sError);
	}

_err_handler:
	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConnection)
	{
		pHttpConnection->Close();
		delete pHttpConnection;
	}
	session.Close();

	if (hFile) CloseHandle(hFile);

	return bResult;
}

BOOL CHttpClient::DeleteFile(LPCTSTR szRemoteURI)
{
	CInternetSession session;
	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	CString strServer = _T(""), strObject = _T("");
	INTERNET_PORT wPort = 0;
	DWORD dwType = 0;

	const int nTimeOut = 2000;
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

	BOOL bResult = FALSE;

	try
	{
		AfxParseURL(szRemoteURI, dwType, strServer, strObject, wPort);
		pHttpConnection = session.GetHttpConnection(strServer, wPort, NULL, NULL);
		//VERIFY_EX(NULL != pHttpConnection);
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_DELETE, strObject);
		//VERIFY_EX(NULL != pHttpFile);
		if (!pHttpFile->SendRequest())
			goto _err_handler;

		DWORD dwStateCode;
		pHttpFile->QueryInfoStatusCode(dwStateCode);
		if (dwStateCode != HTTP_STATUS_OK)
			goto _err_handler;

		bResult = TRUE;
	}
	catch (CFileException* e)
	{
		e->Delete();
		//REPORT_CACHED_EXCEPTION(_T("CFileException"));
	}
	catch (CInternetException* e)
	{
		CString sError;
		sError.Format(_T("Inernet connection error : %d"), e->m_dwError);
		//REPORT_CACHED_EXCEPTION(sError);
	}

_err_handler:
	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConnection)
	{
		pHttpConnection->Close();
		delete pHttpConnection;
	}
	session.Close();

	return bResult;
}

BOOL CHttpClient::CanWebsiteVisit(CString sURI)
{
	CHttpConnection* pHttpConn = NULL;
	CHttpFile* pHttpFile = NULL;
	CInternetSession cis;
	BOOL bResult = FALSE;
	BOOL bRetVal = FALSE;

	DWORD dwType = 0;
	DWORD dwStateCode = 0;
	INTERNET_PORT wPort = 0;
	CString sServer = _T("");
	CString sObject = _T("");
	const int nTimeOut = 5000;

	try
	{
		bResult = AfxParseURL(sURI, dwType, sServer, sObject, wPort);
		if (!bResult)
			return FALSE;

		cis.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut);
		cis.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1);

		pHttpConn = cis.GetHttpConnection(sServer, wPort);

		if (pHttpConn)
		{
			pHttpFile = pHttpConn->OpenRequest(CHttpConnection::HTTP_VERB_GET, sObject);
			if (pHttpFile->SendRequest())
			{
				pHttpFile->QueryInfoStatusCode(dwStateCode);

				if (dwStateCode == HTTP_STATUS_CREATED || dwStateCode == HTTP_STATUS_OK)
					bRetVal = TRUE;
			}
		}
	}
	catch (CInternetException* e)
	{
		e->Delete();
	}
	catch (...)
	{
	}

	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}

	if (pHttpConn)
	{
		pHttpConn->Close();
		delete pHttpConn;
	}

	cis.Close();

	return bRetVal;
}

BOOL CHttpClient::UseHttpSendReqEx(CHttpFile* httpFile, LPCTSTR szLocalFile)
{
	INTERNET_BUFFERS BufferIn;
	DWORD dwTotalWritten = 0;
	BYTE pFileBuffer[__BUFFER_SIZE];
	DWORD dwPostSize = 0;
	CFile file;
	BOOL  bRet = FALSE;

	bRet = file.Open(szLocalFile, CFile::shareDenyNone | CFile::modeRead);
	if (!bRet)
		return FALSE;

	dwPostSize = (DWORD)file.GetLength();
	//ENSURE_EX(dwPostSize >= 0 && dwPostSize < 0x80000000);

	memset(&BufferIn, 0, sizeof(BufferIn));
	BufferIn.dwStructSize = sizeof(INTERNET_BUFFERS); // Must be set or error will occur
	BufferIn.Next = NULL;
	BufferIn.lpcszHeader = NULL;
	BufferIn.dwHeadersLength = 0;
	BufferIn.dwHeadersTotal = 0;
	BufferIn.lpvBuffer = NULL;
	BufferIn.dwBufferLength = 0;
	BufferIn.dwBufferTotal = dwPostSize; // This is the only member used other than dwStructSize
	BufferIn.dwOffsetLow = 0;
	BufferIn.dwOffsetHigh = 0;

	if (!httpFile->SendRequestEx(&BufferIn, NULL, HSR_INITIATE, 1))
	{
		TRACE1("Error on HttpSendRequestEx %d\n", GetLastError());
		return FALSE;
	}

	file.SeekToBegin();
	do
	{
		int nActual = file.Read(pFileBuffer, __BUFFER_SIZE);
		if (nActual <= 0) break;
		httpFile->Write(pFileBuffer, nActual);
		dwTotalWritten += nActual;
	} while (TRUE);

	if (dwTotalWritten != dwPostSize)
	{
		file.Close();
		TRACE1("\nError on InternetWriteFile %lu \n", GetLastError());
		return FALSE;
	}

	if (!httpFile->EndRequest(0, NULL, 1))
	{
		file.Close();
		TRACE1("Error on HttpEndRequest %lu \n", GetLastError());
		return FALSE;
	}

	file.Close();

	return TRUE;
}
