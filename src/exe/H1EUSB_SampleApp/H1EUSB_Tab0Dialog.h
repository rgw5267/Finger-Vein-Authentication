#pragma once
#include "afxwin.h"
/********************************************************************************************/
/* CH1EUSB_Tab0Dialog�N���X�w�b�_															*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20080110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
// CH1EUSB_Tab0Dialog �_�C�A���O

class CH1EUSB_Tab0Dialog : public CDialog
{
	DECLARE_DYNAMIC(CH1EUSB_Tab0Dialog)

public:
	CH1EUSB_Tab0Dialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CH1EUSB_Tab0Dialog();
	virtual BOOL OnInitDialog();								//�_�C�A���O�������C�x���g����
	afx_msg void OnBnClickedButtonEnroll();						//�o�^�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonCopy();						//�e���v���[�g�t�@�C���R�s�[�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonDeltemp();					//�e���v���[�g�폜�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonDelbank();					//�o���N�폜�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonDelall();						//�S�e���v���[�g�폜�{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonSave();						//�e���v���[�g�t�@�C���ۑ��{�^���������b�Z�[�W����
	afx_msg void OnBnClickedButtonOpen();						//�e���v���[�g�t�@�C���J���{�^���������b�Z�[�W����
	afx_msg void OnTimer(UINT_PTR nIDEvent);					//�^�C�}�[�C�x���g����
	afx_msg void OnEnChangeEditEnrollBank();					//�o���N�ԍ��ύX���b�Z�[�W����

	void SetParentDialog(LPVOID pDlg);							//�e�_�C�A���O�̃|�C���^�Z�b�g
	void UpdateContorol();										//�R���g���[���X�V
	void UpdateNummber(CString strBank, CString strTemp);		//�R���g���[���̃o���N�ԍ��ƃe���v���[�g�ԍ����X�V����
	void EnabeleControl(DWORD dwMode, BOOL bEnable);			//�R���g���[���L���E����

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_TAB0 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	int m_iTimeCount;											//�e���v���[�g�t�@�C���R�s�[���̃v���O���X�\���p�̃J�E���^
	CFont m_ftFName;											//�t�H���g�I�u�W�F�N�g
	LPVOID	m_pParetDlg;										//�e�_�C�A���O�̃|�C���^

	void SetAbleToCopyCount(BOOL bSetCopy);						//�\�R�s�[�o���N���Z�b�g							
	int GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode);	//�ėp�g�����U�N�V��������
	void DoEvents();											//�D�惁�b�Z�[�W�������s
	void initEnroolEdit();										//�o�^����o���N�ԍ��ƃe���v���[�g�ԍ����Z�b�g����
	void updateEnroolEdit(BOOL bReplace);						//�o���N�ԍ����A����o�^����e���v���[�g�ԍ����Z�b�g
	BOOL m_bSetEnable;											//�o���N�ԍ��X�V���t���O


};
