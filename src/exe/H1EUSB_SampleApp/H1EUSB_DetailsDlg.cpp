// H1EUSB_DetailsDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "H1EUSB_SampleApp.h"
#include "H1EUSB_DetailsDlg.h"
#include "H1EUSB_Tab0Dialog.h"
#include "H1EUSB_Tab1Dialog.h"
#include "H1EUSB_Tab2Dialog.h"
#include "H1EUSB_TemplateFile.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_MsgGuide.h"
/********************************************************************************************/
/* CH1EUSB_DetailsDlgクラスプログラム														*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20120110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/********************************************************************************************/
// CH1EUSB_DetailsDlg ダイアログ

IMPLEMENT_DYNAMIC(CH1EUSB_DetailsDlg, CDialog)

/********************************************************************************************/
/*[メソッド名] CH1EUSB_DetailsDlgクラスのコンストラクタ										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		CH1EUSB_DetailsDlg(CWnd* pParent)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コンストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1EUSB_DetailsDlg::CH1EUSB_DetailsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CH1EUSB_DetailsDlg::IDD, pParent)
{
	//メンバ変数の初期化
	m_pTemplateFile = 0;
	m_pH1USBCom = 0;
	m_dwSystemCondition = SYSTEM_STARTUP;

	//コミュニケーションクラスの生成
	m_H1EComObjEP.pObj = this;												//本クラスのオブジェクト
	m_H1EComObjEP.H1ECallBacks.OnTuchNotification = OnTuchNotification;		//タッチセンサー情報通知関数エントリー
	m_H1EComObjEP.H1ECallBacks.OnSendEnd = OnSendEndNotification;			//送信終了通知エントリー	
	m_pH1USBCom = new CH1USBCom(&m_H1EComObjEP);							//クラスの生成

	//テンプレートクラスの生成
	m_pTemplateFile = new CTemplateFile;

	//コミュニケーションクラスのエンドポイント機能の開始
	m_pH1USBCom->StartEndPoint();

	//タブのパネルの子ダイアログに本（親）のオブジェクトをセット
	m_dlgTab0.SetParentDialog(this);
	m_dlgTab1.SetParentDialog(this);
	m_dlgTab2.SetParentDialog(this);
}

/********************************************************************************************/
/*[メソッド名] CH1EUSB_DetailsDlgクラスのデストラクタ										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		~CH1EUSB_DetailsDlg()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1EUSB_DetailsDlg::~CH1EUSB_DetailsDlg()
{
	//コミュニケーションクラスのエンドポイント機能の停止
	m_pH1USBCom->StopEndPoint();

	//コミュニケーションクラスの削除
	if(m_pH1USBCom){
		delete m_pH1USBCom;
	}

	//テンプレートクラスの削除
	if(m_pTemplateFile){
		delete m_pTemplateFile;
	}

}

void CH1EUSB_DetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DTAILS_TAB, m_tabCtrlDetails);
	DDX_Control(pDX, IDC_STATIC_CGBOX, m_stComGBox);
	DDX_Control(pDX, IDC_STATIC_FNAME, m_stFName);
	DDX_Control(pDX, IDC_BUTTON_MAIN, m_buMain);
	DDX_Control(pDX, IDC_LIST_TMPLIST, m_listCtrlTempList);
	DDX_Control(pDX, IDC_CHECK_POL, m_btCheckPol);
}


BEGIN_MESSAGE_MAP(CH1EUSB_DetailsDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DTAILS_TAB, &CH1EUSB_DetailsDlg::OnTcnSelchangeDtailsTab)

	ON_BN_CLICKED(IDC_BUTTON_MAIN, &CH1EUSB_DetailsDlg::OnBnClickedButtonMain)
	ON_BN_CLICKED(IDC_BUTTON_COMSET, &CH1EUSB_DetailsDlg::OnBnClickedButtonComset)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TMPLIST, &CH1EUSB_DetailsDlg::OnLvnItemchangedListTmplist)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CH1EUSB_DetailsDlg メッセージ ハンドラ
/********************************************************************************************/
/*[メソッド名] ダイアログの初期化															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		BOOL OnInitDialog()															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		本ダイアログの初期化を行う													*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		BOOL TRUE orFALSE															*/
/********************************************************************************************/
BOOL CH1EUSB_DetailsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定


	// TODO:  ここに初期化を追加してください
	//このダイアログのコントロールの初期化
	ControlInit();									//各コントロールの初期化
	m_dwSystemCondition = SYSTEM_DEVICE_OFF;		//状態変数のセット
	EnabeleControl(m_dwSystemCondition, TRUE);		//コントロール有効

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/********************************************************************************************/
/*[メソッド名] SETボタン押下メッセージ処理													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonComset()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		指定されたCOMをオープンし、デバイスの初期設定を行う							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnBnClickedButtonComset()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	CString strComName;
	int		iReturn;
	DWORD	dwError;
	CString strComment;
	CMsgGuide msgGuide;


	//選択されたＣＯＭポート名を取得する。
	CComboBox *pCom = (CComboBox *)GetDlgItem(IDC_COMBO_COMPORT);
	pCom->GetWindowText(strComName);

	//選択されたＣＯＭポートをオープンする。
	iReturn = m_pH1USBCom->Open(strComName, &dwError);
	if(iReturn != H1USB_OK){
		//COMポートのオープン失敗
		strComment.FormatMessage(IDS_ERR_COMOPEN, strComName);
		SetComment(H1USB_COMMENT_PLUS, strComment, dwError);
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}
	strComment.FormatMessage(IDS_COMOPEN, strComName);
	SetComment(H1USB_COMMENT_NON, strComment, dwError);

	//デバイス初期設定中は、本ダイアログのコントロールを無効にする
	m_dwSystemCondition = SYSTEM_DEVICE_ON;
	EnabeleControl(m_dwSystemCondition, FALSE);

	//タッチセンサーの初期値を表示する
	CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SST);
	pEdit->SetWindowText(_T("OFF"));

	//ハードウェアリセットコマンド送信
	iReturn = GeneralTransaction(H1USB_COM_HWRESET, 0, 0, MSGGUID_SEND_HWRESET, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}
	//暗号化コマンドを送信する
	iReturn = GeneralTransaction(H1USB_COM_SCRAMBLE, 0, 0, MSGGUID_SEND_SCRAMBLE, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}

	//緑のＬＥＤを点灯する。
	iReturn = GeneralTransaction(H1USB_COM_GREENLED_ON, 0, 0, MSGGUID_LED_GREEN, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}

	//ファームウェアVerの取得
	iReturn = GeneralTransaction(H1USB_COM_GETFVER, 0, 0, MSGGUID_SEND_GETFWVER, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}

	//シリアルナンバーの取得
	iReturn = GeneralTransaction(H1USB_COM_GETSNUM, 0, 0, MSGGUID_SEND_GETSERNUM, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}


	//ファームウェアVerを表示する
	CStatic* pSFWVert = (CStatic*)GetDlgItem(IDC_STATIC_SET_FWVER);
	pSFWVert->SetWindowText(m_pH1USBCom->m_cFwVersion);

	//シリアルナンバーを表示する
	CStatic* pStSnum = (CStatic*)GetDlgItem(IDC_STATIC_SET_SERIAL);
	pStSnum->SetWindowText(m_pH1USBCom->m_cSerialNum);


	//メッセージを強制的に処理して、表示を優先する。
	DoEvents();

	//登録済みのテンプレートをデバイスへ送信する
	EnabeleControl(m_dwSystemCondition, FALSE);

	iReturn = SyncDevice();

	////タッチセンサーの通知を許可する。
	//iReturn = GeneralTransaction(H1USB_COM_TUCH_NOTIFY, 0x01, 0, MSGGUID_SEND_TSEN_NOTIFY, H1USB_COMMENT_NON);
	//if(iReturn !=H1USB_OK){	
	//	m_dwSystemCondition = SYSTEM_DEVICE_ERR;
	//	EnabeleControl(m_dwSystemCondition, TRUE);
	//	return;
	//}

	//各タブのパネルを有効にする
	m_dlgTab0.EnableWindow(TRUE);
	m_dlgTab1.EnableWindow(TRUE);
	m_dlgTab2.EnableWindow(TRUE);


	//セッティング終了メッセージ表示
	strComment = msgGuide.EndAppSettingMessage(strComName);
	SetComment(H1USB_COMMENT_NON, strComment, 0);

	//本ダイアログのコントロールの有効
	EnabeleControl(m_dwSystemCondition, TRUE);


	TuchEnable();


}

/********************************************************************************************/
/*[メソッド名]コントロールの初期化															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int ControlInit()															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		本ダイアログのコントロールを初期化する										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ControlInit()
{
	int iReturn = H1USB_OK;

	//各コントロールの初期化
	ControlComportInit();				//COMポートコンボボックス
	ControlTemplateInit();				//テンプレートリスト
	ControlTabInit();					//タブパネル


	//コメント表示エディットボックスを編集不可にする
	CEdit *pEditComment = (CEdit *)GetDlgItem(IDC_EDIT_COMMENT);
	pEditComment->SetReadOnly(TRUE);
	//タッチセンサー状態表示エディットボックスを編集不可にする
	CEdit *pEditTach = (CEdit *)GetDlgItem(IDC_EDIT_SST);
	pEditTach->SetReadOnly(TRUE);

	//機能名を表示する
	m_ftFName.CreateFont( 16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,NULL); 
	m_stFName.SetFont(&m_ftFName);
	m_stFName.SetWindowText("Enroll/Copy/Delete");

	//デバイス情報を初期化する
	CStatic* pSta =  (CStatic*)GetDlgItem(IDC_STATIC_SET_FWVER);
	pSta->SetWindowText(_T(""));									//ファームウェアバージョン
	pSta =  (CStatic*)GetDlgItem(IDC_STATIC_SET_SERIAL);
	pSta->SetWindowText(_T(""));									//シリアルナンバー

	//タッチセンサーポーリング
	m_btCheckPol.SetCheck(TRUE);


	return iReturn;
}

/********************************************************************************************/
/*[メソッド名]	COMポート選択用コンボボックスコントロールの初期化初期化						*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int ControlComportInit()													*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		本ダイアログのコントロールを初期化する										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ControlComportInit()
{
	int iReturn = H1USB_OK;
	int i;

	//COMポート名称（COM1～COM255）をセット
	CString strComPortName;
	CComboBox *pCom = (CComboBox *)GetDlgItem(IDC_COMBO_COMPORT) ;
	for (i = 1; i <= 256; i++ ) {	
		strComPortName.Format(_T("COM%d"), i);
		pCom->InsertString(-1, strComPortName) ;
	}

	// コンボボックスの規定値として、H1E-USBが接続されているCOMポート番号を取得する
	CString strFName = FRIENDLY_NAME ;	// H1E-USBのFriendlyName
	// 関数呼び出し
	int nPort = getCOMPortNumber(strFName);

	// H1E-USB 未接続の場合
	if ( nPort < 0 ) {
		
		// COM1を選択
		pCom->SetCurSel( 0 ) ;
	}
	else {

		// コンボボックスの規定値として、H1E-USBが接続されているCOMポートを表示
		pCom->SetCurSel( nPort - 1 ) ;
	}

	return iReturn;
}

/********************************************************************************************/
/*[メソッド名]	テンプレートリスト表示用リストコントロールの初期化							*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int ControlTemplateInit()													*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレートリスト表示リストコントロールを初期化する						*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ControlTemplateInit()
{
	int iReturn = H1USB_OK;

	ListInit();							//リストコントロール初期化
    ListInsertItem();					//リストアイテム挿入
	Sort();								//ソート処理の起動

	return iReturn;
}

/********************************************************************************************/
/*[メソッド名]	タブコントロールコントロールの初期化										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int ControlTabInit()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		タブパネルを初期化する														*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ControlTabInit()
{
	//テンプレートリストの初期化
	int iReturn = H1USB_OK;

	RECT rectDetDlg;;
	int iXPos,iYpos;

	//タブコントロールを初期化
	this->GetClientRect(&rectDetDlg);
	iXPos = rectDetDlg.right-rectDetDlg.left;
	iYpos = rectDetDlg.bottom-rectDetDlg.top;
	m_tabCtrlDetails.SetWindowPos(&wndTop,0,0,iXPos,iYpos,SWP_SHOWWINDOW);
	m_tabCtrlDetails.InsertItem(0, "Enroll/Copy/Delete");
	m_tabCtrlDetails.InsertItem(1, "Verification");
	m_tabCtrlDetails.InsertItem(2, "Others");
	m_tabCtrlDetails.SetCurSel(0);

	
	//タブ上に張り付けるダイアログを初期化
	RECT rect;
	this->GetWindowRect(&rectDetDlg);
	m_stComGBox.GetWindowRect(&rect);
	iYpos = rect.bottom-rectDetDlg.top;
	ScreenToClient(&rect);

	//Tab0パネルをクリエートして規定の位置に移動する
	m_dlgTab0.Create(IDD_DIALOG_TAB0, &m_tabCtrlDetails);
    m_dlgTab0.SetWindowPos(&wndTop,rect.left,iYpos,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);		//表示

	//Tab1パネルをクリエートして規定の位置に移動する
	m_dlgTab1.Create(IDD_DIALOG_TAB1, &m_tabCtrlDetails);
    m_dlgTab1.SetWindowPos(&wndTop,rect.left,iYpos,0,0,SWP_NOSIZE|SWP_HIDEWINDOW);		//非表示

	//Tab2パネルをクリエートして規定の位置に移動する
	m_dlgTab2.Create(IDD_DIALOG_TAB2, &m_tabCtrlDetails);
    m_dlgTab2.SetWindowPos(&wndTop,rect.left,iYpos,0,0,SWP_NOSIZE|SWP_HIDEWINDOW);		//非表示

	return iReturn;
}

/*====================================================================*/
/* 処理概要  ：H1E-USBが接続されているCOMポートの番号を返却する
/* 引数      ：pFName	H1E-USBのFriendlyName
/* 戻り値    ：COMポートの番号(未接続の場合は -1)
/*====================================================================*/
int CH1EUSB_DetailsDlg::getCOMPortNumber(LPCTSTR pFName)
{

	HDEVINFO hDevInfo = NULL ;		// 列挙デバイス情報
	SP_DEVINFO_DATA DeviceInfoData;	// 1件デバイス情報
	int nPorts = -1 ;				// H1E-USBが接続されているCOMポートの番号
	BYTE byBuffer[1024] = { '\0' } ;

	memset( &DeviceInfoData, 0, sizeof(SP_DEVINFO_DATA) ) ;
	
	// 全ての親デバイス(USBデバイスのみ)としての HDEVINFO を生成
	hDevInfo = SetupDiGetClassDevs( NULL, TEXT("USB"), 0, DIGCF_PRESENT | DIGCF_ALLCLASSES ) ;
	if ( hDevInfo == INVALID_HANDLE_VALUE ) {
		return nPorts;
	}

	// SP_DEVINFO_DATA構造体のサイズをセット
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA) ;

	for ( int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++ ) {

	    // デバイスのフレンドリーネーム(FriendlyName)を取得する

		DWORD dwBuffersize = 0 ;

		int err = SetupDiGetDeviceRegistryProperty(hDevInfo,		// A handle to a device information set
											   &DeviceInfoData,		// A pointer to an SP_DEVINFO_DATA structure
											   SPDRP_FRIENDLYNAME,	// The friendly name of a device
											   NULL,				// PropertyRegDataType (out, optional)
											   byBuffer,			// PropertyBuffer (out, optional)
											   sizeof(byBuffer),	// PropertyBufferSize
											   &dwBuffersize);		// RequiredSize (out, optional)

		// 指定したシリアルポートかどうかをコンペア(H1E-USBのCOMポート以外はスキップ)
		if ( err == FALSE ) {	
			
			continue;
		}
		else {

			// FriendlyName が一致しない場合
			if ( strncmp( (const char*)byBuffer, (const char*)pFName, strlen((const char*)pFName) ) != 0 ) {
			
				continue;
			}
			// FriendlyName が一致
			else {

				// 処理続行

			}
		}

		// H1E-USBのCOMポートが見つかった
		// 装置固有の構成情報取得のためにレジストリキーを開く
		HKEY  hDeviceKey = SetupDiOpenDevRegKey(hDevInfo,	// A handle to a device information set
										  &DeviceInfoData,	// A pointer to an SP_DEVINFO_DATA structure
										  DICS_FLAG_GLOBAL,	// Scope
										  0,				// HwProfile:Scope is DICS_FLAG_GLOBAL, HwProfile is ignored.
										  DIREG_DEV,		// Open a hardware key for the device.
										  KEY_READ);		// registry security access values

		memset( byBuffer, 0, sizeof(byBuffer) ) ;
		DWORD dwPortNameSize = sizeof(byBuffer);

		// 指定されたレジストリキーに所属している、指定されたレジストリエントリのデータ型とデータを取得する
		DWORD dwError = RegQueryValueEx(hDeviceKey,			// キーのハンドル
								  REGSTR_VAL_PORTNAME,		// レジストリエントリ名 "PortName"
								  NULL,						// (予約)
								  NULL,						// データ型が格納されるバッファ(任意)
								  byBuffer,					// データが格納されるバッファ
								  &dwPortNameSize);			// データバッファのサイズ

		if ( dwError != ERROR_SUCCESS ) {
			continue ;
		}

		// 指定されたレジストリキーのハンドルを閉じる
		RegCloseKey( hDeviceKey ) ;

		// COMnの「n」の部分を数値化する
		char buff[50] = { '\0' } ;
		strncpy( buff, (const char *)(byBuffer + 3), 3 ) ;

		// PortName が "COM"から始まっていない場合は飛ばす
		byBuffer[3] = 0 ;
		if (strcmp((const char*)byBuffer, "COM")){
			continue ;
		}
		
		// 数値部分をint型に変換
		nPorts = atoi(buff) ;
	}

	return nPorts ;

}

