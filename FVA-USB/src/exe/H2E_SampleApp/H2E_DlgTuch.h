#pragma once 
#include "afxwin.h"
#include "H2E.h"

// CDlgTuch ダイアログ

class CDlgTuch : public CDialog
{
	DECLARE_DYNAMIC(CDlgTuch)

public:
	CDlgTuch(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CDlgTuch();

	void SetTuchState(DWORD dwState);
	CStatic m_stTuch2;

// ダイアログ データ
	enum { IDD = IDD_TUCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_stTuch1;
};
