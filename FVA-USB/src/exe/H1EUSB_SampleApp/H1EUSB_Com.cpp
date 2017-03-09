#include "stdafx.h"
#include "H1EUSB.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_Dbg.h"
/********************************************************************************************/
/* CH1USBComクラスプログラム																*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20120110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/********************************************************************************************/
/********************************************************************************************/
/*[メソッド名] CH1USBComクラスのコンストラクタ												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		CH1USBCom(PH1ECOMOBJEP pH1EComObjEP)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コンストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1USBCom::CH1USBCom(PH1ECOMOBJEP pH1EComObjEP)
{
	//メンバー変数の初期化
	m_hCom	= 0;
	m_hTEnd	= 0;
	memset( m_byCurrentWorkKey, 0, sizeof(m_byCurrentWorkKey) ) ;	// 暗号鍵
	memset( m_uKttWork, 0, sizeof(m_uKttWork) ) ;					// 拡大鍵
	m_bCamelliaEnabled = FALSE ;									// 暗号通信状態か否かを表すフラグ
	m_pwEnableBank = 0;
	m_wEnableBankCount = 0;
	m_iLastError = H1USB_OK;

	//エンドポイント情報を登録
	m_H1EComObjEP	 = *pH1EComObjEP;

	//変数の初期化
	ZeroMemory(m_cFwVersion, sizeof(m_cFwVersion));
	ZeroMemory(m_cSerialNum, sizeof(m_cSerialNum));

	//テンプレートユニットテーブルの初期化
	SetTempU(NULL, 0, TRUE);

	//トランザクション中フラグOFF
	m_bTransEnable = FALSE;

}
/********************************************************************************************/
/*[メソッド名] CH1USBComクラスのデストラクタ												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		~CH1USBCom(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コンストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1USBCom::~CH1USBCom(void)
{
	if(m_pwEnableBank){
		delete[] m_pwEnableBank;
	}
	if(m_hCom){
		CloseHandle(m_hCom);
	}
	if(m_hTEnd){
		CloseHandle(m_hTEnd);
	}
}




/********************************************************************************************/
/*[メソッド名] エンドポイント処理スタート													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		StartEndPoint(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		エンドポイント情報を登録して、エンドポイントの機能を開始する				*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::StartEndPoint(void)
{
A2GDBG_OUTSTRING(_T("StartEndPoint() Entry\n"));
	int		iResult;
	iResult = H1USB_OK;

	//受信スレッドからのコールバックルーチンを登録する。
	A2GOBJTHREADPROCS	A2objThreadProcs;
	A2objThreadProcs.A2GObjThreadBefor	= OnThreadBefor;			//スレッドループ前
	A2objThreadProcs.A2GObjOnThread		= OnThread;					//スレッドメッセージループ
	A2objThreadProcs.A2GObjThreadAfter	= OnThreadAfter;			//スレッドループ後
	CA2GObjThread::SetThreadProcs(this, &A2objThreadProcs);

	m_hTEnd = CreateEvent(NULL, TRUE, FALSE, NULL);					//トランザクション終了イベントの作成

	//通信イベントの設定
	CA2GObjThread::SetMessageProc(OnComMessage, this);
	iResult = CA2GObjThread::CreateThread(THREAD_PRIORITY_NORMAL);

A2GDBG_OUTSTRING(_T("StartEndPoint() Exit\n"));
	return iResult;
}

/********************************************************************************************/
/*[メソッド名] エンドポイント処理終了														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		StopEndPoint(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		エンドポイントの機能を停止する												*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::StopEndPoint(void)
{
A2GDBG_OUTSTRING(_T("StopEndPoint() Entry\n"));

	//エンドポイントスレッドを終了する。
	CA2GObjThread::EndThread();


A2GDBG_OUTSTRING(_T("StopEndPoint() Exit\n"));
	return H1USB_OK;
}

/********************************************************************************************/
/*[メソッド名] COMポートオープン															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		Open(LPCTSTR pComPort, LPDWORD pdwError)									*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コンストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::Open(LPCTSTR pComPort, LPDWORD pdwError)
{
	int			iReturn;
	BYTE		*bp = NULL ;
	HANDLE		hCom;
	iReturn		= H1USB_OK;
	*pdwError	= 0;
	DWORD		dwError;
	CString		strComPort;

	strComPort	= _T("\\\\.\\");
	strComPort	+= pComPort;
	if(m_hCom){
		//既にポートがオープンされていれば一度クローズする。
		Close(&dwError);
	}

	// COMポートをオープン
	hCom = CreateFile(strComPort,	GENERIC_READ|GENERIC_WRITE, 0, NULL, 
						OPEN_EXISTING, 0, NULL ) ;
	if ( hCom == INVALID_HANDLE_VALUE ) {
		// オープンに失敗した場合
		*pdwError = GetLastError();
		return H1USB_ERR_COMOPEN;
	}
	m_hCom = hCom;

	//DCB構造体セット
	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
	dcb.DCBlength	= sizeof(DCB);
	dcb.fBinary		= 1;
	dcb.fParity		= 1;
	dcb.BaudRate	= CBR_57600;
	dcb.ByteSize	= 8;
	dcb.Parity		= ODDPARITY;
	dcb.StopBits	= ONESTOPBIT;
	SetCommState(m_hCom, &dcb);

	COMMTIMEOUTS cts;
	cts.ReadTotalTimeoutConstant	= 10000;
	cts.ReadTotalTimeoutMultiplier	= 0;
	cts.WriteTotalTimeoutConstant	= 10000;
	cts.WriteTotalTimeoutMultiplier	= 0;
	SetCommTimeouts(m_hCom, &cts);

	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] COMクローズ																	*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		Close(LPDWORD pdwError)														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		COMポートをクローズする														*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::Close(LPDWORD pdwError)
{
	int		iReturn;
	iReturn		= H1USB_OK;
	*pdwError	= 0;

	if(m_hCom){
		CloseHandle(m_hCom);
		m_hCom = 0;
	}
	m_hCom = 0;
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] 有効バンクセット																*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		SetEnableBank(WORD* pwEnableBank, WORD wEnableBankCount)					*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		バンク認証時のバンクナンバーを登録する										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::SetEnableBank(WORD* pwEnableBank, WORD wEnableBankCount)
{
	int iReturn;
	WORD w;

	iReturn		= H1USB_OK;

	if(m_pwEnableBank){
		//以前既に配列が登録されているので、一度削除
		delete[] m_pwEnableBank;
	}
	//指定された配列分有効バンクの配列を確保
	m_pwEnableBank = new WORD[wEnableBankCount];

	//有効バンクの配列と個数をコピー
	for(w=0; w<wEnableBankCount; w++){
		m_pwEnableBank[w] = pwEnableBank[w];
	}
	m_wEnableBankCount = wEnableBankCount;
	return iReturn;
}


/********************************************************************************************/
/*[メソッド名] テンプレート情報セット														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		SetTempU(PTEMPU pTempU, UINT uiCounts, BOOL bInit)							*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレート管理情報に指定されたテンプレート情報をセットする。				*/
/*				bInit:TRUEであれば、テンプレート管理情報の初期化を行ってからセットする		*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::SetTempU(PTEMPU pTempU, UINT uiCounts, BOOL bInit)
{
	int iReturn;
	UINT ui;
	UINT uiMaxCount;

	iReturn		= H1USB_OK;
	if(bInit){
		//初期化フラグがセットされている
		m_uiTempUs = 0;								//セットされているテンプレート情報の個数をクリア
		ZeroMemory(m_TempU, sizeof(m_TempU));		//テンプレート管理情報を初期化
	}

	//指定されたテンプレート情報をセットする
	uiMaxCount = MAX_NINSHO_TEMPS - m_uiTempUs;
	if((pTempU)&&(uiCounts)){
		if(uiCounts <= uiMaxCount){
			for(ui=0; ui<uiCounts; ui++){
				m_TempU[m_uiTempUs] = pTempU[ui];
				m_uiTempUs++;
			}
		}
		else{
			//最大数に達している
			iReturn = H1USB_ERR_BIGGER;
		}
	}
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] テンプレート情報取得															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		GetTempU(PTEMPU pTempU)														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		最新のテンプレートを取得する												*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::GetTempU(PTEMPU pTempU)
{
	int iReturn;
	iReturn = H1USB_OK;

	if(m_uiTempUs > 0){
		*pTempU = m_TempU[m_uiTempUs-1];
	}
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] トランザクション																*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		TransToDevice(UINT uiMsg, WPARAM wParam, LPARAM lParam)						*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デバイスにトランザクション処理を開始し、終了を待つ							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::TransToDevice(UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int iReturn;
	DWORD dwWaitObject;

	//終了イベントをリセット
	ResetEvent(m_hTEnd);

	iReturn = H1USB_OK;

	m_bTransEnable = TRUE;

	//スレッドのメッセージループへトランザクションメッセージをポスト
	::PostThreadMessage(m_uEPThreadID, uiMsg, wParam, lParam);

	//スレッドのトランザクション処理の終了を待つ
	dwWaitObject = WaitForSingleObject(m_hTEnd, 15000);
	if(dwWaitObject == WAIT_OBJECT_0){
		//トランザクション正常終了
		iReturn = m_iLastError;
	}
	else{
		//タイムアウト
		iReturn = H1USB_ERR_TRANSTOUT;
	}

	m_bTransEnable = FALSE;
	return iReturn;
}

/********************************************************************************************/
/* 基本クラスからのコールバックスエントリー													*/
/********************************************************************************************/
/********************************************************************************************/
/*[メソッド名] スレッドメッセージ処理ループ前処理											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		LRESULT OnThreadBefor(LPVOID pObj)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		実装せず																	*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：0																	*/
/********************************************************************************************/
LRESULT CALLBACK CH1USBCom::OnThreadBefor(LPVOID pObj)
{
A2GDBG_OUTSTRING(_T("OnThreadBefor() Entry\n"));


A2GDBG_OUTSTRING(_T("OnThreadBefor() Exit\n"));
	return 0;
}


