#pragma once

// MFCActiveXControltestPropPage.h : CMFCActiveXControltestPropPage ����ҳ���������


// CMFCActiveXControltestPropPage : �й�ʵ�ֵ���Ϣ������� MFCActiveXControltestPropPage.cpp��

class CMFCActiveXControltestPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CMFCActiveXControltestPropPage)
	DECLARE_OLECREATE_EX(CMFCActiveXControltestPropPage)

// ���캯��
public:
	CMFCActiveXControltestPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_MFCACTIVEXCONTROLTEST };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

