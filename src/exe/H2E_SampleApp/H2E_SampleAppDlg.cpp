// H2E_SampleAppDlg.cpp : ŽÀ‘•ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "H2E_SampleApp.h"
#include "H2E_SampleAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()



CH2E_SampleAppDlg::CH2E_SampleAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CH2E_SampleAppDlg::IDD, pParent)
{
	//Initialize member's variables.
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);	
	m_pTemplateFile = new CTemplateFile;				//Create Template files Class.
	m_pH2ECom = new CH2ECom;							//Create Communication Class.
	ZeroMemory(&m_tempInfo, sizeof(m_tempInfo));
	m_bSetEnable = FALSE;
	m_pDlgTuch = 0;

}

void CH2E_SampleAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOUCH_OUT1, m_btnCheckTuchOut1);
	DDX_Control(pDX, IDC_TOUCH_OUT2, m_btnCheckTuchOut2);
}
CH2E_SampleAppDlg::~CH2E_SampleAppDlg()
{
	if(m_pDlgTuch){
		delete m_pDlgTuch;
	}
	if(m_pH2ECom){
		delete m_pH2ECom;
	}
	if(m_pTemplateFile){
		delete m_pTemplateFile;
	}
}


BEGIN_MESSAGE_MAP(CH2E_SampleAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_COMOPEN, &CH2E_SampleAppDlg::OnBnClickedButtonComopen)
	ON_BN_CLICKED(IDC_BUTTON_ENROLL, &CH2E_SampleAppDlg::OnBnClickedButtonEnroll)
	ON_BN_CLICKED(IDC_BUTTON_11VERIF, &CH2E_SampleAppDlg::OnBnClickedButton11verif)
	ON_BN_CLICKED(IDC_BUTTON_VERIF_AB, &CH2E_SampleAppDlg::OnBnClickedButtonVerifAb)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CH2E_SampleAppDlg::OnBnClickedButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CH2E_SampleAppDlg::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_DELTEMP, &CH2E_SampleAppDlg::OnBnClickedButtonDeltemp)
	ON_BN_CLICKED(IDC_BUTTON_DELGROUP, &CH2E_SampleAppDlg::OnBnClickedButtonDelgroup)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CH2E_SampleAppDlg::OnBnClickedButtonDelall)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, &CH2E_SampleAppDlg::OnBnClickedButtonCheck)
	ON_BN_CLICKED(IDC_BUTTON_END, &CH2E_SampleAppDlg::OnBnClickedButtonEnd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TMPLIST, &CH2E_SampleAppDlg::OnLvnItemchangedListTmplist)
	ON_BN_CLICKED(IDC_BUTTON_1NVERIF, &CH2E_SampleAppDlg::OnBnClickedButton1nverif)
	ON_EN_CHANGE(IDC_EDIT_ENROLL_BANK, &CH2E_SampleAppDlg::OnEnChangeEditEnrollBank)
END_MESSAGE_MAP()



BOOL CH2E_SampleAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set Small icon

	//Initialize dialog Controls.
 
	initDialogControl();

	//Enable dialog Controls.
	m_dwSystemCondition = SYSTEM_DEVICE_OFF;
	EnabeleControl(m_dwSystemCondition, TRUE);

	return TRUE;
}

void CH2E_SampleAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}


void CH2E_SampleAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CH2E_SampleAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




///////////////// Initial Part

void CH2E_SampleAppDlg::initDialogControl()
{
	CString strWork; 

	//Set Aplication name
	m_ftFName.CreateFont( 16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
		SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH,NULL);

	CStatic* pAppName = (CStatic*)GetDlgItem(IDC_STATIC_FNAME);
	strWork.FormatMessage(IDS_APP_NAME);
	pAppName->SetFont(&m_ftFName);
	pAppName->SetWindowText(strWork);

	strWork.Empty();
	//Display firmware version.
	CStatic* pSFWVert = (CStatic*)GetDlgItem(IDC_STATIC_SET_FWVER);
	pSFWVert->SetWindowText(strWork);

	//Display serial nummber
	CStatic* pStSnum = (CStatic*)GetDlgItem(IDC_STATIC_SET_SERIAL);
	pStSnum->SetWindowText(strWork);


	//Comport ComboBox
	initComComboBox();

	//Speed ComBox
	initSpeedComboBox();

	//Templates ListContorl
	initTemplateListControl();

	//Set tuch sensor default polling line.
	m_btnCheckTuchOut1.SetCheck(TRUE);
	m_btnCheckTuchOut2.SetCheck(TRUE);

}

//Comport ComboBox Seting
void CH2E_SampleAppDlg::initComComboBox()
{
	CString strComPortName;
	CComboBox *pCom = (CComboBox *)GetDlgItem(IDC_COMBO_COMPORT) ;
	DWORD dw;
	DWORD	dwCOMs;

	//Enumerate COM port classes.
	m_pH2ECom->EnumComPort(this->m_hWnd, 0, &dwCOMs);
	if(dwCOMs){
		CString* pstrCom = new CString[dwCOMs];
		m_pH2ECom->EnumComPort(this->m_hWnd, pstrCom, &dwCOMs);
		for(dw=0; dw<dwCOMs; dw++){
			pCom->InsertString(-1, pstrCom[dw]);
		}
	}
	else{
		//Insert COM port names to ComboBox.
		for (dw = 1; dw <= 256; dw++ ) {	
			strComPortName.Format(_T("COM%d"), dw);
			pCom->InsertString(-1, strComPortName) ;
		}
	}
	pCom->SetCurSel(0) ;
}

//Initialize COM Speed ComboBox.
void CH2E_SampleAppDlg::initSpeedComboBox()
{
	CString strComPortName;
	CComboBox *pCom = (CComboBox *)GetDlgItem(IDC_COMBO_SPEED) ;
	pCom->InsertString(-1, _T("57,600"));
	pCom->InsertString(-1, _T("19,200"));
	pCom->SetCurSel(0) ;

}




//Templates ListContorl Setting.
void CH2E_SampleAppDlg::initTemplateListControl()
{
	formatTemplateListControl();
	insertTenmplateListControl();
	renumberItemTemplateListControl();
	sortTemplateListControl();
}



