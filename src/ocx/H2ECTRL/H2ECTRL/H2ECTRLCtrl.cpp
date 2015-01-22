// H2ECTRLCtrl.cpp : CH2ECTRLCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "H2ECTRL.h"
#include "H2ECTRLCtrl.h"
#include "H2ECTRLPropPage.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CH2ECTRLCtrl, COleControl)

// 消息映射

BEGIN_MESSAGE_MAP(CH2ECTRLCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// 调度映射

BEGIN_DISPATCH_MAP(CH2ECTRLCtrl, COleControl)
END_DISPATCH_MAP()

// 事件映射

BEGIN_EVENT_MAP(CH2ECTRLCtrl, COleControl)
END_EVENT_MAP()

// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CH2ECTRLCtrl, 1)
	PROPPAGEID(CH2ECTRLPropPage::guid)
END_PROPPAGEIDS(CH2ECTRLCtrl)

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CH2ECTRLCtrl, "H2ECTRL.H2ECTRLCtrl.1",
	0x737b1887, 0x28fc, 0x4407, 0xb7, 0x36, 0x70, 0x28, 0x35, 0xd6, 0xc9, 0xb0)

// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CH2ECTRLCtrl, _tlid, _wVerMajor, _wVerMinor)

// 接口 ID

const IID IID_DH2ECTRL = { 0x444B9E74, 0x4E35, 0x40A1, { 0x86, 0x16, 0xE4, 0x45, 0x7, 0xE7, 0xC8, 0xE0 } };
const IID IID_DH2ECTRLEvents = { 0xFCF1FFDC, 0x4DA8, 0x47DE, { 0x8B, 0x83, 0x65, 0xB2, 0x9C, 0xE9, 0x1A, 0x9F } };

// 控件类型信息

static const DWORD _dwH2ECTRLOleMisc =
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CH2ECTRLCtrl, IDS_H2ECTRL, _dwH2ECTRLOleMisc)

// CH2ECTRLCtrl::CH2ECTRLCtrlFactory::UpdateRegistry -
// 添加或移除 CH2ECTRLCtrl 的系统注册表项

BOOL CH2ECTRLCtrl::CH2ECTRLCtrlFactory::UpdateRegistry(BOOL bRegister)
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


// CH2ECTRLCtrl::CH2ECTRLCtrl - 构造函数

CH2ECTRLCtrl::CH2ECTRLCtrl()
{
	InitializeIIDs(&IID_DH2ECTRL, &IID_DH2ECTRLEvents);
	// TODO: 在此初始化控件的实例数据。
}

// CH2ECTRLCtrl::~CH2ECTRLCtrl - 析构函数

CH2ECTRLCtrl::~CH2ECTRLCtrl()
{
	// TODO: 在此清理控件的实例数据。
}

// CH2ECTRLCtrl::OnDraw - 绘图函数

void CH2ECTRLCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CH2ECTRLCtrl::DoPropExchange - 持久性支持

void CH2ECTRLCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}


// CH2ECTRLCtrl::OnResetState - 将控件重置为默认状态

void CH2ECTRLCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}


// CH2ECTRLCtrl 消息处理程序
