#pragma once
#include "afxwin.h"
#include "H2E.h"

// CDlgTuch �_�C�A���O

class CDlgTuch : public CDialog
{
	DECLARE_DYNAMIC(CDlgTuch)

public:
	CDlgTuch(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CDlgTuch();

	void SetTuchState(DWORD dwState);
	CStatic m_stTuch2;

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TUCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
private:
	CStatic m_stTuch1;
};
