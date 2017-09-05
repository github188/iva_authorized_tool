
// JayaSoftKeygenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "JayaSoftKeygen.h"
#include "JayaSoftKeygenDlg.h"
#include "afxdialogex.h"
#include "Connect.h"
#include "ivr_rsa.h"
#include "Base64.h"
#include <time.inl>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void hexdump(const char * pre,const unsigned char *buf, const int num)
{
	int i;
	if (pre)
	{
		TRACE("==%s\n", pre);		
	}
	for(i = 0; i < num; i++)
	{
		TRACE("%02X ", buf[i]);
		if ((i+1)%8 == 0)
			TRACE("\n");
	}
	TRACE("\n");
	return;
}

static void hex2str(const unsigned char *inputdata, const int num, char * buf)
{
	int i;	
	for(i = 0; i < num; i++)
	{
		sprintf(buf + strlen(buf),"%02X",inputdata[i]);
	}
	return;
}

unsigned char char2hex(char c)
{
	unsigned char ic = 0;
	if (c >= '0' && c <= '9')
	{
		ic = (c-'0');
	}
	else if (c >= 'A' && c <= 'F')
	{
		ic = (c-'A')+10;
	}
	else if (c >= 'a' && c <= 'f')
	{
		ic = (c-'a')+10;
	}
	return ic;
}
static void str2hex(const char *inputdata, unsigned char * buf)
{
	size_t  i;
	int j=0;
	for(i = 0; i < strlen(inputdata)-1; i = i+2)
	{
		buf[j++] = char2hex(inputdata[i])*16+char2hex(inputdata[i++]);
	}
	return;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CJayaSoftKeygenDlg 对话框



CJayaSoftKeygenDlg::CJayaSoftKeygenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CJayaSoftKeygenDlg::IDD, pParent)
	, m_strLicense(_T(""))
	, m_tTimeOut(0)
	, m_szMacAddr(_T(""))
	, m_strUser(_T("admin"))
	, m_strPass(_T("admin"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bOnLine = TRUE;
	m_szAuth = "";
}

void CJayaSoftKeygenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_tDevIp);
	DDX_Text(pDX, IDC_EDIT_LICENSE, m_strLicense);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_tTimeOut);
	DDX_Text(pDX, IDC_EDIT1, m_szMacAddr);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_PASS, m_strPass);
}