/********************************************************************************************/
/*[メソッド名]	タブチェンジメッセージ処理													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnTcnSelchangeDtailsTab(NMHDR *pNMHDR, LRESULT *pResult)				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		押されたタブのパネルダイアログを表示する									*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnTcnSelchangeDtailsTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iTab = m_tabCtrlDetails.GetCurSel();

	switch(iTab) {
		case 0:
			m_stFName.SetWindowText("Enroll/Copy/Delete");
			m_dlgTab0.ShowWindow(SW_SHOW);
			m_dlgTab1.ShowWindow(SW_HIDE);
			m_dlgTab2.ShowWindow(SW_HIDE);
			break;
		case 1:
			m_stFName.SetWindowText("Verification");
			m_dlgTab0.ShowWindow(SW_HIDE);
			m_dlgTab1.ShowWindow(SW_SHOW);
			m_dlgTab2.ShowWindow(SW_HIDE);
			break;
	
		case 2:
			m_stFName.SetWindowText("Others");
			m_dlgTab0.ShowWindow(SW_HIDE);
			m_dlgTab1.ShowWindow(SW_HIDE);
			m_dlgTab2.ShowWindow(SW_SHOW);
			break;

		default:
			break;
	
	}
	*pResult = 0;
}

/********************************************************************************************/
/*[メソッド名]	OnOKメッセージ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnOK()																	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		エンターキー押下の場合にメッセージを逃がすようにする						*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//CDialog::OnOK();
}

/********************************************************************************************/
/*[メソッド名]	Endボタン押下時のメッセージ処理												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnOK()																	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デバイスをリセットして、終了する											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnBnClickedButtonMain()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_pH1USBCom->TransToDevice(H1USB_COM_HWRESET, 0, 0);

	CDialog::OnOK();
}

/********************************************************************************************/
/*[メソッド名]	テンプレートリストコントロールの初期化										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int ListInit()																*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デバイスをリセットして、終了する											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ListInit(void)
{
    LVCOLUMN    lvc;
    int         i;
	TCHAR*      caption[] = {_T("Bank"), _T("Template"), _T("Time")};
	int			iWidth[] = {60,60,140};

    const int   clmNum = sizeof caption /sizeof(TCHAR*) ;
    int         err = 0;

	m_listCtrlTempList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    for (i = 0; i < clmNum; i++)
    {
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;	// 有効フラグ
		lvc.fmt			= LVCFMT_CENTER;
		lvc.iSubItem    = i;											// サブアイテム番号
        lvc.pszText     = caption[i];									// 見出しテキスト
        lvc.cx          = iWidth[i];									// 横幅
        if (m_listCtrlTempList.InsertColumn(i, &lvc) == -1)
		{
			err = 1;
			break;
		}
    }
    
    return err;
}

/********************************************************************************************/
/*[メソッド名]	テンプレートリストにアイテムを追加する										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int ListInsertItem()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デバイスをリセットして、終了する											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ListInsertItem(void)
{
 
    LVITEM       lvi;
    CString      str;
    int          i, index = 0;
    int          err = 0;
	int			iLists;
	CString		strBankNum;
	CString		strTempNum;
	CString		strTime;    

	//すべてのアイテムを削除
	m_listCtrlTempList.DeleteAllItems();
	DoEvents();

	//あらかじめ最大数を確保する
	m_listCtrlTempList.SetItemCount(MAX_TEMPLATE_TOTAL);
	DoEvents();

	m_listCtrlTempList.ShowScrollBar( SB_VERT, FALSE );

	//現在ファイルに登録されているテンプレート数を取得
	iLists = m_pTemplateFile->m_uiTotalTemplates;

	 //登録されているテンプレートの個数を表示
	CStatic* pSta = (CStatic*)GetDlgItem(IDC_STATIC_TEMPS);
	str.Format(_T("[%5d/2000]"), iLists);
	pSta->SetWindowText(str);

	//登録されているテンプレートの情報をリストに表示する
    for (i = 0; i < iLists; i++)
    {
        lvi.mask = LVIF_TEXT;
        // バンク番号
        if (!err)
        {
            lvi.iItem = i;
            lvi.iSubItem = 0;
			strBankNum.Format(_T("%04X"), m_pTemplateFile->m_pTempList[i].uiBankNum);
            lvi.pszText = strBankNum.GetBuffer();
            if ((index = m_listCtrlTempList.InsertItem(&lvi)) == -1) err = 1;
        }
        // テンプレート番号
        if (!err)
        {
			lvi.iItem = index;
			lvi.iSubItem = 1;
			strTempNum.Format(_T("%02X"), m_pTemplateFile->m_pTempList[i].uiTempNum);
            lvi.pszText = strTempNum.GetBuffer();
            if (!m_listCtrlTempList.SetItem(&lvi)) err = 1;
        }
        // 登録時間
        if (!err)
        {
            lvi.iItem = index;
            lvi.iSubItem = 2;
			strTime.Format(_T("%4d/%02d/%02d %02d:%02d:%02d"), 
							m_pTemplateFile->m_pTempList[i].time.wYear, 
							m_pTemplateFile->m_pTempList[i].time.wMonth, 
							m_pTemplateFile->m_pTempList[i].time.wDay, 
							m_pTemplateFile->m_pTempList[i].time.wHour,
							m_pTemplateFile->m_pTempList[i].time.wMinute,
							m_pTemplateFile->m_pTempList[i].time.wMinute);
            lvi.pszText = strTime.GetBuffer();
            if (!m_listCtrlTempList.SetItem(&lvi)) err = 1;
        }
        if (err) break;

    }
    
    return err;
}

/********************************************************************************************/
/*[メソッド名]	ソート処理のコールバック処理												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int :CompareFunc(LPARAM param1, LPARAM param2, LPARAM param3)				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		呼ばれるごとに比較して昇順にセットする										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：0																	*/
/********************************************************************************************/
int CALLBACK CH1EUSB_DetailsDlg::CompareFunc(LPARAM param1, LPARAM param2, LPARAM param3)
{

	long lNUm1, lNUm2;
    char *endptr;
	int iReturn;
	iReturn = 0;

	CWinApp* app = AfxGetApp();
	CH1EUSB_DetailsDlg* pdlg = (CH1EUSB_DetailsDlg*)app->GetMainWnd();

	// 比較される２つのアイテムから「Bank」の文字列を取得する
	CString str1 = pdlg->m_listCtrlTempList.GetItemText(param1, 0);
	lNUm1 = strtol(str1.GetString(), &endptr, 16);
	lNUm1 = lNUm1*0x100;
	CString str2 = pdlg->m_listCtrlTempList.GetItemText(param2, 0);
	lNUm2 = strtol(str2.GetString(), &endptr, 16);
	lNUm2 = lNUm2*0x100;
	str1 = pdlg->m_listCtrlTempList.GetItemText(param1, 1);
	lNUm1 += strtol(str1.GetString(), &endptr, 16);
	str2 = pdlg->m_listCtrlTempList.GetItemText(param2, 1);
	lNUm2 += strtol(str2.GetString(), &endptr, 16);

	if(lNUm1>lNUm2){
		iReturn = 1;
	}
	if(lNUm1<lNUm2){
		iReturn = -1;
	}

	return iReturn;
}

