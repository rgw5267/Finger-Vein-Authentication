/**
*@All Rights Reserved.    
*@Copyright (C) 2011, Hitachi Ltd.
*/

// H1E-USB_SampleApp.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "H1EUSB_SampleApp.h"
#include "H1EUSB_DetailsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CH1EUSB_SampleAppApp

BEGIN_MESSAGE_MAP(CH1EUSB_SampleAppApp, CWinApp)
//	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CH1EUSB_SampleAppApp コンストラクション

CH1EUSB_SampleAppApp::CH1EUSB_SampleAppApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CH1EUSB_SampleAppApp オブジェクトです。

CH1EUSB_SampleAppApp theApp;


// CH1EUSB_SampleAppApp 初期化

BOOL CH1EUSB_SampleAppApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	//CH1EUSB_SampleAppDlg dlg;

	CH1EUSB_DetailsDlg dlg;

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//  記述してください。
	}

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
