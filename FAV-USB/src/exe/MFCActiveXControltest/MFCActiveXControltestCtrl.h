#pragma once

// MFCActiveXControltestCtrl.h : CMFCActiveXControltestCtrl ActiveX 控件类的声明。


// CMFCActiveXControltestCtrl : 有关实现的信息，请参阅 MFCActiveXControltestCtrl.cpp。

class CMFCActiveXControltestCtrl : public COleControl
{
	DECLARE_DYNCREATE(CMFCActiveXControltestCtrl)

// 构造函数
public:
	CMFCActiveXControltestCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CMFCActiveXControltestCtrl();

	DECLARE_OLECREATE_EX(CMFCActiveXControltestCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CMFCActiveXControltestCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CMFCActiveXControltestCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CMFCActiveXControltestCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
	};
};

