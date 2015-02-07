#pragma once
#include "H1EUSB_Com.h"
/********************************************************************************************/
/* CH1EUSB_Tab2Dialog�N���X�w�b�_															*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20080110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
// CH1EUSB_Tab2Dialog �_�C�A���O

class CH1EUSB_Tab2Dialog : public CDialog
{
	DECLARE_DYNAMIC(CH1EUSB_Tab2Dialog)

public:
	CH1EUSB_Tab2Dialog(CWnd* pParent = NULL);			//�W���R���X�g���N�^
	virtual ~CH1EUSB_Tab2Dialog();						//�f�X�g���N�^
	virtual BOOL OnInitDialog();						//�_�C�A���O������

	afx_msg void OnBnClickedButtonBuzon();				//�u�U�[���J�n�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonBuzoff();				//�u�U�[����~�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonLedgreen();			//�΂k�d�c�_���{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonLedred();				//�Ԃk�d�c�_���{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonLedgr();				//�΁��Ԃk�d�c�_���{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonLedoff();				//�S�k�d�c�����{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonSeclow();				//�Z�L�����e�B�ݒ�(Low)�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonSecmlow();			//�Z�L�����e�B�ݒ�(MidleLow)�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonSecmid();				//�Z�L�����e�B�ݒ�(Midle)�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonSecmidhg();			//�Z�L�����e�B�ݒ�(MidleHigh)�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonSechigh();			//�Z�L�����e�B�ݒ�(High)�{�^���������b�Z�[�W����

	void SetParentDialog(LPVOID pDlg);					//�e�_�C�A���O�|�C���^�Z�b�g
	void UpdateContorol();								//�R���g���[���X�V
	void EnabeleControl(DWORD dwMode, BOOL bEnable);	//�R���g���[���L���E����


// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_TAB2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:
	CFont m_ftFName;									//�t�H���g�I�u�W�F�N�g
	LPVOID m_pParetDlg;									//�e�_�C�A���O�̃|�C���^
	int OtherTransaction(UINT uiTransID, UINT uiMsgID);	//�g�����U�N�V�����������s
};
