#pragma once

// H2ECTRL.h : H2ECTRL.DLL 的主头文件

#if !defined( __AFXCTL_H__ )
#error "在包括此文件之前包括“afxctl.h”"
#endif

#include "resource.h"       // 主符号
#include "H2ECTRL_i.h"


// CH2ECTRLApp : 有关实现的信息，请参阅 H2ECTRL.cpp。

class CH2ECTRLApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

