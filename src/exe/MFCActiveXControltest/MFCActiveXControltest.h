#pragma once

// MFCActiveXControltest.h : MFCActiveXControltest.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������
#include "MFCActiveXControltest_i.h"


// CMFCActiveXControltestApp : �й�ʵ�ֵ���Ϣ������� MFCActiveXControltest.cpp��

class CMFCActiveXControltestApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

