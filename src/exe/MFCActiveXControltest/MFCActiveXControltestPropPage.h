#pragma once

// MFCActiveXControltestPropPage.h : CMFCActiveXControltestPropPage 属性页类的声明。


// CMFCActiveXControltestPropPage : 有关实现的信息，请参阅 MFCActiveXControltestPropPage.cpp。

class CMFCActiveXControltestPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CMFCActiveXControltestPropPage)
	DECLARE_OLECREATE_EX(CMFCActiveXControltestPropPage)

// 构造函数
public:
	CMFCActiveXControltestPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_MFCACTIVEXCONTROLTEST };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

