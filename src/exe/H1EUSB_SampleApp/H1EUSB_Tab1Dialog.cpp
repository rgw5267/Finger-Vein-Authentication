// CH1EUSB_Tab1Dialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "H1EUSB_SampleApp.h"
#include "H1EUSB_Tab1Dialog.h"
#include "H1EUSB_DetailsDlg.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_Number.h"
#include "H1EUSB_MsgGuide.h"
/********************************************************************************************/
/* CH1EUSB_Tab1Dialog�N���X�v���O����														*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20120110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/********************************************************************************************/
// CH1EUSB_Tab�PDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CH1EUSB_Tab1Dialog, CDialog)

/********************************************************************************************/
/*[���\�b�h��] CH1EUSB_Tab1Dialog�N���X�̃R���X�g���N�^										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CH1EUSB_Tab0Dialog(CWnd* pParent)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CH1EUSB_Tab1Dialog::CH1EUSB_Tab1Dialog(CWnd* pParent /*=NULL*/)
	: CDialog(CH1EUSB_Tab1Dialog::IDD, pParent)
{
	//�����o�[�ϐ�������
	m_pParetDlg = 0;		//�e�_�C�A���O�̃|�C���^�ϐ���������

}

/********************************************************************************************/
/*[���\�b�h��] CH1EUSB_Tab1Dialog�N���X�̃f�X�g���N�^										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		~CH1EUSB_Tab1Dialog()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CH1EUSB_Tab1Dialog::~CH1EUSB_Tab1Dialog()
{
}

void CH1EUSB_Tab1Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CH1EUSB_Tab1Dialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_11VERIF, &CH1EUSB_Tab1Dialog::OnBnClickedButton11verif)
	ON_BN_CLICKED(IDC_BUTTON_1NVERIF, &CH1EUSB_Tab1Dialog::OnBnClickedButton1nverif)
	ON_BN_CLICKED(IDC_BUTTON_VERIF_AB, &CH1EUSB_Tab1Dialog::OnBnClickedButtonVerifAb)
	ON_BN_CLICKED(IDC_BUTTON_VERIFWT, &CH1EUSB_Tab1Dialog::OnBnClickedButtonVerifwt)
END_MESSAGE_MAP()


// CH1EUSB_Tab�PDialog ���b�Z�[�W �n���h��

void CH1EUSB_Tab1Dialog::OnOK()
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
BOOL CH1EUSB_Tab1Dialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	//�@�\�����̃t�H���g�T�C�Y��ύX����
	m_ftFName.CreateFont( 14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,NULL); 
	GetDlgItem(IDC_STATIC_11VER)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_1NVER)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_VERACBANK)->SetFont(&m_ftFName);
	GetDlgItem(IDC_STATIC_VERTEMP)->SetFont(&m_ftFName);

	
	//�e���v���[�g�t���F�؂̃o���N/�e���v���[�g�I���R���{�{�b�N�X������
	SetVerTempList();


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
void CH1EUSB_Tab1Dialog::SetParentDialog(LPVOID pDlg)
{
	m_pParetDlg  = pDlg;
}


/********************************************************************************************/
/*[���\�b�h��] �F�؁i�P�F�P�j�{�^���������b�Z�[�W����										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButton11verif()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�o���N�ԍ��ƃe���v���[�g�ԍ��ɂ��F�؏��������s����						*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab1Dialog::OnBnClickedButton11verif()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	CNumber nb;
	CString strBank;
	CString strTmpNo;
	UINT uiBank;
	UINT uiTemp;
	CMsgGuide msgGuide;
	CString strMessage;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;

	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_11VERBANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//�o���N�ԍ��̓��͒l���
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_11VERTEMP);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		//�e���v���[�g�ԍ��̓��͒l���
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	pDlg->TuchDisable();


	//�o���N��؂�ւ���
	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, uiBank, 0);


	pDlg->TuchEnable();


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


	//�e���v���[�g���w�肵�ĔF�؂�����
	iReturn = GeneralTransaction(H1USB_COM_NINSHO, 0x09, uiTemp,  MSGGUID_VERIF_11, H1USB_COMMENT_UOP);
	if(iReturn != H1USB_OK){
		strMessage = msgGuide.VerificationNGMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->TuchEnable();
		return;
	}
	//�F�؏I�����b�Z�[�W�\��
	strMessage = msgGuide.VerificationFormatMessage(uiBank, uiTemp, iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	pDlg->TuchEnable();

}

/********************************************************************************************/
/*[���\�b�h��] �F�؁i�P�F�m�j�{�^���������b�Z�[�W����										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButton1nverif()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�o���N�ԍ��ɂ��F�؏��������s����											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab1Dialog::OnBnClickedButton1nverif()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;

	CNumber nb;
	CString strBank;
	UINT uiBank;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;

	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_N1BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		//�o���N�ԍ��̓��͒l���
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	CH1USBCom* pCom = pDlg->m_pH1USBCom;

	//�o���N��؂�ւ���
	pDlg->TuchDisable();

	iReturn = pCom->TransToDevice(H1USB_COM_CHANGE_BANK, uiBank, 0);
	pDlg->TuchEnable();

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

	//�P�F�m�F�؂�����B
	iReturn = GeneralTransaction(H1USB_COM_NINSHO, 0X08, 0,  MSGGUID_VERIF_1N, H1USB_COMMENT_UOP);
	if(iReturn != H1USB_OK){
		strMessage = msgGuide.VerificationNGMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->TuchEnable();
		return;
	}

	//�F�؏I�������\��
	strMessage = msgGuide.VerifBankFormatMessage(uiBank, iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[���\�b�h��] �o���N�ԔF�؁i�P�F�m�j�{�^���������b�Z�[�W����								*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonVerifAb()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�w�肳�ꂽ�o���N�Ԃɂ��F�؏��������s����									*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab1Dialog::OnBnClickedButtonVerifAb()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strStartBank;
	CString strEndBank;
	UINT ui;
	UINT uiStartBank;
	UINT uiEndBank;
	UINT uiBanks;
	CNumber nb;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;

	//�J�n�o���N���擾����
	CEdit *pEditStartBank = (CEdit *)GetDlgItem(IDC_EDIT_STARTBANK);
	pEditStartBank->GetWindowText(strStartBank);
	if(!nb.ConvertBankNummber(strStartBank, &uiStartBank)){
		//�o���N�ԍ��̓��͒l���
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}
	//�I���o���N���擾����
	CEdit *pEditEndBank = (CEdit *)GetDlgItem(IDC_EDIT_ENDBANK);
	pEditEndBank->GetWindowText(strEndBank);
	if(!nb.ConvertBankNummber(strEndBank, &uiEndBank)){
		//�o���N�ԍ��̓��͒l���
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}

	pDlg->TuchDisable();


	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	uiBanks = uiEndBank - uiStartBank + 1;
	WORD* pEnableBanlk = new WORD[uiBanks];

	UINT uiFixBanks;
	UINT uii;
	uiFixBanks = 0;
	for(ui=0; ui<uiBanks; ui++){
		for(uii=0; uii<pTpf->m_uiBanks; uii++){
			if(pTpf->m_pBankInfo[uii].uiBankNum == (uiStartBank + ui)){
				pEnableBanlk[uiFixBanks] = pTpf->m_pBankInfo[uii].uiBankNum;
				uiFixBanks++;
			}
		}
		if(uiFixBanks >= pTpf->m_uiBanks){
				break;
		}
	}
	pDlg->TuchEnable();

	if(uiFixBanks < 1){
		//�o���N�ԍ��̓��͒l���
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
		return;
	}


	//�ΏۂƂȂ�o���N����ݒ�
//	iReturn = pCom->SetEnableBank(pEnableBanlk, uiBanks);
	iReturn = pCom->SetEnableBank(pEnableBanlk, uiFixBanks);
	if(pEnableBanlk){
		delete[] pEnableBanlk;
	}

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

	//�o���N�ԔF�؂�����
	//iReturn = GeneralTransaction(H1USB_COM_NINSHO, 0, 0,  MSGGUID_VERIF_ACBANK, H1USB_COMMENT_UOP);
	iReturn = GeneralTransaction(H1USB_COM_NINSHO_BANK, 0, 0,  MSGGUID_VERIF_ACBANK, H1USB_COMMENT_UOP);
	if(iReturn != H1USB_OK){
		strMessage = msgGuide.VerificationNGMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->TuchEnable();
		return;
	}

	//�F�؏I�����b�Z�[�W�\��
	strMessage = msgGuide.VerifAcrossBKFormatMessage(uiStartBank, uiEndBank, iReturn, 0);
	pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	pDlg->TuchEnable();
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�w��F�؁i�P�F�m�j�{�^���������b�Z�[�W����						*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonVerifwt()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�w�肳�ꂽ�e���v���[�g���F�؁i�P�F�m�j���������s����						*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
const UINT uiControlList[]={IDC_COMB_TEMP1, IDC_COMB_TEMP2, IDC_COMB_TEMP3, 
						IDC_COMB_TEMP4, IDC_COMB_TEMP5, IDC_COMB_TEMP6};


void CH1EUSB_Tab1Dialog::OnBnClickedButtonVerifwt()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	CString strBank;
	CString strTemplate;
	UINT uiBank;
	UINT uiTempNum;
	int i;
	int iContorls = sizeof(uiControlList)/sizeof(UINT);
	BOOL bInit;
	TEMPU tempU;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CH1USBCom* pCom = pDlg->m_pH1USBCom;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;

	bInit = TRUE;
	for(i=0; i<iContorls; i++){
		CComboBox *pComboTemp = (CComboBox*)GetDlgItem(uiControlList[i]);
		pComboTemp->GetWindowText(strWork);
		if(strWork.GetLength() == 0){
			break;
		}
		strBank = strWork.Left(4);
		strTemplate = strWork.Mid(7,2);
		iReturn = _stscanf(strBank, "%x", &uiBank);
		if(iReturn <= 0){
			break;
		}
		iReturn = _stscanf(strTemplate, "%x", &uiTempNum);
		if(iReturn <= 0){
			break;
		}
		//�e���v���[�g�t�@�C�����e���v���[�g���擾
		iReturn = pTpf->GetTemplate(uiBank, uiTempNum, &tempU);
		if(iReturn != H1USB_OK){
			break;
		}
		//�F�؂̂��߂̃e���v���[�g���Z�b�g
		iReturn = pCom->SetTempU(&tempU, 1, bInit);
		if(iReturn != H1USB_OK){
			break;
		}
		bInit = FALSE;
	}
	if(!bInit){
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



		//�e���v���[�g�t���F��
		iReturn = GeneralTransaction(H1USB_COM_NINSHO_TEMP, 0, 0,  MSGGUID_VERIF_WTEMP, H1USB_COMMENT_UOP);
		if(iReturn != H1USB_OK){
			strMessage = msgGuide.VerificationNGMessage();
			pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
			pDlg->TuchEnable();
			return;
		}
		//�F�؏I�����b�Z�[�W�\��
		strMessage = msgGuide.VerificationOKFormatMessage();
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
		pDlg->TuchEnable();
	}
	else{
		//�o���N�ƃe���v���[�g���I������Ă��Ȃ�
		strMessage.FormatMessage(IDS_NON_SELECT);
		pDlg->SetComment(H1USB_COMMENT_NON, strMessage, 0);
	}

}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�w��F�؁i�P�F�m�j�R���{�{�b�N�X�Z�b�g							*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void SetVerTempList()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�o�^����Ă���e���v���[�g���A�e���v���[�g�t���F�؂̂��߂̃R���{�{�b�N�X	*/
/*				�ɒl���Z�b�g����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_Tab1Dialog::SetVerTempList()
{
	
	int iContorls = sizeof(uiControlList)/sizeof(UINT);
	int	i;
	int ii;
	CString strWork;

	CH1EUSB_DetailsDlg*  pDlg = (CH1EUSB_DetailsDlg*)m_pParetDlg;
	CTemplateFile* pTpf = pDlg->m_pTemplateFile;
	for(i=0; i<iContorls; i++){
		CComboBox *pComboTemp = (CComboBox*)GetDlgItem(uiControlList[i]);
		//���ɕ\�����̒l������΁A�S�č폜����
		for (ii = pComboTemp->GetCount() - 1; ii >= 0; ii--){
		   pComboTemp->DeleteString(ii);
		}
		if(i<(int)pTpf->m_uiTotalTemplates){
			pComboTemp->EnableWindow(TRUE);
			pComboTemp->SetItemHeight(0,10);
			for(ii=0; ii<(int)pTpf->m_uiTotalTemplates; ii++){
				// "bank nummber/template nummber"�̃t�H�[�}�b�g�Ń��X�g���쐬
				strWork.Format(_T("%04Xh/%02Xh"), pTpf->m_pTempList[ii].uiBankNum, pTpf->m_pTempList[ii].uiTempNum);
				pComboTemp->InsertString(-1,strWork);
			}
			//�ő�\�����U�ɖ����Ȃ��ꍇ�́A"-----/---"��\������
			pComboTemp->InsertString(-1,_T("-----/---"));				
		}
		else{
			pComboTemp->EnableWindow(FALSE);
		}
	}

	pTpf->m_uiTotalTemplates;

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
int CH1EUSB_Tab1Dialog::GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode)
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
	//�R�}���h�I�����b�Z�[�W
	if(uiMode&H1USB_COMMENT_UOP){
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//�R�}���h���M���b�Z�[�W
			pDlg->SetComment(uiMode, strMessage, 0);
		}
	}
	if(iReturn < H1USB_OK){
		//strMessage.Format(_T("Error:%d"), iReturn);
		strMessage = msgGuide.ErrorGeneralFormatMessage(iReturn);
		pDlg->SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	}
	pDlg->EnabeleControl(NULL, TRUE);
	
	return iReturn;
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
void CH1EUSB_Tab1Dialog::UpdateContorol()
{
	SetVerTempList();
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
void CH1EUSB_Tab1Dialog::UpdateNummber(CString strBank, CString strTemp)
{
	//���X�g�R���g���[�����I�����ꂽ�o���N��\��
	CWnd* pWnd;
	int iCtrlID;

	pWnd = GetFocus();
	if(pWnd){
		//�t�H�[�J�X������E�B���h�E�����݂���
		iCtrlID = pWnd->GetDlgCtrlID();
		CEdit *pEditStartBank = (CEdit *)GetDlgItem(IDC_EDIT_STARTBANK);
		CEdit *pEditEndBank = (CEdit *)GetDlgItem(IDC_EDIT_ENDBANK);
		if(iCtrlID == IDC_EDIT_STARTBANK){
			//�X�^�[�g�o���N�Ƀt�H�[�J�X������΃o���N��}��
			pEditStartBank->SetWindowText(strBank);
			return;
		}
		if(iCtrlID == IDC_EDIT_ENDBANK){
			//�G���h�o���N�Ƀt�H�[�J�X������΃o���N��}��
			pEditEndBank->SetWindowText(strBank);
			return;
		}
	}
	CEdit *pEdit11VerBank = (CEdit *)GetDlgItem(IDC_EDIT_11VERBANK);
	CEdit *pEditN1Bank = (CEdit *)GetDlgItem(IDC_EDIT_N1BANK);
	pEdit11VerBank->SetWindowText(strBank);
	pEditN1Bank->SetWindowText(strBank);

	//���X�g�R���g���[�����I�����ꂽ�e���v���[�g��\��
	CEdit *pEdit11VerTemp = (CEdit *)GetDlgItem(IDC_EDIT_11VERTEMP);
	pEdit11VerTemp->SetWindowText(strTemp);
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
void CH1EUSB_Tab1Dialog::EnabeleControl(DWORD dwMode, BOOL bEnable)
{
	CButton*	pBt11Verif = (CButton*)GetDlgItem(IDC_BUTTON_11VERIF);
	CButton*	pBt1NVerif = (CButton*)GetDlgItem(IDC_BUTTON_1NVERIF);
	CButton*	pBtAcVerif = (CButton*)GetDlgItem(IDC_BUTTON_VERIF_AB);
	CButton*	pBtWtVerif= (CButton*)GetDlgItem(IDC_BUTTON_VERIFWT);

	switch(dwMode){
		case SYSTEM_STARTUP:
		case SYSTEM_DEVICE_OFF:
		case SYSTEM_DEVICE_ERR:
			pBt11Verif->EnableWindow(FALSE);
			pBt1NVerif->EnableWindow(FALSE);
			pBtAcVerif->EnableWindow(FALSE);
			pBtWtVerif->EnableWindow(FALSE);
			break;
		case SYSTEM_DEVICE_ON:
		default:
			pBt11Verif->EnableWindow(bEnable);
			pBt1NVerif->EnableWindow(bEnable);
			pBtAcVerif->EnableWindow(bEnable);
			pBtWtVerif->EnableWindow(bEnable);
			break;
	}
}
