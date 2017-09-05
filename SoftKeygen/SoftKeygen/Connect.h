#pragma once
class CConnect
{
public:
	CConnect(void);
	~CConnect(void);
	int GetMac(DWORD &dwAddress, CString &strMac, LPCTSTR pAuthorization);
	int SetAuth(DWORD &dwAddress, CString strLicense, LPCTSTR pAuthorization);
	int DelAuth(DWORD &dwAddress, LPCTSTR pAuthorization);
	int GetAuth(DWORD &dwAddress, int & type, CString & timeout, CString & license, LPCTSTR pAuthorization);
};

#include <afxinet.h>  
#include <string>  
using namespace std;  

#define  IE_AGENT  _T("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)")  

// 操作成功  
#define SUCCESS        0  
// 操作失败  
#define FAILURE        1  
// 操作超时 www.it165.net  
#define OUTTIME        2  

class CHttpClient  
{  
public:  
	CHttpClient(LPCTSTR strAgent = IE_AGENT);  
	virtual ~CHttpClient(void);  

	int HttpGet(LPCTSTR strUrl, string &strResponse, LPCTSTR pAuthorization);  
	int HttpPut(LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse, LPCTSTR pAuthorization);  
	int HttpPost(LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse, LPCTSTR pAuthorization);  
	int HttpDelete(LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse, LPCTSTR pAuthorization);  

private:  
	int ExecuteRequest(LPCTSTR strMethod, LPCTSTR strUrl, LPCTSTR strPostData, string &strResponse, LPCTSTR pAuthorization);  
	void Clear();  

private:  
	CInternetSession *m_pSession;  
	CHttpConnection *m_pConnection;  
	CHttpFile *m_pFile;  
}; 