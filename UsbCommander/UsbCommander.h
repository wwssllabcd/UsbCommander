
// UsbCommander.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CUsbCommanderApp: 
// �аѾ\��@�����O�� UsbCommander.cpp
//

class CUsbCommanderApp : public CWinApp
{
public:
	CUsbCommanderApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CUsbCommanderApp theApp;