// MFCActiveXControltestCtrl.cpp : CMFCActiveXControltestCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "MFCActiveXControltest.h"
#include "MFCActiveXControltestCtrl.h"
#include "MFCActiveXControltestPropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMFCActiveXControltestCtrl, COleControl)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CMFCActiveXControltestCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// ����ӳ��

BEGIN_DISPATCH_MAP(CMFCActiveXControltestCtrl, COleControl)
	DISP_FUNCTION_ID(CMFCActiveXControltestCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// �¼�ӳ��

BEGIN_EVENT_MAP(CMFCActiveXControltestCtrl, COleControl)
END_EVENT_MAP()

// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CMFCActiveXControltestCtrl, 1)
	PROPPAGEID(CMFCActiveXControltestPropPage::guid)
END_PROPPAGEIDS(CMFCActiveXControltestCtrl)

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CMFCActiveXControltestCtrl, "MFCACTIVEXCONTRO.MFCActiveXControCtrl.1",
	0x2fb8ca42, 0xb918, 0x4454, 0x9a, 0x75, 0x5b, 0xb, 0x78, 0x74, 0x3f, 0xa4)

// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CMFCActiveXControltestCtrl, _tlid, _wVerMajor, _wVerMinor)

// �ӿ� ID

const IID IID_DMFCActiveXControltest = { 0x18D04B97, 0x826C, 0x4467, { 0xB1, 0x65, 0xBB, 0x64, 0x62, 0x16, 0x2E, 0x8D } };
const IID IID_DMFCActiveXControltestEvents = { 0xE310A839, 0xE33B, 0x4FD9, { 0xA0, 0x5D, 0xBC, 0x25, 0xD, 0xE6, 0x41, 0x98 } };

// �ؼ�������Ϣ

static const DWORD _dwMFCActiveXControltestOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CMFCActiveXControltestCtrl, IDS_MFCACTIVEXCONTROLTEST, _dwMFCActiveXControltestOleMisc)

// CMFCActiveXControltestCtrl::CMFCActiveXControltestCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CMFCActiveXControltestCtrl ��ϵͳע�����

BOOL CMFCActiveXControltestCtrl::CMFCActiveXControltestCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_MFCACTIVEXCONTROLTEST,
			IDB_MFCACTIVEXCONTROLTEST,
			afxRegApartmentThreading,
			_dwMFCActiveXControltestOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CMFCActiveXControltestCtrl::CMFCActiveXControltestCtrl - ���캯��

CMFCActiveXControltestCtrl::CMFCActiveXControltestCtrl()
{
	InitializeIIDs(&IID_DMFCActiveXControltest, &IID_DMFCActiveXControltestEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
}

// CMFCActiveXControltestCtrl::~CMFCActiveXControltestCtrl - ��������

CMFCActiveXControltestCtrl::~CMFCActiveXControltestCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}

// CMFCActiveXControltestCtrl::OnDraw - ��ͼ����

void CMFCActiveXControltestCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CMFCActiveXControltestCtrl::DoPropExchange - �־���֧��

void CMFCActiveXControltestCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}


// CMFCActiveXControltestCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CMFCActiveXControltestCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}


// CMFCActiveXControltestCtrl::AboutBox - ���û���ʾ�����ڡ���

void CMFCActiveXControltestCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_MFCACTIVEXCONTROLTEST);
	dlgAbout.DoModal();
}


// CMFCActiveXControltestCtrl ��Ϣ�������
