#pragma once
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
// CH1EUSB_Tab�PDialog �_�C�A���O

class CH1EUSB_Tab1Dialog : public CDialog
{
	DECLARE_DYNAMIC(CH1EUSB_Tab1Dialog)

public:
	CH1EUSB_Tab1Dialog(CWnd* pParent = NULL);				//�W���R���X�g���N�^
	virtual ~CH1EUSB_Tab1Dialog();							//�f�X�g���N�^
	virtual BOOL OnInitDialog();							//�_�C�A���O�������C�x���g����
	afx_msg void OnBnClickedButton11verif();				//�F�؃{�^�������i�P�F�P�j���b�Z�[�W����
	afx_msg void OnBnClickedButton1nverif();				//�o���N�F�؃{�^�������i�P�F�m�j���b�Z�[�W����
	afx_msg void OnBnClickedButtonVerifAb();				//�o���N�ԔF�؃{�^�������i�P�F�m�j���b�Z�[�W����
	afx_msg void OnBnClickedButtonVerifwt();				//�e���v���[�g�t���F�؃{�^�������i�P�F�P�j���b�Z�[�W����

	void SetParentDialog(LPVOID pDlg);						//�e�_�C�A���O�|�C���^�Z�b�g
	void UpdateContorol();									//�R���g���[���X�V����
	void UpdateNummber(CString strBank, CString strTemp);	//�o���N�ԍ��A�e���v���[�g�ԍ��X�V����
	void EnabeleControl(DWORD dwMode, BOOL bEnable);		//�R���g���[���L���E����


// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_TAB1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

private:
	CFont	m_ftFName;										//�t�H���g�I�u�W�F�N�g
	LPVOID	m_pParetDlg;									//�e�_�C�A���O�̃|�C���^
	int GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode);	//�ėp�g�����U�N�V����
	void SetVerTempList();									//�e���v���[�g�w��F�؁i�P�F�m�j�R���{�{�b�N�X�Z�b�g

};
