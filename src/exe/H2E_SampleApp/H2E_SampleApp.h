// H2E_SampleApp.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// CH2E_SampleAppApp:
// ���̃N���X�̎����ɂ��ẮAH2E_SampleApp.cpp ���Q�Ƃ��Ă��������B
//

class CH2E_SampleAppApp : public CWinApp
{
public:
	CH2E_SampleAppApp();

// �I�[�o�[���C�h
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CH2E_SampleAppApp theApp;