/********************************************************************************************/
/*[メソッド名]	リストアイテムのリナンバー処理												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void RenumberItem()															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		リスト昇順に振りなおす														*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::RenumberItem()
{
	LV_ITEM lvItem;

	for( int i = 0; i < m_listCtrlTempList.GetItemCount(); i++ ) {
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.mask = LVIF_PARAM;
		lvItem.lParam = i;							// ここで番号をアイテムに指定する
		m_listCtrlTempList.SetItem(&lvItem);
	}
}

/********************************************************************************************/
/*[メソッド名]	テンプレートリストのソートを起動											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void RenumberItem()															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレートリストのソートを開始する										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::Sort() 
{
	if(m_pTemplateFile->m_uiTotalTemplates>0){
		RenumberItem();										// SortItems関数を呼び出す前には必ず実行
		m_listCtrlTempList.SortItems(CompareFunc, FALSE);
	}
}


/********************************************************************************************/
/*[メソッド名]	コメント表示																*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError)				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コメントエディットボックスに表示する										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError)
{
	CString strComment;
	CString strErr;
	CString strWork;

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_COMMENT);
	strComment	= pMessage;
	strErr.Format(_T("[Error=%08Xh]"),dwError);
	strWork.Empty();
	if(dwMode&H1USB_COMMENT_ADD){
		pEdit->GetWindowText(strWork);
		strWork += _T("\r\n");
	}
	strComment = strWork + pMessage;
	if(dwMode&H1USB_COMMENT_PLUS){
		strComment += strErr;
	}
	pEdit->SetWindowText(strComment) ;
	DoEvents();
}

/********************************************************************************************/
/*[メソッド名]	タッチセンサー情報通知コールバック関数										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		LRESULT OnTuchNotification(DWORD dwStatus, LPVOID pObj)						*/
/*																							*/
/*[内容]		タッチセンサー情報により、センサー状態表示エディットボックスにON/OFF表示する*/
/*				CH1USBComクラスよりコールバックされる										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：0																	*/
/********************************************************************************************/
LRESULT CH1EUSB_DetailsDlg::OnTuchNotification(DWORD dwStatus, LPVOID pObj)
{
	CH1EUSB_DetailsDlg* pDlg = (CH1EUSB_DetailsDlg*) pObj;
	CEdit* pEdit = (CEdit *)pDlg->GetDlgItem(IDC_EDIT_SST);

	switch(dwStatus){
		case H1USB_STATUS_TUCH_ON:
			//タッチセンサーステータスにONを表示
			pEdit->SetWindowText(_T("ON"));
			break;
		case H1USB_STATUS_TUCH_OFF:
			//タッチセンサーステータスにOFFを表示
			pEdit->SetWindowText(_T("OFF"));
			break;
		case H1USB_STATUS_INVDATA:
			pEdit = (CEdit *)pDlg->GetDlgItem(IDC_EDIT_COMMENT);
			pEdit->SetWindowText(_T("Recived Invalid data"));
			break;
		default:
			break;
	}

	return 0;
}

