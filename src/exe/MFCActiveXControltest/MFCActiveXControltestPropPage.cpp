// MFCActiveXControltestPropPage.cpp : CMFCActiveXControltestPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "MFCActiveXControltest.h"
#include "MFCActiveXControltestPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMFCActiveXControltestPropPage, COlePropertyPage)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CMFCActiveXControltestPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CMFCActiveXControltestPropPage, "MFCACTIVEXCONT.MFCActiveXContPropPage.1",
	0xa43a01d, 0xe619, 0x4552, 0x8e, 0x48, 0x4d, 0x38, 0x24, 0xe0, 0x63, 0x54)

// CMFCActiveXControltestPropPage::CMFCActiveXControltestPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CMFCActiveXControltestPropPage ��ϵͳע�����

BOOL CMFCActiveXControltestPropPage::CMFCActiveXControltestPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_MFCACTIVEXCONTROLTEST_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CMFCActiveXControltestPropPage::CMFCActiveXControltestPropPage - ���캯��

CMFCActiveXControltestPropPage::CMFCActiveXControltestPropPage() :
	COlePropertyPage(IDD, IDS_MFCACTIVEXCONTROLTEST_PPG_CAPTION)
{
}

// CMFCActiveXControltestPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CMFCActiveXControltestPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CMFCActiveXControltestPropPage ��Ϣ�������
