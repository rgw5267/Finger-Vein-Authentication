#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "H1EUSB_Tab0Dialog.h"
#include "H1EUSB_Tab1Dialog.h"
#include "H1EUSB_Tab2Dialog.h"
#include "H1EUSB_TemplateFile.h"
#include "H1EUSB_Com.h"
#include "Camellia.h"	// �Í����Ŏg�p
#include <setupapi.h>		// getCOMPortNumber�֐��Ŏg�p
#include <regstr.h>			// getCOMPortNumber�֐��Ŏg�p(REGSTR_VAL_PORTNAME)
#pragma comment( lib, "setupapi.lib")	// getCOMPortNumber�֐��Ŏg�p
/********************************************************************************************/
/* CH1EUSB_DetailsDlg�N���X�w�b�_															*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20080110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
#define H1USB_COMMENT_NON		0
#define H1USB_COMMENT_ADD		1
#define H1USB_COMMENT_UOP		0x0080
#define H1USB_COMMENT_PLUS		0x8000

// CH1EUSB_DetailsDlg �_�C�A���O

class CH1EUSB_DetailsDlg : public CDialog
{
	DECLARE_DYNAMIC(CH1EUSB_DetailsDlg)

public:
	CH1EUSB_DetailsDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CH1EUSB_DetailsDlg();


	static int CALLBACK CompareFunc(LPARAM param1, LPARAM param2, LPARAM param3);
	void RenumberItem();


// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_DETAILS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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

	int SyncDevice();															//�e���v���[�g��񓯊�
	void SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError);				//�R�����g�\��
	void UpdateContorol();														//�R���g���[���A�b�v�f�[�g
	DWORD m_dwSystemCondition;													//��ԕϐ�
	void EnabeleControl(DWORD dwMode, BOOL bEnable);							//�R���g���[���L���E����
	static LRESULT CALLBACK OnTuchNotification(DWORD dwStatus, LPVOID pObj);	//�^�b�`�Z���T�[��Ԓʒm	
	static LRESULT CALLBACK OnSendEndNotification(DWORD dwStatus, LPVOID pObj);	//���M�I���ʒm	
	void DoEvents();															//�D�惁�b�Z�[�W����
	BOOL PollingTuchOnState();													//�|�[�����O�^�b�`ON
	int TuchEnable();															//�^�b�`�Z���T�[�L��
	int TuchDisable();															//�^�b�`�Z���T�[����
	
	CH1USBCom*		m_pH1USBCom;												//�R�~���j�P�[�V�����N���X�|�C���^
	CTemplateFile*	m_pTemplateFile;											//�e���v���[�g�N���X�|�C���^

protected:
	virtual void OnOK();
	HICON m_hIcon;
	CTabCtrl m_tabCtrlDetails;													//�^�u�R���g���[���N���X�I�u�W�F�N�g
	CH1EUSB_Tab0Dialog m_dlgTab0;												//Tab0�p�l���_�C�A���O�N���X�I�u�W�F�N�g
	CH1EUSB_Tab1Dialog m_dlgTab1;												//Tab1�p�l���_�C�A���O�N���X�I�u�W�F�N�g
	CH1EUSB_Tab2Dialog m_dlgTab2;												//Tab2�p�l���_�C�A���O�N���X�I�u�W�F�N�g
	CStatic m_stComGBox;														//���ʃG���A�I�u�W�F�N�g
	CStatic m_stFName;															//�@�\���\���G���A�I�u�W�F�g
	CListCtrl m_listCtrlTempList;												//�e���v���[�g���X�g���X�g�R���g���[���I�u�W�F�N�g

private:
	CFont m_ftFName;															//��t�H���g�����p
	CButton m_buMain;															//�I���{�^��
	int ListInit();																//�e���v���[�g���X�g������
	int ListInsertItem();														//�e���v���[�g���X�g�A�C�e���ǉ�
	int ControlInit();															//�R���g���[���̏�����
	int ControlComportInit();													//COM�|�[�g�I���R���{�{�b�N�X������
	int ControlTabInit();														//�^�u������
	int getCOMPortNumber(LPCTSTR pFName) ;										// H1E-USB���ڑ�����Ă���COM�|�[�g�̔ԍ���ԋp
	int ControlTemplateInit();													//�e���v���[�g���X�g������
	void Sort();																//�e���v���[�g���X�g�\�[�g
	int GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode);	//�ėp�g�����U�N�V����
	H1ECOMOBJEP	m_H1EComObjEP;													//�R�~���j�P�[�V�����N���X�G���h�|�C���g���

	CButton m_btCheckPol;
};