/********************************************************************************************/
/*[メソッド名]	送信終了通知コールバック関数												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		LRESULT OnSendEndNotification(DWORD dwStatus, LPVOID pObj)					*/
/*																							*/
/*[内容]		送信に時間がかかるコマンドの場合にコマンド送信が終了でコールバックされる	*/
/*				CH1USBComクラスよりコールバックされる										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：0																	*/
/********************************************************************************************/
LRESULT CH1EUSB_DetailsDlg::OnSendEndNotification(DWORD dwStatus, LPVOID pObj)
{

	return 0;
}

/********************************************************************************************/
/*[メソッド名]	タッチセンサーONポーリング													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		BOOL PollingTuchOnState()													*/
/*																							*/
/*[内容]		タッチセンサーがONをポーリングする											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：TRUE																*/
/********************************************************************************************/
BOOL CH1EUSB_DetailsDlg::PollingTuchOnState()
{
	BOOL bReturn;
	CString strState;
	CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SST);

	bReturn = FALSE;

	EnabeleControl(NULL, FALSE);

	int i;

	for(i=0; i<300; i++){
		if(!m_btCheckPol.GetCheck()){
			//センサーポーリングなし
			bReturn = TRUE;
			break;
		}
		DoEvents();
		Sleep(10);
		pEdit->GetWindowText(strState);
		if(strState == _T("ON")){
			//タッチセンサーがONになった
			bReturn = TRUE;
			break;
		}
	};

	EnabeleControl(NULL, TRUE);


	return bReturn;
}


