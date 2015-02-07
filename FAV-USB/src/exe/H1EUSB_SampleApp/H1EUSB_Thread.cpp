#include "StdAfx.h"
#include <process.h>
#include "H1EUSB_Thread.h"
/********************************************************************************************/
/* CA2GObjThread�N���X�v���O����															*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20120110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/********************************************************************************************/
/********************************************************************************************/
/*[���\�b�h��] CA2GObjThread�N���X�̃R���X�g���N�^											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CA2GObjThread(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CA2GObjThread::CA2GObjThread(LPVOID pObj, PA2GOBJTHREADPROCS pA2gObjThreadProcs)
{
	m_pObjThreadProcs = pObj;
	m_A2gObjTheradProcs = *pA2gObjThreadProcs;
	CA2GObjInitialize();
}

/********************************************************************************************/
/*[���\�b�h��] CA2GObjThread�N���X�̃R���X�g���N�^											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CA2GObjThread(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CA2GObjThread::CA2GObjThread(void)
{
	m_pObjThreadProcs = 0;
	ZeroMemory(&m_A2gObjTheradProcs, sizeof(m_A2gObjTheradProcs));
	CA2GObjInitialize();

}

/********************************************************************************************/
/*[���\�b�h��] CA2GObjThread�N���X�̃f�X�g���N�^											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		~CA2GEndpoint(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CA2GObjThread::~CA2GObjThread(void)
{
	EndThread();
}


/********************************************************************************************/
/*[���\�b�h��] CA2GObjThread�N���X�̏���������												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CA2GObjThread(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		CA2GObjThread�N���X�̏���������												*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CA2GObjThread::CA2GObjInitialize(void)
{
	DWORD	dw;
	m_dwEvents	= 0;
	m_hEPThread	= 0;
	//�C�x���g�R�[���o�b�N�e�[�u����������
	for(dw=0; dw<THREAD_MAX_EVENTS; dw++){
		m_EventInfo.hEventHandles[dw]	= 0;
		m_EventCallBacks[dw].proc = 0;
		m_EventCallBacks[dw].pObj = 0;
	}
	//�C�x���g�^�C���A�E�g�R�[���o�b�N�e�[�u����������
	m_EventTOutCallBacks.proc	= 0;
	m_EventTOutCallBacks.pObj	= 0;
	//���b�Z�[�W�R�[���o�b�N��������
	m_MsgCallBack.Proc		= 0;
	m_MsgCallBack.pObj		= 0;

	m_dwThreadLoopCycle = THREAD_EVENTS_CYCLE;		//�X���b�h���[�v�����̏����l

}

/********************************************************************************************/
/*[���\�b�h��] �X���b�h�̐���																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CreateThread(int nPriority);												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CA2GObjThread::CreateThread(int nPriority)
{
	//�X���b�h�̋N��
	m_hEPThread = (HANDLE)_beginthreadex(NULL, 0, &A2GObjThreadStab, this, 0, &m_uEPThreadID);

	if(m_hEPThread != 0){
		//�X���b�h�̗D�揇�ʂ��Z�b�g����B
		SetThreadPriority(m_hEPThread , nPriority);

		//�X���b�h���N�������܂őҋ@����B
		DWORD dwExitCode;
		dwExitCode = 0;
		while(dwExitCode != STILL_ACTIVE){
			GetExitCodeThread(m_hEPThread, &dwExitCode);
			Sleep(100);
		}
	}
	else{
		//�X���b�h�̋N���Ɏ��s�����B
		return H1USB_ERR_CREATETHREAD;

	}
	return H1USB_OK;
}



/********************************************************************************************/
/*[���\�b�h��] �X���b�h�̏I��																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		EndThread(void);															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�X���b�h�̏I���������s��													*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CA2GObjThread::EndThread(void)
{
	DWORD	dwResult;

	if(m_hEPThread){
		//�I�����b�Z�[�W�𑗐M
		PostThreadMessage(m_uEPThreadID, WM_QUIT, 0, 0);

		//�X���b�h�̏I���҂�
		dwResult = WaitForSingleObject(m_hEPThread, THREAD_END_TIMEOUTS);
		if(dwResult == WAIT_TIMEOUT){
			//�����I��
			TerminateThread(m_hEPThread, THREAD_ENDCODE_FORCED);
		}
		//�n���h���N���[�Y
		CloseHandle(m_hEPThread);
		m_hEPThread = 0;
	}
	return H1USB_OK;
}

/********************************************************************************************/
/*[���\�b�h��] CA2GObjThread�N���X�̃R���X�g���N�^											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		SetThreadProcs(LPVOID pObj, PA2GOBJTHREADPROCS pA2gObjThreadProcs)			*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
void CA2GObjThread::SetThreadProcs(LPVOID pObj, PA2GOBJTHREADPROCS pA2gObjThreadProcs)
{
	m_pObjThreadProcs	= pObj;
	m_A2gObjTheradProcs	= *pA2gObjThreadProcs;
}



/********************************************************************************************/
/*[���\�b�h��] ���b�Z�[�WPROC�̓o�^															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int SetMessageProc(A2GOBJ_MSG_PROC A2gMsgProc, LPVOID pObj);				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		���b�Z�[�WPROC�̓o�^�������s��												*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CA2GObjThread::SetMessageProc(A2GOBJ_MSG_PROC A2gMsgProc, LPVOID pObj)
{
	m_MsgCallBack.Proc	= A2gMsgProc;
	m_MsgCallBack.pObj	= pObj;

	return H1USB_OK;
}

/********************************************************************************************/
/*[���\�b�h��] �C�x���gPROC�̓o�^															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]	int SetEventProc(HANDLE hEvent, HANDLE hDevice,									*/
/*								A2GOBJ_EVENT_PROC A2gEventProc, LPVOID pObj);				*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�C�x���gPROC�̓o�^�̓o�^�������s��											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FAL2P_SUCCESS														*/
/********************************************************************************************/
int CA2GObjThread::SetEventProc(HANDLE hEvent, HANDLE hDevice, A2GOBJ_EVENT_PROC A2gEventProc, LPVOID pObj)
{

	if(m_dwEvents >= THREAD_MAX_EVENTS){
		//�o�^�ł���C�x���g�̍ő吔�𒴂����B
		return H1USB_ERR_MAXEVENTS;
	}
	m_EventInfo.hEventHandles[m_dwEvents]	= hEvent;
	m_EventCallBacks[m_dwEvents].proc = A2gEventProc;
	m_EventCallBacks[m_dwEvents].pObj = pObj;
	m_dwEvents++;

	return H1USB_OK;
}

