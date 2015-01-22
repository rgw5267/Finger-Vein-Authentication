// H2ECTRLPropPage.cpp : CH2ECTRLPropPage 属性页类的实现。

#include "stdafx.h"
#include "H2ECTRL.h"
#include "H2ECTRLPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CH2ECTRLPropPage, COlePropertyPage)

// 消息映射

BEGIN_MESSAGE_MAP(CH2ECTRLPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CH2ECTRLPropPage, "H2ECTRL.H2ECTRLPropPage.1",
	0xcf054b3, 0x4cbf, 0x4beb, 0xb8, 0xc5, 0x5d, 0x21, 0xbd, 0xe5, 0x69, 0x97)

// CH2ECTRLPropPage::CH2ECTRLPropPageFactory::UpdateRegistry -
// 添加或移除 CH2ECTRLPropPage 的系统注册表项

BOOL CH2ECTRLPropPage::CH2ECTRLPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_H2ECTRL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CH2ECTRLPropPage::CH2ECTRLPropPage - 构造函数

CH2ECTRLPropPage::CH2ECTRLPropPage() :
	COlePropertyPage(IDD, IDS_H2ECTRL_PPG_CAPTION)
{
}

// CH2ECTRLPropPage::DoDataExchange - 在页和属性间移动数据

void CH2ECTRLPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CH2ECTRLPropPage 消息处理程序