//Listview Controt Formating
int CH2E_SampleAppDlg::formatTemplateListControl()
{
    LVCOLUMN    lvc;
    int         i;
	TCHAR*      caption[] = {_T("Group"), _T("Template"), _T("QLev.") ,_T("Time")};
	int			iWidth[] = {50,50,50,120};
	CListCtrl* pLCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_TMPLIST) ;
    const int   clmNum = sizeof caption /sizeof(TCHAR*) ;
    int         err = 0;

	pLCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);

    for (i = 0; i < clmNum; i++)
    {
		lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;	// falg
		lvc.fmt			= LVCFMT_CENTER;								// format
		lvc.iSubItem    = i;											// subitem nummber
        lvc.pszText     = caption[i];									// display text
        lvc.cx          = iWidth[i];									// width
        if (pLCtrl->InsertColumn(i, &lvc) == -1)
		{
			err = 1;
			break;
		}
    }
    
    return err;
}


//Insert item to Listview Cotrol
int CH2E_SampleAppDlg::insertTenmplateListControl()
{
 
    LVITEM       lvi;
    CString      str;
    int          i, index = 0;
    int          err = 0;
	CString		strBankNum;
	CString		strTempNum;
	CString		strQLevel;
	CString		strTime;    
	CListCtrl* pLCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_TMPLIST) ;

	pLCtrl->DeleteAllItems();


	int iLists;
	iLists =m_tempInfo.dwTemplates;

	CStatic* pSta = (CStatic*)GetDlgItem(IDC_STATIC_TEMPS);
	str.Format(_T("[%5d]"), iLists);
	pSta->SetWindowText(str);

    for (i = 0; i < iLists; i++)
    {
        lvi.mask = LVIF_TEXT;
        // Group nummber
        if (!err)
        {
            lvi.iItem = i;
            lvi.iSubItem = 0;
			strBankNum.Format(_T("%04X"), m_tempInfo.pTempList[i].uiGroupNum);
            lvi.pszText = strBankNum.GetBuffer();
            if ((index = pLCtrl->InsertItem(&lvi)) == -1) err = 1;
        }
        // template number
        if (!err)
        {
            lvi.iItem = index;
            lvi.iSubItem = 1;
			strTempNum.Format(_T("%04X"), m_tempInfo.pTempList[i].uiTempNum);
            lvi.pszText = strTempNum.GetBuffer();
            if (!pLCtrl->SetItem(&lvi)) err = 1;
        }
        // Quality Level
        if (!err)
        {
            lvi.iItem = index;
            lvi.iSubItem = 2;
			if(m_tempInfo.pTempList[i].uiQLevel == 0xFF){
				strQLevel.Empty();
			}
			else{
				strQLevel.Format(_T("%d"),m_tempInfo.pTempList[i].uiQLevel);
			}
            lvi.pszText = strQLevel.GetBuffer();
            if (!pLCtrl->SetItem(&lvi)) err = 1;
        }
        // Enrolled time.
        if (!err)
        {
            lvi.iItem = index;
            lvi.iSubItem = 3;
			if(m_tempInfo.pTempList[i].time.wYear == 0x0000){
				strTime.Empty();
			}
			else{
				strTime.Format(_T("%4d/%02d/%02d %02d:%02d:%02d"), 
								m_pTemplateFile->m_pTempList[i].time.wYear, 
								m_pTemplateFile->m_pTempList[i].time.wMonth, 
								m_pTemplateFile->m_pTempList[i].time.wDay, 
								m_pTemplateFile->m_pTempList[i].time.wHour,
								m_pTemplateFile->m_pTempList[i].time.wMinute,
								m_pTemplateFile->m_pTempList[i].time.wMinute);
			}
            lvi.pszText = strTime.GetBuffer();
            if (!pLCtrl->SetItem(&lvi)) err = 1;
        }
        if (err) break;
		DoEvents();

    }
    
    return err;
}



//Renumber Listvie item
void CH2E_SampleAppDlg::renumberItemTemplateListControl()
{
	LV_ITEM lvItem;
	CListCtrl* pLCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_TMPLIST) ;

	for( int i = 0; i < pLCtrl->GetItemCount(); i++ ) {
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		lvItem.mask = LVIF_PARAM;
		lvItem.lParam = i;	
		pLCtrl->SetItem(&lvItem);
	}
}



//Sorting Listview item
void CH2E_SampleAppDlg::sortTemplateListControl() 
{
	CListCtrl* pLCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_TMPLIST) ;
	if(m_pTemplateFile->m_uiTotalTemplates>0){
		renumberItemTemplateListControl();
		pLCtrl->SortItems(CompareFuncTemplateListControl, FALSE);
	}
}

//Listview callback function
int CALLBACK CH2E_SampleAppDlg::CompareFuncTemplateListControl(LPARAM param1, LPARAM param2, LPARAM param3)
{

	long lNUm1, lNUm2;
    char *endptr;
	int iReturn;
	iReturn = 0;

	CWinApp* app = AfxGetApp();
	CH2E_SampleAppDlg* pdlg = (CH2E_SampleAppDlg*)app->GetMainWnd();
	CListCtrl* pLCtrl = (CListCtrl*)pdlg->GetDlgItem(IDC_LIST_TMPLIST) ;

	CString str1 = pLCtrl->GetItemText(param1, 0);
	lNUm1 = strtol(str1.GetString(), &endptr, 16);
	lNUm1 = lNUm1*0x10000;
	CString str2 = pLCtrl->GetItemText(param2, 0);
	lNUm2 = strtol(str2.GetString(), &endptr, 16);
	lNUm2 = lNUm2*0x10000;
	str1 = pLCtrl->GetItemText(param1, 1);
	lNUm1 += strtol(str1.GetString(), &endptr, 16);
	str2 =pLCtrl->GetItemText(param2, 1);
	lNUm2 += strtol(str2.GetString(), &endptr, 16);

	if(lNUm1>lNUm2){
		iReturn = 1;
	}
	if(lNUm1<lNUm2){
		iReturn = -1;
	}

	return iReturn;
}