BEGIN_MESSAGE_MAP(CJayaSoftKeygenDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CJayaSoftKeygenDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO2, &CJayaSoftKeygenDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CJayaSoftKeygenDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON1, &CJayaSoftKeygenDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CJayaSoftKeygenDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CJayaSoftKeygenDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CJayaSoftKeygenDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CJayaSoftKeygenDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CJayaSoftKeygenDlg 消息处理程序

BOOL CJayaSoftKeygenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
	char szTargetIp [20] = {0};
	UINT8 byIPAdd4 = 0;
	UINT8 byIPAdd3 = 0;
	UINT8 byIPAdd2 = 0;
	UINT8 byIPAdd1 = 0;
	if (GetProfileString("JAYA_KEYGEN","DEFUALT_IP","192.168.1.219",szTargetIp,20) > 6)
	{
		unsigned long ipl = inet_addr(szTargetIp);
		byIPAdd4 = (UINT8)(ipl>>24)&0xFF;
		byIPAdd3 = (UINT8)(ipl>>16)&0xFF;
		byIPAdd2 = (UINT8)(ipl>>8)&0xFF;
		byIPAdd1 = (UINT8)(ipl)&0xFF;
	}
	m_tDevIp.SetAddress(byIPAdd1, byIPAdd2, byIPAdd3, byIPAdd4);


	m_tTimeOut = CTime(time(NULL));
	CheckDlgItem();
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CJayaSoftKeygenDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CJayaSoftKeygenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CJayaSoftKeygenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CHAR2HEXNUM(char c)
{
	int ic = -1;
	if (c >= '0' && c <= '9')
	{
		ic = (c-'0');
	}
	else if (c >= 'A' && c <= 'F')
	{
		ic = (c-'A')+10;
	}
	else if (c >= 'a' && c <= 'f')
	{
		ic = (c-'a')+10;
	}
	return ic;
}

void CJayaSoftKeygenDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	UpdateAuthorization();

	if (m_bOnLine)
	{
		DWORD dwAddress = 0;
		m_tDevIp.GetAddress(dwAddress);

		// 获取MAC地址
		CConnect theCon;
		int iret = theCon.GetMac(dwAddress,m_szMacAddr, m_szAuth.IsEmpty()?NULL:m_szAuth);
		if (iret != 0)
		{
			//AfxMessageBox("无法连接设备...");
			return;
		}

		BYTE b1,b2,b3,b4;
		m_tDevIp.GetAddress(b1,b2,b3,b4);
		char szTargetIp[20] = {0};
		sprintf_s(szTargetIp,20,"%u.%u.%u.%u",b1,b2,b3,b4);
		WriteProfileString("CEYE_KEYGEN","DEFUALT_IP",szTargetIp);
	}
	
	UpdateData(FALSE);

	//00:04:4B:49:00:39
	UINT8 u8MacAddr[6] = {0};
	char C11='\0',C12='\0',C21='\0',C22='\0',C31='\0',C32='\0',C41='\0',C42='\0',C51='\0',C52='\0',C61='\0',C62='\0';
	int iNum = sscanf(m_szMacAddr,"%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",&C11,&C12,&C21,&C22,&C31,&C32,&C41,&C42,&C51,&C52,&C61,&C62);
	if (iNum != 12)
	{
		AfxMessageBox("格式不正确！");
		return;
	}
	if (CHAR2HEXNUM(C11) == -1 || CHAR2HEXNUM(C12) == -1 || 
		CHAR2HEXNUM(C21) == -1 || CHAR2HEXNUM(C22) == -1 || 
		CHAR2HEXNUM(C31) == -1 || CHAR2HEXNUM(C32) == -1 || 
		CHAR2HEXNUM(C41) == -1 || CHAR2HEXNUM(C42) == -1 || 
		CHAR2HEXNUM(C51) == -1 || CHAR2HEXNUM(C52) == -1 || 
		CHAR2HEXNUM(C61) == -1 || CHAR2HEXNUM(C62) == -1 )
	{
		AfxMessageBox("格式不正确！");
		return;
	}

	u8MacAddr[0] = CHAR2HEXNUM(C11)*16+CHAR2HEXNUM(C12);
	u8MacAddr[1] = CHAR2HEXNUM(C21)*16+CHAR2HEXNUM(C22);
	u8MacAddr[2] = CHAR2HEXNUM(C31)*16+CHAR2HEXNUM(C32);
	u8MacAddr[3] = CHAR2HEXNUM(C41)*16+CHAR2HEXNUM(C42);
	u8MacAddr[4] = CHAR2HEXNUM(C51)*16+CHAR2HEXNUM(C52);
	u8MacAddr[5] = CHAR2HEXNUM(C61)*16+CHAR2HEXNUM(C62);

	hexdump("MAC:",u8MacAddr,6);

	// 明文校验部分 0xFF 0x23 0x45 0x67 6位MAC JAYA  0x76 0x54 0x32 0xFF 2位类型 4位长度
 	UINT8 cleartext[20] = {0xFF,0x23,0x45,0x67,0x00,0x00,0x00,0x00,0x00,0x00,'C','E','Y','E',
		0x76,0x54,0x32,0xFF,0xFA,0x00};
 	
	for (int i = 0; i < 6; i++)
 	{
 		cleartext[4+i] = u8MacAddr[i];
 	}

	int iSel = ((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();
 	cleartext[19] = iSel ? 0x00 : 0x01;

	hexdump("明文:",cleartext,20);

	// MD5加密
	unsigned char u8license[24] = {0};
	MD5_Process(cleartext,20,u8license,3,12);
	hexdump("密文:",u8license,10);

	// 时间加密
	CTime tmout = CTime(time(NULL));
	if (!iSel)
	{
		tmout = m_tTimeOut;
	}
	else
	{
		tmout = CTime(tmout.GetSecond()+2000,tmout.GetSecond()%7+1,tmout.GetSecond()%30+1,tmout.GetSecond()%24,tmout.GetSecond(),tmout.GetSecond());
	}
	Time_Encode(tmout,u8license+9);

	// BASE64加密
	char strlicense[20] = {0};
	Base64encode(strlicense,(const char *)u8license,12);
	TRACE("LICENSE(%d):%s\n",strlen(strlicense),strlicense);

	m_strLicense = strlicense;
	UpdateData(FALSE);
	//CDialogEx::OnOK();
}


void CJayaSoftKeygenDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(TRUE);
}

void CJayaSoftKeygenDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
}

void CJayaSoftKeygenDlg::OnBnClickedButton1()
{
	// TODO:设置授权
	UpdateData(TRUE);
	UpdateAuthorization();

	DWORD dwAddress = 0;
	m_tDevIp.GetAddress(dwAddress);

	if (m_strLicense.IsEmpty())
	{
		AfxMessageBox("先生成授权码！");
		return;
	}

	CConnect theCon;
	CString szAuth;
	int iret = theCon.SetAuth(dwAddress,m_strLicense, m_szAuth.IsEmpty()?NULL:m_szAuth);
	if (iret != 0)
	{
		return;
	}
}


