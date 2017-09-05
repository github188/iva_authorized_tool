#include "stdafx.h"
#include "Connect.h"


CConnect::CConnect(void)
{
}

CConnect::~CConnect(void)
{
}

int CConnect::GetMac( DWORD &dwAddress, CString &strMac, LPCTSTR pAuthorization )
{
	CString strUrl;
	strUrl.Format("http://%d.%d.%d.%d/ISAPI/System/Network/Interfaces",(dwAddress>>24)&0xff,(dwAddress>>16)&0xff,(dwAddress>>8)&0xff,dwAddress&0xff); 

	std::string strRespose;

	CHttpClient tHttpClient("Ripple");
	int iRet = tHttpClient.HttpGet(strUrl,strRespose, pAuthorization);
	if (iRet!=SUCCESS)
	{
		AfxMessageBox("连接设备失败！");
		return -1;
	}
	
	CString strRes = strRespose.c_str();
	int pos1 = strRes.Find("\"mac\":\"");
	if (pos1 < 0)
	{
		if (strRes.Find("401 Unauthorized"))
		{
			AfxMessageBox("用户名密码不正确！");
			return -1;
		}
		AfxMessageBox("获取设备信息失败！");
		return -1;
	}

	//mac:"00:04:4B:49:00:39"
	CString strMacBegin = strRes.Right(strRes.GetLength()-pos1-strlen("\"mac\":\""));
	int pos2 = strMacBegin.Find("\"");
	if (pos2 < 0)
	{
		AfxMessageBox("获取设备信息失败！");
		return -1;
	}
	
	strMac = strMacBegin.Left(pos2);
	return 0;
}

int CConnect::SetAuth( DWORD &dwAddress, CString strLicense, LPCTSTR pAuthorization )
{
	CString strUrl;
	strUrl.Format("http://%d.%d.%d.%d/ISAPI/System/SoftAuth",(dwAddress>>24)&0xff,(dwAddress>>16)&0xff,(dwAddress>>8)&0xff,dwAddress&0xff); 

	CString strPostData;
	strPostData.Format("{\"license\":\"%s\"}",strLicense);

	std::string strRespose;

	CHttpClient tHttpClient("Ripple");
	int iRet = tHttpClient.HttpPut(strUrl,strPostData,strRespose, pAuthorization);
	if (iRet!=SUCCESS)
	{
		AfxMessageBox("连接设备失败！");
		return -1;
	}

	CString strRes = strRespose.c_str();
	int pos1 = strRes.Find("\"code\":");
	if (pos1 < 0)
	{
		if (strRes.Find("401 Unauthorized"))
		{
			AfxMessageBox("用户名密码不正确！");
			return -1;
		}
		AfxMessageBox("回复错误！");
		return -1;
	}
	CString strResBegin = strRes.Right(strRes.GetLength()-pos1-strlen("\"code\":"));
	CString strCode = strResBegin.Left(1);
	//0:成功 1:失败 2:已经授权过(请先删除久的授权)
	if (strCode.CompareNoCase("0") == 0)
	{
		AfxMessageBox("授权成功！");
	}
	else if (strCode.CompareNoCase("1004") == 0)
	{
		AfxMessageBox("已有授权，无法重复授权");
	}
	else// if (strCode.CompareNoCase("1003") == 0)
	{
		AfxMessageBox("授权失败！");
	}
	//else
	//{
	//	AfxMessageBox("未知错误！");
	//}
	return 0;
}

int CConnect::DelAuth( DWORD &dwAddress, LPCTSTR pAuthorization )
{
	CString strUrl;
	strUrl.Format("http://%d.%d.%d.%d/ISAPI/System/SoftAuth",(dwAddress>>24)&0xff,(dwAddress>>16)&0xff,(dwAddress>>8)&0xff,dwAddress&0xff); 

	std::string strRespose;

	CHttpClient tHttpClient("Ripple");
	int iRet = tHttpClient.HttpDelete(strUrl,NULL,strRespose,pAuthorization);
	if (iRet!=SUCCESS)
	{
		AfxMessageBox("连接设备失败！");
		return -1;
	}

	CString strRes = strRespose.c_str();
	int pos1 = strRes.Find("\"code\":");
	if (pos1 < 0)
	{
		if (strRes.Find("401 Unauthorized"))
		{
			AfxMessageBox("用户名密码不正确！");
			return -1;
		}
		AfxMessageBox("回复错误！");
		return -1;
	}
	CString strResBegin = strRes.Right(strRes.GetLength()-pos1-strlen("\"code\":"));
	CString strCode = strResBegin.Left(1);
	
	if (strCode.CompareNoCase("0") == 0)
	{
		AfxMessageBox("删除授权成功！");
	}
	else
	{
		AfxMessageBox("删除授权失败！");
	}
	return 0;
}

