// H1EUSB_Tab2Dialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "H1EUSB_SampleApp.h"
#include "H1EUSB_Tab2Dialog.h"
#include "H1EUSB_DetailsDlg.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_MsgGuide.h"
/********************************************************************************************/
/* CH1EUSB_Tab2Dialogクラスプログラム														*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20120110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/********************************************************************************************/

// CH1EUSB_Tab2Dialog ダイアログ

IMPLEMENT_DYNAMIC(CH1EUSB_Tab2Dialog, CDialog)
/********************************************************************************************/
/*[メソッド名] CH1EUSB_Tab2Dialogクラスのコンストラクタ										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		CH1EUSB_Tab0Dialog(CWnd* pParent)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コンストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1EUSB_Tab2Dialog::CH1EUSB_Tab2Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(CH1EUSB_Tab2Dialog::IDD, pParent)
{
	//メンバー変数の初期化
	m_pParetDlg = 0;				//親ダイアログのポインタを初期化
}

/********************************************************************************************/
/*[メソッド名] CH1EUSB_Tab2Dialogクラスのデストラクタ										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		~CH1EUSB_Tab1Dialog()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1EUSB_Tab2Dialog::~CH1EUSB_Tab2Dialog()
{
}

void CH1EUSB_Tab2Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CH1EUSB_Tab2Dialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BUZON, &CH1EUSB_Tab2Dialog::OnBnClickedButtonBuzon)
	ON_BN_CLICKED(IDC_BUTTON_BUZOFF, &CH1EUSB_Tab2Dialog::OnBnClickedButtonBuzoff)
	ON_BN_CLICKED(IDC_BUTTON_LEDGREEN, &CH1EUSB_Tab2Dialog::OnBnClickedButtonLedgreen)
	ON_BN_CLICKED(IDC_BUTTON_LEDRED, &CH1EUSB_Tab2Dialog::OnBnClickedButtonLedred)
	ON_BN_CLICKED(IDC_BUTTON_LEDGR, &CH1EUSB_Tab2Dialog::OnBnClickedButtonLedgr)
	ON_BN_CLICKED(IDC_BUTTON_LEDOFF, &CH1EUSB_Tab2Dialog::OnBnClickedButtonLedoff)
	ON_BN_CLICKED(IDC_BUTTON_SECLOW, &CH1EUSB_Tab2Dialog::OnBnClickedButtonSeclow)
	ON_BN_CLICKED(IDC_BUTTON_SECMLOW, &CH1EUSB_Tab2Dialog::OnBnClickedButtonSecmlow)
	ON_BN_CLICKED(IDC_BUTTON_SECMID, &CH1EUSB_Tab2Dialog::OnBnClickedButtonSecmid)
	ON_BN_CLICKED(IDC_BUTTON_SECMIDHG, &CH1EUSB_Tab2Dialog::OnBnClickedButtonSecmidhg)
	ON_BN_CLICKED(IDC_BUTTON_SECHIGH, &CH1EUSB_Tab2Dialog::OnBnClickedButtonSechigh)
END_MESSAGE_MAP()


// CH1EUSB_Tab2Dialog メッセージ ハンドラ

void CH1EUSB_Tab2Dialog::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//CDialog::OnOK();
}



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
BOOL CH1EUSB_Tab2Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	//機能名をのフォントサイズを変更する
	m_ftFName.CreateFont( 14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,NULL); 
	GetDlgItem(IDC_STATIC_CTRBUZZER)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_CTRLED)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_SETSECURITY)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_H1ERESET)->SetFont(&m_ftFName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

/********************************************************************************************/
/*[メソッド名] 親ダイアログのポインターセット												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void SetParentDialog(LPVOID pDlg)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		親ダイアログのポインタをメンバー変数に格納する								*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::SetParentDialog(LPVOID pDlg)
{
	m_pParetDlg  = pDlg;
}


/********************************************************************************************/
/*[メソッド名] ブザー鳴動開始ボタン押下メッセージ処理										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonBuzon()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		ブザー鳴動開始トランザクションを実行する									*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonBuzon()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_BUZSTART, MSGGUID_BUZZER_ON);
}

/********************************************************************************************/
/*[メソッド名] ブザー鳴動終了ボタン押下メッセージ処理										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonBuzon()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		ブザー鳴動終了トランザクションを実行する									*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonBuzoff()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_BUZSTOP, MSGGUID_BUZZER_OFF);
}

/********************************************************************************************/
/*[メソッド名] 緑ＬＥＤ点灯ボタン押下メッセージ処理											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonLedgreen()											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		緑ＬＥＤ点灯トランザクションを実行する										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonLedgreen()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_GREENLED_ON, MSGGUID_LED_GREEN);
}

/********************************************************************************************/
/*[メソッド名] 赤ＬＥＤ点灯ボタン押下メッセージ処理											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonLedred()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		赤ＬＥＤ点灯トランザクションを実行する										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonLedred()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_REDLED_ON, MSGGUID_LED_RED);
}

/********************************************************************************************/
/*[メソッド名] 緑＆赤ＬＥＤ点灯ボタン押下メッセージ処理										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonLedgr()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		緑＆赤ＬＥＤ点灯トランザクションを実行する									*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonLedgr()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_GRLED_ON, MSGGUID_LED_GREENRED);
}

/********************************************************************************************/
/*[メソッド名] 全ＬＥＤ消灯ボタン押下メッセージ処理											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonLedoff()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		全ＬＥＤ消灯トランザクションを実行する										*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonLedoff()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_LED_OFF, MSGGUID_LED_OFF);
}

/********************************************************************************************/
/*[メソッド名] セキュリティ設定（Ｌｏｗ）ボタン押下メッセージ処理							*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonSeclow()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セキュリティ（Ｌｏｗ）設定トランザクションを実行する						*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonSeclow()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_SECLOW, MSGGUID_SECURITY_LOW);
}

/********************************************************************************************/
/*[メソッド名] セキュリティ設定（ＭｉｄｌｅＬｏｗ）ボタン押下メッセージ処理					*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonSecmlow()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セキュリティ（ＭｉｄｌｅＬｏｗＬｏｗ）設定トランザクションを実行する		*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonSecmlow()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_SECMIDLOW, MSGGUID_SECURITY_MIDLOW);
}

/********************************************************************************************/
/*[メソッド名] セキュリティ設定（Ｍｉｄｌｅ）ボタン押下メッセージ処理						*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonSecmid()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セキュリティ（Ｍｉｄｌｅ）設定トランザクションを実行する					*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonSecmid()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_SECMIDLE, MSGGUID_SECURITY_MIDDLE);
}

/********************************************************************************************/
/*[メソッド名] セキュリティ設定（ＭｉｄｌｅＨｉｇｈ）ボタン押下メッセージ処理				*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonSecmidhg()											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セキュリティ（ＭｉｄｌｅＨｉｇｈ）設定トランザクションを実行する			*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonSecmidhg()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_SECMIDHG, MSGGUID_SECURITY_MIDHIGH);
}

/********************************************************************************************/
/*[メソッド名] セキュリティ設定（Ｈｉｇｈ）ボタン押下メッセージ処理							*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonSechigh()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セキュリティ（Ｈｉｇｈ）設定トランザクションを実行する						*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::OnBnClickedButtonSechigh()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OtherTransaction(H1USB_COM_SECHIHGH, MSGGUID_SECURITY_HIGH);
}


/********************************************************************************************/
/*[メソッド名]	汎用トランザクション処理													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int OtherTransaction(UINT uiTransID, UINT uiMsgID)							*/
/*																							*/
/*[内容]		ＬＥＤ＆セキュリティ＆ブザートランザクションを実施し、コメントを表示する	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_Tab2Dialog::OtherTransaction(UINT uiTransID, UINT uiMsgID)
{
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	pDlg->TuchDisable();
	pDlg->EnabeleControl(NULL, FALSE);
	if(uiMsgID != MSGGUID_NODISP){
		//コメントを表示する
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//コマンド送信メッセージ
			pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		}
	}
	//トランザクション実行
	iReturn = pCom->TransToDevice(uiTransID, 0, 0);
	//コマンド終了メッセージ
	if(iReturn < H1USB_OK){
		//strMessage.Format(_T("Error:%d"), iReturn);
		strMessage = msgGuide.ErrorGeneralFormatMessage(iReturn);
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	}
	pDlg->EnabeleControl(NULL, TRUE);
	pDlg->TuchEnable();
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名]	コントロール更新															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void UpdateContorol()														*/ 
/*																							*/
/*[内容]		本ダイアログのコントロール更新する											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab2Dialog::UpdateContorol()
{

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
const UINT uiOtherCtrlID[]={
IDC_BUTTON_BUZON,IDC_BUTTON_BUZOFF,IDC_BUTTON_LEDGREEN,IDC_BUTTON_LEDRED,IDC_BUTTON_LEDGR,
IDC_BUTTON_LEDOFF,IDC_BUTTON_SECLOW,IDC_BUTTON_SECMLOW,IDC_BUTTON_SECMID,IDC_BUTTON_SECMIDHG,
IDC_BUTTON_SECHIGH
};

void CH1EUSB_Tab2Dialog::EnabeleControl(DWORD dwMode, BOOL bEnable)
{
	int	i;
	int iCtrls;
	CButton* pBt;
	BOOL bFixEnable = bEnable;

	switch(dwMode){
		case SYSTEM_STARTUP:
		case SYSTEM_DEVICE_OFF:
		case SYSTEM_DEVICE_ERR:
			bFixEnable = FALSE;
			break;
		case SYSTEM_DEVICE_ON:
		default:
			break;
	}

	iCtrls = sizeof(uiOtherCtrlID)/sizeof(UINT);	
	for(i=0; i<iCtrls; i++){
		pBt = (CButton*)GetDlgItem(uiOtherCtrlID[i]);
		pBt->EnableWindow(bFixEnable);
	}

}