/**
*@All Rights Reserved.    
*@Copyright (C) 2011, Hitachi Ltd.
*/

// H1E-USB_SampleApp.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CH1EUSB_SampleAppApp:
// このクラスの実装については、H1E-USB_SampleApp.cpp を参照してください。
//

class CH1EUSB_SampleAppApp : public CWinApp
{
public:
	CH1EUSB_SampleAppApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CH1EUSB_SampleAppApp theApp;