/********************************************************************************************/
/*[メソッド名]	テンプレート情報同期														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int SyncDevice()															*/
/*																							*/
/*[内容]		テンプレートファイルのテンプレート情報をデバイスに送信し同期を行う			*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::SyncDevice()
{
	int iReturn;
	CString strComment;
	CMsgGuide msgGuide;

	iReturn = H1USB_OK;

	if(!m_pTemplateFile){
		//テンプレートファイルクラスが生成されていない
		return H1USB_ERR_TEMP_NOTOPEN;
	}
	UINT uiTotalTempLates =	m_pTemplateFile->m_uiTotalTemplates;	//現在登録されているテンプレイト数
	PTEMPLIST pTempList = m_pTemplateFile->m_pTempList;				//テンプレイトリスト
	if(uiTotalTempLates == 0){
		//登録済みのテンプレートが存在しない
		return H1USB_OK;
	}

	//登録済みのテンプレートをデバイスに送信する
	TEMPU TempU;
	UINT uiSendCounts;
	UINT ui;
	UINT uiTemps;
	UINT uiBankCount;
	UINT uiCurrentBank;
	UINT uiBanks;
	UINT uiLists;
	DWORD dwError;

	dwError  = 0;

	uiBanks = m_pTemplateFile->m_uiBanks;
	uiLists = m_pTemplateFile->m_uiTotalTemplates;

	//すべてのテンプレートをデバイスより削除する
	iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_DELTEMPLATE, -2, 0);
	if(iReturn != H1USB_OK){

	}

	uiSendCounts = 0;

	//存在するバンクの個数分、バンクをチェンジしながらすべてのテンプレート情報をデバイスへ送信する
	for(uiBankCount=0; uiBankCount<uiBanks; uiBankCount++){
		uiCurrentBank = m_pTemplateFile->m_pBankInfo[uiBankCount].uiBankNum;
		uiTemps = m_pTemplateFile->m_pBankInfo[uiBankCount].uiCounts;
		//バンクの設定
		iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_CHANGE_BANK, (WPARAM)uiCurrentBank, 0);
		if(iReturn != H1USB_OK){
			return iReturn;
		}
		//設定したバンクのテンプレートファイルリストをオープン
		iReturn = m_pTemplateFile->Open(uiCurrentBank);
		if(iReturn != H1USB_OK){
			m_pTemplateFile->Close();
			return iReturn;
		}
		//バンクに存在するテンプレート情報を取得
		for(ui=0; ui<uiTemps; ui++){
			iReturn = m_pTemplateFile->Read(&TempU, 1);
			if(iReturn != H1USB_OK){
				break;
			}
			//取得したテンプレートをセット
			iReturn = m_pH1USBCom->SetTempU(&TempU, 1, TRUE);
			if(iReturn != H1USB_OK){
				break;
			}
			//テンプレート送信トランザクションを実行
			iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_SETTEMPLATE, 0, 0);
			if(iReturn != H1USB_OK){
				break;
			}
			//登録したテンプレート情報をコメントエディットボックスに表示
			uiSendCounts++;
			strComment = msgGuide.SendFormatMessage(uiCurrentBank, TempU.uiTmpNum, uiSendCounts, uiLists);
			SetComment(H1USB_COMMENT_NON, strComment, iReturn);
		}
		//現在のバンクのテンプレートファイルをクローズ
		m_pTemplateFile->Close();
		if(iReturn !=H1USB_OK){
			break;
		}
	}
	//同期終了メッセージを表示
	strComment = msgGuide.SendCompFormatMessage(uiCurrentBank, TempU.uiTmpNum, uiSendCounts, uiLists, iReturn, dwError);
	SetComment(H1USB_COMMENT_NON, strComment, iReturn);
	return iReturn;
}


/********************************************************************************************/
/*[メソッド名]	コントロール更新															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int UpdateContorol()														*/
/*																							*/
/*[内容]		各コントロールをアップデートする											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::UpdateContorol()
{
    ListInsertItem();					//リストアイテム挿入
	Sort();								//ソート処理の起動
	DoEvents();
	m_dlgTab0.UpdateContorol();			//Tab0パネルダイアログ更新
	m_dlgTab1.UpdateContorol();			//Tab1パネルダイアログ更新
	m_dlgTab2.UpdateContorol();			//Tab2パネルダイアログ更新
}

/********************************************************************************************/
/*[メソッド名]	メッセージ優先実行															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void DoEvents()																*/
/*																							*/
/*[内容]		キューに溜まっているメッセージを優先的に実行させる							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){
		if (!AfxGetThread()->PumpMessage()){
			return;
		}
	}


}

/********************************************************************************************/
/*[メソッド名]	カラーメッセージ処理														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)						*/
/*																							*/
/*[内容]		編集不可のエディットボックスの背景を白に変更する							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		HBRUSH																		*/
/********************************************************************************************/
HBRUSH CH1EUSB_DetailsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ここで DC の属性を変更してください。
	int nID = pWnd->GetDlgCtrlID();       
	if (( nID == IDC_EDIT_SST)||( nID == IDC_EDIT_COMMENT)) {
       pDC->SetBkColor(RGB(255,255,255));
	   return (HBRUSH)GetStockObject(WHITE_BRUSH);
	}

	// TODO:  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

