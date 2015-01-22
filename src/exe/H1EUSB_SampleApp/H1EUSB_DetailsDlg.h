#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "H1EUSB_Tab0Dialog.h"
#include "H1EUSB_Tab1Dialog.h"
#include "H1EUSB_Tab2Dialog.h"
#include "H1EUSB_TemplateFile.h"
#include "H1EUSB_Com.h"
#include "Camellia.h"	// 暗号化で使用
#include <setupapi.h>		// getCOMPortNumber関数で使用
#include <regstr.h>			// getCOMPortNumber関数で使用(REGSTR_VAL_PORTNAME)
#pragma comment( lib, "setupapi.lib")	// getCOMPortNumber関数で使用
/********************************************************************************************/
/* CH1EUSB_DetailsDlgクラスヘッダ															*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20080110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
#define H1USB_COMMENT_NON		0
#define H1USB_COMMENT_ADD		1
#define H1USB_COMMENT_UOP		0x0080
#define H1USB_COMMENT_PLUS		0x8000

// CH1EUSB_DetailsDlg ダイアログ

class CH1EUSB_DetailsDlg : public CDialog
{
	DECLARE_DYNAMIC(CH1EUSB_DetailsDlg)

public:
	CH1EUSB_DetailsDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CH1EUSB_DetailsDlg();


	static int CALLBACK CompareFunc(LPARAM param1, LPARAM param2, LPARAM param3);
	void RenumberItem();


// ダイアログ データ
	enum { IDD = IDD_DIALOG_DETAILS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangeDtailsTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLvnItemchangedListTmplist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonMain();
	afx_msg void OnBnClickedButtonComset();

	int SyncDevice();															//テンプレート情報同期
	void SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError);				//コメント表示
	void UpdateContorol();														//コントロールアップデート
	DWORD m_dwSystemCondition;													//状態変数
	void EnabeleControl(DWORD dwMode, BOOL bEnable);							//コントロール有効・無効
	static LRESULT CALLBACK OnTuchNotification(DWORD dwStatus, LPVOID pObj);	//タッチセンサー状態通知	
	static LRESULT CALLBACK OnSendEndNotification(DWORD dwStatus, LPVOID pObj);	//送信終了通知	
	void DoEvents();															//優先メッセージ処理
	BOOL PollingTuchOnState();													//ポーリングタッチON
	int TuchEnable();															//タッチセンサー有効
	int TuchDisable();															//タッチセンサー無効
	
	CH1USBCom*		m_pH1USBCom;												//コミュニケーションクラスポインタ
	CTemplateFile*	m_pTemplateFile;											//テンプレートクラスポインタ

protected:
	virtual void OnOK();
	HICON m_hIcon;
	CTabCtrl m_tabCtrlDetails;													//タブコントロールクラスオブジェクト
	CH1EUSB_Tab0Dialog m_dlgTab0;												//Tab0パネルダイアログクラスオブジェクト
	CH1EUSB_Tab1Dialog m_dlgTab1;												//Tab1パネルダイアログクラスオブジェクト
	CH1EUSB_Tab2Dialog m_dlgTab2;												//Tab2パネルダイアログクラスオブジェクト
	CStatic m_stComGBox;														//共通エリアオブジェクト
	CStatic m_stFName;															//機能名表示エリアオブジェト
	CListCtrl m_listCtrlTempList;												//テンプレートリストリストコントロールオブジェクト

private:
	CFont m_ftFName;															//大フォント生成用
	CButton m_buMain;															//終了ボタン
	int ListInit();																//テンプレートリスト初期化
	int ListInsertItem();														//テンプレートリストアイテム追加
	int ControlInit();															//コントロールの初期化
	int ControlComportInit();													//COMポート選択コンボボックス初期化
	int ControlTabInit();														//タブ初期化
	int getCOMPortNumber(LPCTSTR pFName) ;										// H1E-USBが接続されているCOMポートの番号を返却
	int ControlTemplateInit();													//テンプレートリスト初期化
	void Sort();																//テンプレートリストソート
	int GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode);	//汎用トランザクション
	H1ECOMOBJEP	m_H1EComObjEP;													//コミュニケーションクラスエンドポイント情報

	CButton m_btCheckPol;
};
