// H2E_SampleAppDlg.h : 
//

#pragma once
#include "afxwin.h"
#include "H2E_TemplateFile.h"
#include "H2E_MsgGuide.h"
#include "H2E_Number.h"
#include "H2E_Com.h"
#include "H2E_DlgTuch.h"

#define H2E_COMMENT_NON			0
#define H2E_COMMENT_ADD			1
#define H2E_COMMENT_UOP			0x0080
#define H2E_COMMENT_PLUS		0x8000

// CH2E_SampleAppDlg dialog
class CH2E_SampleAppDlg : public CDialog
{
public:
	CH2E_SampleAppDlg(CWnd* pParent = NULL);
	virtual ~CH2E_SampleAppDlg();
	DWORD m_dwSystemCondition;

	enum { IDD = IDD_H2E_SAMPLEAPP_DIALOG };
	afx_msg void OnBnClickedButton1nverif();
	afx_msg void OnBnClickedButtonComopen();
	afx_msg void OnBnClickedButtonEnroll();
	afx_msg void OnBnClickedButton11verif();
	afx_msg void OnBnClickedButtonVerifAb();
	afx_msg void OnBnClickedButtonUpload();
	afx_msg void OnBnClickedButtonDownload();
	afx_msg void OnBnClickedButtonDeltemp();
	afx_msg void OnBnClickedButtonDelgroup();
	afx_msg void OnBnClickedButtonDelall();
	afx_msg void OnBnClickedButtonCheck();
	afx_msg void OnBnClickedButtonEnd();
	afx_msg void OnLvnItemchangedListTmplist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditEnrollBank();

	static int CALLBACK CompareFuncTemplateListControl(LPARAM param1, LPARAM param2, LPARAM param3);
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	HICON m_hIcon;
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void initDialogControl();
	void initComComboBox();
	void initSpeedComboBox();
	void initTemplateListControl();
	int OpenTuchSensorPanel();
	int WaitTuchSensorState();
	int CloseTuchSensorPanel();
	int formatTemplateListControl();
	int insertTenmplateListControl();
	void renumberItemTemplateListControl();
	void sortTemplateListControl();
	int GeneralTransaction(UINT uiTransID, BYTE byParam, LPVOID pOption,  WORD wOptionLen, UINT uiMsgID, UINT uiMode);
	void UpdateNummber(CString strBank, CString strTemp);
	void SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError);
	void EnabeleControl(DWORD dwMode, BOOL bEnable);
	void UpdateContorol();
	void DoEvents();
	void initEnroolEdit();
	void updateEnroolEdit(BOOL bReplace);
	int enrollOpelation(UINT uiGroup, UINT uiTemplate, UINT uiTimes, BOOL bCheck);

	CTemplateFile*	m_pTemplateFile;
	CH2ECom* m_pH2ECom;
	TEMPINFO	m_tempInfo;
	BOOL m_bSetEnable;
	CButton m_btnCheckTuchOut1;
	CButton m_btnCheckTuchOut2;
	CDlgTuch* m_pDlgTuch;
	CFont m_ftFName;

};