/////////////////  Initial Part End
void CH2E_SampleAppDlg::OnBnClickedButtonComopen()
{
	int iReturn;
	DWORD dwError;
	CString strComName;
	CString strComment;
	CMsgGuide msgGuide;
	CString strMessage;
	UINT uiSpeed;
	int iSpeed;

	iReturn = H2E_OK;

	m_dwSystemCondition = SYSTEM_DEVICE_OFF;
	EnabeleControl(m_dwSystemCondition, FALSE);

	//Get COM port name
	CComboBox *pCom = (CComboBox *)GetDlgItem(IDC_COMBO_COMPORT);
	pCom->GetWindowText(strComName);

	//Get Transmit Speed
	CComboBox *pSpeed = (CComboBox *)GetDlgItem(IDC_COMBO_SPEED);
	iSpeed = pSpeed->GetCurSel();
	uiSpeed = H2E_SPEED_57600;
	if(iSpeed > 0){
		uiSpeed = H2E_SPEED_19200;
	}

	//COM port open
	iReturn = m_pH2ECom->Open(strComName, uiSpeed, &dwError);
	if(iReturn != H2E_OK){
		strComment.FormatMessage(IDS_ERR_COMOPEN, strComName);
		SetComment(H2E_COMMENT_PLUS, strComment, dwError);
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;
	}
	//Display COM port name.
	strComment.FormatMessage(IDS_COMOPEN, strComName);
	SetComment(H2E_COMMENT_NON, strComment, dwError);
	m_dwSystemCondition = SYSTEM_DEVICE_ON;

	//Send Device info command(firmware version).
	iReturn = GeneralTransaction
				(H2E_COM_GETDEVINFO, 0x00, NULL, 0, MSGGUID_SEND_GETDEVINFO, H2E_COMMENT_ADD);
	if(iReturn < H2E_OK){
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;		
	}
	//Display firmware version.
	CStatic* pSFWVert = (CStatic*)GetDlgItem(IDC_STATIC_SET_FWVER);
	pSFWVert->SetWindowText(m_pH2ECom->m_cFwVersion);

	//Send Device info command(serial nummber).
	iReturn = GeneralTransaction
				(H2E_COM_GETDEVINFO, 0x01, NULL, 0, MSGGUID_SEND_GETDEVINFO, H2E_COMMENT_ADD);
	if(iReturn < H2E_OK){
		m_dwSystemCondition = SYSTEM_DEVICE_ERR;
		EnabeleControl(m_dwSystemCondition, TRUE);
		return;		
	}
	//Display serial nummber
	CStatic* pStSnum = (CStatic*)GetDlgItem(IDC_STATIC_SET_SERIAL);
	pStSnum->SetWindowText(m_pH2ECom->m_cSerialNum);


	//Get template from managetable.
	DWORD dwTemplates;
	PTEMPLIST pTempLateList;
	m_pTemplateFile->GetTemplateLists(&dwTemplates, &pTempLateList);
	m_tempInfo.dwTemplates = dwTemplates;
	m_tempInfo.pTempList = pTempLateList;
	//Send Read Template Command.
	iReturn = GeneralTransaction
				(H2E_COM_GET_TMP_INFO, 0x00, &m_tempInfo, 0, MSGGUID_SEND_GET_TMPINFO, H2E_COMMENT_ADD);
	if(iReturn != H2E_OK){
		return;
	}
	//intial Listview Cotorl.
	m_pTemplateFile->InitialTemplateLists(&m_tempInfo);

	//Update controls setting.
	UpdateContorol();

	//Enable Contorls.
	EnabeleControl(m_dwSystemCondition, TRUE);

	//Display End Setting message. 
	strMessage = msgGuide.EndAppSettingMessage(strComName);
	SetComment(H2E_COMMENT_NON, strMessage, 0);

}




void CH2E_SampleAppDlg::OnBnClickedButtonEnroll()
{
	UINT uiGroup;
	UINT uiTemp;
	CString strGroup;
	CString strTmpNo;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	int	iReturn;

	iReturn = H2E_OK;

	//Get Group nummber & Template Nummber
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	pEditBank->GetWindowText(strGroup);
	if(!nb.ConvertBankNummber(strGroup, &uiGroup)){
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_TMPNO);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}

	//Open Tuch Sensor Panel
	OpenTuchSensorPanel();

	int i;
	for(i=0; i<2; i++){
		//Display "place finger message"
		strMessage = msgGuide.GuidePlaceFingerFormatMessage(TRUE, i+1, 2);
		SetComment(H2E_COMMENT_NON, strMessage, 0);

		//Wait Tuch Sensor State!
		iReturn = WaitTuchSensorState();
		if(iReturn != H2E_OK){
			//Polling timeout!!
			if(i > 0){
				//Do dummy enroll opelation.
				enrollOpelation(uiGroup, uiTemp, 1, TRUE);
			}
			//display Timeout message
			strMessage.FormatMessage(IDS_POLLING_TIMEOUT);
			SetComment(H2E_COMMENT_NON, strMessage, 0);
			break;
		}

		//Enroll sequence
		iReturn = enrollOpelation(uiGroup, uiTemp, i, FALSE);
		if(iReturn < H2E_OK){
			strMessage = msgGuide.EnrollNGMessage();
			SetComment(H2E_COMMENT_ADD, strMessage, 0);
			break;
		}
//		Sleep(700);

		if (i == 0) {
			MessageBox(msgGuide.GuidePlaceFingerFormatMessage(FALSE, 1, 2));
		}
	}

	//Close Tuch Sensor Panel
	CloseTuchSensorPanel();

	if(iReturn < H2E_OK){
		return;
	}

	TEMPU tempU;
	SYSTEMTIME	st;
	GetLocalTime(&st);

	//Add template infomation into template manage table.
	uiGroup = m_pH2ECom->m_wEnroll_Group;
	tempU.uiTmpNum = m_pH2ECom->m_wEnroll_Template;
	tempU.uiQLevel = m_pH2ECom->m_byEnroll_QLevel;
	tempU.time = st;
	m_pTemplateFile->AddTemplateList(uiGroup, &tempU);

	//get template infomation.
	DWORD dwTemplates;
	PTEMPLIST pTempLateList;
	m_pTemplateFile->GetTemplateLists(&dwTemplates, &pTempLateList);
	m_tempInfo.dwTemplates = dwTemplates;
	m_tempInfo.pTempList = pTempLateList;

	//Display Comlete message.
	strMessage.FormatMessage(IDS_ENROLL_COMPLETE);
	strWork.FormatMessage(IDS_GROUPTEMP_INFO, uiGroup, uiTemp, m_pH2ECom->m_byEnroll_QLevel);
	strMessage += strWork;
	SetComment(H2E_COMMENT_NON, strMessage, 0);

	//Update Contorls
	UpdateContorol();
}

