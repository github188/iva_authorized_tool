
// SoftKeygen.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSoftKeygenApp:
// �йش����ʵ�֣������ SoftKeygen.cpp
//

class CSoftKeygenApp : public CWinApp
{
public:
	CSoftKeygenApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSoftKeygenApp theApp;