#pragma once

// H2ECTRL.h : H2ECTRL.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������
#include "H2ECTRL_i.h"


// CH2ECTRLApp : �й�ʵ�ֵ���Ϣ������� H2ECTRL.cpp��

class CH2ECTRLApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