int CH2E_SampleAppDlg::enrollOpelation(UINT uiGroup, UINT uiTemplate, UINT uiTimes, BOOL bCheck)
{
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	WORD wOption[2];
	BYTE byParam;
	UINT uiCounts;
	UINT uiFixTimes;

	iReturn = H2E_OK;
	byParam = 0x84 | uiTimes;
	uiFixTimes = uiTimes + 1;
	uiCounts = 2;
	if(bCheck){
		uiCounts = 1;
		if(uiTimes > 0){
			byParam = 0xFF;
			uiFixTimes = uiTimes;
		}
	}
	wOption[0] = uiGroup;
	wOption[1] = uiTemplate;

	//Send Enroll Command.
	iReturn = GeneralTransaction
				(H2E_COM_TMP_ENROLL, byParam, wOption, sizeof(wOption), MSGGUID_SEND_ENROLL, H2E_COMMENT_UOP);
	if(iReturn == H2E_TAKEEND){
		//Get response from device.
		iReturn = GeneralTransaction
				(H2E_COM_SENDNOP, 0x00, wOption, sizeof(wOption), MSGGUID_NODISP, H2E_COMMENT_UOP);
		if(iReturn < H2E_OK){
			return iReturn;
		}

		//Display "releace finger message"
		strMessage = msgGuide.GuidePlaceFingerFormatMessage(FALSE,uiFixTimes, uiCounts);
		SetComment(H2E_COMMENT_NON, strMessage, 0);
	}
	return iReturn;
}

void CH2E_SampleAppDlg::OnBnClickedButton11verif()
{
	UINT uiBank;
	UINT uiTemp;
	CString strBank;
	CString strTmpNo;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	int iReturn;

	//Get Group nummber & Template Nummber
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_11VERBANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_11VERTEMP);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}

	//Open tuch sensor panel
	OpenTuchSensorPanel();



	//Display "place finger message"
	strMessage = msgGuide.GuidePlaceFingerFormatMessage(TRUE, 0, 0);
	SetComment(H2E_COMMENT_NON, strMessage, 0);

	//Wait Tuch Sensor State!
	iReturn = WaitTuchSensorState();
	if(iReturn != H2E_OK){
		//display Timeout message
		strMessage.FormatMessage(IDS_POLLING_TIMEOUT);
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		CloseTuchSensorPanel();
		return;
	}

	WORD wOption[2];
	wOption[0] = uiBank;
	wOption[1] = uiTemp;
	//Send verification command(1:1)
	iReturn = GeneralTransaction
				(H2E_COM_VER1TO1, 0x80, wOption, sizeof(wOption), MSGGUID_SEND_VERIF_11, H2E_COMMENT_UOP);

	//Close tuch sensor panel
	CloseTuchSensorPanel();

	if(iReturn != H2E_OK){
		strMessage = msgGuide.VerificationNGMessage();
		SetComment(H2E_COMMENT_ADD, strMessage, 0);
		return;
	}

	//Display "Commplete" message
	strMessage.FormatMessage(IDS_VERIF_COMPLETE);
	strWork.FormatMessage(IDS_VERIF_INFO, uiBank, uiTemp, m_pH2ECom->m_byVerifLevel);
	strMessage += strWork;
	SetComment(H2E_COMMENT_NON, strMessage, 0);


}

void CH2E_SampleAppDlg::OnBnClickedButton1nverif()
{
	UINT uiBank;
	CString strBank;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	int iReturn;

	//Get Group nummber
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_N1BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}


	OpenTuchSensorPanel();


	//Display "place finger message"
	strMessage = msgGuide.GuidePlaceFingerFormatMessage(TRUE, 0, 0);
	SetComment(H2E_COMMENT_NON, strMessage, 0);

	//Wait Tuch Sensor State!
	iReturn = WaitTuchSensorState();
	if(iReturn != H2E_OK){
		//display Timeout message
		strMessage.FormatMessage(IDS_POLLING_TIMEOUT);
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		CloseTuchSensorPanel();
		return;
	}


	WORD wOption;
	wOption = uiBank;
	//Send verification command(1:N).
	iReturn = GeneralTransaction
				(H2E_COM_VER_G_1TON, 0x80, &wOption, sizeof(wOption), MSGGUID_SEND_VERIF_1N_GROUP, H2E_COMMENT_UOP);
	
	CloseTuchSensorPanel();

	if(iReturn != H2E_OK){
		strMessage = msgGuide.VerificationNGMessage();
		SetComment(H2E_COMMENT_ADD, strMessage, 0);
		return;
	}

	//Display "Commplete" message
	strMessage.FormatMessage(IDS_VERIF_COMPLETE);
	strWork.FormatMessage(IDS_VERIF_INFO, uiBank, m_pH2ECom->m_wVerifTemp, m_pH2ECom->m_byVerifLevel);
	strMessage += strWork;
	SetComment(H2E_COMMENT_NON, strMessage, 0);


}


void CH2E_SampleAppDlg::OnBnClickedButtonVerifAb()
{
	UINT uiSBank;
	UINT uiEBank;
	CString strSBank;
	CString strEBank;


	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	int iReturn;

	//Get Start Group Nummber & End Group Nummber
	CEdit *pEditSBank = (CEdit *)GetDlgItem(IDC_EDIT_STARTBANK);
	pEditSBank->GetWindowText(strSBank);
	if(!nb.ConvertBankNummber(strSBank, &uiSBank)){
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}

	CEdit *pEditEBank = (CEdit *)GetDlgItem(IDC_EDIT_ENDBANK);
	pEditEBank->GetWindowText(strEBank);
	if(!nb.ConvertBankNummber(strEBank, &uiEBank)){
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}

	int iGroups = uiEBank - uiSBank + 1;
	if(iGroups < 0){
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}

	DWORD dwTemplates;
	PTEMPLIST pTempLateList;
	m_pTemplateFile->GetTemplateLists(&dwTemplates, &pTempLateList);
	m_tempInfo.dwTemplates = dwTemplates;
	m_tempInfo.pTempList = pTempLateList;


	UINT uiTGroup;
	DWORD dwCounts;
	DWORD dw;
	WORD* pwOption = new WORD[iGroups];
	uiTGroup = uiSBank;
	dwCounts = 0;

	for(; uiTGroup<=uiEBank; uiTGroup++){
		for(dw=0; dw<dwTemplates; dw++){
			if(pTempLateList[dw].uiGroupNum == uiTGroup){
				pwOption[dwCounts++] = uiTGroup;
				break;
			}
		}
	}
	if(dwCounts > 0){
		OpenTuchSensorPanel();

		//Display "Please place finger" message
		strMessage = msgGuide.GuidePlaceFingerFormatMessage(TRUE, 0, 0);
		SetComment(H2E_COMMENT_NON, strMessage, 0);

		//Wait Tuch Sensor State!
		iReturn = WaitTuchSensorState();
		if(iReturn != H2E_OK){
			//display Timeout message
			strMessage.FormatMessage(IDS_POLLING_TIMEOUT);
			SetComment(H2E_COMMENT_NON, strMessage, 0);
			CloseTuchSensorPanel();
			if(pwOption){
				delete[] pwOption;
			}
			return;
		}

		iReturn = GeneralTransaction
					(H2E_COM_VER_G_1TON, 0x80, pwOption, (WORD)(sizeof(WORD)*dwCounts), MSGGUID_SEND_VERIF_1N_GROUP, H2E_COMMENT_UOP);
		if(pwOption){
			delete[] pwOption;
		}

		CloseTuchSensorPanel();

		if(iReturn == H2E_OK){
			strMessage.FormatMessage(IDS_VERIF_COMPLETE);
			strWork.FormatMessage(IDS_VERIF_INFO, m_pH2ECom->m_wVerifGroup, m_pH2ECom->m_wVerifTemp, m_pH2ECom->m_byVerifLevel);
			strMessage += strWork;
			SetComment(H2E_COMMENT_NON, strMessage, 0);
		}
		else{
			strMessage = msgGuide.VerificationNGMessage();
			SetComment(H2E_COMMENT_ADD, strMessage, 0);
		}
	}
	else{
		strMessage.FormatMessage(IDS_TEMP_NON);
		SetComment(H2E_COMMENT_NON, strMessage, 0);
	}
}

