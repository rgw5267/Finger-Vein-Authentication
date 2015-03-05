#pragma once

// H2ECTRLPropPage.h : CH2ECTRLPropPage 属性页类的声明。


// CH2ECTRLPropPage : 有关实现的信息，请参阅 H2ECTRLPropPage.cpp。

class CH2ECTRLPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CH2ECTRLPropPage)
	DECLARE_OLECREATE_EX(CH2ECTRLPropPage)

// 构造函数
public:
	CH2ECTRLPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_H2ECTRL };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

