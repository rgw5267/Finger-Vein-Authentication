// CH1EUSB_Tab1Dialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "H1EUSB_SampleApp.h"
#include "H1EUSB_Tab1Dialog.h"
#include "H1EUSB_DetailsDlg.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_Number.h"
#include "H1EUSB_MsgGuide.h"
/********************************************************************************************/
/* CH1EUSB_Tab1Dialogクラスプログラム														*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20120110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/********************************************************************************************/
// CH1EUSB_Tab１Dialog ダイアログ

IMPLEMENT_DYNAMIC(CH1EUSB_Tab1Dialog, CDialog)

/********************************************************************************************/
/*[メソッド名] CH1EUSB_Tab1Dialogクラスのコンストラクタ										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		CH1EUSB_Tab0Dialog(CWnd* pParent)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コンストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1EUSB_Tab1Dialog::CH1EUSB_Tab1Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(CH1EUSB_Tab1Dialog::IDD, pParent)
{
	//メンバー変数初期化
	m_pParetDlg = 0;		//親ダイアログのポインタ変数を初期化

}

/********************************************************************************************/
/*[メソッド名] CH1EUSB_Tab1Dialogクラスのデストラクタ										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		~CH1EUSB_Tab1Dialog()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CH1EUSB_Tab1Dialog::~CH1EUSB_Tab1Dialog()
{
}

void CH1EUSB_Tab1Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CH1EUSB_Tab1Dialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_11VERIF, &CH1EUSB_Tab1Dialog::OnBnClickedButton11verif)
	ON_BN_CLICKED(IDC_BUTTON_1NVERIF, &CH1EUSB_Tab1Dialog::OnBnClickedButton1nverif)
	ON_BN_CLICKED(IDC_BUTTON_VERIF_AB, &CH1EUSB_Tab1Dialog::OnBnClickedButtonVerifAb)
	ON_BN_CLICKED(IDC_BUTTON_VERIFWT, &CH1EUSB_Tab1Dialog::OnBnClickedButtonVerifwt)
END_MESSAGE_MAP()


// CH1EUSB_Tab１Dialog メッセージ ハンドラ

void CH1EUSB_Tab1Dialog::OnOK()
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
BOOL CH1EUSB_Tab1Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	//機能名をのフォントサイズを変更する
	m_ftFName.CreateFont( 14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,NULL); 
	GetDlgItem(IDC_STATIC_11VER)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_1NVER)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_VERACBANK)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_VERTEMP)->SetFont(&m_ftFName);

	
	//テンプレート付き認証のバンク/テンプレート選択コンボボックス初期化
	SetVerTempList();


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
void CH1EUSB_Tab1Dialog::SetParentDialog(LPVOID pDlg)
{
	m_pParetDlg  = pDlg;
}


/********************************************************************************************/
/*[メソッド名] 認証（１：１）ボタン押下メッセージ処理										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButton11verif()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		バンク番号とテンプレート番号により認証処理を実行する						*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab1Dialog::OnBnClickedButton11verif()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	CNumber nb;
	CString strBank;
	CString strTmpNo;
	UINT uiBank;
	UINT uiTemp;
	CMsgGuide msgGuide;
	CString strMessage;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;

	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_11VERBANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//バンク番号の入力値誤り
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_11VERTEMP);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		//テンプレート番号の入力値誤り
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	pDlg->TuchDisable();


	//バンクを切り替える
	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, uiBank, 0);


	pDlg->TuchEnable();


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


	//テンプレートを指定して認証をする
	iReturn = GeneralTransaction(H1USB_COM_NINSHO, 0x09, uiTemp,  MSGGUID_VERIF_11, H1USB_COMMENT_UOP);
	if(iReturn != H1USB_OK){
		strMessage = msgGuide.VerificationNGMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->TuchEnable();
		return;
	}
	//認証終了メッセージ表示
	strMessage = msgGuide.VerificationFormatMessage(uiBank, uiTemp, iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	pDlg->TuchEnable();

}

/********************************************************************************************/
/*[メソッド名] 認証（１：Ｎ）ボタン押下メッセージ処理										*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButton1nverif()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		バンク番号により認証処理を実行する											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab1Dialog::OnBnClickedButton1nverif()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;

	CNumber nb;
	CString strBank;
	UINT uiBank;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;

	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_N1BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//バンク番号の入力値誤り
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CH1USBCom* pCom = pDlg->m_pH1USBCom;

	//バンクを切り替える
	pDlg->TuchDisable();

	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, uiBank, 0);
	pDlg->TuchEnable();

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

	//１：Ｎ認証をする。
	iReturn = GeneralTransaction(H1USB_COM_NINSHO, 0X08, 0,  MSGGUID_VERIF_1N, H1USB_COMMENT_UOP);
	if(iReturn != H1USB_OK){
		strMessage = msgGuide.VerificationNGMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->TuchEnable();
		return;
	}

	//認証終了処理表示
	strMessage = msgGuide.VerifBankFormatMessage(uiBank, iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[メソッド名] バンク間認証（１：Ｎ）ボタン押下メッセージ処理								*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonVerifAb()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		指定されたバンク間により認証処理を実行する									*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab1Dialog::OnBnClickedButtonVerifAb()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strStartBank;
	CString strEndBank;
	UINT ui;
	UINT uiStartBank;
	UINT uiEndBank;
	UINT uiBanks;
	CNumber nb;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;

	//開始バンクを取得する
	CEdit *pEditStartBank = (CEdit *)GetDlgItem(IDC_EDIT_STARTBANK);
	pEditStartBank->GetWindowText(strStartBank);
	if(!nb.ConvertBankNummber(strStartBank, &uiStartBank)){
		//バンク番号の入力値誤り
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	//終了バンクを取得する
	CEdit *pEditEndBank = (CEdit *)GetDlgItem(IDC_EDIT_ENDBANK);
	pEditEndBank->GetWindowText(strEndBank);
	if(!nb.ConvertBankNummber(strEndBank, &uiEndBank)){
		//バンク番号の入力値誤り
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}

	pDlg->TuchDisable();


	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	uiBanks = uiEndBank - uiStartBank + 1;
	WORD* pEnableBanlk = new WORD[uiBanks];

	UINT uiFixBanks;
	UINT uii;
	uiFixBanks = 0;
	for(ui=0; ui<uiBanks; ui++){
		for(uii=0; uii<pTpf->m_uiBanks; uii++){
			if(pTpf->m_pBankInfo[uii].uiBankNum == (uiStartBank + ui)){
				pEnableBanlk[uiFixBanks] = pTpf->m_pBankInfo[uii].uiBankNum;
				uiFixBanks++;
			}
		}
		if(uiFixBanks >= pTpf->m_uiBanks){
				break;
		}
	}
	pDlg->TuchEnable();

	if(uiFixBanks < 1){
		//バンク番号の入力値誤り
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}


	//対象となるバンク情報を設定
//	iReturn = pCom->SetEnableBank(pEnableBanlk, uiBanks);
	iReturn = pCom->SetEnableBank(pEnableBanlk, uiFixBanks);
	if(pEnableBanlk){
		delete[] pEnableBanlk;
	}

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

	//バンク間認証をする
	//iReturn = GeneralTransaction(H1USB_COM_NINSHO, 0, 0,  MSGGUID_VERIF_ACBANK, H1USB_COMMENT_UOP);
	iReturn = GeneralTransaction(H1USB_COM_NINSHO_BANK, 0, 0,  MSGGUID_VERIF_ACBANK, H1USB_COMMENT_UOP);
	if(iReturn != H1USB_OK){
		strMessage = msgGuide.VerificationNGMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->TuchEnable();
		return;
	}

	//認証終了メッセージ表示
	strMessage = msgGuide.VerifAcrossBKFormatMessage(uiStartBank, uiEndBank, iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[メソッド名] テンプレート指定認証（１：Ｎ）ボタン押下メッセージ処理						*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void OnBnClickedButtonVerifwt()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		指定されたテンプレートより認証（１：Ｎ）処理を実行する						*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
const UINT uiControlList[]={IDC_COMB_TEMP1, IDC_COMB_TEMP2, IDC_COMB_TEMP3, 
						IDC_COMB_TEMP4, IDC_COMB_TEMP5, IDC_COMB_TEMP6};


void CH1EUSB_Tab1Dialog::OnBnClickedButtonVerifwt()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	CString strBank;
	CString strTemplate;
	UINT uiBank;
	UINT uiTempNum;
	int i;
	int iContorls = sizeof(uiControlList)/sizeof(UINT);
	BOOL bInit;
	TEMPU tempU;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;

	bInit = TRUE;
	for(i=0; i<iContorls; i++){
		CComboBox *pComboTemp = (CComboBox*)GetDlgItem(uiControlList[i]);
		pComboTemp->GetWindowText(strWork);
		if(strWork.GetLength() == 0){
			break;
		}
		strBank = strWork.Left(4);
		strTemplate = strWork.Mid(7,2);
		iReturn = _stscanf(strBank, "%x", &uiBank);
		if(iReturn <= 0){
			break;
		}
		iReturn = _stscanf(strTemplate, "%x", &uiTempNum);
		if(iReturn <= 0){
			break;
		}
		//テンプレートファイルよりテンプレートを取得
		iReturn = pTpf->GetTemplate(uiBank, uiTempNum, &tempU);
		if(iReturn != H1USB_OK){
			break;
		}
		//認証のためのテンプレートをセット
		iReturn = pCom->SetTempU(&tempU, 1, bInit);
		if(iReturn != H1USB_OK){
			break;
		}
		bInit = FALSE;
	}
	if(!bInit){
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



		//テンプレート付き認証
		iReturn = GeneralTransaction(H1USB_COM_NINSHO_TEMP, 0, 0,  MSGGUID_VERIF_WTEMP, H1USB_COMMENT_UOP);
		if(iReturn != H1USB_OK){
			strMessage = msgGuide.VerificationNGMessage();
			pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
			pDlg->TuchEnable();
			return;
		}
		//認証終了メッセージ表示
		strMessage = msgGuide.VerificationOKFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->TuchEnable();
	}
	else{
		//バンクとテンプレートが選択されていない
		strMessage.FormatMessage(IDS_NON_SELECT);
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	}

}

/********************************************************************************************/
/*[メソッド名] テンプレート指定認証（１：Ｎ）コンボボックスセット							*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void SetVerTempList()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		登録されているテンプレートより、テンプレート付き認証のためのコンボボックス	*/
/*				に値をセットする															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
void CH1EUSB_Tab1Dialog::SetVerTempList()
{
	
	int iContorls = sizeof(uiControlList)/sizeof(UINT);
	int	i;
	int ii;
	CString strWork;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	for(i=0; i<iContorls; i++){
		CComboBox *pComboTemp = (CComboBox*)GetDlgItem(uiControlList[i]);
		//既に表示中の値があれば、全て削除する
		for (ii = pComboTemp->GetCount() - 1; ii >= 0; ii--){
		   pComboTemp->DeleteString(ii);
		}
		if(i<(int)pTpf->m_uiTotalTemplates){
			pComboTemp->EnableWindow(TRUE);
			pComboTemp->SetItemHeight(0,10);
			for(ii=0; ii<(int)pTpf->m_uiTotalTemplates; ii++){
				// "bank nummber/template nummber"のフォーマットでリストを作成
				strWork.Format(_T("%04Xh/%02Xh"), pTpf->m_pTempList[ii].uiBankNum, pTpf->m_pTempList[ii].uiTempNum);
				pComboTemp->InsertString(-1,strWork);
			}
			//最大表示個数６に満たない場合は、"-----/---"を表示する
			pComboTemp->InsertString(-1,_T("-----/---"));				
		}
		else{
			pComboTemp->EnableWindow(FALSE);
		}
	}

	pTpf->m_uiTotalTemplates;

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
int CH1EUSB_Tab1Dialog::GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode)
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
	//コマンド終了メッセージ
	if(uiMode&H1USB_COMMENT_UOP){
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//コマンド送信メッセージ
			pDlg->SetComment(uiMode, strMessage, 0);
		}
	}
	if(iReturn < H1USB_OK){
		//strMessage.Format(_T("Error:%d"), iReturn);
		strMessage = msgGuide.ErrorGeneralFormatMessage(iReturn);
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	}
	pDlg->EnabeleControl(NULL, TRUE);
	
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
void CH1EUSB_Tab1Dialog::UpdateContorol()
{
	SetVerTempList();
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
void CH1EUSB_Tab1Dialog::UpdateNummber(CString strBank, CString strTemp)
{
	//リストコントロールより選択されたバンクを表示
	CWnd* pWnd;
	int iCtrlID;

	pWnd = GetFocus();
	if(pWnd){
		//フォーカスがあるウィンドウが存在する
		iCtrlID = pWnd->GetDlgCtrlID();
		CEdit *pEditStartBank = (CEdit *)GetDlgItem(IDC_EDIT_STARTBANK);
		CEdit *pEditEndBank = (CEdit *)GetDlgItem(IDC_EDIT_ENDBANK);
		if(iCtrlID == IDC_EDIT_STARTBANK){
			//スタートバンクにフォーカスがあればバンクを挿入
			pEditStartBank->SetWindowText(strBank);
			return;
		}
		if(iCtrlID == IDC_EDIT_ENDBANK){
			//エンドバンクにフォーカスがあればバンクを挿入
			pEditEndBank->SetWindowText(strBank);
			return;
		}
	}
	CEdit *pEdit11VerBank = (CEdit *)GetDlgItem(IDC_EDIT_11VERBANK);
	CEdit *pEditN1Bank = (CEdit *)GetDlgItem(IDC_EDIT_N1BANK);
	pEdit11VerBank->SetWindowText(strBank);
	pEditN1Bank->SetWindowText(strBank);

	//リストコントロールより選択されたテンプレートを表示
	CEdit *pEdit11VerTemp = (CEdit *)GetDlgItem(IDC_EDIT_11VERTEMP);
	pEdit11VerTemp->SetWindowText(strTemp);
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
void CH1EUSB_Tab1Dialog::EnabeleControl(DWORD dwMode, BOOL bEnable)
{
	CButton*	pBt11Verif = (CButton*)GetDlgItem(IDC_BUTTON_11VERIF);
	CButton*	pBt1NVerif = (CButton*)GetDlgItem(IDC_BUTTON_1NVERIF);
	CButton*	pBtAcVerif = (CButton*)GetDlgItem(IDC_BUTTON_VERIF_AB);
	CButton*	pBtWtVerif= (CButton*)GetDlgItem(IDC_BUTTON_VERIFWT);

	switch(dwMode){
		case SYSTEM_STARTUP:
		case SYSTEM_DEVICE_OFF:
		case SYSTEM_DEVICE_ERR:
			pBt11Verif->EnableWindow(FALSE);
			pBt1NVerif->EnableWindow(FALSE);
			pBtAcVerif->EnableWindow(FALSE);
			pBtWtVerif->EnableWindow(FALSE);
			break;
		case SYSTEM_DEVICE_ON:
		default:
			pBt11Verif->EnableWindow(bEnable);
			pBt1NVerif->EnableWindow(bEnable);
			pBtAcVerif->EnableWindow(bEnable);
			pBtWtVerif->EnableWindow(bEnable);
			break;
	}
}