/********************************************************************************************/
/*[メソッド名] スレッドループ定周期処理														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		LRESULT OnThread(LPVOID pObj)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		発見サービス指示のコールバックスタブ										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：0																	*/
/********************************************************************************************/
LRESULT CALLBACK CH1USBCom::OnThread(LPVOID pObj)
{
A2GDBG_OUTSTRING(_T("OnThread() Entry\n"));
	CH1USBCom* pH1USBCom  = (CH1USBCom*)pObj;

	int		iReturn;
	DWORD	dwStat;

	//デバイスよりタッチセンサーの状態通知（非同期）があったかチェックする
	iReturn = pH1USBCom->CheckASyncData(&dwStat);
	if(iReturn == H1USB_NOTIFY){
		//通知があったので非同期受信の通知をダイアログにコールバックする
		pH1USBCom->m_H1EComObjEP.H1ECallBacks.OnTuchNotification(dwStat, pH1USBCom->m_H1EComObjEP.pObj);
	}

A2GDBG_OUTSTRING(_T("OnThread() Exit\n"));
	return 0;
}



/********************************************************************************************/
/*[メソッド名] スレッドメッセージ処理ループ後処理											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		LRESULT OnThreadAfter(LPVOID pObj)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		実装せず																	*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：0																	*/
/********************************************************************************************/
LRESULT CALLBACK CH1USBCom::OnThreadAfter(LPVOID pObj)
{
A2GDBG_OUTSTRING(_T("OnThreadAfter() Entry\n"));



A2GDBG_OUTSTRING(_T("OnThreadAfter() Exit\n"));

	return 0;
}

/********************************************************************************************/
/*[メソッド名] 非同期データチェック処理														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int CheckASyncData(LPDWORD pdwState)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		非同期の受信データにより、タッチセンサーの状態をチェックする				*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：0																	*/
/********************************************************************************************/
int CH1USBCom::CheckASyncData(LPDWORD pdwState)
{
	int			iReturn;
	DWORD		dwRecvSize;
	BYTE		recv[2048] ;								// 受信用
	BYTEWORD	bw;	

	iReturn = H1USB_OK;

	//非同期でのデータ受信チェック
	dwRecvSize = getBuffSize() ;								// 受信データバッファのバイト数取得

	if(dwRecvSize > 0){
		memset( recv, 0, sizeof(recv) ) ;
		Sleep(10);
		dwRecvSize = getBuffSize() ;							// 受信データバッファのバイト数取得
		// 受信処理
		if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	
			// ReadFile失敗
			iReturn = H1USB_ERR_READFILE;
		}
		else {
			iReturn = H1USB_NOTIFY;
			bw.byte[0] = recv[2];
			bw.byte[1] = recv[1];
			if ((recv[0] == 0x00) && (bw.word == 2) && (recv[3] == 0x74) ){
				//タッチセンサーの状態に変化があった
				if( recv[4] == 0x01){
					//タッチセンサーON
					*pdwState = H1USB_STATUS_TUCH_ON;
				}
				else{
					//タッチセンサーOFF
					*pdwState = H1USB_STATUS_TUCH_OFF;
				}
			}
			else{
				//サービス外のデータを受信した
				*pdwState = H1USB_STATUS_INVDATA;
			}
		}
	}
	return iReturn;
}


