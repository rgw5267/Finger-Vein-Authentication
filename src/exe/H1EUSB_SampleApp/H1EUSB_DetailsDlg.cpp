// H1EUSB_DetailsDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "H1EUSB_SampleApp.h"
#include "H1EUSB_DetailsDlg.h"
#include "H1EUSB_Tab0Dialog.h"
#include "H1EUSB_Tab1Dialog.h"
#include "H1EUSB_Tab2Dialog.h"
#include "H1EUSB_TemplateFile.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_MsgGuide.h"
/********************************************************************************************/
/* CH1EUSB_DetailsDlg�N���X�v���O����														*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20120110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/********************************************************************************************/
// CH1EUSB_DetailsDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CH1EUSB_DetailsDlg, CDialog)

/********************************************************************************************/
/*[���\�b�h��] CH1EUSB_DetailsDlg�N���X�̃R���X�g���N�^										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CH1EUSB_DetailsDlg(CWnd* pParent)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CH1EUSB_DetailsDlg::CH1EUSB_DetailsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CH1EUSB_DetailsDlg::IDD, pParent)
{
	//�����o�ϐ��̏�����
	m_pTemplateFile = 0;
	m_pH1USBCom = 0;
	m_dwSystemCondition = SYSTEM_STARTUP;

	//�R�~���j�P�[�V�����N���X�̐���
	m_H1EComObjEP.pObj = this;												//�{�N���X�̃I�u�W�F�N�g
	m_H1EComObjEP.H1ECallBacks.OnTuchNotification = OnTuchNotification;		//�^�b�`�Z���T�[���ʒm�֐��G���g���[
	m_H1EComObjEP.H1ECallBacks.OnSendEnd = OnSendEndNotification;			//���M�I���ʒm�G���g���[	
	m_pH1USBCom = new CH1USBCom(&m_H1EComObjEP);							//�N���X�̐���

	//�e���v���[�g�N���X�̐���
	m_pTemplateFile = new CTemplateFile;

	//�R�~���j�P�[�V�����N���X�̃G���h�|�C���g�@�\�̊J�n
	m_pH1USBCom->StartEndPoint();

	//�^�u�̃p�l���̎q�_�C�A���O�ɖ{�i�e�j�̃I�u�W�F�N�g���Z�b�g
	m_dlgTab0.SetParentDialog(this);
	m_dlgTab1.SetParentDialog(this);
	m_dlgTab2.SetParentDialog(this);
}

/********************************************************************************************/
/*[���\�b�h��] CH1EUSB_DetailsDlg�N���X�̃f�X�g���N�^										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		~CH1EUSB_DetailsDlg()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CH1EUSB_DetailsDlg::~CH1EUSB_DetailsDlg()
{
	//�R�~���j�P�[�V�����N���X�̃G���h�|�C���g�@�\�̒�~
	m_pH1USBCom->StopEndPoint();

	//�R�~���j�P�[�V�����N���X�̍폜
	if(m_pH1USBCom){
		delete m_pH1USBCom;
	}

	//�e���v���[�g�N���X�̍폜
	if(m_pTemplateFile){
		delete m_pTemplateFile;
	}

}

void CH1EUSB_DetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DTAILS_TAB, m_tabCtrlDetails);
	DDX_Control(pDX, IDC_STATIC_CGBOX, m_stComGBox);
	DDX_Control(pDX, IDC_STATIC_FNAME, m_stFName);
	DDX_Control(pDX, IDC_BUTTON_MAIN, m_buMain);
	DDX_Control(pDX, IDC_LIST_TMPLIST, m_listCtrlTempList);
	DDX_Control(pDX, IDC_CHECK_POL, m_btCheckPol);
}


BEGIN_MESSAGE_MAP(CH1EUSB_DetailsDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DTAILS_TAB, &CH1EUSB_DetailsDlg::OnTcnSelchangeDtailsTab)

	ON_BN_CLICKED(IDC_BUTTON_MAIN, &CH1EUSB_DetailsDlg::OnBnClickedButtonMain)
	ON_BN_CLICKED(IDC_BUTTON_COMSET, &CH1EUSB_DetailsDlg::OnBnClickedButtonComset)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TMPLIST, &CH1EUSB_DetailsDlg::OnLvnItemchangedListTmplist)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CH1EUSB_DetailsDlg ���b�Z�[�W �n���h��
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
BOOL CH1EUSB_DetailsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�


	// TODO:  �����ɏ�������ǉ����Ă�������
	//���̃_�C�A���O�̃R���g���[���̏�����
	ControlInit();									//�e�R���g���[���̏�����
	m_dwSystemCondition = SYSTEM_DEVICE_OFF;		//��ԕϐ��̃Z�b�g
	EnabeleControl(m_dwSystemCondition, TRUE);		//�R���g���[���L��

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

/********************************************************************************************/
/*[���\�b�h��] SET�{�^���������b�Z�[�W����													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnBnClickedButtonComset()												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�w�肳�ꂽCOM���I�[�v�����A�f�o�C�X�̏����ݒ���s��							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnBnClickedButtonComset()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B

	CString strComName;
	int		iReturn;
	DWORD	dwError;
	CString strComment;
	CMsgGuide msgGuide;


	//�I�����ꂽ�b�n�l�|�[�g�����擾����B
	CComboBox *pCom = (CComboBox *)GetDlgItem(IDC_COMBO_COMPORT);
	pCom->GetWindowText(strComName);

	//�I�����ꂽ�b�n�l�|�[�g���I�[�v������B
	iReturn = m_pH1USBCom->Open(strComName, &dwError);
	if(iReturn != H1USB_OK){
		//COM�|�[�g�̃I�[�v�����s
		strComment.FormatMessage(IDS_ERR_COMOPEN, strComName);
		SetComment(H1USB_COMMENT_PLUS, strComment, dwError);
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}
	strComment.FormatMessage(IDS_COMOPEN, strComName);
	SetComment(H1USB_COMMENT_NON, strComment, dwError);

	//�f�o�C�X�����ݒ蒆�́A�{�_�C�A���O�̃R���g���[���𖳌��ɂ���
	m_dwSystemCondition = SYSTEM_DEVICE_ON;
	EnabeleControl(m_dwSystemCondition, FALSE);

	//�^�b�`�Z���T�[�̏����l��\������
	CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SST);
	pEdit->SetWindowText(_T("OFF"));

	//�n�[�h�E�F�A���Z�b�g�R�}���h���M
	iReturn = GeneralTransaction(H1USB_COM_HWRESET, 0, 0, MSGGUID_SEND_HWRESET, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}
	//�Í����R�}���h�𑗐M����
	iReturn = GeneralTransaction(H1USB_COM_SCRAMBLE, 0, 0, MSGGUID_SEND_SCRAMBLE, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}

	//�΂̂k�d�c��_������B
	iReturn = GeneralTransaction(H1USB_COM_GREENLED_ON, 0, 0, MSGGUID_LED_GREEN, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}

	//�t�@�[���E�F�AVer�̎擾
	iReturn = GeneralTransaction(H1USB_COM_GETFVER, 0, 0, MSGGUID_SEND_GETFWVER, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}

	//�V���A���i���o�[�̎擾
	iReturn = GeneralTransaction(H1USB_COM_GETSNUM, 0, 0, MSGGUID_SEND_GETSERNUM, H1USB_COMMENT_NON);
	if(iReturn !=H1USB_OK){	
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}


	//�t�@�[���E�F�AVer��\������
	CStatic* pSFWVert = (CStatic*)GetDlgItem(IDC_STATIC_SET_FWVER);
	pSFWVert->SetWindowText(m_pH1USBCom->m_cFwVersion);

	//�V���A���i���o�[��\������
	CStatic* pStSnum = (CStatic*)GetDlgItem(IDC_STATIC_SET_SERIAL);
	pStSnum->SetWindowText(m_pH1USBCom->m_cSerialNum);


	//���b�Z�[�W�������I�ɏ������āA�\����D�悷��B
	DoEvents();

	//�o�^�ς݂̃e���v���[�g���f�o�C�X�֑��M����
	EnabeleControl(m_dwSystemCondition, FALSE);

	iReturn = SyncDevice();

	////�^�b�`�Z���T�[�̒ʒm��������B
	//iReturn = GeneralTransaction(H1USB_COM_TUCH_NOTIFY, 0x01, 0, MSGGUID_SEND_TSEN_NOTIFY, H1USB_COMMENT_NON);
	//if(iReturn !=H1USB_OK){	
	//	m_dwSystemCondition = SYSTEM_DEVICE_ERR;
	//	EnabeleControl(m_dwSystemCondition, TRUE);
	//	return;
	//}

	//�e�^�u�̃p�l����L���ɂ���
	m_dlgTab0.EnableWindow(TRUE);
	m_dlgTab1.EnableWindow(TRUE);
	m_dlgTab2.EnableWindow(TRUE);


	//�Z�b�e�B���O�I�����b�Z�[�W�\��
	strComment = msgGuide.EndAppSettingMessage(strComName);
	SetComment(H1USB_COMMENT_NON, strComment, 0);

	//�{�_�C�A���O�̃R���g���[���̗L��
	EnabeleControl(m_dwSystemCondition, TRUE);


	TuchEnable();


}

/********************************************************************************************/
/*[���\�b�h��]�R���g���[���̏�����															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int ControlInit()															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�{�_�C�A���O�̃R���g���[��������������										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ControlInit()
{
	int iReturn = H1USB_OK;

	//�e�R���g���[���̏�����
	ControlComportInit();				//COM�|�[�g�R���{�{�b�N�X
	ControlTemplateInit();				//�e���v���[�g���X�g
	ControlTabInit();					//�^�u�p�l��


	//�R�����g�\���G�f�B�b�g�{�b�N�X��ҏW�s�ɂ���
	CEdit *pEditComment = (CEdit *)GetDlgItem(IDC_EDIT_COMMENT);
	pEditComment->SetReadOnly(TRUE);
	//�^�b�`�Z���T�[��ԕ\���G�f�B�b�g�{�b�N�X��ҏW�s�ɂ���
	CEdit *pEditTach = (CEdit *)GetDlgItem(IDC_EDIT_SST);
	pEditTach->SetReadOnly(TRUE);

	//�@�\����\������
	m_ftFName.CreateFont( 16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,NULL); 
	m_stFName.SetFont(&m_ftFName);
	m_stFName.SetWindowText("Enroll/Copy/Delete");

	//�f�o�C�X��������������
	CStatic* pSta =  (CStatic*)GetDlgItem(IDC_STATIC_SET_FWVER);
	pSta->SetWindowText(_T(""));									//�t�@�[���E�F�A�o�[�W����
	pSta =  (CStatic*)GetDlgItem(IDC_STATIC_SET_SERIAL);
	pSta->SetWindowText(_T(""));									//�V���A���i���o�[

	//�^�b�`�Z���T�[�|�[�����O
	m_btCheckPol.SetCheck(TRUE);


	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��]	COM�|�[�g�I��p�R���{�{�b�N�X�R���g���[���̏�����������						*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int ControlComportInit()													*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�{�_�C�A���O�̃R���g���[��������������										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ControlComportInit()
{
	int iReturn = H1USB_OK;
	int i;

	//COM�|�[�g���́iCOM1�`COM255�j���Z�b�g
	CString strComPortName;
	CComboBox *pCom = (CComboBox *)GetDlgItem(IDC_COMBO_COMPORT) ;
	for (i = 1; i <= 256; i++ ) {	
		strComPortName.Format(_T("COM%d"), i);
		pCom->InsertString(-1, strComPortName) ;
	}

	// �R���{�{�b�N�X�̋K��l�Ƃ��āAH1E-USB���ڑ�����Ă���COM�|�[�g�ԍ����擾����
	CString strFName = FRIENDLY_NAME ;	// H1E-USB��FriendlyName
	// �֐��Ăяo��
	int nPort = getCOMPortNumber(strFName);

	// H1E-USB ���ڑ��̏ꍇ
	if ( nPort < 0 ) {
		
		// COM1��I��
		pCom->SetCurSel( 0 ) ;
	}
	else {

		// �R���{�{�b�N�X�̋K��l�Ƃ��āAH1E-USB���ڑ�����Ă���COM�|�[�g��\��
		pCom->SetCurSel( nPort - 1 ) ;
	}

	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��]	�e���v���[�g���X�g�\���p���X�g�R���g���[���̏�����							*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int ControlTemplateInit()													*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g���X�g�\�����X�g�R���g���[��������������						*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ControlTemplateInit()
{
	int iReturn = H1USB_OK;

	ListInit();							//���X�g�R���g���[��������
    ListInsertItem();					//���X�g�A�C�e���}��
	Sort();								//�\�[�g�����̋N��

	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��]	�^�u�R���g���[���R���g���[���̏�����										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int ControlTabInit()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�^�u�p�l��������������														*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ControlTabInit()
{
	//�e���v���[�g���X�g�̏�����
	int iReturn = H1USB_OK;

	RECT rectDetDlg;;
	int iXPos,iYpos;

	//�^�u�R���g���[����������
	this->GetClientRect(&rectDetDlg);
	iXPos = rectDetDlg.right-rectDetDlg.left;
	iYpos = rectDetDlg.bottom-rectDetDlg.top;
	m_tabCtrlDetails.SetWindowPos(&wndTop,0,0,iXPos,iYpos,SWP_SHOWWINDOW);
	m_tabCtrlDetails.InsertItem(0, "Enroll/Copy/Delete");
	m_tabCtrlDetails.InsertItem(1, "Verification");
	m_tabCtrlDetails.InsertItem(2, "Others");
	m_tabCtrlDetails.SetCurSel(0);

	
	//�^�u��ɒ���t����_�C�A���O��������
	RECT rect;
	this->GetWindowRect(&rectDetDlg);
	m_stComGBox.GetWindowRect(&rect);
	iYpos = rect.bottom-rectDetDlg.top;
	ScreenToClient(&rect);

	//Tab0�p�l�����N���G�[�g���ċK��̈ʒu�Ɉړ�����
	m_dlgTab0.Create(IDD_DIALOG_TAB0, &m_tabCtrlDetails);
    m_dlgTab0.SetWindowPos(&wndTop,rect.left,iYpos,0,0,SWP_NOSIZE|SWP_SHOWWINDOW);		//�\��

	//Tab1�p�l�����N���G�[�g���ċK��̈ʒu�Ɉړ�����
	m_dlgTab1.Create(IDD_DIALOG_TAB1, &m_tabCtrlDetails);
    m_dlgTab1.SetWindowPos(&wndTop,rect.left,iYpos,0,0,SWP_NOSIZE|SWP_HIDEWINDOW);		//��\��

	//Tab2�p�l�����N���G�[�g���ċK��̈ʒu�Ɉړ�����
	m_dlgTab2.Create(IDD_DIALOG_TAB2, &m_tabCtrlDetails);
    m_dlgTab2.SetWindowPos(&wndTop,rect.left,iYpos,0,0,SWP_NOSIZE|SWP_HIDEWINDOW);		//��\��

	return iReturn;
}

/*====================================================================*/
/* �����T�v  �FH1E-USB���ڑ�����Ă���COM�|�[�g�̔ԍ���ԋp����
/* ����      �FpFName	H1E-USB��FriendlyName
/* �߂�l    �FCOM�|�[�g�̔ԍ�(���ڑ��̏ꍇ�� -1)
/*====================================================================*/
int CH1EUSB_DetailsDlg::getCOMPortNumber(LPCTSTR pFName)
{

	HDEVINFO hDevInfo = NULL ;		// �񋓃f�o�C�X���
	SP_DEVINFO_DATA DeviceInfoData;	// 1���f�o�C�X���
	int nPorts = -1 ;				// H1E-USB���ڑ�����Ă���COM�|�[�g�̔ԍ�
	BYTE byBuffer[1024] = { '\0' } ;

	memset( &DeviceInfoData, 0, sizeof(SP_DEVINFO_DATA) ) ;
	
	// �S�Ă̐e�f�o�C�X(USB�f�o�C�X�̂�)�Ƃ��Ă� HDEVINFO �𐶐�
	hDevInfo = SetupDiGetClassDevs( NULL, TEXT("USB"), 0, DIGCF_PRESENT | DIGCF_ALLCLASSES ) ;
	if ( hDevInfo == INVALID_HANDLE_VALUE ) {
		return nPorts;
	}

	// SP_DEVINFO_DATA�\���̂̃T�C�Y���Z�b�g
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA) ;

	for ( int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++ ) {

	    // �f�o�C�X�̃t�����h���[�l�[��(FriendlyName)���擾����

		DWORD dwBuffersize = 0 ;

		int err = SetupDiGetDeviceRegistryProperty(hDevInfo,		// A handle to a device information set
											   &DeviceInfoData,		// A pointer to an SP_DEVINFO_DATA structure
											   SPDRP_FRIENDLYNAME,	// The friendly name of a device
											   NULL,				// PropertyRegDataType (out, optional)
											   byBuffer,			// PropertyBuffer (out, optional)
											   sizeof(byBuffer),	// PropertyBufferSize
											   &dwBuffersize);		// RequiredSize (out, optional)

		// �w�肵���V���A���|�[�g���ǂ������R���y�A(H1E-USB��COM�|�[�g�ȊO�̓X�L�b�v)
		if ( err == FALSE ) {	
			
			continue;
		}
		else {

			// FriendlyName ����v���Ȃ��ꍇ
			if ( strncmp( (const char*)byBuffer, (const char*)pFName, strlen((const char*)pFName) ) != 0 ) {
			
				continue;
			}
			// FriendlyName ����v
			else {

				// �������s

			}
		}

		// H1E-USB��COM�|�[�g����������
		// ���u�ŗL�̍\�����擾�̂��߂Ƀ��W�X�g���L�[���J��
		HKEY  hDeviceKey = SetupDiOpenDevRegKey(hDevInfo,	// A handle to a device information set
										  &DeviceInfoData,	// A pointer to an SP_DEVINFO_DATA structure
										  DICS_FLAG_GLOBAL,	// Scope
										  0,				// HwProfile:Scope is DICS_FLAG_GLOBAL, HwProfile is ignored.
										  DIREG_DEV,		// Open a hardware key for the device.
										  KEY_READ);		// registry security access values

		memset( byBuffer, 0, sizeof(byBuffer) ) ;
		DWORD dwPortNameSize = sizeof(byBuffer);

		// �w�肳�ꂽ���W�X�g���L�[�ɏ������Ă���A�w�肳�ꂽ���W�X�g���G���g���̃f�[�^�^�ƃf�[�^���擾����
		DWORD dwError = RegQueryValueEx(hDeviceKey,			// �L�[�̃n���h��
								  REGSTR_VAL_PORTNAME,		// ���W�X�g���G���g���� "PortName"
								  NULL,						// (�\��)
								  NULL,						// �f�[�^�^���i�[�����o�b�t�@(�C��)
								  byBuffer,					// �f�[�^���i�[�����o�b�t�@
								  &dwPortNameSize);			// �f�[�^�o�b�t�@�̃T�C�Y

		if ( dwError != ERROR_SUCCESS ) {
			continue ;
		}

		// �w�肳�ꂽ���W�X�g���L�[�̃n���h�������
		RegCloseKey( hDeviceKey ) ;

		// COMn�́un�v�̕����𐔒l������
		char buff[50] = { '\0' } ;
		strncpy( buff, (const char *)(byBuffer + 3), 3 ) ;

		// PortName �� "COM"����n�܂��Ă��Ȃ��ꍇ�͔�΂�
		byBuffer[3] = 0 ;
		if (strcmp((const char*)byBuffer, "COM")){
			continue ;
		}
		
		// ���l������int�^�ɕϊ�
		nPorts = atoi(buff) ;
	}

	return nPorts ;

}

