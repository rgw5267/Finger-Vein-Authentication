// H1EUSB_Tab0Dialog.cpp : �����t�@�C��
//
#include "stdafx.h"
#include "H1EUSB_SampleApp.h"
#include "H1EUSB_Tab0Dialog.h"
#include "H1EUSB_DetailsDlg.h"
#include "H1EUSB_TemplateFile.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_Number.h"
#include "H1EUSB_MsgGuide.h"
/********************************************************************************************/
/* CH1EUSB_Tab0Dialog�N���X�v���O����														*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20120110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/********************************************************************************************/

// CH1EUSB_Tab0Dialog �_�C�A���O

IMPLEMENT_DYNAMIC(CH1EUSB_Tab0Dialog, CDialog)
/********************************************************************************************/
/*[���\�b�h��] CH1EUSB_Tab0Dialog�N���X�̃R���X�g���N�^										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CH1EUSB_Tab0Dialog(CWnd* pParent)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CH1EUSB_Tab0Dialog::CH1EUSB_Tab0Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(CH1EUSB_Tab0Dialog::IDD, pParent)
{
	//�����o�[�ϐ��̏�����
	m_pParetDlg = 0;			//�e�_�C�A���O�̃I�u�W�F�N�g
	m_iTimeCount = 0;			//�R�s�[�t�@�C�����̃v���O���X�\���p�̃J�E���^
	m_bSetEnable = FALSE;		//�G�f�B�b�g�{�b�N�X�̃`�F���W�C�x���g�}�X�N�p�t���O

}

/********************************************************************************************/
/*[���\�b�h��] CH1EUSB_Tab0Dialog�N���X�̃f�X�g���N�^										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		~CH1EUSB_DetailsDlg()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CH1EUSB_Tab0Dialog::~CH1EUSB_Tab0Dialog()
{


}

void CH1EUSB_Tab0Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CH1EUSB_Tab0Dialog, CDialog)

	ON_BN_CLICKED(IDC_BUTTON_ENROLL, &CH1EUSB_Tab0Dialog::OnBnClickedButtonEnroll)
	ON_BN_CLICKED(IDC_BUTTON_COPY, &CH1EUSB_Tab0Dialog::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_DELTEMP, &CH1EUSB_Tab0Dialog::OnBnClickedButtonDeltemp)
	ON_BN_CLICKED(IDC_BUTTON_DELBANK, &CH1EUSB_Tab0Dialog::OnBnClickedButtonDelbank)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CH1EUSB_Tab0Dialog::OnBnClickedButtonDelall)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CH1EUSB_Tab0Dialog::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CH1EUSB_Tab0Dialog::OnBnClickedButtonOpen)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_ENROLL_BANK, &CH1EUSB_Tab0Dialog::OnEnChangeEditEnrollBank)
END_MESSAGE_MAP()


// CH1EUSB_Tab0Dialog ���b�Z�[�W �n���h��


void CH1EUSB_Tab0Dialog::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//CDialog::OnOK();
}
/********************************************************************************************/
/*[���\�b�h��] �_�C�A���O�̏�����															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		BOOL OnInitDialog()															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�{�_�C�A���O�̏��������s��													*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		BOOL TRUE orFALSE															*/
/********************************************************************************************/
BOOL CH1EUSB_Tab0Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������


	//�@�\�����̃t�H���g�T�C�Y��ύX����
	m_ftFName.CreateFont( 14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,NULL); 
	GetDlgItem(IDC_STATIC_ENROLL)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_COPYTEMP)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_DELETETMP)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_DELETEBANK)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_DELETEALL)->SetFont(&m_ftFName);

	//�e���v���[�g�R�s�[�\���̃Z�b�g
	SetAbleToCopyCount(TRUE);

	//�e���v���[�g�o�^�̃o���N�ԍ��ƃe���v���[�g�ԍ����Z�b�g
	initEnroolEdit();

	//�o�^�o���N�ԍ��G�f�B�b�g�̃}�X�N�t���O���Z�b�g����
	m_bSetEnable = TRUE;


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/********************************************************************************************/
/*[���\�b�h��] �e�_�C�A���O�̃|�C���^�[�Z�b�g												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void SetParentDialog(LPVOID pDlg)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e�_�C�A���O�̃|�C���^�������o�[�ϐ��Ɋi�[����								*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::SetParentDialog(LPVOID pDlg)
{
	//�e�_�C�A���O�̃|�C���^�[�������o�[�ϐ��ɃZ�b�g
	m_pParetDlg  = pDlg;
}


/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�o�^�{�^���������b�Z�[�W����										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonEnroll()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�Z�b�g���ꂽ�o���N�ԍ��ƃe���v���[�g�ԍ��ɂ��o�^�������s��				*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonEnroll()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	TEMPU tempu;
	UINT uiBank;
	UINT uiTemp;
	CString strBank;
	CString strTmpNo;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;


	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;

	if(pTpf->m_uiTotalTemplates >= MAX_TEMPLATE_TOTAL){
		//�o�^�ς݂̃e���v���[�g���Q�O�O�O�{����B
		strMessage = msgGuide.TemplateFullMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}

	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//�o���N�ԍ��̓��͒l���
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_TMPNO);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		//�e���v���[�g�ԍ��̓��͒l���
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}

	pDlg->EnabeleControl(NULL, FALSE);
	//�w��u���Ă��������K�C�_���X�\��
	strMessage = msgGuide.GuidePlaceFingerFormatMessage();
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);

	//�^�b�`�Z���T�[�|�[�����O
	if(!pDlg->PollingTuchOnState()){
		strMessage.FormatMessage(IDS_POLLING_TIMEOUT);
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	pDlg->TuchDisable();

	//�o�^�g�����U�N�V�������s
	iReturn = GeneralTransaction(H1USB_COM_GETTEMPLATE, 0, 0,  MSGGUID_SEND_GETTEMP, H1USB_COMMENT_UOP);
	if(iReturn != H1USB_OK){
		strMessage = msgGuide.EnrollNGMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->EnabeleControl(NULL, TRUE);
		pDlg->TuchEnable();
		return;
	}

	//�R���g���[����L���ɂ���
	pDlg->EnabeleControl(NULL, TRUE);

	//��M�����e���v���[�g���擾
	iReturn = pCom->GetTempU(&tempu);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}
	tempu.uiTmpNum = uiTemp;

	//�e���v���[�g�t�@�C���֒ǉ�
	iReturn = pTpf->AddTemplate(uiBank, &tempu);
	if(iReturn != H1USB_OK){
		//�e���v���[�g�t�@�C���ւ̒ǉ��Ɏ��s����
		pDlg->TuchEnable();
		return;
	}

	//�R���g���[�����X�V����
	pDlg->UpdateContorol();

	//�w�肳�ꂽ�o���N�ɐ؂�ւ���
	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, (WPARAM)uiBank, 0);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}

	iReturn = pCom->SetTempU(&tempu, 1, TRUE);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}
	//�e���v���[�g���g�P�t�r�a�ɑ��M����
	iReturn = pCom->TransToDevice(H1USB_COM_SETTEMPLATE, 0, 0);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}

	//�o�^�I�����b�Z�[�W�\��
	strMessage = msgGuide.EnrollFormatMessage(uiBank, uiTemp,iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�R�s�[�{�^���������b�Z�[�W����									*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonCopy()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�Z�b�g���ꂽ�o���N�ԍ��ƃe���v���[�g�ԍ��ɂ��o�^�������s��				*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonCopy()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CNumber nb;
	CString strAbleBanks;
	CString strCopyBanks;
	int iAbleBanks;
	int iCopyBanks;
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;

	
	iReturn =H1USB_OK;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	CEdit *pAbleBanks = (CEdit *)GetDlgItem(IDC_EDIT_COPYBANK);
	pAbleBanks->GetWindowText(strAbleBanks);
	iAbleBanks = _ttoi(strAbleBanks);
	if(iAbleBanks == 0){
		//�o���N0000h�̃e���v���[�g�����݂��Ȃ�
		strMessage = msgGuide.GuideNotExistBank0000FormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pACopyBanks = (CEdit *)GetDlgItem(IDC_EDIT_COPYCOUNT);
	pACopyBanks->GetWindowText(strCopyBanks);
	iCopyBanks = _ttoi(strCopyBanks);
	if(iCopyBanks > iAbleBanks ){
		//�R�s�[�����R�s�[�\���𒴂��Ă���
		strMessage = msgGuide.GuideCopyNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}

	pDlg->TuchDisable();

	//�R���g���[������
	pDlg->EnabeleControl(NULL, FALSE);
	strMessage = msgGuide.CopyFileFormatMessage();
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);

	//�e���v���[�g�t�@�C���R�s�[���̃v���O���X�\���̂��߃^�C�}�X�^�[�g
	m_iTimeCount = 0;
	SetTimer(1, 500,NULL);
	
	//�e���v���[�g�t�@�C�����R�s�[�����R�s�[����
	pTpf->CopyBank0000(iCopyBanks);
	//�R���g���[�����X�V����
	pDlg->UpdateContorol();
	//�^�C�}�[��~
	KillTimer(1);

	//�f�o�C�X�Ɠ�������
	iReturn = pDlg->SyncDevice();

	//�I�����b�Z�[�W�\��
	strMessage = msgGuide.CompleteCopyFormatMessage(iCopyBanks, iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	pDlg->EnabeleControl(NULL, TRUE);

	pDlg->TuchEnable();

}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�폜�{�^���������b�Z�[�W����										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonDeltemp()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�Z�b�g���ꂽ�o���N�ԍ��ƃe���v���[�g�ԍ��ɂ��e���v���[�g���폜����		*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonDeltemp()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	UINT uiBank;
	UINT uiTemp;
	CString strBank;
	CString strTmpNo;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//�o���N�ԍ��̓��͒l���
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_TEMP);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		//�e���v���[�g�ԍ��̓��͒l���
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	//�w�߂��ꂽ�o���N�̃e���v���[�g���폜����
	iReturn = pTpf->DeleteOneTemplate(uiBank, uiTemp);
	if(iReturn != H1USB_OK){

	}

	pDlg->TuchDisable();


	//�R���g���[�����X�V����
	pDlg->UpdateContorol();

	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	//�w�肳�ꂽ�o���N�ɐ؂�ւ���
	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, (WPARAM)uiBank, 0);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}



	//�R���g���[������
	pDlg->EnabeleControl(NULL, FALSE);

	//�w�肳�ꂽ�e���v���[�g���폜����
	iReturn = pCom->TransToDevice(H1USB_COM_DELTEMPLATE, uiTemp, 0);

	//�I�����b�Z�[�W�\��
	strMessage = msgGuide.CompleteDelFormatMessage(uiBank, uiTemp, iReturn , 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);

	//�R���g���[����L��
	pDlg->EnabeleControl(NULL, TRUE);

	pDlg->TuchEnable();


}

/********************************************************************************************/
/*[���\�b�h��] �o���N�폜�{�^���������b�Z�[�W����											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonDelbank()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�Z�b�g���ꂽ�o���N�ԍ��ɂ��e���v���[�g���폜����							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonDelbank()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	CNumber nb;
	UINT uiBank;
	CString strBank;
	CMsgGuide msgGuide;
	CString strMessage;

	iReturn =H1USB_OK;
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_DELBANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//�o���N�ԍ��̓��͒l���
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}


	pDlg->TuchDisable();


	//�w�肳�ꂽ�o���N�̃e���v���[�g�t�@�C�����폜
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	iReturn = pTpf->DeleteBank(uiBank);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}
	//�R���g���[�����X�V����
	pDlg->UpdateContorol();

	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	//�w�肳�ꂽ�o���N�ɐ؂�ւ���
	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, (WPARAM)uiBank, 0);
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}

	pDlg->EnabeleControl(NULL, FALSE);
	//�f�o�C�X����w�肳�ꂽ�o���N�̃e���v���[�g���폜����
	iReturn = pCom->TransToDevice(H1USB_COM_DELTEMPLATE, -1, 0);
	strMessage = msgGuide.CompleteDelBankFormatMessage(uiBank, iReturn , 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	pDlg->EnabeleControl(NULL, TRUE);

	pDlg->TuchEnable();

}

/********************************************************************************************/
/*[���\�b�h��] �S�e���v���[�g�폜�{�^���������b�Z�[�W����									*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonDelall()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		���ׂẴe���v���[�g���폜����												*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonDelall()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	
	iReturn =H1USB_OK;

	//�m�F���b�Z�[�W�\��
	iReturn = AfxMessageBox(IDS_GUIDE_DELALL,MB_OKCANCEL|MB_ICONINFORMATION);
	if(iReturn != IDOK){
		return;
	}

	
	//�e���v���[�g�t�@�C�������ׂč폜����
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	pDlg->TuchDisable();


	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	iReturn = pTpf->DeleteAll();
	if(iReturn != H1USB_OK){
		pDlg->TuchEnable();
		return;
	}

	//�R���g���[�����X�V����
	pDlg->UpdateContorol();

	pDlg->EnabeleControl(NULL, FALSE);
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	//�f�o�C�X�̂��ׂẴe���v���[�g���폜����
	iReturn = pCom->TransToDevice(H1USB_COM_DELTEMPLATE, -2, 0);

	//�I�����b�Z�[�W�\��
	strMessage = msgGuide.CompleteDelAllFormatMessage(iReturn , 0);
	pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	pDlg->EnabeleControl(NULL, TRUE);
	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[���\�b�h��] �ۑ��{�^���������b�Z�[�W����													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonSave()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�C�ӂ̃t�H���_�[�Ƀe���v���[�g�t�@�C�����i�[����							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonSave()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	
	iReturn =H1USB_OK;
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	pDlg->TuchDisable();

	CTemplateFile* pTpf = pDlg->m_pTemplateFile;

	//�e���v���[�g��ۑ�����
	iReturn = pTpf->SaveTemplates(this->m_hWnd);

	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[���\�b�h��] �I�[�v���{�^���������b�Z�[�W����												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonOpen()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�i�[����Ă���e���v���[�g�t�@�C�������[�h���A�f�o�C�X�Ɠ�������			*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnBnClickedButtonOpen()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	pDlg->TuchDisable();


	//�R���g���[���𖳌��ɂ���
	pDlg->EnabeleControl(NULL, FALSE);

	//�e���v���[�g�t�@�C�����I�[�v�����ă��[�h����
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	iReturn = pTpf->OpenTemplates(this->m_hWnd);
	if(iReturn==H1USB_OK){
		//�f�o�C�X�Ɠ�������
		pDlg->SyncDevice();
	}
	//�R���g���[�����X�V����
	pDlg->UpdateContorol();	

	//�R���g���[����L���ɂ���
	pDlg->EnabeleControl(NULL, TRUE);

	pDlg->TuchEnable();

}

/********************************************************************************************/
/*[���\�b�h��]	�ėp�g�����U�N�V��������													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,		*/ 
/*															UINT uiMsgID, UINT uiMode)		*/
/*																							*/
/*[���e]		�w��̃g�����U�N�V�������������{���A�Ή�����R�����g��\������				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_Tab0Dialog::GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode)
{
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	pDlg->EnabeleControl(NULL, FALSE);
	if((uiMsgID != MSGGUID_NODISP) && (!(uiMode & H1USB_COMMENT_UOP))){	
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//�R�}���h���M���b�Z�[�W
			pDlg->SetComment(uiMode, strMessage, 0);
		}
	}
	//�g�����U�N�V�������s
	iReturn = pCom->TransToDevice(uiTransID, wParam, lParam);
	if(uiMode & H1USB_COMMENT_UOP){
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//�R�}���h���M���b�Z�[�W
			pDlg->SetComment(uiMode, strMessage, 0);
		}
	}
	//�R�}���h�I�����b�Z�[�W
	if(iReturn < H1USB_OK){
		//strMessage.Format(_T("Error:%d"), iReturn);
		strMessage = msgGuide.ErrorGeneralFormatMessage(iReturn);
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	}
	pDlg->EnabeleControl(NULL, TRUE);
	
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��]	�R�s�[�\�����擾����														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void SetAbleToCopyCount(BOOL bSetCopy)										*/ 
/*																							*/
/*[���e]		0000�o���N�̃e���v���[�g�����A�R�s�[�\�ȃo���N�����擾����				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::SetAbleToCopyCount(BOOL bSetCopy)
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;

	UINT uiAbles;
	uiAbles = 0;
	UINT ui;
	for(ui=0; pTpf->m_uiBanks>ui; ui++){
		if(pTpf->m_pBankInfo[ui].uiBankNum == 0x0000){
			//0x0000�o���N�����݂���
			if(pTpf->m_pBankInfo[ui].uiCounts > 0){
				uiAbles = 2000/pTpf->m_pBankInfo[ui].uiCounts - 1;
				break;
			}
		}
	}
	CString strWork;
	strWork.Format(_T("%d"), uiAbles);
	//�\�R�s�[�����G�f�B�b�g�{�b�N�X�ɐݒ肷��
	CEdit *pAbleBanks = (CEdit *)GetDlgItem(IDC_EDIT_COPYBANK);
	pAbleBanks->SetWindowText(strWork);
	if(bSetCopy){
		//�\�R�s�[���������l�Ƃ��ăG�f�B�b�g�{�b�N�X�ɐݒ肷��
		CEdit *pCopyBanks = (CEdit *)GetDlgItem(IDC_EDIT_COPYCOUNT);
		pCopyBanks->SetWindowText(strWork);
	}
}

/********************************************************************************************/
/*[���\�b�h��]	�R���g���[���X�V															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void UpdateContorol()														*/ 
/*																							*/
/*[���e]		�{�_�C�A���O�̃R���g���[���X�V����											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::UpdateContorol()
{
	SetAbleToCopyCount(TRUE);
	updateEnroolEdit(TRUE);

}

/********************************************************************************************/
/*[���\�b�h��]	�o���N�ԍ��A�e���v���[�g�ԍ�												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void UpdateNummber(CString strBank, CString strTemp)						*/ 
/*																							*/
/*[���e]		�w�肳�ꂽ�o���N�ԍ��ƃe���v���[�g�ԍ����e�R���g���[���ɃZ�b�g����			*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::UpdateNummber(CString strBank, CString strTemp)
{
	//���X�g�R���g���[�����I�����ꂽ�o���N��\��
	CEdit *pEditDelOneBank = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_BANK);
	CEdit *pEditeBank = (CEdit *)GetDlgItem(IDC_EDIT_DELBANK);
	pEditDelOneBank->SetWindowText(strBank);
	pEditeBank->SetWindowText(strBank);

	//���X�g�R���g���[�����I�����ꂽ�e���v���[�g��\��
	CEdit *pEditDelOneTemp = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_TEMP);
	pEditDelOneTemp->SetWindowText(strTemp);

}
/********************************************************************************************/
/*[���\�b�h��]	�R���g���[���̗L���E����													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void EnabeleControl(DWORD dwMode, BOOL bEnable)								*/
/*																							*/
/*[���e]		�e�R���g���[���̗L���E������ݒ肷��										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::EnabeleControl(DWORD dwMode, BOOL bEnable)
{
	CButton*	pBtEnroll = (CButton*)GetDlgItem(IDC_BUTTON_ENROLL);
	CButton*	pBtCopy = (CButton*)GetDlgItem(IDC_BUTTON_COPY);
	CButton*	pBtDelTemp = (CButton*)GetDlgItem(IDC_BUTTON_DELTEMP);
	CButton*	pBtDelBank= (CButton*)GetDlgItem(IDC_BUTTON_DELBANK);
	CButton*	pBtDelAll = (CButton*)GetDlgItem(IDC_BUTTON_DELALL);
	CButton*	pBtDelSave= (CButton*)GetDlgItem(IDC_BUTTON_SAVE);
	CButton*	pBtDelOpen = (CButton*)GetDlgItem(IDC_BUTTON_OPEN);
	switch(dwMode){
		//�X�^�[�g�A�b�v���
		case SYSTEM_STARTUP:
			pBtEnroll->EnableWindow(FALSE);
			pBtCopy->EnableWindow(FALSE);
			pBtDelTemp->EnableWindow(FALSE);
			pBtDelBank->EnableWindow(FALSE);
			pBtDelAll->EnableWindow(FALSE);
			pBtDelSave->EnableWindow(FALSE);
			pBtDelOpen->EnableWindow(FALSE);
			break;
		//�f�o�C�XOFF�A�G���[���
		case SYSTEM_DEVICE_OFF:
		case SYSTEM_DEVICE_ERR:
			pBtEnroll->EnableWindow(FALSE);
			pBtCopy->EnableWindow(FALSE);
			pBtDelTemp->EnableWindow(FALSE);
			pBtDelBank->EnableWindow(FALSE);
			pBtDelAll->EnableWindow(FALSE);
			pBtDelSave->EnableWindow(bEnable);
			pBtDelOpen->EnableWindow(FALSE);
			break;
		//�f�o�C�XON���
		case SYSTEM_DEVICE_ON:
		default:
			pBtEnroll->EnableWindow(bEnable);
			pBtCopy->EnableWindow(bEnable);
			pBtDelTemp->EnableWindow(bEnable);
			pBtDelBank->EnableWindow(bEnable);
			pBtDelAll->EnableWindow(bEnable);
			pBtDelSave->EnableWindow(bEnable);
			pBtDelOpen->EnableWindow(bEnable);
			break;
	}
}

/********************************************************************************************/
/*[���\�b�h��]	�^�C�}�C�x���g����															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnTimer(UINT_PTR nIDEvent)												*/
/*																							*/
/*[���e]		�e���v���[�g�t�@�C���R�s�[���̃v���O���X�\��								*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if(nIDEvent == 1){
		CMsgGuide msgGuide;
		CString strMessage;
		CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
		m_iTimeCount++;
		if(m_iTimeCount > 5){
			m_iTimeCount = 0;
			strMessage = msgGuide.CopyFileFormatMessage();
			pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		}
		else{
			strMessage = msgGuide.CopyFileFormatMessage();
			pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

/********************************************************************************************/
/*[���\�b�h��]	���b�Z�[�W�D����s															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void DoEvents()																*/
/*																							*/
/*[���e]		�L���[�ɗ��܂��Ă��郁�b�Z�[�W��D��I�Ɏ��s������							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){
		if (!AfxGetThread()->PumpMessage()){
			return;
		}
	}
}

/********************************************************************************************/
/*[���\�b�h��]	�e���v���[�g�o�^�o���N�ԍ��A�e���v���[�g�ԍ���������						*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void initEnroolEdit()														*/
/*																							*/
/*[���e]		�{�N���X�N�����̃e���v���[�g�o�^�o���N�ԍ��A�e���v���[�g�ԍ����Z�b�g����	*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::initEnroolEdit()
{
	int iTemps;
	int	i;
	CString strGroup;
	CString strtemp;
	WORD wGroup = 0;
	WORD wTemp = 0;
	WORDDWORD wdw;

	DWORD dwTarget = 0x000000000;
	DWORD dwWork;

	CEdit* pEdiGroupt;
	pEdiGroupt = (CEdit*)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	CEdit* pEditTemp = (CEdit*)GetDlgItem(IDC_EDIT_ENROLL_TMPNO);

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CListCtrl* pLCtrl = (CListCtrl*)pDlg->GetDlgItem(IDC_LIST_TMPLIST) ;
	//�e���v���[�g���X�g���A�o�^�����擾
	iTemps = pLCtrl->GetItemCount();
	if(iTemps > 0){
		for(i=0; i<iTemps; i++ ){
			strGroup = pLCtrl->GetItemText(i, 0);
			strtemp = pLCtrl->GetItemText(i, 1);
			wGroup = (WORD)_tcstol(strGroup,NULL,16);
			wTemp = (WORD)_tcstol(strtemp,NULL,16);
			dwWork = wGroup*0x10000 + wTemp;
			if((dwWork-dwTarget) > 1){
				//���̃e���v���[�g�ԍ��Ɍ��Ԃ��Ȃ�
				break;
			}
			dwTarget = dwWork;
		}
		dwTarget++;
		wdw.dword = dwTarget;
		if(wdw.word[0] > 0xFFFF){
			//�o���N�ԍ����ő�l�𒴂�����O�ɖ߂�
			wdw.word[0] = 0;
			wdw.word[1]++;
		}
		if(wdw.word[1] > 0xFF){
			//�e���v���[�g�ԍ����ő�l�𒴂�����O�ɖ߂�
			wdw.dword = 0;
		}
	}
	wdw.dword = dwTarget;

	//�o���N�ԍ��ƃe���v���[�g�ԍ����Z�b�g����
	strGroup.Format(_T("%04X"), wdw.word[1]);
	strtemp.Format(_T("%02X"), wdw.word[0]);
	pEdiGroupt->SetWindowText(strGroup);
	pEditTemp->SetWindowText(strtemp);

}

/********************************************************************************************/
/*[���\�b�h��]	�e���v���[�g�o�^�o���N�ԍ��A�e���v���[�g�ԍ��X�V����						*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void updateEnroolEdit()														*/
/*																							*/
/*[���e]		�{�N���X�N�����̃e���v���[�g�o�^�o���N�ԍ��A�e���v���[�g�ԍ����X�V����		*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::updateEnroolEdit(BOOL bReplace)
{
	int iTemps;
	int	i;
	CString strGroup;
	CString strtemp;
	WORD wGroup = 0;
	WORD wTemp = 0;

	WORDDWORD wdwTarget;

	CEdit* pEdiGroupt;
	pEdiGroupt = (CEdit*)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	CEdit* pEditTemp = (CEdit*)GetDlgItem(IDC_EDIT_ENROLL_TMPNO);
	//���݃Z�b�g���ꂽ�o���N�ԍ����擾
	pEdiGroupt->GetWindowText(strGroup);
	if(strGroup.IsEmpty()){
		//���͂���Ă��Ȃ���΂O���Z�b�g
		wdwTarget.word[1] = 0;
	}
	else{
		wdwTarget.word[1] = (WORD)_tcstol(strGroup,NULL,16);
	}
	wdwTarget.word[0] = 0;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CListCtrl* pLCtrl = (CListCtrl*)pDlg->GetDlgItem(IDC_LIST_TMPLIST) ;
	//�e���v���[�g���X�g���A�e���v���[�g�̓o�^�����擾����
	iTemps = pLCtrl->GetItemCount();
	if(iTemps > 0){
		for(i=0; i<iTemps; i++ ){
			strGroup = pLCtrl->GetItemText(i, 0);
			wGroup = (WORD)_tcstol(strGroup,NULL,16);
			if(wdwTarget.word[1] < wGroup){
				break;
			}
			if(wdwTarget.word[1] == wGroup){
				strtemp = pLCtrl->GetItemText(i, 1);
				wTemp = (WORD)_tcstol(strtemp,NULL,16);
				if(wdwTarget.word[0] == wTemp){
					wdwTarget.dword++;
				}
				else{
					break;
				}
			}
		}
	}
	//�o���N�ԍ��ƃe���v���[�g�ԍ����X�V����
	strGroup.Format(_T("%04X"), wdwTarget.word[1]);
	strtemp.Format(_T("%02X"), wdwTarget.word[0]);
	m_bSetEnable = FALSE;
	if(bReplace){
		//���͒��̓o���N�ԍ��̍X�V�͍s��Ȃ�
		pEdiGroupt->SetWindowText(strGroup);
	}
	pEditTemp->SetWindowText(strtemp);
	m_bSetEnable = TRUE;
	

}

/********************************************************************************************/
/*[���\�b�h��]	�o�^�o���N�`�F���W���b�Z�[�W����											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnEnChangeEditEnrollBank()												*/
/*																							*/
/*[���e]		�o�^�o���N�G�f�B�b�g�{�b�N�X���ҏW���ꂽ��A�󂢂Ă�e���v���[�g�ԍ����Z�b�g*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab0Dialog::OnEnChangeEditEnrollBank()
{
	// TODO:  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A
	// �܂��ACDialog::OnInitDialog() �֐����I�[�o�[���C�h���āAOR ��Ԃ� ENM_CHANGE
	// �t���O���}�X�N�ɓ���āACRichEditCtrl().SetEventMask() ���Ăяo���Ȃ�����A
	// �R���g���[���́A���̒ʒm�𑗐M���܂���B

	// TODO:  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B
	UINT uiBank;
	CString strBank;
	CNumber nb;

	if(!m_bSetEnable){
		//�X�V��������Ă��Ȃ��i�v���O������ōX�V����肪�s���Ă���j
		return;
	}

	//�o���N�ԍ��擾
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		return;
	}

	//���̃o���N�ԍ��ƃe���v���[�g�ԍ��ɍX�V
	updateEnroolEdit(FALSE);


}