/********************************************************************************************/
/*[���\�b�h��] �C�x���gPROC�̓o�^															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]	int SetEventTOutProc(A2GOBJ_EVENT_TIMEOUT_PROC A2gEventTOutProc, LPVOID pObj);	*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�C�x���gPROC�̓o�^�̓o�^�������s��											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CA2GObjThread::SetEventTOutProc(A2GOBJ_EVENT_TIMEOUT_PROC A2gEventTOutProc, LPVOID pObj)
{
	m_EventTOutCallBacks.proc	= A2gEventTOutProc;
	m_EventTOutCallBacks.pObj	= pObj;
	return H1USB_OK;
}

/********************************************************************************************/
/*[���\�b�h��] �X���b�h���[�v�����Z�b�g														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]	int SetThreadCycle(DWORD dwCycle);												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�X���b�h�̃��[�v�������Z�b�g����B											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CA2GObjThread::SetThreadCycle(DWORD dwCycle)
{
	m_dwThreadLoopCycle = dwCycle;
	return H1USB_OK;
}


/********************************************************************************************/
/*[���\�b�h��] �X���b�h�X�^�u																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		DWORD WINAPI EPThreadStab(void* pParam)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�X���b�h�X�^�u																*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
UINT WINAPI CA2GObjThread::A2GObjThreadStab(void* pParam)
{
	UINT	ui;
	int		iResult;

	CA2GObjThread* pEp = (CA2GObjThread*)(pParam);

	//�X���b�h�O����
	iResult = pEp->A2GObjThreadBefor();
	//�X���b�h�̎��̂��R�[��
	ui = pEp->A2GObjThread();
	//�X���b�h�㏈��
	iResult = pEp->A2GObjThreadBefor();
	//�X���b�h�̏I��
	_endthreadex(ui);
	return ui;
}

/********************************************************************************************/
/*[���\�b�h��] �X���b�h�O�Ăяo������														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		UINT CA2GObjThread::A2GObjThreadBefor(void)									*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�X���b�h�̃��b�Z�[�W���[�v�O�Ăяo������									*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CA2GObjThread::A2GObjThreadBefor(void)
{	
	UINT uiResult;
	int iResult = H1USB_OK;

	if(m_A2gObjTheradProcs.A2GObjThreadBefor)
	{
		uiResult = m_A2gObjTheradProcs.A2GObjThreadBefor(m_pObjThreadProcs);
	}
	return iResult;
}

/********************************************************************************************/
/*[���\�b�h��] �X���b�h���b�Z�[�W�Ăяo������												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		UINT CA2GObjThread::A2GObjOnThread(void)									*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�X���b�h�̃��b�Z�[�W���[�v�Ăяo������										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CA2GObjThread::A2GObjOnThread(LPBOOL pOnEvnet)
{
	UINT uiResult;
	int iResult = H1USB_OK;

	if(m_A2gObjTheradProcs.A2GObjOnThread)
	{
		uiResult = m_A2gObjTheradProcs.A2GObjOnThread(m_pObjThreadProcs);
	}
	return iResult;
}

/********************************************************************************************/
/*[���\�b�h��] �X���b�h��Ăяo������														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		UINT CA2GObjThread::A2GObjThreadAfter(void)									*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�X���b�h�̃��b�Z�[�W���[�v��Ăяo������									*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CA2GObjThread::A2GObjThreadAfter(void)
{
	UINT uiResult;
	int iResult = H1USB_OK;

	if(m_A2gObjTheradProcs.A2GObjThreadAfter)
	{
		uiResult = m_A2gObjTheradProcs.A2GObjThreadAfter(m_pObjThreadProcs);
	}
	return iResult;
}



/********************************************************************************************/
/*[���\�b�h��] �X���b�h�̎���																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		UINT CA2GObjThread::A2GObjThread(void)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�X���b�h����																*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
UINT CA2GObjThread::A2GObjThread(void)
{
	BOOL						bResult;
	BOOL						bEnable;
	MSG							msg;
	LRESULT						lResult;
	DWORD						dwWaitObject;
	DWORD						dwEvent;
	A2GOBJ_MSG_PROC				A2gMsgProc;		
	A2GOBJ_EVENT_PROC			A2gEventProc;
	A2GOBJ_EVENT_TIMEOUT_PROC	A2gEventTOutProc;
	int							iResult;
	BOOL						bOnEvent;
	DWORD						dwError;

	A2gMsgProc			= m_MsgCallBack.Proc;
	A2gEventTOutProc	= m_EventTOutCallBacks.proc;


	bEnable		=TRUE;
	bOnEvent	=FALSE;
	//�X���b�h���b�Z�[�W���[�v
	while(bEnable)
	{
		//���b�Z�[�W����
		bResult = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if(bResult){
			if(A2gMsgProc){
				//��ʂ̃N���X���烁���o�[���o�^����Ă���ΌĂяo���B
				lResult = A2gMsgProc(msg.message, msg.wParam, msg.lParam, m_MsgCallBack.pObj);
			}
			switch(msg.message)
			{
			case WM_QUIT:
				bEnable = FALSE;
				continue;
			default:
				break;
			}
		}

		//OnThread����
		if(!bOnEvent){
			//�C�x���g�������łȂ���ΌĂяo���B
			iResult = A2GObjOnThread(&bOnEvent);
		}

		if(m_dwEvents){
			//�C�x���g�҂�
			if(!bResult)
			{
				dwError = GetLastError();
				if(dwError == ERROR_IO_PENDING){
					do {
						dwWaitObject = WaitForSingleObject(m_EventInfo.hEventHandles[0], INFINITE);
					}while(dwWaitObject == WAIT_TIMEOUT);
				}

			};

			dwWaitObject = WaitForMultipleObjects(m_dwEvents, m_EventInfo.hEventHandles, FALSE, m_dwThreadLoopCycle);
			if(dwWaitObject ==WAIT_TIMEOUT){
				//�C�x���g�^�C���A�E�g����
				if(A2gEventTOutProc){
					//�C�x���g�^�C���A�E�g�������R�[���o�b�N
					lResult = A2gEventTOutProc(m_EventTOutCallBacks.pObj);
				}
			}
			else{
				//�C�x���g����
				dwEvent = WAIT_OBJECT_0-dwWaitObject;
				A2gEventProc = m_EventCallBacks[dwEvent].proc;
				//�C�x���g�������R�[���o�b�N
				lResult = A2gEventProc(dwEvent, m_EventCallBacks[dwEvent].pObj);

				//�C�x���g�������I��
				bOnEvent = FALSE;
				//�X���b�h�X���[�v
				Sleep(m_dwThreadLoopCycle);
			}
		}
		else{
			//�X���b�h�X���[�v
			Sleep(m_dwThreadLoopCycle);
		}


#ifdef _DEBUG		
	//Beep(880, 100);
#endif

	}
	return THREAD_ENDCODE_NORMAL;
}

