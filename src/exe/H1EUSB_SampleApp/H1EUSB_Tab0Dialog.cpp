// H1EUSB_Tab0Dialog.cpp : 実装ファイル
//
#include "stdafx.h"
#include "H1EUSB_SampleApp.h"
#include "H1EUSB_Tab0Dialog.h"
#include "H1EUSB_DetailsDlg.h"
#include "H1EUSB_TemplateFile.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_Number.h"
#include "H1EUSB_MsgGuide.h"
/********************************************************************************************/
/* CH1EUSB_Tab0Dialogクラスプログラム														*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20120110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/********************************************************************************************/

// CH1EUSB_Tab0Dialog ダイアログ

IMPLEMENT_DYNAMIC(CH1EUSB_Tab0Dialog, CDialog)
/********************************************************************************************/
/*[メソッド名] CH1EUSB_Tab0Dialogクラスのコンストラクタ										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		CH1EUSB_Tab0Dialog(CWnd* pParent)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コンストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1EUSB_Tab0Dialog::CH1EUSB_Tab0Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(CH1EUSB_Tab0Dialog::IDD, pParent)
{
	//メンバー変数の初期化
	m_pParetDlg = 0;			//親ダイアログのオブジェクト
	m_iTimeCount = 0;			//コピーファイル時のプログレス表示用のカウンタ
	m_bSetEnable = FALSE;		//エディットボックスのチェンジイベントマスク用フラグ

}

/********************************************************************************************/
/*[メソッド名] CH1EUSB_Tab0Dialogクラスのデストラクタ										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		~CH1EUSB_DetailsDlg()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1EUSB_Tab0Dialog::~CH1EUSB_Tab0Dialog()
{


}

void CH1EUSB_Tab0Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CH1EUSB_Tab0Dialog, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_ENROLL, &CH1EUSB_Tab0Dialog::OnBnClickedButtonEnroll)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CH1EUSB_Tab0Dialog::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_DELTEMP, &CH1EUSB_Tab0Dialog::OnBnClickedButtonDeltemp)
	ON_BN_CLICKED(IDC_BUTTON_DELBANK, &CH1EUSB_Tab0Dialog::OnBnClickedButtonDelbank)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CH1EUSB_Tab0Dialog::OnBnClickedButtonDelall)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CH1EUSB_Tab0Dialog::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CH1EUSB_Tab0Dialog::OnBnClickedButtonOpen)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_ENROLL_BANK, &CH1EUSB_Tab0Dialog::OnEnChangeEditEnrollBank)
END_MESSAGE_MAP()


// CH1EUSB_Tab0Dialog メッセージ ハンドラ


void CH1EUSB_Tab0Dialog::OnOK()
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
BOOL CH1EUSB_Tab0Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください


	//機能名をのフォントサイズを変更する
	m_ftFName.CreateFont( 14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,NULL); 
	GetDlgItem(IDC_STATIC_ENROLL)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_COPYTEMP)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_DELETETMP)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_DELETEBANK)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_DELETEALL)->SetFont(&m_ftFName);

	//テンプレートコピー可能数のセット
	SetAbleToCopyCount(TRUE);

	//テンプレート登録のバンク番号とテンプレート番号をセット
	initEnroolEdit();

	//登録バンク番号エディットのマスクフラグをセット許可
	m_bSetEnable = TRUE;


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
void CH1EUSB_Tab0Dialog::SetParentDialog(LPVOID pDlg)
{
	//親ダイアログのポインターをメンバー変数にセット
	m_pParetDlg  = pDlg;
}


/********************************************************************************************/
/*[メソッド名] テンプレート登録ボタン押下メッセージ処理										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonEnroll()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セットされたバンク番号とテンプレート番号により登録処理を行う				*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonEnroll()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	TEMPU tempu;
	UINT uiBank;
	UINT uiTemp;
	CString strBank;
	CString strTmpNo;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;


	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;

	if(pTpf->m_uiTotalTemplates >= MAX_TEMPLATE_TOTAL){
		//登録済みのテンプレートが２０００本ある。
		strMessage = msgGuide.TemplateFullMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}

	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//バンク番号の入力値誤り
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_TMPNO);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		//テンプレート番号の入力値誤り
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}

	pDlg->EnabeleControl(NULL, FALSE);
	//指を置いてくださいガイダンス表示
	strMessage = msgGuide.GuidePlaceFingerFormatMessage();
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);

	//タッチセンサーポーリング
	if(!pDlg->PollingTuchOnState()){
		strMessage.FormatMessage(IDS_POLLING_TIMEOUT);
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	pDlg->TuchDisable();

	//登録トランザクション実行
	iReturn = GeneralTransaction(H1USB_COM_GETTEMPLATE, 0, 0,  MSGGUID_SEND_GETTEMP, H1USB_COMMENT_UOP);
	if(iReturn != H1USB_OK){
		strMessage = msgGuide.EnrollNGMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->EnabeleControl(NULL, TRUE);
		pDlg->TuchEnable();
		return;
	}

	//コントロールを有効にする
	pDlg->EnabeleControl(NULL, TRUE);

	//受信したテンプレートを取得
	iReturn = pCom->GetTempU(&tempu);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}
	tempu.uiTmpNum = uiTemp;

	//テンプレートファイルへ追加
	iReturn = pTpf->AddTemplate(uiBank, &tempu);
	if(iReturn != H1USB_OK){
		//テンプレートファイルへの追加に失敗した
		pDlg->TuchEnable();
		return;
	}

	//コントロールを更新する
	pDlg->UpdateContorol();

	//指定されたバンクに切り替える
	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, (WPARAM)uiBank, 0);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}

	iReturn = pCom->SetTempU(&tempu, 1, TRUE);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}
	//テンプレートをＨ１ＵＳＢに送信する
	iReturn = pCom->TransToDevice(H1USB_COM_SETTEMPLATE, 0, 0);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}

	//登録終了メッセージ表示
	strMessage = msgGuide.EnrollFormatMessage(uiBank, uiTemp,iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[メソッド名] テンプレートコピーボタン押下メッセージ処理									*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonCopy()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セットされたバンク番号とテンプレート番号により登録処理を行う				*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonCopy()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CNumber nb;
	CString strAbleBanks;
	CString strCopyBanks;
	int iAbleBanks;
	int iCopyBanks;
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;

	
	iReturn =H1USB_OK;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	CEdit *pAbleBanks = (CEdit *)GetDlgItem(IDC_EDIT_COPYBANK);
	pAbleBanks->GetWindowText(strAbleBanks);
	iAbleBanks = _ttoi(strAbleBanks);
	if(iAbleBanks == 0){
		//バンク0000hのテンプレートが存在しない
		strMessage = msgGuide.GuideNotExistBank0000FormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pACopyBanks = (CEdit *)GetDlgItem(IDC_EDIT_COPYCOUNT);
	pACopyBanks->GetWindowText(strCopyBanks);
	iCopyBanks = _ttoi(strCopyBanks);
	if(iCopyBanks > iAbleBanks ){
		//コピー数がコピー可能数を超えている
		strMessage = msgGuide.GuideCopyNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}

	pDlg->TuchDisable();

	//コントロール無効
	pDlg->EnabeleControl(NULL, FALSE);
	strMessage = msgGuide.CopyFileFormatMessage();
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);

	//テンプレートファイルコピー中のプログレス表示のためタイマスタート
	m_iTimeCount = 0;
	SetTimer(1, 500,NULL);
	
	//テンプレートファイルをコピー数分コピーする
	pTpf->CopyBank0000(iCopyBanks);
	//コントロールを更新する
	pDlg->UpdateContorol();
	//タイマー停止
	KillTimer(1);

	//デバイスと同期する
	iReturn = pDlg->SyncDevice();

	//終了メッセージ表示
	strMessage = msgGuide.CompleteCopyFormatMessage(iCopyBanks, iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	pDlg->EnabeleControl(NULL, TRUE);

	pDlg->TuchEnable();

}

/********************************************************************************************/
/*[メソッド名] テンプレート削除ボタン押下メッセージ処理										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonDeltemp()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セットされたバンク番号とテンプレート番号によりテンプレートを削除する		*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonDeltemp()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	UINT uiBank;
	UINT uiTemp;
	CString strBank;
	CString strTmpNo;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//バンク番号の入力値誤り
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_TEMP);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		//テンプレート番号の入力値誤り
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	//指令されたバンクのテンプレートを削除する
	iReturn = pTpf->DeleteOneTemplate(uiBank, uiTemp);
	if(iReturn != H1USB_OK){

	}

	pDlg->TuchDisable();


	//コントロールを更新する
	pDlg->UpdateContorol();

	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	//指定されたバンクに切り替える
	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, (WPARAM)uiBank, 0);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}



	//コントロール無効
	pDlg->EnabeleControl(NULL, FALSE);

	//指定されたテンプレートを削除する
	iReturn = pCom->TransToDevice(H1USB_COM_DELTEMPLATE, uiTemp, 0);

	//終了メッセージ表示
	strMessage = msgGuide.CompleteDelFormatMessage(uiBank, uiTemp, iReturn , 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);

	//コントロールを有効
	pDlg->EnabeleControl(NULL, TRUE);

	pDlg->TuchEnable();


}

/********************************************************************************************/
/*[メソッド名] バンク削除ボタン押下メッセージ処理											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonDelbank()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		セットされたバンク番号によりテンプレートを削除する							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonDelbank()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	CNumber nb;
	UINT uiBank;
	CString strBank;
	CMsgGuide msgGuide;
	CString strMessage;

	iReturn =H1USB_OK;
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_DELBANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//バンク番号の入力値誤り
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}


	pDlg->TuchDisable();


	//指定されたバンクのテンプレートファイルを削除
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	iReturn = pTpf->DeleteBank(uiBank);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}
	//コントロールを更新する
	pDlg->UpdateContorol();

	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	//指定されたバンクに切り替える
	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, (WPARAM)uiBank, 0);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}

	pDlg->EnabeleControl(NULL, FALSE);
	//デバイスから指定されたバンクのテンプレートを削除する
	iReturn = pCom->TransToDevice(H1USB_COM_DELTEMPLATE, -1, 0);
	strMessage = msgGuide.CompleteDelBankFormatMessage(uiBank, iReturn , 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	pDlg->EnabeleControl(NULL, TRUE);

	pDlg->TuchEnable();

}

/********************************************************************************************/
/*[メソッド名] 全テンプレート削除ボタン押下メッセージ処理									*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonDelall()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		すべてのテンプレートを削除する												*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonDelall()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	
	iReturn =H1USB_OK;

	//確認メッセージ表示
	iReturn = AfxMessageBox(IDS_GUIDE_DELALL,MB_OKCANCEL|MB_ICONINFORMATION);
	if(iReturn != IDOK){
		return;
	}

	
	//テンプレートファイルをすべて削除する
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	pDlg->TuchDisable();


	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	iReturn = pTpf->DeleteAll();
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}

	//コントロールを更新する
	pDlg->UpdateContorol();

	pDlg->EnabeleControl(NULL, FALSE);
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	//デバイスのすべてのテンプレートを削除する
	iReturn = pCom->TransToDevice(H1USB_COM_DELTEMPLATE, -2, 0);

	//終了メッセージ表示
	strMessage = msgGuide.CompleteDelAllFormatMessage(iReturn , 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	pDlg->EnabeleControl(NULL, TRUE);
	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[メソッド名] 保存ボタン押下メッセージ処理													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonSave()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		任意のフォルダーにテンプレートファイルを格納する							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonSave()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	
	iReturn =H1USB_OK;
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	pDlg->TuchDisable();

	CTemplateFile* pTpf = pDlg->m_pTemplateFile;

	//テンプレートを保存する
	iReturn = pTpf->SaveTemplates(this->m_hWnd);

	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[メソッド名] オープンボタン押下メッセージ処理												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonOpen()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		格納されているテンプレートファイルをロードし、デバイスと同期する			*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonOpen()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	pDlg->TuchDisable();


	//コントロールを無効にする
	pDlg->EnabeleControl(NULL, FALSE);

	//テンプレートファイルをオープンしてロードする
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	iReturn = pTpf->OpenTemplates(this->m_hWnd);
	if(iReturn==H1USB_OK){
		//デバイスと同期する
		pDlg->SyncDevice();
	}
	//コントロールを更新する
	pDlg->UpdateContorol();	

	//コントロールを有効にする
	pDlg->EnabeleControl(NULL, TRUE);

	pDlg->TuchEnable();

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
int CH1EUSB_Tab0Dialog::GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode)
{
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	pDlg->EnabeleControl(NULL, FALSE);
	if((uiMsgID != MSGGUID_NODISP) && (!(uiMode & H1USB_COMMENT_UOP))){	
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//コマンド送信メッセージ
			pDlg->SetComment(uiMode, strMessage, 0);
		}
	}
	//トランザクション実行
	iReturn = pCom->TransToDevice(uiTransID, wParam, lParam);
	if(uiMode & H1USB_COMMENT_UOP){
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//コマンド送信メッセージ
			pDlg->SetComment(uiMode, strMessage, 0);
		}
	}
	//コマンド終了メッセージ
	if(iReturn < H1USB_OK){
		//strMessage.Format(_T("Error:%d"), iReturn);
		strMessage = msgGuide.ErrorGeneralFormatMessage(iReturn);
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	}
	pDlg->EnabeleControl(NULL, TRUE);
	
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名]	コピー可能数を取得する														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void SetAbleToCopyCount(BOOL bSetCopy)										*/ 
/*																							*/
/*[内容]		0000バンクのテンプレート数より、コピー可能なバンク数を取得する				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::SetAbleToCopyCount(BOOL bSetCopy)
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;

	UINT uiAbles;
	uiAbles = 0;
	UINT ui;
	for(ui=0; pTpf->m_uiBanks>ui; ui++){
		if(pTpf->m_pBankInfo[ui].uiBankNum == 0x0000){
			//0x0000バンクが存在する
			if(pTpf->m_pBankInfo[ui].uiCounts > 0){
				uiAbles = 2000/pTpf->m_pBankInfo[ui].uiCounts - 1;
				break;
			}
		}
	}
	CString strWork;
	strWork.Format(_T("%d"), uiAbles);
	//可能コピー数をエディットボックスに設定する
	CEdit *pAbleBanks = (CEdit *)GetDlgItem(IDC_EDIT_COPYBANK);
	pAbleBanks->SetWindowText(strWork);
	if(bSetCopy){
		//可能コピー数を初期値としてエディットボックスに設定する
		CEdit *pCopyBanks = (CEdit *)GetDlgItem(IDC_EDIT_COPYCOUNT);
		pCopyBanks->SetWindowText(strWork);
	}
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
void CH1EUSB_Tab0Dialog::UpdateContorol()
{
	SetAbleToCopyCount(TRUE);
	updateEnroolEdit(TRUE);

}

/********************************************************************************************/
/*[メソッド名]	バンク番号、テンプレート番号												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void UpdateNummber(CString strBank, CString strTemp)						*/ 
/*																							*/
/*[内容]		指定されたバンク番号とテンプレート番号を各コントロールにセットする			*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::UpdateNummber(CString strBank, CString strTemp)
{
	//リストコントロールより選択されたバンクを表示
	CEdit *pEditDelOneBank = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_BANK);
	CEdit *pEditeBank = (CEdit *)GetDlgItem(IDC_EDIT_DELBANK);
	pEditDelOneBank->SetWindowText(strBank);
	pEditeBank->SetWindowText(strBank);

	//リストコントロールより選択されたテンプレートを表示
	CEdit *pEditDelOneTemp = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_TEMP);
	pEditDelOneTemp->SetWindowText(strTemp);

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
void CH1EUSB_Tab0Dialog::EnabeleControl(DWORD dwMode, BOOL bEnable)
{
	CButton*	pBtEnroll = (CButton*)GetDlgItem(IDC_BUTTON_ENROLL);
	CButton*	pBtCopy = (CButton*)GetDlgItem(IDC_BUTTON_COPY);
	CButton*	pBtDelTemp = (CButton*)GetDlgItem(IDC_BUTTON_DELTEMP);
	CButton*	pBtDelBank= (CButton*)GetDlgItem(IDC_BUTTON_DELBANK);
	CButton*	pBtDelAll = (CButton*)GetDlgItem(IDC_BUTTON_DELALL);
	CButton*	pBtDelSave= (CButton*)GetDlgItem(IDC_BUTTON_SAVE);
	CButton*	pBtDelOpen = (CButton*)GetDlgItem(IDC_BUTTON_OPEN);
	switch(dwMode){
		//スタートアップ状態
		case SYSTEM_STARTUP:
			pBtEnroll->EnableWindow(FALSE);
			pBtCopy->EnableWindow(FALSE);
			pBtDelTemp->EnableWindow(FALSE);
			pBtDelBank->EnableWindow(FALSE);
			pBtDelAll->EnableWindow(FALSE);
			pBtDelSave->EnableWindow(FALSE);
			pBtDelOpen->EnableWindow(FALSE);
			break;
		//デバイスOFF、エラー状態
		case SYSTEM_DEVICE_OFF:
		case SYSTEM_DEVICE_ERR:
			pBtEnroll->EnableWindow(FALSE);
			pBtCopy->EnableWindow(FALSE);
			pBtDelTemp->EnableWindow(FALSE);
			pBtDelBank->EnableWindow(FALSE);
			pBtDelAll->EnableWindow(FALSE);
			pBtDelSave->EnableWindow(bEnable);
			pBtDelOpen->EnableWindow(FALSE);
			break;
		//デバイスON状態
		case SYSTEM_DEVICE_ON:
		default:
			pBtEnroll->EnableWindow(bEnable);
			pBtCopy->EnableWindow(bEnable);
			pBtDelTemp->EnableWindow(bEnable);
			pBtDelBank->EnableWindow(bEnable);
			pBtDelAll->EnableWindow(bEnable);
			pBtDelSave->EnableWindow(bEnable);
			pBtDelOpen->EnableWindow(bEnable);
			break;
	}
}

/********************************************************************************************/
/*[メソッド名]	タイマイベント処理															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnTimer(UINT_PTR nIDEvent)												*/
/*																							*/
/*[内容]		テンプレートファイルコピー時のプログレス表示								*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if(nIDEvent == 1){
		CMsgGuide msgGuide;
		CString strMessage;
		CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
		m_iTimeCount++;
		if(m_iTimeCount > 5){
			m_iTimeCount = 0;
			strMessage = msgGuide.CopyFileFormatMessage();
			pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		}
		else{
			strMessage = msgGuide.CopyFileFormatMessage();
			pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		}
	}
	CDialog::OnTimer(nIDEvent);
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
void CH1EUSB_Tab0Dialog::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){
		if (!AfxGetThread()->PumpMessage()){
			return;
		}
	}
}

/********************************************************************************************/
/*[メソッド名]	テンプレート登録バンク番号、テンプレート番号初期処理						*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void initEnroolEdit()														*/
/*																							*/
/*[内容]		本クラス起動時のテンプレート登録バンク番号、テンプレート番号をセットする	*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::initEnroolEdit()
{
	int iTemps;
	int	i;
	CString strGroup;
	CString strtemp;
	WORD wGroup = 0;
	WORD wTemp = 0;
	WORDDWORD wdw;

	DWORD dwTarget = 0x000000000;
	DWORD dwWork;

	CEdit* pEdiGroupt;
	pEdiGroupt = (CEdit*)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	CEdit* pEditTemp = (CEdit*)GetDlgItem(IDC_EDIT_ENROLL_TMPNO);

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CListCtrl* pLCtrl = (CListCtrl*)pDlg->GetDlgItem(IDC_LIST_TMPLIST) ;
	//テンプレートリストより、登録数を取得
	iTemps = pLCtrl->GetItemCount();
	if(iTemps > 0){
		for(i=0; i<iTemps; i++ ){
			strGroup = pLCtrl->GetItemText(i, 0);
			strtemp = pLCtrl->GetItemText(i, 1);
			wGroup = (WORD)_tcstol(strGroup,NULL,16);
			wTemp = (WORD)_tcstol(strtemp,NULL,16);
			dwWork = wGroup*0x10000 + wTemp;
			if((dwWork-dwTarget) > 1){
				//次のテンプレート番号に隙間がない
				break;
			}
			dwTarget = dwWork;
		}
		dwTarget++;
		wdw.dword = dwTarget;
		if(wdw.word[0] > 0xFFFF){
			//バンク番号が最大値を超えたら０に戻す
			wdw.word[0] = 0;
			wdw.word[1]++;
		}
		if(wdw.word[1] > 0xFF){
			//テンプレート番号が最大値を超えたら０に戻す
			wdw.dword = 0;
		}
	}
	wdw.dword = dwTarget;

	//バンク番号とテンプレート番号をセットする
	strGroup.Format(_T("%04X"), wdw.word[1]);
	strtemp.Format(_T("%02X"), wdw.word[0]);
	pEdiGroupt->SetWindowText(strGroup);
	pEditTemp->SetWindowText(strtemp);

}

/********************************************************************************************/
/*[メソッド名]	テンプレート登録バンク番号、テンプレート番号更新処理						*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void updateEnroolEdit()														*/
/*																							*/
/*[内容]		本クラス起動時のテンプレート登録バンク番号、テンプレート番号を更新する		*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::updateEnroolEdit(BOOL bReplace)
{
	int iTemps;
	int	i;
	CString strGroup;
	CString strtemp;
	WORD wGroup = 0;
	WORD wTemp = 0;

	WORDDWORD wdwTarget;

	CEdit* pEdiGroupt;
	pEdiGroupt = (CEdit*)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	CEdit* pEditTemp = (CEdit*)GetDlgItem(IDC_EDIT_ENROLL_TMPNO);
	//現在セットされたバンク番号を取得
	pEdiGroupt->GetWindowText(strGroup);
	if(strGroup.IsEmpty()){
		//入力されていなければ０をセット
		wdwTarget.word[1] = 0;
	}
	else{
		wdwTarget.word[1] = (WORD)_tcstol(strGroup,NULL,16);
	}
	wdwTarget.word[0] = 0;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CListCtrl* pLCtrl = (CListCtrl*)pDlg->GetDlgItem(IDC_LIST_TMPLIST) ;
	//テンプレートリストより、テンプレートの登録数を取得する
	iTemps = pLCtrl->GetItemCount();
	if(iTemps > 0){
		for(i=0; i<iTemps; i++ ){
			strGroup = pLCtrl->GetItemText(i, 0);
			wGroup = (WORD)_tcstol(strGroup,NULL,16);
			if(wdwTarget.word[1] < wGroup){
				break;
			}
			if(wdwTarget.word[1] == wGroup){
				strtemp = pLCtrl->GetItemText(i, 1);
				wTemp = (WORD)_tcstol(strtemp,NULL,16);
				if(wdwTarget.word[0] == wTemp){
					wdwTarget.dword++;
				}
				else{
					break;
				}
			}
		}
	}
	//バンク番号とテンプレート番号を更新する
	strGroup.Format(_T("%04X"), wdwTarget.word[1]);
	strtemp.Format(_T("%02X"), wdwTarget.word[0]);
	m_bSetEnable = FALSE;
	if(bReplace){
		//入力中はバンク番号の更新は行わない
		pEdiGroupt->SetWindowText(strGroup);
	}
	pEditTemp->SetWindowText(strtemp);
	m_bSetEnable = TRUE;
	

}

/********************************************************************************************/
/*[メソッド名]	登録バンクチェンジメッセージ処理											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnEnChangeEditEnrollBank()												*/
/*																							*/
/*[内容]		登録バンクエディットボックスが編集されたら、空いてるテンプレート番号をセット*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnEnChangeEditEnrollBank()
{
	// TODO:  これが RICHEDIT コントロールの場合、
	// まず、CDialog::OnInitDialog() 関数をオーバーライドして、OR 状態の ENM_CHANGE
	// フラグをマスクに入れて、CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO:  ここにコントロール通知ハンドラ コードを追加してください。
	UINT uiBank;
	CString strBank;
	CNumber nb;

	if(!m_bSetEnable){
		//更新が許可されていない（プログラム上で更新しょりが行われている）
		return;
	}

	//バンク番号取得
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		return;
	}

	//次のバンク番号とテンプレート番号に更新
	updateEnroolEdit(FALSE);


}