/********************************************************************************************/
/*[メソッド名] メッセージ処理																*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int OnComMessage(UINT uiMsg, WPARAM wParam, LPARAM lParam, LPVOID pObj)		*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		スレッド処理のメッセージコールバック処理									*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：THREAD_CBCODE_NORMAL												*/
/********************************************************************************************/
LRESULT CALLBACK CH1USBCom::OnComMessage(UINT uiMsg, WPARAM wParam, LPARAM lParam, LPVOID pObj)
{
A2GDBG_OUTSTRING(_T("OnComMessage() Entry\n"));
	CH1USBCom* pH1USBCom  = (CH1USBCom*)pObj;
	BYTE	byMatchMemNo;
	WORD	wMachBank;
	TEMPU	tempu;

	int		iReturn;

	iReturn = H1USB_OK;

	//トランザクションIDにより、処理をコールする
	switch(uiMsg){
		case H1USB_COM_SCRAMBLE:	iReturn = pH1USBCom->TransScranmble();								break;
		case H1USB_COM_GREENLED_ON:	iReturn = pH1USBCom->TransLedGreen();								break;
		case H1USB_COM_REDLED_ON:	iReturn = pH1USBCom->TransLedRed();									break;
		case H1USB_COM_GRLED_ON:	iReturn = pH1USBCom->TransLedGR();									break;
		case H1USB_COM_LED_OFF:		iReturn = pH1USBCom->TransLedOff();									break;
		case H1USB_COM_TUCH_NOTIFY:	iReturn = pH1USBCom->TransTuchNotify(wParam);						break;
		case H1USB_COM_GETFVER:		iReturn = pH1USBCom->TransGetFwVersion(pH1USBCom->m_cFwVersion);	break;
		case H1USB_COM_GETSNUM:		iReturn = pH1USBCom->TransGetSerialNum(pH1USBCom->m_cSerialNum);	break;
		case H1USB_COM_BUZSTART:	iReturn = pH1USBCom->TransBUZStart();								break;
		case H1USB_COM_BUZSTOP:		iReturn = pH1USBCom->TransBUZStop();								break;
		case H1USB_COM_SECLOW:		iReturn = pH1USBCom->TransSetSecurity(0x01);						break;
		case H1USB_COM_SECMIDLOW:	iReturn = pH1USBCom->TransSetSecurity(0x02);						break;
		case H1USB_COM_SECMIDLE:	iReturn = pH1USBCom->TransSetSecurity(0x03);						break;
		case H1USB_COM_SECMIDHG:	iReturn = pH1USBCom->TransSetSecurity(0x04);						break;
		case H1USB_COM_SECHIHGH:	iReturn = pH1USBCom->TransSetSecurity(0x05);						break;
		case H1USB_COM_NINSHO:		iReturn = pH1USBCom->TransNinsho(wParam, lParam, &byMatchMemNo);	break;
		case H1USB_COM_NINSHO_BANK:
			if(pH1USBCom->m_pwEnableBank){
				iReturn = pH1USBCom->TransNinshoBank(pH1USBCom->m_pwEnableBank, 
											pH1USBCom->m_wEnableBankCount, &byMatchMemNo, &wMachBank);
			}
			break;
		case H1USB_COM_NINSHO_TEMP:
			iReturn = pH1USBCom->TransNinshoWithTemplate(pH1USBCom->m_TempU, pH1USBCom->m_uiTempUs, &byMatchMemNo);			break;
		case H1USB_COM_GETTEMPLATE:			
			iReturn = pH1USBCom->TransGetTemplate(&tempu);
			if(iReturn == H1USB_OK){
				iReturn = pH1USBCom->SetTempU(&tempu, 1, TRUE);
			}			
			break;
		case H1USB_COM_CHANGE_BANK: iReturn = pH1USBCom->TransChangeBank((WORD)wParam); 				break;
		case H1USB_COM_SETTEMPLATE:	iReturn = pH1USBCom->TransSetTemplate(pH1USBCom->m_TempU);			break;
		case H1USB_COM_DELTEMPLATE:	iReturn = pH1USBCom->TransDelTemplate((int)wParam);					break;
		case H1USB_COM_HWRESET:		iReturn = pH1USBCom->TransHardWare();								break;
		default: break;

	}

	//エラーセット
	pH1USBCom->m_iLastError = iReturn;
	//終了イベントをセットする
	SetEvent(pH1USBCom->m_hTEnd);

A2GDBG_OUTSTRING(_T("OnComMessage() Exit\n"));

	return THREAD_CBCODE_NORMAL;

}