/********************************************************************************************/
/*[���\�b�h��]	�^�u�`�F���W���b�Z�[�W����													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnTcnSelchangeDtailsTab(NMHDR *pNMHDR, LRESULT *pResult)				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�����ꂽ�^�u�̃p�l���_�C�A���O��\������									*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnTcnSelchangeDtailsTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iTab = m_tabCtrlDetails.GetCurSel();

	switch(iTab) {
		case 0:
			m_stFName.SetWindowText("Enroll/Copy/Delete");
			m_dlgTab0.ShowWindow(SW_SHOW);
			m_dlgTab1.ShowWindow(SW_HIDE);
			m_dlgTab2.ShowWindow(SW_HIDE);
			break;
		case 1:
			m_stFName.SetWindowText("Verification");
			m_dlgTab0.ShowWindow(SW_HIDE);
			m_dlgTab1.ShowWindow(SW_SHOW);
			m_dlgTab2.ShowWindow(SW_HIDE);
			break;
	
		case 2:
			m_stFName.SetWindowText("Others");
			m_dlgTab0.ShowWindow(SW_HIDE);
			m_dlgTab1.ShowWindow(SW_HIDE);
			m_dlgTab2.ShowWindow(SW_SHOW);
			break;

		default:
			break;
	
	}
	*pResult = 0;
}

/********************************************************************************************/
/*[���\�b�h��]	OnOK���b�Z�[�W����															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnOK()																	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�G���^�[�L�[�����̏ꍇ�Ƀ��b�Z�[�W�𓦂����悤�ɂ���						*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//CDialog::OnOK();
}

/********************************************************************************************/
/*[���\�b�h��]	End�{�^���������̃��b�Z�[�W����												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnOK()																	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�o�C�X�����Z�b�g���āA�I������											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnBnClickedButtonMain()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	m_pH1USBCom->TransToDevice(H1USB_COM_HWRESET, 0, 0);

	CDialog::OnOK();
}

/********************************************************************************************/
/*[���\�b�h��]	�e���v���[�g���X�g�R���g���[���̏�����										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int ListInit()																*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�o�C�X�����Z�b�g���āA�I������											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ListInit(void)
{
    LVCOLUMN    lvc;
    int         i;
	TCHAR*      caption[] = {_T("Bank"), _T("Template"), _T("Time")};
	int			iWidth[] = {60,60,140};

    const int   clmNum = sizeof caption /sizeof(TCHAR*) ;
    int         err = 0;

	m_listCtrlTempList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

    for (i = 0; i < clmNum; i++)
    {
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;	// �L���t���O
		lvc.fmt			= LVCFMT_CENTER;
		lvc.iSubItem    = i;											// �T�u�A�C�e���ԍ�
        lvc.pszText     = caption[i];									// ���o���e�L�X�g
        lvc.cx          = iWidth[i];									// ����
        if (m_listCtrlTempList.InsertColumn(i, &lvc) == -1)
		{
			err = 1;
			break;
		}
    }
    
    return err;
}

/********************************************************************************************/
/*[���\�b�h��]	�e���v���[�g���X�g�ɃA�C�e����ǉ�����										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int ListInsertItem()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�o�C�X�����Z�b�g���āA�I������											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::ListInsertItem(void)
{
 
    LVITEM       lvi;
    CString      str;
    int          i, index = 0;
    int          err = 0;
	int			iLists;
	CString		strBankNum;
	CString		strTempNum;
	CString		strTime;    

	//���ׂẴA�C�e�����폜
	m_listCtrlTempList.DeleteAllItems();
	DoEvents();

	//���炩���ߍő吔���m�ۂ���
	m_listCtrlTempList.SetItemCount(MAX_TEMPLATE_TOTAL);
	DoEvents();

	m_listCtrlTempList.ShowScrollBar( SB_VERT, FALSE );

	//���݃t�@�C���ɓo�^����Ă���e���v���[�g�����擾
	iLists = m_pTemplateFile->m_uiTotalTemplates;

	 //�o�^����Ă���e���v���[�g�̌���\��
	CStatic* pSta = (CStatic*)GetDlgItem(IDC_STATIC_TEMPS);
	str.Format(_T("[%5d/2000]"), iLists);
	pSta->SetWindowText(str);

	//�o�^����Ă���e���v���[�g�̏������X�g�ɕ\������
    for (i = 0; i < iLists; i++)
    {
        lvi.mask = LVIF_TEXT;
        // �o���N�ԍ�
        if (!err)
        {
            lvi.iItem = i;
            lvi.iSubItem = 0;
			strBankNum.Format(_T("%04X"), m_pTemplateFile->m_pTempList[i].uiBankNum);
            lvi.pszText = strBankNum.GetBuffer();
            if ((index = m_listCtrlTempList.InsertItem(&lvi)) == -1) err = 1;
        }
        // �e���v���[�g�ԍ�
        if (!err)
        {
			lvi.iItem = index;
			lvi.iSubItem = 1;
			strTempNum.Format(_T("%02X"), m_pTemplateFile->m_pTempList[i].uiTempNum);
            lvi.pszText = strTempNum.GetBuffer();
            if (!m_listCtrlTempList.SetItem(&lvi)) err = 1;
        }
        // �o�^����
        if (!err)
        {
            lvi.iItem = index;
            lvi.iSubItem = 2;
			strTime.Format(_T("%4d/%02d/%02d %02d:%02d:%02d"), 
							m_pTemplateFile->m_pTempList[i].time.wYear, 
							m_pTemplateFile->m_pTempList[i].time.wMonth, 
							m_pTemplateFile->m_pTempList[i].time.wDay, 
							m_pTemplateFile->m_pTempList[i].time.wHour,
							m_pTemplateFile->m_pTempList[i].time.wMinute,
							m_pTemplateFile->m_pTempList[i].time.wMinute);
            lvi.pszText = strTime.GetBuffer();
            if (!m_listCtrlTempList.SetItem(&lvi)) err = 1;
        }
        if (err) break;

    }
    
    return err;
}

/********************************************************************************************/
/*[���\�b�h��]	�\�[�g�����̃R�[���o�b�N����												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int :CompareFunc(LPARAM param1, LPARAM param2, LPARAM param3)				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�Ă΂�邲�Ƃɔ�r���ď����ɃZ�b�g����										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���F0																	*/
/********************************************************************************************/
int CALLBACK CH1EUSB_DetailsDlg::CompareFunc(LPARAM param1, LPARAM param2, LPARAM param3)
{

	long lNUm1, lNUm2;
    char *endptr;
	int iReturn;
	iReturn = 0;

	CWinApp* app = AfxGetApp();
	CH1EUSB_DetailsDlg* pdlg = (CH1EUSB_DetailsDlg*)app->GetMainWnd();

	// ��r�����Q�̃A�C�e������uBank�v�̕�������擾����
	CString str1 = pdlg->m_listCtrlTempList.GetItemText(param1, 0);
	lNUm1 = strtol(str1.GetString(), &endptr, 16);
	lNUm1 = lNUm1*0x100;
	CString str2 = pdlg->m_listCtrlTempList.GetItemText(param2, 0);
	lNUm2 = strtol(str2.GetString(), &endptr, 16);
	lNUm2 = lNUm2*0x100;
	str1 = pdlg->m_listCtrlTempList.GetItemText(param1, 1);
	lNUm1 += strtol(str1.GetString(), &endptr, 16);
	str2 = pdlg->m_listCtrlTempList.GetItemText(param2, 1);
	lNUm2 += strtol(str2.GetString(), &endptr, 16);

	if(lNUm1>lNUm2){
		iReturn = 1;
	}
	if(lNUm1<lNUm2){
		iReturn = -1;
	}

	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��]	���X�g�A�C�e���̃��i���o�[����												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void RenumberItem()															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		���X�g�����ɐU��Ȃ���														*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::RenumberItem()
{
	LV_ITEM lvItem;

	for( int i = 0; i < m_listCtrlTempList.GetItemCount(); i++ ) {
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.mask = LVIF_PARAM;
		lvItem.lParam = i;							// �����Ŕԍ����A�C�e���Ɏw�肷��
		m_listCtrlTempList.SetItem(&lvItem);
	}
}

/********************************************************************************************/
/*[���\�b�h��]	�e���v���[�g���X�g�̃\�[�g���N��											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void RenumberItem()															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g���X�g�̃\�[�g���J�n����										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::Sort() 
{
	if(m_pTemplateFile->m_uiTotalTemplates>0){
		RenumberItem();										// SortItems�֐����Ăяo���O�ɂ͕K�����s
		m_listCtrlTempList.SortItems(CompareFunc, FALSE);
	}
}


/********************************************************************************************/
/*[���\�b�h��]	�R�����g�\��																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError)				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R�����g�G�f�B�b�g�{�b�N�X�ɕ\������										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError)
{
	CString strComment;
	CString strErr;
	CString strWork;

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_COMMENT);
	strComment	= pMessage;
	strErr.Format(_T("[Error=%08Xh]"),dwError);
	strWork.Empty();
	if(dwMode&H1USB_COMMENT_ADD){
		pEdit->GetWindowText(strWork);
		strWork += _T("\r\n");
	}
	strComment = strWork + pMessage;
	if(dwMode&H1USB_COMMENT_PLUS){
		strComment += strErr;
	}
	pEdit->SetWindowText(strComment) ;
	DoEvents();
}

/********************************************************************************************/
/*[���\�b�h��]	�^�b�`�Z���T�[���ʒm�R�[���o�b�N�֐�										*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		LRESULT OnTuchNotification(DWORD dwStatus, LPVOID pObj)						*/
/*																							*/
/*[���e]		�^�b�`�Z���T�[���ɂ��A�Z���T�[��ԕ\���G�f�B�b�g�{�b�N�X��ON/OFF�\������*/
/*				CH1USBCom�N���X���R�[���o�b�N�����										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���F0																	*/
/********************************************************************************************/
LRESULT CH1EUSB_DetailsDlg::OnTuchNotification(DWORD dwStatus, LPVOID pObj)
{
	CH1EUSB_DetailsDlg* pDlg = (CH1EUSB_DetailsDlg*) pObj;
	CEdit* pEdit = (CEdit *)pDlg->GetDlgItem(IDC_EDIT_SST);

	switch(dwStatus){
		case H1USB_STATUS_TUCH_ON:
			//�^�b�`�Z���T�[�X�e�[�^�X��ON��\��
			pEdit->SetWindowText(_T("ON"));
			break;
		case H1USB_STATUS_TUCH_OFF:
			//�^�b�`�Z���T�[�X�e�[�^�X��OFF��\��
			pEdit->SetWindowText(_T("OFF"));
			break;
		case H1USB_STATUS_INVDATA:
			pEdit = (CEdit *)pDlg->GetDlgItem(IDC_EDIT_COMMENT);
			pEdit->SetWindowText(_T("Recived Invalid data"));
			break;
		default:
			break;
	}

	return 0;
}

/********************************************************************************************/
/*[���\�b�h��]	���M�I���ʒm�R�[���o�b�N�֐�												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		LRESULT OnSendEndNotification(DWORD dwStatus, LPVOID pObj)					*/
/*																							*/
/*[���e]		���M�Ɏ��Ԃ�������R�}���h�̏ꍇ�ɃR�}���h���M���I���ŃR�[���o�b�N�����	*/
/*				CH1USBCom�N���X���R�[���o�b�N�����										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���F0																	*/
/********************************************************************************************/
LRESULT CH1EUSB_DetailsDlg::OnSendEndNotification(DWORD dwStatus, LPVOID pObj)
{

	return 0;
}

/********************************************************************************************/
/*[���\�b�h��]	�^�b�`�Z���T�[ON�|�[�����O													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		BOOL PollingTuchOnState()													*/
/*																							*/
/*[���e]		�^�b�`�Z���T�[��ON���|�[�����O����											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FTRUE																*/
/********************************************************************************************/
BOOL CH1EUSB_DetailsDlg::PollingTuchOnState()
{
	BOOL bReturn;
	CString strState;
	CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SST);

	bReturn = FALSE;

	EnabeleControl(NULL, FALSE);

	int i;

	for(i=0; i<300; i++){
		if(!m_btCheckPol.GetCheck()){
			//�Z���T�[�|�[�����O�Ȃ�
			bReturn = TRUE;
			break;
		}
		DoEvents();
		Sleep(10);
		pEdit->GetWindowText(strState);
		if(strState == _T("ON")){
			//�^�b�`�Z���T�[��ON�ɂȂ���
			bReturn = TRUE;
			break;
		}
	};

	EnabeleControl(NULL, TRUE);


	return bReturn;
}


