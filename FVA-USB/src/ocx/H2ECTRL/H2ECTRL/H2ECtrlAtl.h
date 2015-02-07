// H2ECtrlAtl.h : CH2ECtrlAtl 的声明
#pragma once
#include "resource.h"       // 主符号
#include <atlctl.h>
#include "H2ECTRL_i.h"
#include "_IH2ECtrlAtlEvents_CP.h"
#include "H2E_Com.h"
#include "H2E_TemplateFile.h"
#include "H2E_MsgGuide.h"


#define H2E_COMMENT_NON			0
#define H2E_COMMENT_ADD			1
#define H2E_COMMENT_UOP			0x0080
#define H2E_COMMENT_PLUS		0x8000

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;



// CH2ECtrlAtl
class ATL_NO_VTABLE CH2ECtrlAtl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IH2ECtrlAtl, &IID_IH2ECtrlAtl, &LIBID_H2ECTRLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IOleControlImpl<CH2ECtrlAtl>,
	public IOleObjectImpl<CH2ECtrlAtl>,
	public IOleInPlaceActiveObjectImpl<CH2ECtrlAtl>,
	public IViewObjectExImpl<CH2ECtrlAtl>,
	public IOleInPlaceObjectWindowlessImpl<CH2ECtrlAtl>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CH2ECtrlAtl>,
	public CProxy_IH2ECtrlAtlEvents<CH2ECtrlAtl>,
	public IQuickActivateImpl<CH2ECtrlAtl>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CH2ECtrlAtl>,
#endif
	public IProvideClassInfo2Impl<&CLSID_H2ECtrlAtl, &__uuidof(_IH2ECtrlAtlEvents), &LIBID_H2ECTRLLib>,
	public IObjectSafetyImpl<CH2ECtrlAtl, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComCoClass<CH2ECtrlAtl, &CLSID_H2ECtrlAtl>,
	public CComControl<CH2ECtrlAtl>,
	
	public CH2ECom
{
public:


	CH2ECtrlAtl()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_INVISIBLEATRUNTIME |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_H2ECTRLATL)


BEGIN_COM_MAP(CH2ECtrlAtl)
	COM_INTERFACE_ENTRY(IH2ECtrlAtl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IQuickActivate)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
END_COM_MAP()

BEGIN_PROP_MAP(CH2ECtrlAtl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// 示例项
	// PROP_ENTRY_TYPE("属性名", dispid, clsid, vtType)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(CH2ECtrlAtl)
	CONNECTION_POINT_ENTRY(__uuidof(_IH2ECtrlAtlEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(CH2ECtrlAtl)
	CHAIN_MSG_MAP(CComControl<CH2ECtrlAtl>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// 处理程序原型:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* const arr[] =
		{
			&IID_IH2ECtrlAtl,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

// IH2ECtrlAtl
private:

	int formatTemplateListControl();
	int insertTenmplateListControl();
	void renumberItemTemplateListControl();
	void sortTemplateListControl();
	int GeneralTransaction(UINT uiTransID, BYTE byParam, LPVOID pOption,  WORD wOptionLen, UINT uiMsgID, UINT uiMode);
	void UpdateNummber(CString strBank, CString strTemp);
	void SetComment(DWORD dwMode, LPCTSTR pMessage, DWORD dwError);
	void EnabeleControl(DWORD dwMode, BOOL bEnable);
	void UpdateContorol();

	int DeviceInit();//初始化设备，获取设备串口号，设定串口号和波特率。

	void initEnroolEdit();
	void updateEnroolEdit(BOOL bReplace);
	HRESULT SetArrayNumberOfIndex(IDispatch* pDisp, int index, BYTE* pTemplete);
	HRESULT GetArrayNumberOfIndex(IDispatch* pDisp, int index, BYTE * pTemplete);
	HRESULT SetWordArrayNumberOfIndex(IDispatch* pDisp, int index, WORD* pTemplete);
	HRESULT GetWordArrayNumberOfIndex(IDispatch* pDisp, int index, WORD * pTemplete);
//	CTemplateFile*	m_pTemplateFile;
//	CH2ECom* m_pH2ECom;
	TEMPINFO	m_tempInfo;
	BOOL m_bSetEnable;
	HANDLE m_hCom ;

public:
	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
#if FTDI_VCP
	STDMETHOD(OpenCom)(int nIdx, UINT uiSpeed, LONG* retval);
#elif FTDI_D2XX
	STDMETHOD(OpenCom)(LONG* retval);
#endif
	STDMETHOD(Enroll)(WORD wBank, WORD wIdx, UINT uiTimes, LONG lTouchInfo, LONG* retval);
	STDMETHOD(TouchSensorState)(BYTE bTouchMode, LONG* retval);
	STDMETHOD(Verify1_1)(WORD wBank, WORD wIdx, INT* iMatchMemNo);
	STDMETHOD(Verify1_N)(WORD wBank, INT* iMatchMemNo);
	STDMETHOD(DeleteTemplete)(WORD wBank, WORD wIdx, INT iMode, LONG* retval);
	STDMETHOD(GetTemplate)(WORD wBank, WORD wIdx, VARIANT vTemplate, LONG* retval);
	STDMETHOD(SetTemplate)(WORD wBank, WORD wIdx, VARIANT vTemplate, LONG* retval);
	STDMETHOD(VerifyAcrossBanks)(VARIANT vAcrossBanks, WORD wTotalBankNumbers, VARIANT vMatchedBankAndTemplate, LONG* retval);
};

OBJECT_ENTRY_AUTO(__uuidof(H2ECtrlAtl), CH2ECtrlAtl)
