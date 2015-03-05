// H2ECTRLCtrl.cpp : CH2ECTRLCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "H2ECTRL.h"
#include "H2ECTRLCtrl.h"
#include "H2ECTRLPropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CH2ECTRLCtrl, COleControl)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CH2ECTRLCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// ����ӳ��

BEGIN_DISPATCH_MAP(CH2ECTRLCtrl, COleControl)
END_DISPATCH_MAP()

// �¼�ӳ��

BEGIN_EVENT_MAP(CH2ECTRLCtrl, COleControl)
END_EVENT_MAP()

// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CH2ECTRLCtrl, 1)
	PROPPAGEID(CH2ECTRLPropPage::guid)
END_PROPPAGEIDS(CH2ECTRLCtrl)

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CH2ECTRLCtrl, "H2ECTRL.H2ECTRLCtrl.1",
	0x737b1887, 0x28fc, 0x4407, 0xb7, 0x36, 0x70, 0x28, 0x35, 0xd6, 0xc9, 0xb0)

// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CH2ECTRLCtrl, _tlid, _wVerMajor, _wVerMinor)

// �ӿ� ID

const IID IID_DH2ECTRL = { 0x444B9E74, 0x4E35, 0x40A1, { 0x86, 0x16, 0xE4, 0x45, 0x7, 0xE7, 0xC8, 0xE0 } };
const IID IID_DH2ECTRLEvents = { 0xFCF1FFDC, 0x4DA8, 0x47DE, { 0x8B, 0x83, 0x65, 0xB2, 0x9C, 0xE9, 0x1A, 0x9F } };

// �ؼ�������Ϣ

static const DWORD _dwH2ECTRLOleMisc =
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CH2ECTRLCtrl, IDS_H2ECTRL, _dwH2ECTRLOleMisc)

// CH2ECTRLCtrl::CH2ECTRLCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CH2ECTRLCtrl ��ϵͳע�����

BOOL CH2ECTRLCtrl::CH2ECTRLCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_H2ECTRL,
			IDB_H2ECTRL,
			afxRegApartmentThreading,
			_dwH2ECTRLOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CH2ECTRLCtrl::CH2ECTRLCtrl - ���캯��

CH2ECTRLCtrl::CH2ECTRLCtrl()
{
	InitializeIIDs(&IID_DH2ECTRL, &IID_DH2ECTRLEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
}

// CH2ECTRLCtrl::~CH2ECTRLCtrl - ��������

CH2ECTRLCtrl::~CH2ECTRLCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}

// CH2ECTRLCtrl::OnDraw - ��ͼ����

void CH2ECTRLCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CH2ECTRLCtrl::DoPropExchange - �־���֧��

void CH2ECTRLCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}


// CH2ECTRLCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CH2ECTRLCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}


// CH2ECTRLCtrl ��Ϣ�������
