/**
*@All Rights Reserved.    
*@Copyright (C) 2011, Hitachi Ltd.
*/

// H1E-USB_SampleApp.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CH1EUSB_SampleAppApp:
// ���̃N���X�̎����ɂ��ẮAH1E-USB_SampleApp.cpp ���Q�Ƃ��Ă��������B
//

class CH1EUSB_SampleAppApp : public CWinApp
{
public:
	CH1EUSB_SampleAppApp();

// �I�[�o�[���C�h
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CH1EUSB_SampleAppApp theApp;