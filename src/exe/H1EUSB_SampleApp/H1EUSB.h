#pragma once
/********************************************************************************************/
/* 本アプリケーションのプロジェクトヘッダ													*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20080110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
typedef union _byteword
{
	BYTE	byte[2];
	WORD	word;
} BYTEWORD, *PBYTEWORD;

typedef union _worddword
{
	BYTE	byte[4];
	WORD	word[2];
	DWORD	dword;
}WORDDWORD, *PWORDDWORD;


#define MAX_TEMPLATE_TOTAL		2000		//テンプレートの最大登録可能数
#define MAX_TEMPLATE_INBANK		100			//１バンク内の最大登録可能数

typedef struct _tempu
{
	UINT		uiTmpNum;					//テンプレート№
	SYSTEMTIME	time;						//作成された時間
	BYTE		byTemplate[536];			//テンプレート
}TEMPU, *PTEMPU;



// アドレス変換マクロ
#define ADDRCONV(s,d,sb,db) if (s & (1<<sb) ) {d |= 1<<db;}

// メッセージボックスのタイトル
#define TITLE	"H1E-USB_SampleApp"	

// H1E-USB の FriendlyName
#define FRIENDLY_NAME	"Hitachi USB Finger Vein Biometric Authentication Unit"

//状態遷移
#define SYSTEM_STARTUP			1					//起動時状態
#define SYSTEM_DEVICE_OFF		2					//デバイス未接続状態
#define SYSTEM_DEVICE_ON		3					//デバイス接続状態
#define SYSTEM_DEVICE_ERR		4					//エラー状態状態



//Error Code Define;
#define H1USB_OK							0
#define H1USB_NOTIFY						1
#define H1USB_CANCEL						2

#define H1USB_ERR_TRANSTOUT					-10			//トランザクションエラー（タイムアウト）
#define H1USB_ERR_BIGGER					-100		//パラメータの値が大きすぎる
#define H1USB_ERR_INPNUMMBER				-101		//入力値エラー
#define H1USB_ERR_CREATETHREAD				-120		//スレッドの作成に失敗した
#define H1USB_ERR_MAXEVENTS					-121		//イベントの作成が規定最大個数に達した。


#define H1USB_ERR_COMOPEN					-200		//COMポートのオープンエラー
#define H1USB_ERR_COMCLOSE					-201		//COMポートのオープンエラー
#define H1USB_ERR_READFILE					-202		//COMポートのReadFileエラー
#define H1USB_ERR_WRITEFILE					-203		//COMポートのReadFileエラー
#define H1USB_ERR_RDATA_NON					-204		//受信データなし
#define H1USB_ERR_RESPONS					-205		//異常レスポンスを受信した


#define H1USB_ERR_TEMP_NOTOPEN				-500		//テンプレートファイルクラスがオープンされていない
#define H1USB_ERR_TEMPFILE_OPEN				-501		//テンプレートファイルがオープンできない
#define H1USB_ERR_TEMPFILE_NON				-502		//テンプレートが存在しない
#define H1USB_ERR_TEMPFILE_COPY				-503		//テンプレートファイルのコピーに失敗した
#define H1USB_ERR_TEMPFILE_DEL				-504		//テンプレートファイルのコピーに失敗した


//通信処理メッセージ定義

#define H1USB_COM_SCRAMBLE				WM_USER + 0			//暗号化コマンド送信
#define H1USB_COM_GREENLED_ON			WM_USER + 1			//緑ＬＥＤＯＮ
#define H1USB_COM_REDLED_ON				WM_USER + 2			//赤ＬＥＤＯＮ
#define H1USB_COM_GRLED_ON				WM_USER + 3			//緑＋赤ＬＥＤＯＮ
#define H1USB_COM_LED_OFF				WM_USER + 4			//ＬＥＤＯＦＦ
#define H1USB_COM_TUCH_NOTIFY			WM_USER + 5			//タッチセンサー状態通知
#define H1USB_COM_GETFVER				WM_USER + 6			//タッチセンサー状態通知
#define H1USB_COM_GETSNUM				WM_USER + 7			//タッチセンサー状態通知
#define H1USB_COM_BUZSTART				WM_USER + 8			//ブザー鳴動開始
#define H1USB_COM_BUZSTOP				WM_USER + 9			//ブザー鳴動開始
#define H1USB_COM_SECLOW				WM_USER + 10		//ブザー鳴動停止
#define H1USB_COM_SECMIDLOW				WM_USER + 11		//ブザー鳴動停止
#define H1USB_COM_SECMIDLE				WM_USER + 12		//ブザー鳴動停止
#define H1USB_COM_SECMIDHG				WM_USER + 13		//ブザー鳴動停止
#define H1USB_COM_SECHIHGH				WM_USER + 14		//ブザー鳴動停止
#define H1USB_COM_NINSHO				WM_USER + 20		//認証指示
#define H1USB_COM_NINSHO_BANK			WM_USER + 22		//バンク間認証
#define H1USB_COM_NINSHO_TEMP			WM_USER + 23		//テンプレート付き認証
#define H1USB_COM_GETTEMPLATE			WM_USER + 24		//ブザー鳴動停止
#define H1USB_COM_CHANGE_BANK			WM_USER + 25		//バンク変更
#define H1USB_COM_SETTEMPLATE			WM_USER + 26		//テンプレート送信
#define H1USB_COM_DELTEMPLATE			WM_USER + 27		//テンプレート削除
#define H1USB_COM_HWRESET				WM_USER + 100		//ハードウェアリセット







