#pragma once

// H2ECTRLCtrl.h : CH2ECTRLCtrl ActiveX 控件类的声明。


// CH2ECTRLCtrl : 有关实现的信息，请参阅 H2ECTRLCtrl.cpp。

class CH2ECTRLCtrl : public COleControl
{
	DECLARE_DYNCREATE(CH2ECTRLCtrl)

// 构造函数
public:
	CH2ECTRLCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CH2ECTRLCtrl();

	DECLARE_OLECREATE_EX(CH2ECTRLCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CH2ECTRLCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CH2ECTRLCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CH2ECTRLCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
	};
};