/********************************************************************************************/
/*[���\�b�h��]	�e���v���[�g��񓯊�														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int SyncDevice()															*/
/*																							*/
/*[���e]		�e���v���[�g�t�@�C���̃e���v���[�g�����f�o�C�X�ɑ��M���������s��			*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::SyncDevice()
{
	int iReturn;
	CString strComment;
	CMsgGuide msgGuide;

	iReturn = H1USB_OK;

	if(!m_pTemplateFile){
		//�e���v���[�g�t�@�C���N���X����������Ă��Ȃ�
		return H1USB_ERR_TEMP_NOTOPEN;
	}
	UINT uiTotalTempLates =	m_pTemplateFile->m_uiTotalTemplates;	//���ݓo�^����Ă���e���v���C�g��
	PTEMPLIST pTempList = m_pTemplateFile->m_pTempList;				//�e���v���C�g���X�g
	if(uiTotalTempLates == 0){
		//�o�^�ς݂̃e���v���[�g�����݂��Ȃ�
		return H1USB_OK;
	}

	//�o�^�ς݂̃e���v���[�g���f�o�C�X�ɑ��M����
	TEMPU TempU;
	UINT uiSendCounts;
	UINT ui;
	UINT uiTemps;
	UINT uiBankCount;
	UINT uiCurrentBank;
	UINT uiBanks;
	UINT uiLists;
	DWORD dwError;

	dwError  = 0;

	uiBanks = m_pTemplateFile->m_uiBanks;
	uiLists = m_pTemplateFile->m_uiTotalTemplates;

	//���ׂẴe���v���[�g���f�o�C�X���폜����
	iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_DELTEMPLATE, -2, 0);
	if(iReturn != H1USB_OK){

	}

	uiSendCounts = 0;

	//���݂���o���N�̌����A�o���N���`�F���W���Ȃ��炷�ׂẴe���v���[�g�����f�o�C�X�֑��M����
	for(uiBankCount=0; uiBankCount<uiBanks; uiBankCount++){
		uiCurrentBank = m_pTemplateFile->m_pBankInfo[uiBankCount].uiBankNum;
		uiTemps = m_pTemplateFile->m_pBankInfo[uiBankCount].uiCounts;
		//�o���N�̐ݒ�
		iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_CHANGE_BANK, (WPARAM)uiCurrentBank, 0);
		if(iReturn != H1USB_OK){
			return iReturn;
		}
		//�ݒ肵���o���N�̃e���v���[�g�t�@�C�����X�g���I�[�v��
		iReturn = m_pTemplateFile->Open(uiCurrentBank);
		if(iReturn != H1USB_OK){
			m_pTemplateFile->Close();
			return iReturn;
		}
		//�o���N�ɑ��݂���e���v���[�g�����擾
		for(ui=0; ui<uiTemps; ui++){
			iReturn = m_pTemplateFile->Read(&TempU, 1);
			if(iReturn != H1USB_OK){
				break;
			}
			//�擾�����e���v���[�g���Z�b�g
			iReturn = m_pH1USBCom->SetTempU(&TempU, 1, TRUE);
			if(iReturn != H1USB_OK){
				break;
			}
			//�e���v���[�g���M�g�����U�N�V���������s
			iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_SETTEMPLATE, 0, 0);
			if(iReturn != H1USB_OK){
				break;
			}
			//�o�^�����e���v���[�g�����R�����g�G�f�B�b�g�{�b�N�X�ɕ\��
			uiSendCounts++;
			strComment = msgGuide.SendFormatMessage(uiCurrentBank, TempU.uiTmpNum, uiSendCounts, uiLists);
			SetComment(H1USB_COMMENT_NON, strComment, iReturn);
		}
		//���݂̃o���N�̃e���v���[�g�t�@�C�����N���[�Y
		m_pTemplateFile->Close();
		if(iReturn !=H1USB_OK){
			break;
		}
	}
	//�����I�����b�Z�[�W��\��
	strComment = msgGuide.SendCompFormatMessage(uiCurrentBank, TempU.uiTmpNum, uiSendCounts, uiLists, iReturn, dwError);
	SetComment(H1USB_COMMENT_NON, strComment, iReturn);
	return iReturn;
}


/********************************************************************************************/
/*[���\�b�h��]	�R���g���[���X�V															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int UpdateContorol()														*/
/*																							*/
/*[���e]		�e�R���g���[�����A�b�v�f�[�g����											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::UpdateContorol()
{
    ListInsertItem();					//���X�g�A�C�e���}��
	Sort();								//�\�[�g�����̋N��
	DoEvents();
	m_dlgTab0.UpdateContorol();			//Tab0�p�l���_�C�A���O�X�V
	m_dlgTab1.UpdateContorol();			//Tab1�p�l���_�C�A���O�X�V
	m_dlgTab2.UpdateContorol();			//Tab2�p�l���_�C�A���O�X�V
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
void CH1EUSB_DetailsDlg::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){
		if (!AfxGetThread()->PumpMessage()){
			return;
		}
	}


}

/********************************************************************************************/
/*[���\�b�h��]	�J���[���b�Z�[�W����														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)						*/
/*																							*/
/*[���e]		�ҏW�s�̃G�f�B�b�g�{�b�N�X�̔w�i�𔒂ɕύX����							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		HBRUSH																		*/
/********************************************************************************************/
HBRUSH CH1EUSB_DetailsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ������ DC �̑�����ύX���Ă��������B
	int nID = pWnd->GetDlgCtrlID();       
	if (( nID == IDC_EDIT_SST)||( nID == IDC_EDIT_COMMENT)) {
       pDC->SetBkColor(RGB(255,255,255));
	   return (HBRUSH)GetStockObject(WHITE_BRUSH);
	}

	// TODO:  ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}

