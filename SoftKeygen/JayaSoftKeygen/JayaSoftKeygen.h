
// JayaSoftKeygen.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CJayaSoftKeygenApp:
// �йش����ʵ�֣������ JayaSoftKeygen.cpp
//

class CJayaSoftKeygenApp : public CWinApp
{
public:
	CJayaSoftKeygenApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CJayaSoftKeygenApp theApp;