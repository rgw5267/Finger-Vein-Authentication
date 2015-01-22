#pragma once
#include "H1EUSB_Com.h"
/********************************************************************************************/
/* CH1EUSB_Tab2Dialogクラスヘッダ															*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20080110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
// CH1EUSB_Tab2Dialog ダイアログ

class CH1EUSB_Tab2Dialog : public CDialog
{
	DECLARE_DYNAMIC(CH1EUSB_Tab2Dialog)

public:
	CH1EUSB_Tab2Dialog(CWnd* pParent = NULL);			//標準コンストラクタ
	virtual ~CH1EUSB_Tab2Dialog();						//デストラクタ
	virtual BOOL OnInitDialog();						//ダイアログ初期化

	afx_msg void OnBnClickedButtonBuzon();				//ブザー鳴動開始ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonBuzoff();				//ブザー鳴動停止ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonLedgreen();			//緑ＬＥＤ点灯ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonLedred();				//赤ＬＥＤ点灯ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonLedgr();				//緑＆赤ＬＥＤ点灯ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonLedoff();				//全ＬＥＤ消灯ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonSeclow();				//セキュリティ設定(Low)ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonSecmlow();			//セキュリティ設定(MidleLow)ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonSecmid();				//セキュリティ設定(Midle)ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonSecmidhg();			//セキュリティ設定(MidleHigh)ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonSechigh();			//セキュリティ設定(High)ボタン押下メッセージ処理

	void SetParentDialog(LPVOID pDlg);					//親ダイアログポインタセット
	void UpdateContorol();								//コントロール更新
	void EnabeleControl(DWORD dwMode, BOOL bEnable);	//コントロール有効・無効


// ダイアログ データ
	enum { IDD = IDD_DIALOG_TAB2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:
	CFont m_ftFName;									//フォントオブジェクト
	LPVOID m_pParetDlg;									//親ダイアログのポインタ
	int OtherTransaction(UINT uiTransID, UINT uiMsgID);	//トランザクション処理実行
};