/********************************************************************************************/
/*[���\�b�h��]	�e���v���[�g���X�g�̑I��ύX���b�Z�[�W����									*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnLvnItemchangedListTmplist(NMHDR *pNMHDR, LRESULT *pResult)			*/
/*																							*/
/*[���e]		�e���v���[�g�̑I�����ꂽ�s�̃o���N�ԍ��ƃe���v���[�g�ԍ���					*/
/*				�e�G�f�B�b�g�{�b�N�X�ɂ���													*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnLvnItemchangedListTmplist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B

	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) {

		int index;
		CString strBank;
		CString strTemp;

		//�I�����ꂽ�s���擾
		index = ListView_GetNextItem(m_listCtrlTempList.m_hWnd, -1, LVNI_SELECTED);
		if(index >= 0){
			strBank = m_listCtrlTempList.GetItemText(index, 0);		//�o���N�ԍ����擾
			strTemp = m_listCtrlTempList.GetItemText(index, 1);		//�e���v���[�g�ԍ����擾
			m_dlgTab0.UpdateNummber(strBank, strTemp);				//Tab0�p�l���֒ʒm���ăA�b�v�f�[�g
			m_dlgTab1.UpdateNummber(strBank, strTemp);				//Tab1�p�l���֒ʒm���ăA�b�v�f�[�g
		}
    }

	*pResult = 0;
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
void CH1EUSB_DetailsDlg::EnabeleControl(DWORD dwMode, BOOL bEnable)
{
	CButton*	pBtComSet = (CButton*)GetDlgItem(IDC_BUTTON_COMSET);
	CButton*	pBtComMain = (CButton*)GetDlgItem(IDC_BUTTON_MAIN);
	CTabCtrl*	pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_DTAILS_TAB);

	pBtComSet->EnableWindow(bEnable);				//COM�ݒ�{�^��
	pBtComMain->EnableWindow(bEnable);				//�I���{�^��
	pTabCtrl->EnableWindow(bEnable);				//�^�u�I���{�^��

	DWORD dwFixMode = dwMode;
	if(!dwMode){
		dwFixMode = m_dwSystemCondition;
	}
	m_dlgTab0.EnabeleControl(dwFixMode, bEnable);	//Tab0�֎w��
	m_dlgTab1.EnabeleControl(dwFixMode, bEnable);	//Tab1�֎w��
	m_dlgTab2.EnabeleControl(dwFixMode, bEnable);	//Tab2�֎w��

	DoEvents();

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
int CH1EUSB_DetailsDlg::GeneralTransaction(UINT uiTransID, WPARAM wParam, LPARAM lParam,  UINT uiMsgID, UINT uiMode)
{
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	EnabeleControl(NULL, FALSE);
	
	if(uiMsgID != MSGGUID_NODISP){	
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID);
		if(!strMessage.IsEmpty()){
			//�R�}���h���M���b�Z�[�W
			SetComment(uiMode, strMessage, 0);
		}
	}
	//�g�����U�N�V�������s
	iReturn = m_pH1USBCom->TransToDevice(uiTransID, wParam, lParam);
	//�R�}���h�I�����b�Z�[�W
	if(iReturn < H1USB_OK){
		//strMessage.Format(_T("Error:%d"), iReturn);
		strMessage = msgGuide.ErrorGeneralFormatMessage(iReturn);
		SetComment(H1USB_COMMENT_ADD, strMessage, 0);
	}
	EnabeleControl(NULL, TRUE);
	
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��]	�`�惁�b�Z�[�W����															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void OnPaint()																*/
/*																							*/
/*[���e]		�`�惁�b�Z�[�W�ɂ��A�e���v���[�g���X�g�̐����X�N���[���o�[���\���ɂ���	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CH1EUSB_DetailsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CDialog::OnPaint() ���Ăяo���Ȃ��ł��������B

	//�e���v���[�g���X�g�̐����X�N���[���o�[���\���ɂ���
	m_listCtrlTempList.ShowScrollBar( SB_VERT, TRUE );

}


/********************************************************************************************/
/*[���\�b�h��]	�^�b�`�Z���T�[�L��															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void TuchEnable()																*/
/*																							*/
/*[���e]		�`�惁�b�Z�[�W�ɂ��A�e���v���[�g���X�g�̐����X�N���[���o�[���\���ɂ���	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::TuchEnable()
{
	int iReturn;
	//�^�b�`�Z���T�[�̒ʒm��������B
	//iReturn = GeneralTransaction(H1USB_COM_TUCH_NOTIFY, 0x01, 0, MSGGUID_SEND_TSEN_NOTIFY, H1USB_COMMENT_NON);
	iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_TUCH_NOTIFY, 0x01, 0);
	if(iReturn ==H1USB_OK){
		////�^�b�`�Z���T�[�̏����l��\������
		//CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SST);
		//pEdit->SetWindowText(_T("OFF"));
	}
	
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��]	�^�b�`�Z���T�[����															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void TuchDisable()															*/
/*																							*/
/*[���e]		�`�惁�b�Z�[�W�ɂ��A�e���v���[�g���X�g�̐����X�N���[���o�[���\���ɂ���	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
int CH1EUSB_DetailsDlg::TuchDisable()
{
	int iReturn;
	//�^�b�`�Z���T�[�̒ʒm��������B
	//iReturn = GeneralTransaction(H1USB_COM_TUCH_NOTIFY, 0x02, 0, MSGGUID_SEND_TSEN_NOTIFY, H1USB_COMMENT_NON);
	iReturn = m_pH1USBCom->TransToDevice(H1USB_COM_TUCH_NOTIFY, 0x02, 0);
	if(iReturn ==H1USB_OK){
		//�^�b�`�Z���T�[�̏����l��\������
		//CEdit* pEdit = (CEdit *)GetDlgItem(IDC_EDIT_SST);
		//pEdit->SetWindowText(_T(""));
	}
	
	return iReturn;
}

