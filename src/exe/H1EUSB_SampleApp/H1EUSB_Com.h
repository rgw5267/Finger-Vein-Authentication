#pragma once
#include "afx.h"
#include "H1EUSB_Thread.h"
#include "Camellia.h"	// 暗号化で使用
#include "H1EUSB.h"
/********************************************************************************************/
/* CH1USBComクラスヘッダ																	*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20080110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
#define MAX_NINSHO_TEMPS				6					//テンプレート付認証の最大数
#define H1USB_STATUS_TUCH_ON			0x0001				//タッチセンサーがＯＮ状態
#define H1USB_STATUS_TUCH_OFF			0x0002				//タッチセンサーがＯＦＦ状態
#define H1USB_STATUS_INVDATA			0xFFFF				//予期しないデータを受信した

//コールバックエントリー定義
typedef LRESULT (CALLBACK* H1ECALBACK_TUCH_NTOTIFICATION)(DWORD dwStatus, LPVOID pObj);	//タッチセンサー状態通知
typedef LRESULT (CALLBACK* H1ECALBACK_SENNOTIFI)(DWORD dwStatus, LPVOID pObj);			//送信終了通知


//コールバックエントリー構造体
typedef struct _h1ecallbacks
{
	H1ECALBACK_TUCH_NTOTIFICATION			OnTuchNotification;		//タッチセンサー情報通知先
	H1ECALBACK_SENNOTIFI					OnSendEnd;				//送信終了通知	
} H1ECALLBACKS, *PH1ECALLBACKS;

//エンドポイント情報（H1ECOMOBJEP構造体）
typedef struct _h1ecomobjep
{
	LPVOID			pObj;				//コールバック先のオブジェクト
	H1ECALLBACKS	H1ECallBacks;		//コールバックエントリー		
} H1ECOMOBJEP, *PH1ECOMOBJEP;


class CH1USBCom :
	public CA2GObjThread
{
public:
	CH1USBCom(PH1ECOMOBJEP pH1EComObjEP);														//本クラスのコンストラクタ
	virtual ~CH1USBCom(void);																	//本クラスのデストラクタ
	int Open(LPCTSTR pComPort, LPDWORD pdwError);												//COMポートオープン
	int Close(LPDWORD pdwError);																//COMポートクローズ
	int StartEndPoint(void);																	//エンドポイントの機能開始
	int StopEndPoint(void);																		//エンドポイントの機能停止
	int TransToDevice(UINT uiMsg, WPARAM wParam, LPARAM lParam);								//トランザクションメイン処理
	int SetEnableBank(WORD* pwEnableBank, WORD wEnableBankCount);								//有効なバンクの配列をセットする
	int SetTempU(PTEMPU pTempU, UINT uiCounts, BOOL bInit);										//テンプレート情報をセットする
	int GetTempU(PTEMPU pTempU);																//テンプレート情報を取得する
	char m_cFwVersion[50];																		//ファームウェアバージョン格納配列
	char m_cSerialNum[50];																		//シリアルナンバー格納配列
	WORD* m_pwEnableBank;																		//バンクの配列のポインタ
	WORD m_wEnableBankCount;																	//有効なバンク数
	HANDLE			m_hCom;																		//COMポートハンドル


private:
	static LRESULT CALLBACK OnThreadBefor(LPVOID pObj);											//スレッド前処理のコールバック処理
	static LRESULT CALLBACK OnThread(LPVOID pObj);												//スレッド定周期コールバック処理
	static LRESULT CALLBACK OnThreadAfter(LPVOID pObj);											//スレッド後処理のコールバック処理
	static LRESULT CALLBACK OnComMessage(UINT uiMsg, WPARAM wParam, LPARAM lParam, LPVOID pObj);//トランザクションメッセージ処理

	int decodeMasterKey512( const BYTE *, BYTE * ) ;											// 512バイトの乱数から暗号キーを生成
	int TransScranmble(void);																	//暗号化コマンドトランザクション
	int TransLedGreen(void);																	//緑LEDトランザクション
	int TransLedRed(void);																		//赤LEDトランザクション
	int TransLedGR(void);																		//緑＋赤LEDトランザクション
	int TransLedOff(void);																		//LED消灯トランザクション
	int TransTuchNotify(BYTE byParam);																//タッチセンサー状態通知許可トランザクション
	int TransGetFwVersion( char *pFwVersion );													//ファームウェアバージョン取得トランザクション
	int TransGetSerialNum( char *pSerialNum );													//シリアルナンバー取得トランザクション
	int TransBUZStart( void );																	//ブザー鳴動開始トランザクション
	int TransBUZStop( void );																	//ブザー鳴動停止トランザクション
	int TransSetSecurity(BYTE byMode);															//セキュリティレベル設定トランザクション
	int TransNinsho( const BYTE byNinshoMode, const int nMemNo, BYTE *pMatchMemNo );			//１：１認証トランザクション
	int TransNinshoBank( const WORD wEnableBank[], const WORD wEnableBankCnt, BYTE *pMatchMemNo, WORD *pMatchBank );	//バンク認証トランザクション
	int TransNinshoWithTemplate(const PTEMPU pTempU, const BYTE byCount, BYTE *pMatchNum);		//テンプレート付認証トランザクション
	int TransGetTemplate(PTEMPU pTempU);														//テンプレート取得トランザクション
	int TransChangeBank( const WORD wBankNo);													//バンク変更トランザクション
	int TransSetTemplate(PTEMPU pTempU);														//テンプレート登録トランザクション
	int TransDelTemplate(const int nMemNo);														//テンプレート削除トランザクション
	int TransHardWare( void );																	//ハードウェアリセットトランザクション
	int CheckASyncData(LPDWORD pdwState);														//認証時の同期レスポンス取得
	int ledAllOff(void);
	int ledOnOff( const BYTE byColor, const BYTE byOnOff );
	int ledStartBlink( const BYTE byOffTime, const BYTE byOnTime);
	int ledBlink( const BYTE byOffTime, const BYTE byOnTime );
	int beepOK( void );
	int beepNG( void );
	int beepCancel( void );
	int beepSound( const BYTE byPattern, const BYTE byCount );

	BOOL sendComm( const BYTE *, const DWORD ) ;										//コマンドをH1E-USBに送信(ノーマル状態)
	BOOL sendCommEnc( const BYTE *, DWORD ) ;											//コマンドをH1E-USBに送信(暗号通信状態)
	DWORD getBuffSize( void ) ;															//受信バッファのバイト数を返却
	BOOL recvComm( BYTE *, const DWORD ) ;												//H1E-USBからデータを受信する(ノーマル状態)
	BOOL recvCommEnc( BYTE *, DWORD ) ;													//H1E-USBからデータを受信する(暗号通信状態)
	BYTE m_byCurrentWorkKey[16] ;														//暗号鍵
	BOOL			m_bCamelliaEnabled ;												//暗号通信状態か否かを表すフラグ(TRUE: 暗号通信状態, FALSE:ノーマル状態)
	KEY_TABLE_TYPE	m_uKttWork ;														//拡大鍵
	H1ECOMOBJEP		m_H1EComObjEP;														//エンドポイント情報（H1ECOMOBJEP構造体）
	HANDLE			m_hTEnd;															//トランザクション終了イベントハンドル
	TEMPU			m_TempU[MAX_NINSHO_TEMPS];											//テンプレートユニットテーブル
	UINT			m_uiTempUs;															//テンプレートユニットの個数
	int				m_iLastError;														//エラー格納変数
	BOOL			m_bTransEnable;														//トランザクション中


};
