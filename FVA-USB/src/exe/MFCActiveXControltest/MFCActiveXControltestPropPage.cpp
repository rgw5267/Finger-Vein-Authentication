// MFCActiveXControltestPropPage.cpp : CMFCActiveXControltestPropPage 属性页类的实现。

#include "stdafx.h"
#include "MFCActiveXControltest.h"
#include "MFCActiveXControltestPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMFCActiveXControltestPropPage, COlePropertyPage)

// 消息映射

BEGIN_MESSAGE_MAP(CMFCActiveXControltestPropPage, COlePropertyPage)
END_MESSAGE_MAP()

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CMFCActiveXControltestPropPage, "MFCACTIVEXCONT.MFCActiveXContPropPage.1",
	0xa43a01d, 0xe619, 0x4552, 0x8e, 0x48, 0x4d, 0x38, 0x24, 0xe0, 0x63, 0x54)

// CMFCActiveXControltestPropPage::CMFCActiveXControltestPropPageFactory::UpdateRegistry -
// 添加或移除 CMFCActiveXControltestPropPage 的系统注册表项

BOOL CMFCActiveXControltestPropPage::CMFCActiveXControltestPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_MFCACTIVEXCONTROLTEST_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

// CMFCActiveXControltestPropPage::CMFCActiveXControltestPropPage - 构造函数

CMFCActiveXControltestPropPage::CMFCActiveXControltestPropPage() :
	COlePropertyPage(IDD, IDS_MFCACTIVEXCONTROLTEST_PPG_CAPTION)
{
}

// CMFCActiveXControltestPropPage::DoDataExchange - 在页和属性间移动数据

void CMFCActiveXControltestPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}

// CMFCActiveXControltestPropPage 消息处理程序