int CConnect::GetAuth( DWORD &dwAddress, int & type, CString & timeout, CString & license, LPCTSTR pAuthorization )
{
	CString strUrl;
	strUrl.Format("http://%d.%d.%d.%d/ISAPI/System/SoftAuth",(dwAddress>>24)&0xff,(dwAddress>>16)&0xff,(dwAddress>>8)&0xff,dwAddress&0xff); 

	std::string strRespose;

	CHttpClient tHttpClient("Ripple");
	int iRet = tHttpClient.HttpGet(strUrl,strRespose,pAuthorization);
	if (iRet!=SUCCESS)
	{
		AfxMessageBox("连接设备失败！");
		return -1;
	}

	CString strRes = strRespose.c_str();
	int pos1 = strRes.Find("\"code\":");
	if (pos1 < 0)
	{
		if (strRes.Find("401 Unauthorized"))
		{
			AfxMessageBox("用户名密码不正确！");
			return -1;
		}

		AfxMessageBox("回复错误！");
		return -1;
	}


	CString strResBegin = strRes.Right(strRes.GetLength()-pos1-strlen("\"code\":"));
	CString strCode = strResBegin.Left(1);
	if (strCode.CompareNoCase("0") != 0)
	{
		AfxMessageBox("获取授权失败！");
		return 0;
	}

	pos1 = strRes.Find("\"authtype\":");
	if (pos1 < 0)
	{
		AfxMessageBox("回复错误！");
		return -1;
	}
	strResBegin = strRes.Right(strRes.GetLength()-pos1-strlen("\"authtype\":"));	
	CString strType = strResBegin.Left(1);
	type = atoi(strType.GetBuffer(0));

	pos1 = strRes.Find("\"timeout\":\"");
	if (pos1 < 0)
	{
		AfxMessageBox("回复错误！");
		return -1;
	}

	strResBegin = strRes.Right(strRes.GetLength()-pos1-strlen("\"timeout\":\""));
	int pos2 = strResBegin.Find("\"");
	if (pos2 < 0)
	{
		AfxMessageBox("回复错误！");
		return -1;
	}
	timeout = strResBegin.Left(pos2);

	pos1 = strRes.Find("\"license\":\"");
	if (pos1 < 0)
	{
		AfxMessageBox("回复错误！");
		return -1;
	}

	strResBegin = strRes.Right(strRes.GetLength()-pos1-strlen("\"license\":\""));
	int pos3 = strResBegin.Find("\"");
	if (pos3 < 0)
	{
		AfxMessageBox("回复错误！");
		return -1;
	}
	license = strResBegin.Left(pos3);
	return 0;
}

#define  BUFFER_SIZE       1024  
  
#define  NORMAL_CONNECT             INTERNET_FLAG_KEEP_CONNECTION  
#define  SECURE_CONNECT             NORMAL_CONNECT | INTERNET_FLAG_SECURE  
#define  NORMAL_REQUEST             INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE   
#define  SECURE_REQUEST             NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID  
  
CHttpClient::CHttpClient(LPCTSTR strAgent)  
{  
    m_pSession = new CInternetSession(strAgent);  
    m_pConnection = NULL;  
    m_pFile = NULL;  
}  
  
  
CHttpClient::~CHttpClient(void)  
{  
    Clear();  
    if(NULL != m_pSession)  
    {  
        m_pSession->Close();  
        delete m_pSession;  
        m_pSession = NULL;  
    }  
}  
  
void CHttpClient::Clear()  
{  
    if(NULL != m_pFile)  
    {  
        m_pFile->Close();  
        delete m_pFile;  
        m_pFile = NULL;  
    }  
  
    if(NULL != m_pConnection)  
    {  
        m_pConnection->Close();  
        delete m_pConnection;  
        m_pConnection = NULL;  
    }  
}  
  
int CHttpClient::ExecuteRequest(LPCTSTR strMethod, LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse, LPCTSTR pAuthorization)  
{  
    CString strServer;  
    CString strObject;  
    DWORD dwServiceType;  
    INTERNET_PORT nPort;  
    strResponse = "";  
  
    AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);  
  
    if(AFX_INET_SERVICE_HTTP != dwServiceType && AFX_INET_SERVICE_HTTPS != dwServiceType)  
    {  
        return FAILURE;  
    }  
  
    try  
    {  
        m_pConnection = m_pSession->GetHttpConnection(strServer,  
            dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_CONNECT : SECURE_CONNECT,  
            nPort);  
        m_pFile = m_pConnection->OpenRequest(strMethod, strObject,   
            NULL, 1, NULL, NULL,   
            (dwServiceType == AFX_INET_SERVICE_HTTP ? NORMAL_REQUEST : SECURE_REQUEST));  
  
        //DWORD dwFlags;  
        //m_pFile->QueryOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);  
        //dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;  
        ////set web server option  
        //m_pFile->SetOption(INTERNET_OPTION_SECURITY_FLAGS, dwFlags);  
  
		if (pAuthorization)
		{
			//m_pFile->AddRequestHeaders("Authorization: Basic YWRtaW46YWRtaW4=");
			m_pFile->AddRequestHeaders(pAuthorization);
		}
		//m_pFile->AddRequestHeaders("Accept: *,*/*");
		//m_pFile->AddRequestHeaders("Accept-Encoding: gzip, deflate");  
        //m_pFile->AddRequestHeaders("Accept-Language: zh-cn");
		if (strPostData)
		{
			m_pFile->AddRequestHeaders("Content-Type: application/json; charset=\"UTF-8\"");  
		}
  
        m_pFile->SendRequest(NULL, 0, (LPVOID)(LPCTSTR)strPostData, strPostData == NULL ? 0 : _tcslen(strPostData));  
  
        char szChars[BUFFER_SIZE + 1] = {0};  
        string strRawResponse = "";  
        UINT nReaded = 0;  
        while ((nReaded = m_pFile->Read((void*)szChars, BUFFER_SIZE)) > 0)  
        {  
            szChars[nReaded] = '\0';  
            strRawResponse += szChars;  
            memset(szChars, 0, BUFFER_SIZE + 1);  
        }  
  
        int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, strRawResponse.c_str(), -1, NULL, 0);  
        WCHAR *pUnicode = new WCHAR[unicodeLen + 1];  
        memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
  
        MultiByteToWideChar(CP_UTF8,0,strRawResponse.c_str(),-1, pUnicode,unicodeLen);  
        CString cs(pUnicode);  
        delete []pUnicode;   
        pUnicode = NULL;  
  
        strResponse = cs;  
  
        Clear();  
    }  
    catch (CInternetException* e)  
    {  
        Clear();  
        DWORD dwErrorCode = e->m_dwError;  
        e->Delete();  
  
        DWORD dwError = GetLastError();  
  
        //PRINT_LOG("dwError = %d", dwError, 0);  
  
        if (ERROR_INTERNET_TIMEOUT == dwErrorCode)  
        {  
            return OUTTIME;  
        }  
        else  
        {  
            return FAILURE;  
        }  
    }  
    return SUCCESS;  
}  
  
int CHttpClient::HttpGet(LPCTSTR strUrl, string &strResponse, LPCTSTR pAuthorization)  
{  
    return ExecuteRequest("GET", strUrl, NULL, strResponse, pAuthorization);  
}  
  
int CHttpClient::HttpPut( LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse, LPCTSTR pAuthorization )
{
	return ExecuteRequest("PUT", strUrl, strPostData, strResponse, pAuthorization);  
}

int CHttpClient::HttpPost(LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse, LPCTSTR pAuthorization)  
{  
    return ExecuteRequest("POST", strUrl, strPostData, strResponse, pAuthorization);  
}

int CHttpClient::HttpDelete( LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse, LPCTSTR pAuthorization )
{
	return ExecuteRequest("DELETE", strUrl, strPostData, strResponse, pAuthorization);  
}
