#pragma once

// H2ECTRLPropPage.h : CH2ECTRLPropPage ����ҳ���������


// CH2ECTRLPropPage : �й�ʵ�ֵ���Ϣ������� H2ECTRLPropPage.cpp��

class CH2ECTRLPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CH2ECTRLPropPage)
	DECLARE_OLECREATE_EX(CH2ECTRLPropPage)

// ���캯��
public:
	CH2ECTRLPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_H2ECTRL };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

