#pragma once

// MFCActiveXControltestCtrl.h : CMFCActiveXControltestCtrl ActiveX �ؼ����������


// CMFCActiveXControltestCtrl : �й�ʵ�ֵ���Ϣ������� MFCActiveXControltestCtrl.cpp��

class CMFCActiveXControltestCtrl : public COleControl
{
	DECLARE_DYNCREATE(CMFCActiveXControltestCtrl)

// ���캯��
public:
	CMFCActiveXControltestCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CMFCActiveXControltestCtrl();

	DECLARE_OLECREATE_EX(CMFCActiveXControltestCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CMFCActiveXControltestCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CMFCActiveXControltestCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CMFCActiveXControltestCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
	};
};

