// H2E_SampleApp.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CH2E_SampleAppApp:
// このクラスの実装については、H2E_SampleApp.cpp を参照してください。
//

class CH2E_SampleAppApp : public CWinApp
{
public:
	CH2E_SampleAppApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CH2E_SampleAppApp theApp;