void CJayaSoftKeygenDlg::OnBnClickedButton3()
{
	// TODO: 保存LICENSE文件
	if (m_strLicense.IsEmpty())
	{
		AfxMessageBox("先生成授权码！");
		return;
	}
	int nSize = 254;
	CString defaultFolder =	"";
	GetProfileString("CEYE_KEYGEN","DEFUALT_FOLDER","",defaultFolder.GetBuffer(254),254);
	CFolderPickerDialog dlg(defaultFolder, 0, this, 0);
	if (dlg.DoModal()== IDOK)
	{
		CString des;
		des = dlg.GetFolderPath();
		WriteProfileString("CEYE_KEYGEN","DEFUALT_FOLDER",des);

		FILE* fd = NULL;
		CString strFilePath = des+"\\LICENSE";
		fd = fopen(strFilePath,"wb+");
		if(fd == NULL)
		{
			AfxMessageBox("打开文件失败\n");
			return;
		}

		if(fwrite (m_strLicense.GetBuffer(0), m_strLicense.GetLength(), 1, fd) != 1)
		{
			fclose(fd);
			return;
		}

		fclose(fd);
		fd = NULL;

		AfxMessageBox("保存文件成功\n");
	}
}

void CJayaSoftKeygenDlg::OnBnClickedButton2()
{
	// TODO:获取授权
	UpdateData(TRUE);
	UpdateAuthorization();

	DWORD dwAddress = 0;
	m_tDevIp.GetAddress(dwAddress);

	m_tTimeOut = CTime(time(NULL));
	m_strLicense = "";
	int type = 0;
	CString strTimeout = 0;
	CString strLic = 0;
	CConnect theCon;
	int iret = theCon.GetAuth(dwAddress, type, strTimeout, strLic, m_szAuth.IsEmpty()?NULL:m_szAuth);
	if (iret != 0)
	{
		return;
	}

	m_strLicense = strLic;

	//0:未授权 1:永久授权 2：临时授权
	if (type == 0)
	{
		AfxMessageBox("未授权");
	}
	else if (type == 1)
	{
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
		AfxMessageBox("永久授权");
	}
	else if (type == 2)
	{
		CString info;
		info.Format("临时授权,截止到%s",strTimeout);
		CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO2);
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(TRUE);
		int iY = 0, iM = 0, iD = 0;
		sscanf(strTimeout.GetBuffer(0),"%04d-%02d-%02d", &iY, &iM, &iD);
		
		if( iY >= 1900  && iM >= 1 && iM <= 12 && iD >= 1 && iD <= 31 )
		{
			m_tTimeOut = CTime(iY,iM,iD,0,0,0);
		}
		AfxMessageBox(info);
	}
	else
	{
		AfxMessageBox("未知类型");
	}

	UpdateData(FALSE);
	CheckDlgItem();
}

void CJayaSoftKeygenDlg::OnBnClickedButton4()
{
	// TODO:删除授权
	UpdateData(TRUE);
	UpdateAuthorization();

	DWORD dwAddress = 0;
	m_tDevIp.GetAddress(dwAddress);

	CConnect theCon;
	theCon.DelAuth(dwAddress, m_szAuth.IsEmpty()?NULL:m_szAuth);
}


void CJayaSoftKeygenDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bOnLine = !m_bOnLine;
	CheckDlgItem();
}

void CJayaSoftKeygenDlg::CheckDlgItem()
{
	GetDlgItem(IDC_BUTTON5)->SetWindowText(m_bOnLine?"切换到离线模式":"切换到在线模式");
	GetDlgItem(IDC_STATIC_PRE)->SetWindowText(m_bOnLine?"IP:":"MAC:");
	GetDlgItem(IDC_BUTTON1)->EnableWindow(m_bOnLine);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(m_bOnLine);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(m_bOnLine);
	GetDlgItem(IDC_EDIT1)->ShowWindow(m_bOnLine?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_IPADDRESS1)->ShowWindow(m_bOnLine?SW_SHOW:SW_HIDE);
}

void CJayaSoftKeygenDlg::UpdateAuthorization()
{
	if (m_strPass.IsEmpty() || m_strUser.IsEmpty())
	{
		m_szAuth = "";
	}
	else
	{
		CString szUserPass = m_strUser+":"+m_strPass;
		char szCodedStr[512] = {0};//"YWRtaW46YWRtaW4="
		Base64encode(szCodedStr, szUserPass.GetBuffer(0), szUserPass.GetLength());
		m_szAuth.Format("Authorization: Basic %s",szCodedStr);
	}
}
