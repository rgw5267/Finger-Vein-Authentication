#pragma once

// H2ECTRLCtrl.h : CH2ECTRLCtrl ActiveX �ؼ����������


// CH2ECTRLCtrl : �й�ʵ�ֵ���Ϣ������� H2ECTRLCtrl.cpp��

class CH2ECTRLCtrl : public COleControl
{
	DECLARE_DYNCREATE(CH2ECTRLCtrl)

// ���캯��
public:
	CH2ECTRLCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CH2ECTRLCtrl();

	DECLARE_OLECREATE_EX(CH2ECTRLCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CH2ECTRLCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CH2ECTRLCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CH2ECTRLCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
	};
};

