// DlgTuch.cpp : 実装ファイル 
//

#include "stdafx.h"
#include "H2E_SampleApp.h"
#include "H2E_DlgTuch.h"


// CDlgTuch ダイアログ

IMPLEMENT_DYNAMIC(CDlgTuch, CDialog)

CDlgTuch::CDlgTuch(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTuch::IDD, pParent)
{
#ifndef _WIN32_WCE
	EnableActiveAccessibility();
#endif

}

CDlgTuch::~CDlgTuch()
{
}

void CDlgTuch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TUCH1, m_stTuch1);
	DDX_Control(pDX, IDC_STATIC_TUCH2, m_stTuch2);
}


BEGIN_MESSAGE_MAP(CDlgTuch, CDialog)
END_MESSAGE_MAP()



// CDlgTuch メッセージ ハンドラ


void CDlgTuch::SetTuchState(DWORD dwState)
{

	if(dwState & H2E_TOUCH_OUT1){
		m_stTuch1.ShowWindow(SW_SHOW);
	}
	else{
		m_stTuch1.ShowWindow(SW_HIDE);

	}
	if(dwState & H2E_TOUCH_OUT2){
		m_stTuch2.ShowWindow(SW_SHOW);
	}
	else{
		m_stTuch2.ShowWindow(SW_HIDE);

	}

}
