// H2ECTRLPropPage.cpp : CH2ECTRLPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "H2ECTRL.h"
#include "H2ECTRLPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CH2ECTRLPropPage, COlePropertyPage)

// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CH2ECTRLPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CH2ECTRLPropPage, "H2ECTRL.H2ECTRLPropPage.1",
	0xcf054b3, 0x4cbf, 0x4beb, 0xb8, 0xc5, 0x5d, 0x21, 0xbd, 0xe5, 0x69, 0x97)

// CH2ECTRLPropPage::CH2ECTRLPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CH2ECTRLPropPage ��ϵͳע�����

BOOL CH2ECTRLPropPage::CH2ECTRLPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_H2ECTRL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CH2ECTRLPropPage::CH2ECTRLPropPage - ���캯��

CH2ECTRLPropPage::CH2ECTRLPropPage() :
	COlePropertyPage(IDD, IDS_H2ECTRL_PPG_CAPTION)
{
}

// CH2ECTRLPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CH2ECTRLPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CH2ECTRLPropPage ��Ϣ�������
