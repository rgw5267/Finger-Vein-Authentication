#pragma once
/********************************************************************************************/
/* CH1EUSB_Tab0Dialogクラスヘッダ															*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20080110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
// CH1EUSB_Tab１Dialog ダイアログ

class CH1EUSB_Tab1Dialog : public CDialog
{
	DECLARE_DYNAMIC(CH1EUSB_Tab1Dialog)

public:
	CH1EUSB_Tab1Dialog(CWnd* pParent = NULL);				//標準コンストラクタ
	virtual ~CH1EUSB_Tab1Dialog();							//デストラクタ
	virtual BOOL OnInitDialog();							//ダイアログ初期化イベント処理
	afx_msg void OnBnClickedButton11verif();				//認証ボタン押下（１：１）メッセージ処理
	afx_msg void OnBnClickedButton1nverif();				//バンク認証ボタン押下（１：Ｎ）メッセージ処理
	afx_msg void OnBnClickedButtonVerifAb();				//バンク間認証ボタン押下（１：Ｎ）メッセージ処理
	afx_msg void OnBnClickedButtonVerifwt();				//テンプレート付き認証ボタン押下（１：１）メッセージ処理

	void SetParentDialog(LPVOID pDlg);						//親ダイアログポインタセット
	void UpdateContorol();									//コントロール更新処理
	void UpdateNummber(CString strBank, CString strTemp);	//バンク番号、テンプレート番号更新処理
	void EnabeleControl(DWORD dwMode, BOOL bEnable);		//コントロール有効・無効


// ダイアログ データ
	enum { IDD = IDD_DIALOG_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:
	CFont	m_ftFName;										//フォントオブジェクト
	LPVOID	m_pParetDlg;									//親ダイアログのポインタ
	int GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode);	//汎用トランザクション
	void SetVerTempList();									//テンプレート指定認証（１：Ｎ）コンボボックスセット

};