/*====================================================================*/
/* 処理概要  ：暗号化状態にする
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransScranmble()
{

	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[1024] ;	// 送信用
	BYTE recv[1024] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;
	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数
	BYTE *bp = NULL ;


	send[0] = 0x1F ;	// 暗号キー送信コマンド //加密密钥发送命令
	send[1] = 0x02 ;	// 後続データのバイト数(512バイト)//随后的数据字节 （512 字节）
	send[2] = 0x00 ;	// 〃
	

	// 暗号キーの元となる512バイトの乱数を生成し、send[3]〜send[514]にセット
	// 从 512 字节，生成加密密钥的随机数和发送 [3]-设置为发送 [514]
	int i = 0 ;
	for ( bp = send + 3, i = 0; i < 512; bp++, i++ ) {

		*bp = rand() ;
	}

	dwSendSize = 3 + 512 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// フラグの更新
	m_bCamelliaEnabled = TRUE ;	// TRUE: 暗号通信状態

	// 512バイトの乱数から暗号キー(m_byCurrentWorkKey)を生成する
	// 从 512 字节的随机数生成加密密钥 (m_byCurrentWorkKey)
	decodeMasterKey512( send + 3, m_byCurrentWorkKey ) ;

	// 暗号キーを元に拡大鍵(m_uKttWork)を生成(暗号化・復号を行う前に必ず拡大鍵を生成すること)
	// 一度拡大鍵を生成すれば、同じ鍵で暗号化・復号を行う限り、再度拡大鍵の生成を行う必要はない
	// 不需要扩展关键代再次因为如果加密密钥扩展的关键 (m_uKttWork) 代 （加密和解密的事先必须扩展的密钥以产生） 
	// 一旦扩大关键生产，用相同的密钥来加密和解密
	Camellia_Ekeygen( 128, m_byCurrentWorkKey, m_uKttWork ) ;

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
		//MessageBox( "受信データが存在しないため、処理を終了します", TITLE, MB_ICONEXCLAMATION ) ;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_READFILE; 
	}
	else {	// ReadFileには成功したが、コマンドが異常終了

		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;
}

/*====================================================================*/
/* 処理概要  ：緑LEDを点灯させる
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransLedGreen( void )
{	
	int iReturn;
	iReturn = H1USB_OK;

	// 赤LED, 緑LEDを消灯
	iReturn = ledAllOff();
	if ( iReturn != H1USB_OK ) {
		return iReturn ;
	}

	// 緑LED点灯
	iReturn = ledOnOff( 0x02, 0x01 ) ;

	return iReturn ;
}

/*====================================================================*/
/* 処理概要  ：赤LEDを点灯させる
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransLedRed( void )
{
	int iReturn;
	iReturn = H1USB_OK;

	// 赤LED, 緑LEDを消灯
	iReturn = ledAllOff();
	if ( iReturn != H1USB_OK ) {
		return iReturn ;
	}

	// 赤LED点灯
	iReturn = ledOnOff( 0x01, 0x01 ) ;

	return iReturn;
}
/*====================================================================*/
/* 処理概要  ：赤LEDを点灯させる
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransLedGR( void )
{
	int iReturn;
	iReturn = H1USB_OK;

	// 赤LED, 緑LEDを消灯
	iReturn = ledAllOff();
	if ( iReturn != H1USB_OK ) {
		return iReturn ;
	}

	// 赤LED点灯
	iReturn = ledOnOff( 0x03, 0x01 ) ;

	return iReturn ;
}


/*====================================================================*/
/* 処理概要  ：LEDを全て消灯させる
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransLedOff( void )
{

	// 赤LED, 緑LEDを消灯
	return ledOnOff( 0x03, 0x00 ) ;

}




/*====================================================================*/
/* 処理概要  ：タッチセンサーの通知を受け取るように設定する。
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransTuchNotify(BYTE byParam)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x28 ;	// タッチセンサー操作コマンド
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x01 ;	// 〃
	send[3] = byParam ;	// タッチセンサーの状態が変化したちときレスポンスありを設定

	dwSendSize = 4 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_WRITEFILE;
	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;
}

/*====================================================================*/
/* 処理概要  ：ファームウェアのバージョン情報を返却する
/* 引数      ：pFwVersion	ファームウェアのバージョン情報
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransGetFwVersion( char *pFwVersion )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x14 ;		// 情報取得コマンド
	send[1] = 0x00 ;		// 後続データのバイト数
	send[2] = 0x01 ;		// 〃
	send[3] = 0x00 ;		// ファームウェアのバージョン情報を取得

	dwSendSize = 4 ;		// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	//受信データ確認処理(V1.10---2011/5/27)
	//16バイトの復号化した受信データから全体の受信データを計算してから、
	//全体の受信データから16バイトを引いた長さ分のデータを受信して復号化する。
	//begin
	DWORD dwRecvSizeRest = 0;//後続の残りバイト数
	DWORD dwRecvSizeAll = 0;//レスポンスデータ全体の長さ

	BYTE *pRecv = recv ;	// recvへのポインタ
	DWORD dwTmplTotalSize = 0 ;	// 受信テンプレートデータの合計サイズ

	// 全データを受信するか、エラーコードをリターンするまで待機する
	for ( ; ; ) {

		if(dwTmplTotalSize == 0){

			while (dwRecvSize < 16) {
				// 受信バッファのバイト数を取得
				dwRecvSize = getBuffSize() ;
			}
			// 受信処理
			if ( recvCommEnc( pRecv, 16 ) == FALSE ) {	// ReadFile失敗
				return H1USB_ERR_READFILE;
			}
			else {	// ReadFile成功
				pRecv += 16 ;	// 受信した分だけポインタを進める
				dwTmplTotalSize += 16 ;	// 受信データ合計サイズ(dwTmplTotalSize)に加算
				dwRecvSizeAll = ((recv[1]*256+recv[2]+18) & 0xFFFFFFF0);// 受信データ全体サイズ
				dwRecvSizeRest = dwRecvSizeAll-16;
				if ( recv[0] != 0x00 ) { // 異常終了
					return H1USB_ERR_RESPONS;
				}
			}
		}
		

		// 受信バッファのバイト数を取得
		dwRecvSize = getBuffSize() ;

		// 受信データがある場合
		if ( dwRecvSize > 0 ) {
			// 受信処理
			if ( recvCommEnc( pRecv, 16 ) == FALSE ) {	// ReadFile失敗
				return H1USB_ERR_READFILE;
			}

			// 受信データ合計サイズ(dwTmplTotalSize)に加算
			dwTmplTotalSize += 16 ;
			pRecv += 16 ;	// 受信した分だけポインタを進める

			// 全データを受信したらループを抜ける
			if ( dwTmplTotalSize == dwRecvSizeAll ) {

				break ;
			}
			// まだ全テンプレートデータを受信しきれていない
			else {

				// 処理続行
			}
		}
	}
	//受信データ確認処理(V1.10---2011/5/27)
	//end

	// ファームウェアのバージョン情報をコピー
	memcpy( pFwVersion, recv + 3, 14 ) ;
	pFwVersion[14] = '\0' ;
	return iReturn;

}

/*====================================================================*/
/* 処理概要  ：シリアル番号を返却する
/* 引数      ：pSerialNum	シリアル番号
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransGetSerialNum( char *pSerialNum )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x14 ;		// 情報取得コマンド
	send[1] = 0x00 ;		// 後続データのバイト数
	send[2] = 0x01 ;		// 〃
	send[3] = 0x01 ;		// シリアル番号を取得

	dwSendSize = 4 ;		// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		//MessageBox( "受信データが存在しないため、処理を終了します", TITLE, MB_ICONEXCLAMATION ) ;
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFile成功

		if ( recv[0] != 0x00 ) { // 異常終了
			return H1USB_ERR_RESPONS;
		}
		else { // 正常終了
			// シリアル番号情報をコピー
			memcpy( pSerialNum, recv + 3, 12 ) ;
			pSerialNum[12] = '\0' ;
		}
	}
	return iReturn;

}

/*====================================================================*/
/* 処理概要  ：緑LEDを点灯させる
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransBUZStart( void )
{	
	int iReturn;
	iReturn = H1USB_OK;

	iReturn = beepSound(0XFF, 0);

	return iReturn;
}

/*====================================================================*/
/* 処理概要  ：緑LEDを点灯させる
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransBUZStop( void )
{
	int iReturn;
	iReturn = H1USB_OK;

	iReturn = beepSound(0, 0);

	return iReturn;
}

/*====================================================================*/
/* 処理概要  ：タッチセンサーの通知を受け取るように設定する。
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransSetSecurity(BYTE byMode)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x20 ;	// タッチセンサー操作コマンド
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x01 ;	// 〃
	send[3] = byMode ;	// タッチセンサーの状態が変化したちときレスポンスありを設定

	dwSendSize = 4 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;
}

/*====================================================================*/
/* 処理概要  ：1:1認証または1:N認証を行う
/* 引数      ：byNinshoMode	0x00=>1:N認証, 0x01=>1:1認証
/*             nMemNo		照合するテンプレートメモリ番号(1:N認証の場合は任意)
/*             pMatchMemNo	一致したテンプレートメモリ番号
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransNinsho( const BYTE byNinshoMode, const int nMemNo, BYTE *pMatchMemNo )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[2048] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x13 ;		// 認証指示コマンド
	send[1] = 0x00 ;		// 後続データのバイト数
	send[2] = 0x02 ;		// 〃
	send[3] = byNinshoMode ;// 認証モードおよびオプション
	send[4] = nMemNo ;		// テンプレート番号(1:N認証の場合は任意)

	dwSendSize = 5 ;		// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		iReturn = H1USB_ERR_WRITEFILE;
		goto ERR_TREAT ;

	}

	// データを受信するまで待機する
	for ( ; ; ) {
		dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
		// 16バイトの受信データがあればループを抜ける
		if ( 16 == dwRecvSize ) {
			break ;
		}
		else{
			if(!m_bTransEnable){
				iReturn = H1USB_ERR_TRANSTOUT;
				goto ERR_TREAT ;
			}
		}
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		iReturn = H1USB_ERR_READFILE;
		goto ERR_TREAT ;

	}
	else {	// ReadFile成功
		if ( recv[0] != 0x00 ) { // 認証失敗
			// 撮影時ダイアログのキャンセルボタンが押下された場合
			if ( recv[3] == 0x10 ) {
				// 緑LED点灯
				TransLedGreen() ;				
				// Beep音(キャンセル)を鳴らす
				beepCancel() ;
				// メッセージ表示
				return H1USB_CANCEL;
			}
			// 上記以外はエラー
			else {
				iReturn = H1USB_ERR_RESPONS;
				goto ERR_TREAT ;
			}
		}
		else { // 認証成功
			// 一致したテンプレートメモリ番号をコピー
			*pMatchMemNo = recv[3] ;
		}
	}
	// 緑LED点灯
	TransLedGreen() ;	
	// Beep音(正常)を鳴らす
	beepOK() ;
	return iReturn;

ERR_TREAT:
	// 赤LED点灯
	TransLedRed() ;
	// Beep音(異常)を鳴らす
	beepNG() ;
	return iReturn;

}

/*====================================================================*/
/* 処理概要  ：バンク間認証を行う
/* 引数      ：wEnableBank		認証の対象とするバンク番号の配列
/*             wEnableBankCnt	認証の対象とするバンクの数
/*             pMatchMemNo		一致したテンプレートメモリ番号
/*             pMatchBank		一致したテンプレートが存在するバンク番号
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransNinshoBank( const WORD wEnableBank[], const WORD wEnableBankCnt, BYTE *pMatchMemNo, WORD *pMatchBank )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[8192] ;	// 送信用
	BYTE recv[2048] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x18 ;	// 認証指示コマンド(バンク間認証)
	send[1] = (BYTE)( ((1 + 2 * wEnableBankCnt) & 0xFF00) >> 8 ) ;			// 後続データのバイト数(認証で用いるバンクの数によって変わる)
	send[2] = (BYTE)( (1 + 2 * wEnableBankCnt) & 0x00FF ) ;	// 〃
	send[3] = 0x00 ;	// オプションビット
	
	// 認証で用いるバンク番号を、send[4]以降に格納する(2バイトで1バンク番号)
	//for ( WORD i = 0; i < wEnableBankCnt; i++ ) {
	//	send[2*(i+2)] = (BYTE)( (wEnableBank[i] & 0xFF00) >> 8 ) ;	
	//	send[2*(i+2)+1] = (BYTE)(wEnableBank[i] & 0x00FF ) ;
	//}
	WORD w;
	BYTEWORD bw;
	for(w=0; w<wEnableBankCnt; w++)
	{
		bw.word = wEnableBank[w];
		send[4+w*2]		=  	bw.byte[1];
		send[4+w*2+1]	=	bw.byte[0];
	}



	dwSendSize = 4 + 2 * wEnableBankCnt ;	// 送信データバッファのバイト数(認証で用いるバンクの数によって変わる)
	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		iReturn = iReturn;
		goto ERR_TREAT ;

	}
	// データを受信するまで待機する
	for ( ; ; ) {
		dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
		// 16バイトの受信データがあればループを抜ける
		if ( 16 <= dwRecvSize ) {
			break ;
		}
		else{
			if(!m_bTransEnable){
				iReturn = H1USB_ERR_TRANSTOUT;
				goto ERR_TREAT ;
			}
		}

	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		iReturn = H1USB_ERR_READFILE;
		goto ERR_TREAT ;
	}

	else {	// ReadFile成功
		if ( recv[0] != 0x00 ) { // 認証失敗
			// 撮影時ダイアログのキャンセルボタンが押下された場合
			if ( recv[3] == 0x10 ) {
				// 緑LED点灯
				TransLedGreen() ;				
				// Beep音(キャンセル)を鳴らす
				beepCancel() ;
				// メッセージ表示
				return H1USB_CANCEL;

			}
			// 上記以外はエラー
			else {
				iReturn = H1USB_ERR_RESPONS;
				goto ERR_TREAT ;
			}
		}
		else { // 認証成功
			// 一致したテンプレートメモリ番号をコピー
			*pMatchMemNo = recv[3] ;
			// バンク番号をコピー
			*pMatchBank = ( recv[4] << 8 ) | recv[5] ;
		}
	}

	// 緑LED点灯
	TransLedGreen() ;
	
	// Beep音(正常)を鳴らす
	beepOK() ;

	return iReturn ;

ERR_TREAT:

	// 赤LED点灯
	TransLedRed() ;
	// Beep音(異常)を鳴らす
	beepNG() ;
	// エラーメッセージ表示
	//	MessageBox( cMsg, TITLE, MB_ICONSTOP ) ;
	return iReturn ;

}

/*====================================================================*/
/* 処理概要  ：テンプレート付き認証を行う
/* 引数      ：pTempU			テンプレートユニット構造体の配列
/*             byCount			テンプレートユニット構造体の配列の数
/*             pMatchNum		一致したテンプレートメモリ番号
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransNinshoWithTemplate(const PTEMPU pTempU, const BYTE byCount, BYTE *pMatchNum)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE* pSend;
	BYTE recv[2048] ;	// 受信用
	UINT uiSendBuffSize;
	UINT uiSendLength;
	UINT uiTemplateSize;
	BYTEWORD bw;
	int i;
	uiTemplateSize = 536*byCount;
	uiSendLength = uiTemplateSize + 8;
	uiSendBuffSize = uiSendLength + 8;
	bw.word = uiTemplateSize + 5;
	pSend = new BYTE[uiSendBuffSize];
	ZeroMemory(pSend, uiSendBuffSize);
	memset( recv, 0, sizeof(recv) ) ;
	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数
	// 送信データをセット
	pSend[0] = 0x19 ;		// 認証指示コマンド(テンプレート付き)
	pSend[1] = bw.byte[1] ;	// 後続データのバイト数
	pSend[2] = bw.byte[0] ;	// 〃
	pSend[3] = 0x08 ;			// 送信先のテンプレートメモリ番号(0x00〜0x63)
	// テンプレートデータをsend[4]〜send[539]にコピー
	for(i=0; i<byCount; i++){
		memcpy(&pSend[4+i*536], pTempU[i].byTemplate, 536);
	}

	// チェックサムの計算
	DWORD dwCheckSum = 0 ;	// チェックサム
	DWORD *pd = ( DWORD * )pSend ;

	for ( UINT ui = 0; ui < (uiTemplateSize+4)/4; ui++, pd++ ) {
		dwCheckSum ^= *pd ;
	}

	// チェックサムをsend[540]〜send[543]にコピー
	*pd = dwCheckSum ;

	dwSendSize = uiSendLength ;		// 送信データサイズのバイト数

	// 送信処理
	if ( sendCommEnc( pSend, dwSendSize ) == FALSE ) {
		iReturn = H1USB_ERR_WRITEFILE;
		goto ERR_TREAT ;
	}

	for ( ; ; ) {
		dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
		// 16バイトの受信データがあればループを抜ける
		if ( 16 == dwRecvSize ) {
			break ;
		}
		else{
			if(!m_bTransEnable){
				iReturn = H1USB_ERR_TRANSTOUT;
				goto ERR_TREAT ;
			}
		}
	}


	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		iReturn = H1USB_ERR_READFILE;
		goto ERR_TREAT ;

	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) {
			iReturn = H1USB_ERR_RESPONS;
			goto ERR_TREAT ;
		}
	}
	// 緑LED点灯
	TransLedGreen() ;
	
	// Beep音(正常)を鳴らす
	beepOK() ;

	if(pSend){
		delete[] pSend;
	}
	return iReturn;

ERR_TREAT:

	// 赤LED点灯
	TransLedRed() ;
	// Beep音(異常)を鳴らす
	beepNG() ;

	if(pSend){
		delete[] pSend;
	}

	return iReturn;
}


/*====================================================================*/
/* 処理概要  ：テンプレートを撮影し、H1E-USBから受信する
/* 引数      ：pTemplate	テンプレート
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransGetTemplate(PTEMPU pTempU)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[640] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;
	BYTE *pRecv = recv ;	// recvへのポインタ
	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数
	DWORD dwTmplTotalSize = 0 ;	// 受信テンプレートデータの合計サイズ

	// 送信データをセット
	send[0] = 0x15 ;	// テンプレート受信コマンド(H1E-USB => ホスト機)
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x00 ;	// 〃

	dwSendSize = 3 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		iReturn = H1USB_ERR_WRITEFILE;
		goto ERR_TREAT ;
	}
	//受信データ確認処理(V1.10---2011/5/27)
	//16バイトの復号化した受信データから全体の受信データを計算してから、
	//全体の受信データから16バイトを引いた長さ分のデータを受信して復号化する。
	//begin
	DWORD dwRecvSizeRest = 0;//後続の残りバイト数
	DWORD dwRecvSizeAll = 0;//レスポンスデータ全体の長さ
	// 全テンプレートデータ(544バイト)を受信するか、エラーコードをリターンするまで待機する
	for ( ; ; ) {
		if(dwTmplTotalSize == 0){
			while (dwRecvSize < 16) {
				// 撮影時ダイアログのキャンセルボタンが押された場合
				//if ( m_pDlg->IsCancel() ) {
				//	cancel() ;
				//}
				// 受信バッファのバイト数を取得
				dwRecvSize = getBuffSize() ;
				// メッセージポンプ
				//messagePump() ;
			}
			// 受信処理
			if ( recvCommEnc( pRecv, 16 ) == FALSE ) {	// ReadFile失敗
				iReturn = H1USB_ERR_READFILE;
				goto ERR_TREAT ;
			}
			else {	// ReadFile成功				
				pRecv += 16 ;	// 受信した分だけポインタを進める
				dwTmplTotalSize += 16 ;	// 受信データ合計サイズ(dwTmplTotalSize)に加算
				dwRecvSizeAll = ((recv[1]*256+recv[2]+18) & 0xFFFFFFF0);// 受信データ全体サイズ
				dwRecvSizeRest = dwRecvSizeAll-16;
				if ( recv[0] != 0 ) {	// 異常終了
					// 撮影時ダイアログのキャンセルボタンが押下された場合
					if ( recv[3] == 0x10 ) {
						// 緑LED点灯
						TransLedGreen() ;						
						// Beep音(キャンセル)を鳴らす
						beepCancel() ;
						// メッセージ表示
						return H1USB_CANCEL;

					}
					// 上記以外はエラー
					else {
						// ダイアログを閉じる
					iReturn = H1USB_ERR_RESPONS;
						goto ERR_TREAT ;
					}
				}
			}
		}
		

		// 受信バッファのバイト数を取得
		dwRecvSize = getBuffSize() ;

		// 受信データがある場合
		if ( dwRecvSize > 0 ) {
			// 受信処理
			if ( recvCommEnc( pRecv, 16 ) == FALSE ) {	// ReadFile失敗
				iReturn = H1USB_ERR_READFILE;
				goto ERR_TREAT ;
			}
			// 受信データ合計サイズ(dwTmplTotalSize)に加算
			dwTmplTotalSize += 16 ;
			pRecv += 16 ;	// 受信した分だけポインタを進める
			// 全テンプレートデータ(544バイト)を受信したらループを抜ける
			if ( dwTmplTotalSize == dwRecvSizeAll ) {
				break ;
			}
			// まだ全テンプレートデータを受信しきれていない
			else {
				// 処理続行
			}
		}
	}
	//受信データ確認処理(V1.10---2011/5/27)
	//end

	// H1E-USBから受信したデータ列からチェックサムを計算する
	DWORD dwCheckSum = 0 ;
	DWORD *pd = ( DWORD * )recv ;
	for ( int i = 0; i < 540/4; i++, pd++ ) {
		dwCheckSum ^= *pd ;
	}
	// H1E-USBから受信したチェックサムと同じであるか確認
	if ( *pd != dwCheckSum ) {
		goto ERR_TREAT ;
	}
	// 受信したテンプレートを pTemplate にコピー
	SYSTEMTIME st;
	GetLocalTime(&st);
	pTempU->uiTmpNum = 0xFF;
	pTempU->time = st;
	memcpy( pTempU->byTemplate, recv+4, 536 ) ;

	// 緑LED点灯
	TransLedGreen() ;	
	// Beep音(正常)を鳴らす
	beepOK() ;
	return iReturn;

ERR_TREAT:

	// チェックサムエラーが起きた場合、データ全体を受信し切れていない可能性がある
	// 送受信バッファにデータが残っていると後続処理に支障をきたすため、送受信バッファを初期化しておく
	dwRecvSize = getBuffSize() ;	// 受信バッファのバイト数を取得
	if ( dwRecvSize != 0 ) {
		// 送受信バッファの初期化
		PurgeComm( m_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR ) ;
	}
	// 赤LED点灯
	TransLedRed() ;
	// Beep音(異常)を鳴らす
	beepNG() ;
	// エラーメッセージ表示
	//MessageBox( cMsg, TITLE, MB_ICONSTOP ) ;	
	return iReturn ;

}


/*====================================================================*/
/* 処理概要  ：テンプレート格納バンクを切り替える
/* 引数      ：wBankNo	切替先のバンク番号
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransChangeBank( const WORD wBankNo )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;
	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数
	// 送信データをセット
	send[0] = 0x1C ;	// テンプレート格納バンク指定コマンド
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x02 ;	// 〃
	send[3] = (BYTE)(( wBankNo & 0xFF00 ) >> 8) ;	// バンク番号(上位バイト)
	send[4] = (BYTE)( wBankNo & 0x00FF ) ;		// バンク番号(下位バイト)
	dwSendSize = 5 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}
	
	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;

}


/*====================================================================*/
/* 処理概要  ：テンプレートを指定したメモリに送信する
/* 引数      ：pTemplate	テンプレート
/*             nMemNo		送信先のテンプレートメモリ番号
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransSetTemplate(PTEMPU pTempU)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[640] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;
	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数
	// 送信データをセット
	send[0] = 0x12 ;			// テンプレート送信コマンド(ホスト機 => H1E-USB)
	send[1] = 0x02 ;			// 後続データのバイト数
	send[2] = 0x1D ;			// 〃
	send[3] = pTempU->uiTmpNum;	// 送信先のテンプレートメモリ番号(0x00〜0x63)
	// テンプレートデータをsend[4]〜send[539]にコピー
	memcpy( send + 4, pTempU->byTemplate, 536 ) ;	
	// チェックサムの計算
	DWORD dwCheckSum = 0 ;	// チェックサム
	DWORD *pd = ( DWORD * )send ;
	for ( int i = 0; i < 540/4; i++, pd++ ) {
		dwCheckSum ^= *pd ;
	}
	// チェックサムをsend[540]〜send[543]にコピー
	*pd = dwCheckSum ;
	dwSendSize = 544 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;

}


/*====================================================================*/
/* 処理概要  ：H1E-USBからテンプレートを削除する
/* 引数      ：nMemNo	テンプレート番号
                          0〜99 : 指定したテンプレート番号から削除
						  -1    : 現在のバンクの全テンプレートを削除
                          -2    : 全バンクの全テンプレートを削除
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransDelTemplate(const int nMemNo)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x16 ;	// テンプレート削除コマンド
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x01 ;	// 〃

	// 現在のバンクの指定したテンプレート番号から削除
	if ( 0x00 <= nMemNo && nMemNo <= 0x63 ) {
		send[3] = nMemNo ;	// テンプレート番号
	}
	else {

		// 現在のバンクの全テンプレートを削除
		if ( nMemNo == -1 ) {
			send[3] = 0xFF ;	// テンプレート番号
		}
		// 全バンクの全テンプレートを削除
		else if ( nMemNo == -2 ) {
			send[3] = 0xFE ;	// テンプレート番号
		}
		// 上記以外
		else {

		}
	}

	dwSendSize = 4 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		iReturn = H1USB_ERR_READFILE;
	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) { 
			return H1USB_ERR_RESPONS;
		}
	}

	return iReturn;

}

/*====================================================================*/
/* 処理概要  ：ハードウェアをリセットする
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::TransHardWare( void )
{
	int iReturn;
	iReturn = H1USB_OK;



	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;


	DWORD dwRecvSize;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	// 送信データをセット
	send[0] = 0x17 ;	// ハードウェアリセットコマンド
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x00 ;	// 〃
	dwSendSize = 3 ;	// 送信データバッファのバイト数
	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	Sleep(200);
	m_bCamelliaEnabled = FALSE;
	do{
		if(!m_bTransEnable){
			iReturn = H1USB_ERR_TRANSTOUT;
			break;
		}

	// 受信バッファのバイト数を取得
		dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
		recvCommEnc( recv, dwRecvSize );

	}while(dwRecvSize > 0);



	return iReturn;

}


/*====================================================================*/
/* 処理概要  ：LEDを全て消灯させる
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::ledAllOff( void )
{

	// 赤LED, 緑LEDを消灯
	return ledOnOff( 0x03, 0x00 ) ;

}

/*====================================================================*/
/* 処理概要  ：LED(緑)の点滅を開始する
/* 引数      ：byOffTime	点滅時の消灯時間(10ms単位で指定)
/*             byOnTime		点滅時の点灯時間(10ms単位で指定)
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::ledStartBlink( const BYTE byOffTime, const BYTE byOnTime )
{
	int iReturn;

	iReturn = H1USB_OK;

	// 赤LED, 緑LEDを消灯
	iReturn = ledAllOff();
	if(iReturn != H1USB_OK){
		return iReturn;
	}
	// 緑LED点滅
	iReturn = ledBlink( byOffTime, byOnTime ) ;

	return iReturn;

}

/*====================================================================*/
/* 処理概要  ：LEDを点灯、または消灯させる
/* 引数      ：byColor	0x01:赤, 0x02:緑, 0x01:赤と緑
/*             byOnOff	0x00:消灯, 0x01:点灯
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::ledOnOff( const BYTE byColor, const BYTE byOnOff )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x11 ;	// LED制御コマンド
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x02 ;	// 〃
	send[3] = byColor ; // 0x01:赤, 0x02:緑, 0x01:赤と緑
	send[4] = byOnOff ;	// 0x00:消灯, 0x01:点灯

	dwSendSize = 5 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}

	return iReturn;
}

/*====================================================================*/
/* 処理概要  ：LED(緑)の点滅させる
/* 引数      ：byOffTime	点滅時の消灯時間(10ms単位で指定)
/*             byOnTime		点滅時の点灯時間(10ms単位で指定)
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::ledBlink( const BYTE byOffTime, const BYTE byOnTime )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x11 ;	// LED制御コマンド
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x04 ;	// 〃
	send[3] = 0x02 ;	// 0x02:緑
	send[4] = 0x02 ;	// 0x02:点滅
	send[5] = byOffTime ;	// 点滅時の消灯時間(10ms単位で指定)
	send[6] = byOnTime ;	// 点滅時の点灯時間(10ms単位で指定)

	dwSendSize = 7 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}

	return iReturn;
}



/*====================================================================*/
/* 処理概要  ：正常終了時のBeep音を鳴らす
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::beepOK( void )
{

	// ピ・ピ と鳴らす
	return beepSound( 0x00, 0x02 ) ;

}

/*====================================================================*/
/* 処理概要  ：異常終了時のBeep音を鳴らす
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::beepNG( void )
{

	// ピ・ピー と鳴らす
	return beepSound( 0x40, 0x02 ) ;

}

/*====================================================================*/
/* 処理概要  ：キャンセル時のBeep音を鳴らす
/* 引数      ：なし
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::beepCancel( void )
{

	// ピー と鳴らす
	return beepSound( 0x80, 0x01 ) ;

}

/*====================================================================*/
/* 処理概要  ：指定されたパターンでBeep音を鳴らす
/* 引数      ：byPattern	鳴動パターン(ビットの値 => 0:短い音, 1:長い音)
/*             byCount		鳴動パターン数(先頭から何ビット分鳴らすかを1〜8で指定)
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
int CH1USBCom::beepSound( const BYTE byPattern, const BYTE byCount )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// 送信用
	BYTE recv[128] ;	// 受信用
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// 送信データバッファのバイト数
	DWORD dwRecvSize = 0 ;	// 受信データバッファのバイト数

	// 送信データをセット
	send[0] = 0x10 ;	// ブザー制御コマンド
	send[1] = 0x00 ;	// 後続データのバイト数
	send[2] = 0x03 ;	// 〃
	if((byPattern == 0) && (byCount == 0)){
		send[3] = 0x00 ;	// 0x00:鳴動停止
	}
	else{
		if(byCount == 0){
			send[3] = 0x01 ;	// 0x01:鳴動開始
		}
		else{
			send[3] = 0x02 ;	// 0x02:鳴動パターンに従って鳴動開始
		}
	}
	send[4] = byPattern ;	// 鳴動パターン(ビットの値 => 0:短い音, 1:長い音)
	send[5] = byCount ;		// 鳴動パターン数(先頭から何ビット分鳴らすかを1〜8で指定)

	dwSendSize = 6 ;	// 送信データバッファのバイト数

	// 送信処理
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// 受信バッファのバイト数を取得
	dwRecvSize = getBuffSize() ;	// 受信データバッファのバイト数
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// 受信処理
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile失敗
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFileには成功したが、コマンドが異常終了
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}

	return iReturn;
}








/*====================================================================*/
/* 処理概要  ：コマンドをH1E-USBに送信する(ノーマル状態)
/* 引数      ：pComm	データバッファ
/*             dwSize	送信するデータバッファのバイト数
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
BOOL CH1USBCom::sendComm( const BYTE *pComm, const DWORD dwSize )
{

	//----- 送信処理 -----//
	DWORD dwLen = 0 ;

	BOOL bRet = WriteFile(
		m_hCom,	// デバイスのハンドル
		pComm,	// データバッファ
		dwSize,	// 書き込み対象のバイト数
		&dwLen,	// 書き込んだバイト数
		NULL	// オーバーラップ構造体のバッファ
		) ;

	// WriteFile()直後にClearCommError()を呼び出すと、ComStat.cbInQueが
	// しばしば0になる(受信バッファのバイト数を取得出来ない)ため、Sleepを入れる
	Sleep(100) ;

	return bRet ;
}

/*====================================================================*/
/* 処理概要  ：コマンドをH1E-USBに送信する(暗号通信状態)
/* 引数      ：pComm	データバッファ
/*             dwSize	送信するデータバッファのバイト数
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
BOOL CH1USBCom::sendCommEnc( const BYTE *pComm, DWORD dwSize )
{

	// 暗号通信状態ではない場合
	if ( m_bCamelliaEnabled == FALSE ) {

		return sendComm( pComm, dwSize) ;
	}

	//----- 暗号化処理 -----//
	// dwSizeに応じて16バイトの倍数の領域を確保する
	// (dwSizeが1〜16の場合:16バイト, 17〜32の場合:32バイト …)
	BYTE *pBuff = NULL ;
	pBuff = (BYTE *)malloc( dwSize = (dwSize + 15) & ~15 ) ;
	memset( pBuff, 0, dwSize ) ;

	// 16バイト毎にpCommの内容を暗号化してpBuffに格納する
	for ( DWORD i = 0; i < dwSize; i+=16 ) {
		Camellia_EncryptBlock(
			128,		// 鍵長
			pComm + i,	// 平文のポインタ
			m_uKttWork,	// 拡大鍵
			pBuff + i	// 暗号化された文のポインタ
			) ;
	}

	//----- 送信処理 -----//
	DWORD dwLen = 0 ;
	
	BOOL bRet = WriteFile(
			m_hCom,	// デバイスのハンドル
			pBuff,	// 暗号化されたデータバッファ
			dwSize,	// 暗号化された書き込み対象のバイト数
			&dwLen,	// 書き込んだバイト数
			NULL	// オーバーラップ構造体のバッファ
			) ;

	// 確保したメモリを解放する
	free( pBuff ) ;

	// WriteFile()直後にClearCommError()を呼び出すと、ComStat.cbInQueが
	// しばしば0になる(受信バッファのバイト数を取得出来ない)ため、Sleepを入れる
	Sleep(100) ;

	return bRet ;

}

/*====================================================================*/
/* 処理概要  ：H1E-USBからデータを受信する(ノーマル状態)
/* 引数      ：pComm	データバッファ
/*             dwSize	受信するデータバッファのバイト数
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
BOOL CH1USBCom::recvComm( BYTE *pComm, const DWORD dwSize )
{

	//----- 受信処理 -----//
	DWORD dwLen = 0 ;

	return ReadFile(
		m_hCom,	// デバイスのハンドル
		pComm,	// データバッファ
		dwSize,	// 読み取り対象のバイト数
		&dwLen,	// 読み取ったバイト数
		NULL	// オーバーラップ構造体のバッファ
		) ;

}

/*====================================================================*/
/* 処理概要  ：H1E-USBからデータを受信する(暗号通信状態)
/* 引数      ：pComm	データバッファ
/*             dwSize	受信するデータバッファのバイト数
/* 戻り値    ：成功:0以外、失敗:0
/*====================================================================*/
BOOL CH1USBCom::recvCommEnc( BYTE *pComm, DWORD dwSize )
{

	// 暗号通信状態ではない場合
	if ( m_bCamelliaEnabled == FALSE ) {

		return recvComm( pComm, dwSize) ;
	}

	//----- 受信処理 -----//
	// dwSizeに応じて16バイトの倍数の領域を確保する
	// (dwSizeが1〜16の場合:16バイト, 17〜32の場合:32バイト …)
	BYTE *pBuff = NULL ;
	pBuff = (BYTE *)malloc( dwSize = (dwSize + 15) & ~15 ) ;
	memset( pBuff, 0, dwSize ) ;

	DWORD dwLen = 0 ;

	BOOL bRet = ReadFile(
			m_hCom,	// デバイスのハンドル
			pBuff,	// 暗号化されたデータバッファ
			dwSize,	// 暗号化された読み取り対象のバイト数
			&dwLen,	// 読み取ったバイト数
			NULL	// オーバーラップ構造体のバッファ
 			) ;

	if ( bRet == FALSE ) {
		goto EXIT_TREAT ;
	}

	//読み取り対象のバイト数と読み取ったバイト数が異なる場合は受信エラーとする。(V1.10---2011/5/25)
	//begin
	if ( dwSize != dwLen ) {

		return FALSE;
	}
	//読み取り対象のバイト数と読み取ったバイト数が異なる場合は受信エラーとする。(V1.10---2011/5/25)
	//end

	//----- 復号処理 -----//
	// 16バイト毎にpBuffの内容を復号してpCommに格納する
	//	解码 pBuff 内容，在 pComm 中存储的每个 16 字节
	for ( DWORD i = 0; i < dwSize; i+=16 ) {
		Camellia_DecryptBlock(
			128,		// 鍵長
			pBuff + i,	// 暗号文のポインタ//密码文本指针
			m_uKttWork, // 拡大鍵//扩大的关键
			pComm + i	// 復号された文のポインタ//解码的文本指针
			) ;
	}

EXIT_TREAT:

	// 確保したメモリを解放する
	free( pBuff ) ;

	return bRet ;

}