void CH2E_SampleAppDlg::OnBnClickedButtonUpload()
{
	int iReturn;
	CString strFileName;
	CString strMessage;
	CString strWork;

	DWORD dwTemplates;
	PTEMPLIST pTempLateList;

	//Get Template infomation.
	m_pTemplateFile->GetTemplateLists(&dwTemplates, &pTempLateList);
	m_tempInfo.dwTemplates = dwTemplates;
	m_tempInfo.pTempList = pTempLateList;
	if(dwTemplates == 0){
		strMessage.FormatMessage(IDS_TEMP_NON);
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}

	//Get Upload file path
	strFileName = m_pTemplateFile->GetFileName(FALSE);
	if(strFileName.IsEmpty()){
		return;
	}

	PTEMPU pTempu = new TEMPU[dwTemplates];
	DWORD dw;
	WORD wOption[2];
	for(dw=0; dw<dwTemplates; dw++){
		wOption[0] = pTempLateList[dw].uiGroupNum;
		wOption[1] = pTempLateList[dw].uiTempNum;

		//Send Read Template Comand.
		iReturn = GeneralTransaction
					(H2E_COM_READ_TMP, 0x00, wOption, sizeof(wOption), MSGGUID_SEND_READ_TMP, H2E_COMMENT_NON);
		if(iReturn != H2E_OK){
			break;
		}
		m_pH2ECom->GetTempU(&pTempu[dw]);
		strMessage.FormatMessage(IDS_READ_COMPLETE);
		strWork.FormatMessage(IDS_DEL_INFO, wOption[0], wOption[1]);
		strMessage += strWork;
		SetComment(H2E_COMMENT_ADD, strMessage, 0);

	}
	if(iReturn == H2E_OK){
		//Save Template in file.
		m_pTemplateFile->Open(1, strFileName);
		m_pTemplateFile->Write(pTempu, dwTemplates);
		m_pTemplateFile->Close();
		strMessage.FormatMessage(IDS_UPLOAD_COMPLETE);
		SetComment(H2E_COMMENT_ADD, strMessage, 0);
	}
	if(pTempu){
		delete[] pTempu;
	}

}

void CH2E_SampleAppDlg::OnBnClickedButtonDownload()
{
	int iReturn;
	CString strFileName;
	DWORD dwTemplates;
	PTEMPLIST pTempLateList;
	CString strMessage;
	CString strWork;


	//Get Download  File Path.
	strFileName = m_pTemplateFile->GetFileName(TRUE);
	if(strFileName.IsEmpty()){
		return;
	}
	m_pTemplateFile->GetTemplateLists(&dwTemplates, &pTempLateList);
	m_tempInfo.dwTemplates = dwTemplates;
	m_tempInfo.pTempList = pTempLateList;

	ULONGLONG ullFileSize;
	//Get template file size.
	iReturn = m_pTemplateFile->GetFileSize(strFileName, &ullFileSize);
	if(iReturn){
		return;
	}

	//Load Template from template file.
	DWORD dwTemps = (DWORD)(ullFileSize/sizeof(TEMPU));
	PTEMPU pTempU = new TEMPU[dwTemps];
	iReturn = m_pTemplateFile->Open(0, strFileName);			//Open template file.
	if(iReturn != H2E_OK){
		return;
	}
	m_pTemplateFile->Read(pTempU, dwTemps);						//Read Template/
	m_pTemplateFile->Close();									//Close template file.

	WORD wOption[2];
	DWORD dw;
	DWORD dw2;
	for(dw=0; dw<dwTemps; dw++){
		wOption[0] = pTempU[dw].uiGroupNum;
		wOption[1] = pTempU[dw].uiTmpNum;
		for(dw2=0; dw2<dwTemplates; dw2++){
			if((pTempLateList[dw2].uiGroupNum == pTempU[dw].uiGroupNum)
				&& (pTempLateList[dw2].uiTempNum == pTempU[dw].uiTmpNum)){
				break;
			}

		}
		if(dw2<dwTemplates){
			//Send delete template command.
			iReturn = GeneralTransaction
						(H2E_COM_TMP_DELETE, 0x00, wOption, sizeof(wOption), MSGGUID_SEND_DELTMP, H2E_COMMENT_NON);
			if(iReturn == H2E_OK){
				m_pTemplateFile->DeleteOneTemplateList(pTempU[dw].uiGroupNum, pTempU[dw].uiTmpNum);
			}
		}

		//Send wtite template command
		iReturn = GeneralTransaction
					(H2E_COM_WRITE_TMPC2, 0x00, &pTempU[dw], sizeof(TEMPU), MSGGUID_SEND_WRITE_TMP, H2E_COMMENT_NON);
		if(iReturn != H2E_OK){
			break;
		}
		//Add Template infomation to template manage table.
		m_pTemplateFile->AddTemplateList(pTempU[dw].uiGroupNum, &pTempU[dw]);

		strMessage.FormatMessage(IDS_WRITE_COMPLETE);
		strWork.FormatMessage(IDS_DEL_INFO, wOption[0], wOption[1]);
		strMessage += strWork;
		SetComment(H2E_COMMENT_ADD, strMessage, 0);

	}
	if(pTempU){
		delete[] pTempU;
	}
	m_pTemplateFile->GetTemplateLists(&dwTemplates, &pTempLateList);
	m_tempInfo.dwTemplates = dwTemplates;
	m_tempInfo.pTempList = pTempLateList;

	strMessage.FormatMessage(IDS_DOWNLOAD_COMPLETE);
	SetComment(H2E_COMMENT_ADD, strMessage, 0);

	UpdateContorol();
}

