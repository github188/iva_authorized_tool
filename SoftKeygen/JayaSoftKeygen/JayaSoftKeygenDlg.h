
// JayaSoftKeygenDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "atltime.h"


// CJayaSoftKeygenDlg �Ի���
class CJayaSoftKeygenDlg : public CDialogEx
{
// ����
public:
	CJayaSoftKeygenDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_JAYASOFTKEYGEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BOOL    m_bOnLine;
	CIPAddressCtrl m_tDevIp;
	CString m_strLicense;
	CTime m_tTimeOut;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();

	void CheckDlgItem();
	void UpdateAuthorization();

	CString m_szMacAddr;
	CString m_strUser;
	CString m_strPass;

	CString m_szAuth;
};