/*====================================================================*/
/* 処理概要  ：受信バッファのバイト数を返却する
/* 引数      ：なし
/* 戻り値    ：受信バッファのバイト数
/*====================================================================*/
/* ReadFile()で指定するバイト数が、実際に読込むバイト数と一致していないと
   受信待ちで止まってしまうため、本関数で受信バッファのサイズを取得しておく */
DWORD CH1USBCom::getBuffSize( void )
{
	DWORD dwErrors = 0 ;
	DWORD dwCount = 0 ;
	COMSTAT ComStat ;
	memset( &ComStat, 0, sizeof(COMSTAT) ) ;
	
	ClearCommError(
		m_hCom,		// 通信デバイスのハンドル
		&dwErrors,	// エラーコードを受け取る変数へのポインタ
		&ComStat	// 通信状態バッファへのポインタ
		) ;
	
	return ComStat.cbInQue ;

}

/*====================================================================*/
/* 処理概要  ：512バイトの乱数から暗号キーを生成する
				从 512 字节的随机数生成一个加密密钥
/*  step1 4096ビット(512バイト)のビットアドレス変換により、ビットを交換
			更换钻头 4096 位 （512 字节） 的地址位转换
/*  step2 32個の16バイトブロックで論理演算  32 16 字节逻辑块
/* 引数      ：src	512バイトの乱数 512 字节的随机数
/*             dst	スクランブルを解除した16バイトのデータ 你解锁炒 16 个字节的数据
/* 戻り値    ：成功:0
/*====================================================================*/
// H1E-USBコマンドインターフェース仕様書(スクランブル暗号キー編)より引用
//（置乱加密密钥司） 的报价 H1E USB 命令接口规范
int CH1USBCom::decodeMasterKey512( const BYTE *src, BYTE *dst )
{

	DWORD _work[512/4], srcAddr, dstAddr ;
	BYTE *work, *sp, *bp, *cp, *dp, cmd ;
	int i, j, k ;
	
	work = (BYTE*)_work ;
	for ( i = 0; i < sizeof(_work) / 4; i++ ) {  // 作業領域の初期化
		_work[i] = 0 ;
	}
	
	// ビットアドレス変換
	for ( srcAddr = 0; srcAddr < 4096; srcAddr++ ) {

		dstAddr = 0 ;

		ADDRCONV( srcAddr, dstAddr, 0,  5 ) ; 
		ADDRCONV( srcAddr, dstAddr, 1, 10 ) ; 
		ADDRCONV( srcAddr, dstAddr, 2,  8 ) ; 
		ADDRCONV( srcAddr, dstAddr, 3,  6 ) ; 
		ADDRCONV( srcAddr, dstAddr, 4,  7 ) ; 
		ADDRCONV( srcAddr, dstAddr, 5,  4 ) ; 
		ADDRCONV( srcAddr, dstAddr, 6, 11 ) ; 
		ADDRCONV( srcAddr, dstAddr, 7,  2 ) ; 
		ADDRCONV( srcAddr, dstAddr, 8,  3 ) ; 
		ADDRCONV( srcAddr, dstAddr, 9,  1 ) ; 
		ADDRCONV( srcAddr, dstAddr,10,  0 ) ;
		ADDRCONV( srcAddr, dstAddr,11,  9 ) ;

		if ( src[srcAddr/8] & ( (BYTE)0x80 >> ( srcAddr&7 ) ) ) {
			work[dstAddr/8] |= (BYTE)0x80 >> ( dstAddr&7 ) ;
		}
	}

	// 論理演算
	bp = work + 16 ;
	for ( i = 0; i < 10; i++, bp += 16*3 ) {
		
		cp = bp ;
		dp = bp + 16 ;
		for ( j = 0; j < 2; j++ ) {
			
			sp = work ;
			for ( k = 0; k < 16; k++, dp++, sp++ ) {
			
				// 論理演算コマンドの取り出し
				if ( k & 1 ) {
					cmd = cp[k/2] & 0x0F ;
					cp++ ;
				}
				else {
					cmd = cp[k/2] >> 4 ;
				}

				// 論理演算の実施(演算結果の下位バイトを左辺に代入)
				// 执行的逻辑操作 （分配给左边的结果的低序位字节）
				switch( cmd ) {
					case 0x00:
						*sp = *sp ^ *dp ;
						break ;

					case 0x01:
						*sp = *sp + *dp ;
						break ;

					case 0x02:
						*sp = *sp - *dp ;
						break ;

					case 0x03:
						*sp = ( *sp << 1 ) | ( *sp & 0x80 ) ? 0x01 : 0x00 ;
						break ;

					case 0x04:
						*sp = ( *sp >> 1 ) | ( *sp & 0x01 ) ? 0x80 : 0x00 ;
						break ;

					case 0x05:
						#pragma warning( push ) // 既存の警告状態を保存
						#pragma warning( disable:4244 ) // 4244番の警告を抑制
						
						*sp = (DWORD)*sp * (DWORD)*dp ;
						
						#pragma warning( pop ) // 保存した警告状態を復元
						break ;

					case 0x06:
						*sp = ~*sp ^ *dp ;
						break ;

					case 0x07:
						*sp = ~*dp ^ *sp ;
						break ;

					case 0x08:
						*sp = ~*sp ;
						break ;

					case 0x09:
						*sp = ( *sp<<4 ) | ( *dp>>4 ) ;
						break ;

					case 0x0A:
						*sp = ( *sp>>4 ) | ( *dp<<4 ) ;
						break ;

					case 0x0B:
						*sp = ( *sp<<4 ) | ( *dp>>4 ) ;
						break ;

					case 0x0C:
						*sp = *sp << ( *dp&3 ) ;
						break ;

					case 0x0D:
						*sp = *sp >> ( *dp>>6 ) ;
						break ;

					case 0x0E:
						*sp = *dp << ( *sp&3 ) ;
						break ;

					case 0x0F:
						*sp = *dp >> ( *sp >> 6 ) ;
						break ;
				
				} // end switch

			} // end for 

		} // end for 

	} // end for 

	for ( i = 0; i < 16; i++ ) {
		work[i] = dp[i] ^ work[i] ;
	}

	memcpy(dst, work, 16) ;

	return 0 ;
}