void CH2E_SampleAppDlg::OnBnClickedButtonDeltemp()
{
	UINT uiGroup;
	UINT uiTemp;
	CString strGroup;
	CString strTmpNo;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	int iReturn;

	//Get Group Nummber & Template Nummber.
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_BANK);
	pEditBank->GetWindowText(strGroup);
	if(!nb.ConvertBankNummber(strGroup, &uiGroup)){
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}
	CEdit *pEditTemp = (CEdit *)GetDlgItem(IDC_EDIT_DELONE_TEMP);
	pEditTemp->GetWindowText(strTmpNo);
	if(!nb.ConvertTemlateNummber(strTmpNo, &uiTemp)){
		strMessage = msgGuide.GuideTemplateNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}

	WORD wOption[2];
	wOption[0] = uiGroup;
	wOption[1] = uiTemp;
	//Send delete template command to device.
	iReturn = GeneralTransaction
				(H2E_COM_TMP_DELETE, 0x00, wOption, sizeof(wOption), MSGGUID_SEND_DELTMP, H2E_COMMENT_NON);
	if(iReturn != H2E_OK){
		return;
	}

	//Delete template infomation in template manage table.
	m_pTemplateFile->DeleteOneTemplateList(uiGroup, uiTemp);

	DWORD dwTemplates;
	PTEMPLIST pTempLateList;
	m_pTemplateFile->GetTemplateLists(&dwTemplates, &pTempLateList);
	m_tempInfo.dwTemplates = dwTemplates;
	m_tempInfo.pTempList = pTempLateList;


	strMessage.FormatMessage(IDS_DEL_COMPLETE_TEMP);
	strWork.FormatMessage(IDS_DEL_INFO, uiGroup, uiTemp);
	strMessage += strWork;
	SetComment(H2E_COMMENT_NON, strMessage, 0);

	UpdateContorol();

}

void CH2E_SampleAppDlg::OnBnClickedButtonDelgroup()
{
	UINT uiGroup;
	CString strGroup;
	CNumber nb;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	int iReturn;

	//Get Group Nummber.
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_DELBANK);
	pEditBank->GetWindowText(strGroup);
	if(!nb.ConvertBankNummber(strGroup, &uiGroup)){
		strMessage = msgGuide.GuideBankNummberFormatMessage();
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		return;
	}

	WORD wOption;
	wOption = uiGroup;
	//Send delete group command.
	iReturn = GeneralTransaction
				(H2E_COM_TMP_G_DELETE, 0x00, &wOption, sizeof(wOption), MSGGUID_SEND_DELTMP_GROUP, H2E_COMMENT_NON);
	if(iReturn != H2E_OK){
		return;
	}

	//Delete group in tempalte manage table.
	m_pTemplateFile->DeleteGroup(uiGroup);
	DWORD dwTemplates;
	PTEMPLIST pTempLateList;
	m_pTemplateFile->GetTemplateLists(&dwTemplates, &pTempLateList);
	m_tempInfo.dwTemplates = dwTemplates;
	m_tempInfo.pTempList = pTempLateList;


	strMessage.FormatMessage(IDS_DEL_COMPLETE_TEMP);
	strWork.FormatMessage(IDS_DEL_INFO2, uiGroup);
	strMessage += strWork;
	SetComment(H2E_COMMENT_NON, strMessage, 0);


	UpdateContorol();
}

void CH2E_SampleAppDlg::OnBnClickedButtonDelall()
{
	int iReturn;

	BYTE byOption = 0xC2;
	//Send initial template command.
	iReturn = GeneralTransaction
				(H2E_COM_TMP_TMP_INIT, 0x03, &byOption, 1, MSGGUID_SEND_DELTMP_ALL, H2E_COMMENT_NON);
	if(iReturn != H2E_OK){
		return;
	}

	m_tempInfo.dwTemplates = 0;

	//Initialaize template manage table. 
	m_pTemplateFile->InitialTemplateLists(&m_tempInfo);

	CString strMessage;
	strMessage.FormatMessage(IDS_DEL_COMPLETE_ALL);
	SetComment(H2E_COMMENT_NON, strMessage, 0);

	UpdateContorol();

}

void CH2E_SampleAppDlg::OnBnClickedButtonCheck()
{
	int iReturn;
	UINT uiGroup;
	UINT uiTemp;
	CString strMessage;
	CString strWork;
	CMsgGuide msgGuide;

	WORD wOption[2];

	wOption[0] = 0x8000;
	wOption[1] = 0x0000;
	uiGroup = 0x8000;
	uiTemp = 0x0000;

	//Open Touch Sensor Panel
	OpenTuchSensorPanel();

	//Display "Please place finger message"
	strMessage = msgGuide.GuidePlaceFingerFormatMessage(TRUE, 1, 1);
	SetComment(H2E_COMMENT_NON, strMessage, 0);

	//Wait Touch Sensor State!
	iReturn = WaitTuchSensorState();
	if(iReturn != H2E_OK){
		//display Timeout message
		strMessage.FormatMessage(IDS_POLLING_TIMEOUT);
		SetComment(H2E_COMMENT_NON, strMessage, 0);
		CloseTuchSensorPanel();		
		return;
	}

	//Do enroll opelation.
	iReturn = enrollOpelation(uiGroup, uiTemp, 0, TRUE);

	//Close Touch Sensor Panel.
	CloseTuchSensorPanel();

	if(iReturn < H2E_OK){
		return;
	}

	//Do dummy enroll opelation.
	enrollOpelation(uiGroup, uiTemp, 1, TRUE);

	strMessage.FormatMessage(IDS_ENROLL_COMPLETE);
	strWork.FormatMessage(IDS_GROUPTEMP_INFO, uiGroup, uiTemp, m_pH2ECom->m_byEnroll_QLevel);
	strMessage += strWork;
	SetComment(H2E_COMMENT_NON, strMessage, 0);

}

