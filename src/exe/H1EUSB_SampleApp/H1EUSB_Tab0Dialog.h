#pragma once
#include "afxwin.h"
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
// CH1EUSB_Tab0Dialog ダイアログ

class CH1EUSB_Tab0Dialog : public CDialog
{
	DECLARE_DYNAMIC(CH1EUSB_Tab0Dialog)

public:
	CH1EUSB_Tab0Dialog(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CH1EUSB_Tab0Dialog();
	virtual BOOL OnInitDialog();								//ダイアログ初期化イベント処理
	afx_msg void OnBnClickedButtonEnroll();						//登録ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonCopy();						//テンプレートファイルコピーボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonDeltemp();					//テンプレート削除ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonDelbank();					//バンク削除ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonDelall();						//全テンプレート削除ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonSave();						//テンプレートファイル保存ボタン押下メッセージ処理
	afx_msg void OnBnClickedButtonOpen();						//テンプレートファイル開くボタン押下メッセージ処理
	afx_msg void OnTimer(UINT_PTR nIDEvent);					//タイマーイベント処理
	afx_msg void OnEnChangeEditEnrollBank();					//バンク番号変更メッセージ処理

	void SetParentDialog(LPVOID pDlg);							//親ダイアログのポインタセット
	void UpdateContorol();										//コントロール更新
	void UpdateNummber(CString strBank, CString strTemp);		//コントロールのバンク番号とテンプレート番号を更新する
	void EnabeleControl(DWORD dwMode, BOOL bEnable);			//コントロール有効・無効

// ダイアログ データ
	enum { IDD = IDD_DIALOG_TAB0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	int m_iTimeCount;											//テンプレートファイルコピー時のプログレス表示用のカウンタ
	CFont m_ftFName;											//フォントオブジェクト
	LPVOID	m_pParetDlg;										//親ダイアログのポインタ

	void SetAbleToCopyCount(BOOL bSetCopy);						//可能コピーバンク数セット							
	int GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode);	//汎用トランザクション処理
	void DoEvents();											//優先メッセージ処理実行
	void initEnroolEdit();										//登録するバンク番号とテンプレート番号をセットする
	void updateEnroolEdit(BOOL bReplace);						//バンク番号より、次回登録するテンプレート番号をセット
	BOOL m_bSetEnable;											//バンク番号更新許可フラグ


};
