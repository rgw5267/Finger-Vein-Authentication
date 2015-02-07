// MFCActiveXControltestCtrl.cpp : CMFCActiveXControltestCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "MFCActiveXControltest.h"
#include "MFCActiveXControltestCtrl.h"
#include "MFCActiveXControltestPropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMFCActiveXControltestCtrl, COleControl)

// 消息映射

BEGIN_MESSAGE_MAP(CMFCActiveXControltestCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// 调度映射

BEGIN_DISPATCH_MAP(CMFCActiveXControltestCtrl, COleControl)
	DISP_FUNCTION_ID(CMFCActiveXControltestCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// 事件映射

BEGIN_EVENT_MAP(CMFCActiveXControltestCtrl, COleControl)
END_EVENT_MAP()

// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CMFCActiveXControltestCtrl, 1)
	PROPPAGEID(CMFCActiveXControltestPropPage::guid)
END_PROPPAGEIDS(CMFCActiveXControltestCtrl)

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CMFCActiveXControltestCtrl, "MFCACTIVEXCONTRO.MFCActiveXControCtrl.1",
	0x2fb8ca42, 0xb918, 0x4454, 0x9a, 0x75, 0x5b, 0xb, 0x78, 0x74, 0x3f, 0xa4)

// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CMFCActiveXControltestCtrl, _tlid, _wVerMajor, _wVerMinor)

// 接口 ID

const IID IID_DMFCActiveXControltest = { 0x18D04B97, 0x826C, 0x4467, { 0xB1, 0x65, 0xBB, 0x64, 0x62, 0x16, 0x2E, 0x8D } };
const IID IID_DMFCActiveXControltestEvents = { 0xE310A839, 0xE33B, 0x4FD9, { 0xA0, 0x5D, 0xBC, 0x25, 0xD, 0xE6, 0x41, 0x98 } };

// 控件类型信息

static const DWORD _dwMFCActiveXControltestOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CMFCActiveXControltestCtrl, IDS_MFCACTIVEXCONTROLTEST, _dwMFCActiveXControltestOleMisc)

// CMFCActiveXControltestCtrl::CMFCActiveXControltestCtrlFactory::UpdateRegistry -
// 添加或移除 CMFCActiveXControltestCtrl 的系统注册表项

BOOL CMFCActiveXControltestCtrl::CMFCActiveXControltestCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

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


// CMFCActiveXControltestCtrl::CMFCActiveXControltestCtrl - 构造函数

CMFCActiveXControltestCtrl::CMFCActiveXControltestCtrl()
{
	InitializeIIDs(&IID_DMFCActiveXControltest, &IID_DMFCActiveXControltestEvents);
	// TODO: 在此初始化控件的实例数据。
}

// CMFCActiveXControltestCtrl::~CMFCActiveXControltestCtrl - 析构函数

CMFCActiveXControltestCtrl::~CMFCActiveXControltestCtrl()
{
	// TODO: 在此清理控件的实例数据。
}

// CMFCActiveXControltestCtrl::OnDraw - 绘图函数

void CMFCActiveXControltestCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CMFCActiveXControltestCtrl::DoPropExchange - 持久性支持

void CMFCActiveXControltestCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}


// CMFCActiveXControltestCtrl::OnResetState - 将控件重置为默认状态

void CMFCActiveXControltestCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}


// CMFCActiveXControltestCtrl::AboutBox - 向用户显示“关于”框

void CMFCActiveXControltestCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_MFCACTIVEXCONTROLTEST);
	dlgAbout.DoModal();
}


// CMFCActiveXControltestCtrl 消息处理程序