void CH2E_SampleAppDlg::OnBnClickedButtonEnd()
{
	CDialog::OnOK();
}

void CH2E_SampleAppDlg::OnLvnItemchangedListTmplist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CListCtrl* pLCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_TMPLIST) ;

	if ((pNMLV->uNewState & LVIS_SELECTED) && !(pNMLV->uOldState & LVIS_SELECTED)) {

		int index;
		CString strBank;
		CString strTemp;

		index = ListView_GetNextItem(pLCtrl->m_hWnd, -1, LVNI_SELECTED);
		if(index >= 0){
			strBank = pLCtrl->GetItemText(index, 0);
			strTemp = pLCtrl->GetItemText(index, 1);
			UpdateNummber(strBank, strTemp);
		}
    }
	*pResult = 0;
}

///////////////// Commmon Part
int CH2E_SampleAppDlg::GeneralTransaction(UINT uiTransID, BYTE byParam, LPVOID pOption,  WORD wOptionLen, UINT uiMsgID, UINT uiMode)
{
	int iReturn;
	CMsgGuide msgGuide;
	CString strMessage;
	CString strWork;
	DWORD	dwError;

	dwError = 0;
	EnabeleControl(NULL, FALSE);
	if((uiMsgID != MSGGUID_NODISP) && (!(uiMode & H2E_COMMENT_UOP))){	
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID, byParam);
		if(!strMessage.IsEmpty()){
			SetComment(uiMode, strMessage, 0);
		}
	}
	//Send command to device.
	iReturn = m_pH2ECom->TransToDevice(this->m_hWnd, uiTransID, byParam, pOption, wOptionLen, &dwError);
	if(uiMode&H2E_COMMENT_UOP){
		strMessage = msgGuide.SendGeneralFormatMessage(uiMsgID, 0);
		if(!strMessage.IsEmpty()){
			//Display comment.
			SetComment(uiMode, strMessage, 0);
		}
	}
	if(iReturn < H2E_OK){
		strMessage.Format(_T("Error:%d[From device:%02xh]"), iReturn, m_pH2ECom->m_DevErrCode);
		SetComment(H2E_COMMENT_ADD, strMessage, 0);
	}
	EnabeleControl(NULL, TRUE);
	
	return iReturn;
}

void CH2E_SampleAppDlg::UpdateNummber(CString strBank,CString  strTemp)
{
	CWnd* pWnd;
	int iCtrlID;

	pWnd = GetFocus();
	if(pWnd){
		iCtrlID = pWnd->GetDlgCtrlID();
		CEdit *pEditStartBank = (CEdit *)GetDlgItem(IDC_EDIT_STARTBANK);
		CEdit *pEditEndBank = (CEdit *)GetDlgItem(IDC_EDIT_ENDBANK);
		if(iCtrlID == IDC_EDIT_STARTBANK){
			pEditStartBank->SetWindowText(strBank);
			return;
		}
		if(iCtrlID == IDC_EDIT_ENDBANK){
			pEditEndBank->SetWindowText(strBank);
			return;
		}
	}


	CEdit* pEdiGroupt;
	CEdit* pEditTemp;

	pEdiGroupt = (CEdit*)GetDlgItem(IDC_EDIT_11VERBANK);
	pEditTemp = (CEdit*)GetDlgItem(IDC_EDIT_11VERTEMP);
	pEdiGroupt->SetWindowText(strBank);
	pEditTemp->SetWindowText(strTemp);

	pEdiGroupt = (CEdit*)GetDlgItem(IDC_EDIT_N1BANK);
	pEdiGroupt->SetWindowText(strBank);

	pEdiGroupt = (CEdit*)GetDlgItem(IDC_EDIT_DELONE_BANK);
	pEditTemp = (CEdit*)GetDlgItem(IDC_EDIT_DELONE_TEMP);
	pEdiGroupt->SetWindowText(strBank);
	pEditTemp->SetWindowText(strTemp);

	pEdiGroupt = (CEdit*)GetDlgItem(IDC_EDIT_DELBANK);
	pEdiGroupt->SetWindowText(strBank);
}

void CH2E_SampleAppDlg::SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError)
{
	CString strComment;
	CString strErr;
	CString strWork;

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_COMMENT);
	strComment	= pMessage;
	strErr.Format(_T("[Error=%08Xh]"),dwError);
	strWork.Empty();
	if(dwMode&H2E_COMMENT_ADD){
		pEdit->GetWindowText(strWork);
		strWork += _T("\r\n");
	}
	strComment = strWork + pMessage;
	if(dwMode&H2E_COMMENT_PLUS){
		strComment += strErr;
	}
	pEdit->SetWindowText(strComment) ;
	DoEvents();
}


static UINT uiAllButton[]=
{
	IDC_BUTTON_COMOPEN,	IDC_BUTTON_ENROLL,	IDC_BUTTON_11VERIF,	IDC_BUTTON_1NVERIF,	IDC_BUTTON_VERIF_AB,
	IDC_BUTTON_UPLOAD,	IDC_BUTTON_DOWNLOAD,IDC_BUTTON_DELTEMP, IDC_BUTTON_DELGROUP,IDC_BUTTON_DELALL,
	IDC_BUTTON_CHECK,	IDC_BUTTON_END
};


void CH2E_SampleAppDlg::EnabeleControl(DWORD dwMode, BOOL bEnable)
{
	CButton*	pBt;
	int			i;
	int			iCount;

	iCount = sizeof(uiAllButton)/sizeof(UINT);
	switch(dwMode){
		case SYSTEM_STARTUP:
		case SYSTEM_DEVICE_OFF:
			for(i=0; i<iCount; i++){
				pBt = (CButton*)GetDlgItem(uiAllButton[i]);
				pBt->EnableWindow(FALSE);
			}
			pBt = (CButton*)GetDlgItem(IDC_BUTTON_COMOPEN);
			pBt->EnableWindow(bEnable);
			pBt = (CButton*)GetDlgItem(IDC_BUTTON_END);
			pBt->EnableWindow(bEnable);
			break;
		default:
			for(i=0; i<iCount; i++){
				pBt = (CButton*)GetDlgItem(uiAllButton[i]);
				pBt->EnableWindow(bEnable);
			}
			break;
	}
	DoEvents();
}