/********************************************************************************************/
/*[メソッド名]	テンプレートリストの選択変更メッセージ処理									*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnLvnItemchangedListTmplist(NMHDR *pNMHDR, LRESULT *pResult)			*/
/*																							*/
/*[内容]		テンプレートの選択された行のバンク番号とテンプレート番号を					*/
/*				各エディットボックスにする													*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnLvnItemchangedListTmplist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) {

		int index;
		CString strBank;
		CString strTemp;

		//選択された行を取得
		index = ListView_GetNextItem(m_listCtrlTempList.m_hWnd, -1, LVNI_SELECTED);
		if(index >= 0){
			strBank = m_listCtrlTempList.GetItemText(index, 0);		//バンク番号を取得
			strTemp = m_listCtrlTempList.GetItemText(index, 1);		//テンプレート番号を取得
			m_dlgTab0.UpdateNummber(strBank, strTemp);				//Tab0パネルへ通知してアップデート
			m_dlgTab1.UpdateNummber(strBank, strTemp);				//Tab1パネルへ通知してアップデート
		}
    }

	*pResult = 0;
}

/********************************************************************************************/
/*[メソッド名]	コントロールの有効・無効													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void EnabeleControl(DWORD dwMode, BOOL bEnable)								*/
/*																							*/
/*[内容]		各コントロールの有効・無効を設定する										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::EnabeleControl(DWORD dwMode, BOOL bEnable)
{
	CButton*	pBtComSet = (CButton*)GetDlgItem(IDC_BUTTON_COMSET);
	CButton*	pBtComMain = (CButton*)GetDlgItem(IDC_BUTTON_MAIN);
	CTabCtrl*	pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_DTAILS_TAB);

	pBtComSet->EnableWindow(bEnable);				//COM設定ボタン
	pBtComMain->EnableWindow(bEnable);				//終了ボタン
	pTabCtrl->EnableWindow(bEnable);				//タブ選択ボタン

	DWORD dwFixMode = dwMode;
	if(!dwMode){
		dwFixMode = m_dwSystemCondition;
	}
	m_dlgTab0.EnabeleControl(dwFixMode, bEnable);	//Tab0へ指示
	m_dlgTab1.EnabeleControl(dwFixMode, bEnable);	//Tab1へ指示
	m_dlgTab2.EnabeleControl(dwFixMode, bEnable);	//Tab2へ指示

	DoEvents();

}
/********************************************************************************************/
/*[メソッド名]	汎用トランザクション処理													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,		*/ 
/*															UINT uiMsgID, UINT uiMode)		*/
/*																							*/
/*[内容]		指定のトランザクション処理を実施し、対応するコメントを表示する				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode)
{
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	EnabeleControl(NULL, FALSE);
	
	if(uiMsgID != MSGGUID_NODISP){	
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//コマンド送信メッセージ
			SetComment(uiMode, strMessage, 0);
		}
	}
	//トランザクション実行
	iReturn = m_pH1USBCom->TransToDevice(uiTransID, wParam, lParam);
	//コマンド終了メッセージ
	if(iReturn < H1USB_OK){
		//strMessage.Format(_T("Error:%d"), iReturn);
		strMessage = msgGuide.ErrorGeneralFormatMessage(iReturn);
		SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	}
	EnabeleControl(NULL, TRUE);
	
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名]	描画メッセージ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnPaint()																*/
/*																							*/
/*[内容]		描画メッセージにより、テンプレートリストの水平スクロールバーを非表示にする	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CDialog::OnPaint() を呼び出さないでください。

	//テンプレートリストの水平スクロールバーを非表示にする
	m_listCtrlTempList.ShowScrollBar( SB_VERT, TRUE );

}


/********************************************************************************************/
/*[メソッド名]	タッチセンサー有効															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void TuchEnable()																*/
/*																							*/
/*[内容]		描画メッセージにより、テンプレートリストの水平スクロールバーを非表示にする	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::TuchEnable()
{
	int iReturn;
	//タッチセンサーの通知を許可する。
	//iReturn = GeneralTransaction(H1USB_COM_TUCH_NOTIFY, 0x01, 0, MSGGUID_SEND_TSEN_NOTIFY, H1USB_COMMENT_NON);
	iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_TUCH_NOTIFY, 0x01, 0);
	if(iReturn ==H1USB_OK){
		////タッチセンサーの初期値を表示する
		//CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SST);
		//pEdit->SetWindowText(_T("OFF"));
	}
	
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名]	タッチセンサー無効															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void TuchDisable()															*/
/*																							*/
/*[内容]		描画メッセージにより、テンプレートリストの水平スクロールバーを非表示にする	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::TuchDisable()
{
	int iReturn;
	//タッチセンサーの通知を許可する。
	//iReturn = GeneralTransaction(H1USB_COM_TUCH_NOTIFY, 0x02, 0, MSGGUID_SEND_TSEN_NOTIFY, H1USB_COMMENT_NON);
	iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_TUCH_NOTIFY, 0x02, 0);
	if(iReturn ==H1USB_OK){
		//タッチセンサーの初期値を表示する
		//CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SST);
		//pEdit->SetWindowText(_T(""));
	}
	
	return iReturn;
}