void CH2E_SampleAppDlg::UpdateContorol()
{
    insertTenmplateListControl();
	DoEvents();
	sortTemplateListControl();
	DoEvents();

	updateEnroolEdit(TRUE);
}


void CH2E_SampleAppDlg::initEnroolEdit()
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

	CListCtrl* pLCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_TMPLIST) ;
	iTemps = pLCtrl->GetItemCount();
	if(iTemps > 0){
		for(i=0; i<iTemps; i++ ){
			strGroup = pLCtrl->GetItemText(i, 0);
			strtemp = pLCtrl->GetItemText(i, 1);
			wGroup = (WORD)_tcstol(strGroup,NULL,16);
			wTemp = (WORD)_tcstol(strtemp,NULL,16);
			dwWork = wGroup*0x10000 + wTemp;
			if((dwWork-dwTarget) > 1){
				break;
			}
			dwTarget = dwWork;
		}
		dwTarget++;
		wdw.dword = dwTarget;
		if(wdw.word[0] > 0x7FFF){
			wdw.word[0] = 0;
			wdw.word[1]++;
		}
		if(wdw.word[1] > 0x7FFF){
			wdw.dword = 0;
		}
	}
	wdw.dword = dwTarget;
	
	strGroup.Format(_T("%04X"), wdw.word[1]);
	strtemp.Format(_T("%04X"), wdw.word[0]);
	pEdiGroupt->SetWindowText(strGroup);
	pEditTemp->SetWindowText(strtemp);

}

void CH2E_SampleAppDlg::updateEnroolEdit(BOOL bReplace)
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
	pEdiGroupt->GetWindowText(strGroup);
	if(strGroup.IsEmpty()){
		wdwTarget.word[1] = 0;
	}
	else{
		wdwTarget.word[1] = (WORD)_tcstol(strGroup,NULL,16);
	}
	wdwTarget.word[0] = 0;

	CListCtrl* pLCtrl = (CListCtrl*)GetDlgItem(IDC_LIST_TMPLIST) ;
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
					wdwTarget.word[0]++;
					if(wdwTarget.word[0] > 0x7FFF){
						wdwTarget.word[0] = 0;
						wdwTarget.word[1]++;
					}
				}
				else{
					break;
				}
			}
		}
	}	
	strGroup.Format(_T("%04X"), wdwTarget.word[1]);
	strtemp.Format(_T("%04X"), wdwTarget.word[0]);
	m_bSetEnable = FALSE;
	if(bReplace){
		pEdiGroupt->SetWindowText(strGroup);
	}
	pEditTemp->SetWindowText(strtemp);
	m_bSetEnable = TRUE;
	

}


void CH2E_SampleAppDlg::OnEnChangeEditEnrollBank()
{

	UINT uiBank;
	CString strBank;
	CNumber nb;

	if(!m_bSetEnable){
		return;
	}

	//Get Group nummber & Template Nummber
	CEdit *pEditBank = (CEdit *)GetDlgItem(IDC_EDIT_ENROLL_BANK);
	pEditBank->GetWindowText(strBank);
	if(!nb.ConvertBankNummber(strBank, &uiBank)){
		return;
	}	
	updateEnroolEdit(FALSE);


}
//
//Open tuch senser panel
//
int CH2E_SampleAppDlg::OpenTuchSensorPanel()
{
	int iReturn;
	RECT rectDetDlg;;
	RECT rect;
	int iXPos,iYpos;

	iReturn = H2E_OK;
	
	if((m_btnCheckTuchOut1.GetCheck())
		|| (m_btnCheckTuchOut2.GetCheck())){

		if(!m_pDlgTuch){
			EnabeleControl(m_dwSystemCondition, FALSE);
			ZeroMemory(&rect, sizeof(rect));

			this->GetWindowRect(&rectDetDlg);
			iXPos = rectDetDlg.right + 5;
			iYpos = rectDetDlg.top + 100;

			m_pDlgTuch = new CDlgTuch;
			m_pDlgTuch->Create( IDD_TUCH_DIALOG, this ) ;
			m_pDlgTuch->SetWindowPos(&wndTop, iXPos, iYpos, 0, 0, SWP_NOSIZE|SWP_SHOWWINDOW);
			m_pDlgTuch->ShowWindow( SW_SHOW ) ;
			m_pDlgTuch->UpdateWindow() ;
		}

	}
	return iReturn;
}


//
//Wait Tuch Sensor State!
//
int  CH2E_SampleAppDlg::WaitTuchSensorState()
{
	int iReturn;
	int iTimeOut;
	iReturn = H2E_OK;

	DWORD dwTuchInfo;
	DWORD dwTuchMode;

	dwTuchInfo = 0;
	dwTuchMode = 0;
	if(m_btnCheckTuchOut1.GetCheck()){
		dwTuchMode |= H2E_TOUCH_OUT1;
	}
	if(m_btnCheckTuchOut2.GetCheck()){
		dwTuchMode |= H2E_TOUCH_OUT2;
	}

	iTimeOut = 0;
	while(dwTuchMode != dwTuchInfo){
		m_pH2ECom->SensingLine(&dwTuchInfo);
		m_pDlgTuch->SetTuchState(dwTuchInfo);
		DoEvents();
		Sleep(100);
		dwTuchMode = 0;
		if(m_btnCheckTuchOut1.GetCheck()){
			dwTuchMode |= H2E_TOUCH_OUT1;
		}
		if(m_btnCheckTuchOut2.GetCheck()){
			dwTuchMode |= H2E_TOUCH_OUT2;
		}
		iTimeOut++;
		if(iTimeOut > 50){
			iReturn = H2E_ERR_TSPOLTOUT;
			break;
		}
	}

	return iReturn;
}

//
//Close tuch sensor panel
//
int CH2E_SampleAppDlg::CloseTuchSensorPanel()
{
	int iReturn;
	iReturn = H2E_OK;

	if(m_pDlgTuch){
		m_pDlgTuch->DestroyWindow();
		delete m_pDlgTuch;
		m_pDlgTuch = 0;
		EnabeleControl(m_dwSystemCondition, TRUE);

	}

	return iReturn;
}

void CH2E_SampleAppDlg::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){
		if (!AfxGetThread()->PumpMessage()){
			return;
		}
	}
}
