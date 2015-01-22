#pragma once
/********************************************************************************************/
/* CA2GObjThread�N���X�w�b�_																*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20080110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
#define THREAD_END_TIMEOUTS			5000			//�X���b�h�I���҂��^�C���A�E�g�l(msec)
#define THREAD_ENDCODE_NORMAL		0x00000000		//�X���b�h����I��				
#define THREAD_ENDCODE_FORCED		0x80000000		//�X���b�h�����I��
#define THREAD_MAX_EVENTS			8				//�X���b�h���ŏ�������C�x���g�̍ő吔
#define THREAD_EVENTS_CYCLE			5				//�C�x���g���[�v����(msec)

//�X���b�h
#define THREAD_CBCODE_NORMAL		0x00000000		//����I��
#define THREAD_CBCODE_E_PENDING		0x00000001		//�G���[���y���f�B���O
#define THREAD_CBCODE_E_STOP		0x00010000		//�X���b�h��~���w��
#define THREAD_CBCODE_E_FORCED		0x80000000		//�X���b�h�̋����I�����w��


//���b�Z�[�W���[�v�R�[���o�b�N�֐��̒�`
typedef LRESULT (CALLBACK* A2GOBJ_THREAD_PROC)(LPVOID pObj);
typedef struct _a2gobjthreadprocs
{
	A2GOBJ_THREAD_PROC	A2GObjThreadBefor;
	A2GOBJ_THREAD_PROC	A2GObjOnThread;
	A2GOBJ_THREAD_PROC	A2GObjThreadAfter;
}A2GOBJTHREADPROCS, *PA2GOBJTHREADPROCS;

//OS�C�x���g�EOS���b�Z�[�W�ł̃R�[���o�b�N�֐��̒�`
typedef LRESULT (CALLBACK* A2GOBJ_MSG_PROC)(UINT uiMsg, WPARAM wParam, LPARAM lParam, LPVOID pObj);
typedef LRESULT (CALLBACK* A2GOBJ_EVENT_PROC)(DWORD dwEvent, LPVOID pObj);
typedef LRESULT (CALLBACK* A2GOBJ_EVENT_TIMEOUT_PROC)(LPVOID pObj);

typedef struct _a2gmsgcallback
{
	A2GOBJ_MSG_PROC		Proc;
	LPVOID				pObj;
} A2GMSGCALLBACK, *PA2GMSGCALLBACK;

typedef struct _a2gevcallback
{
	A2GOBJ_EVENT_PROC	proc;
	LPVOID				pObj;
} A2GEVCALLBACK, *PA2GEVCALLBACK;

typedef struct _a2gevtoutcallback
{
	A2GOBJ_EVENT_TIMEOUT_PROC	proc;
	LPVOID						pObj;
} A2GEVTOUTCALLBACK, *PA2GEVTOUTCALLBACK;

typedef struct _eventinfo
{
	HANDLE		hEventHandles[THREAD_MAX_EVENTS];
} EVENT_INFO, *PEVENT_INFO;


class CA2GObjThread
{
public:
	CA2GObjThread(LPVOID pObj, PA2GOBJTHREADPROCS pA2gObjThreadProcs);
	CA2GObjThread(void);
	virtual ~CA2GObjThread(void);
	void SetThreadProcs(LPVOID pObj, PA2GOBJTHREADPROCS pA2gObjThreadProcs);
	int SetMessageProc(A2GOBJ_MSG_PROC A2gMsgProc, LPVOID pObj);									//���b�Z�[�WPROC�̓o�^
	int SetEventProc(HANDLE hEvent, HANDLE hDevice, A2GOBJ_EVENT_PROC A2gEventProc, LPVOID pObj);	//�C�x���gPROC�̓o�^
	int SetEventTOutProc(A2GOBJ_EVENT_TIMEOUT_PROC A2gEventTOutProc, LPVOID pObj);					//�C�x���g�^�C���A�E�gPROC�̓o�^
	int SetThreadCycle(DWORD dwCycle);																//�X���b�h�����Z�b�g
	int SetHandle(HANDLE hHandle);																	//�Ď�����n���h�����Z�b�g����
	int CreateThread(int nPriority);																//�X���b�h�N��
	int EndThread(void);																			//�X���b�h�I��

protected:
	HANDLE	m_hEPThread;														//�G���h�|�C���g�X���b�h�n���h��
	UINT	m_uEPThreadID;														//�G���h�|�C���g�X���b�h�n���h��

private:
	static	UINT WINAPI A2GObjThreadStab(void* pParam);							//�X���b�h�X�^�u
	int		A2GObjThreadBefor(void);											//�X���b�h�O����
	UINT	A2GObjThread(void);													//�X���b�h�̎���
	int		A2GObjOnThread(LPBOOL pOnEvnet);									//�X���b�h����
	int		A2GObjThreadAfter(void);											//�X���b�h�㏈��
	void	CA2GObjInitialize(void);

	EVENT_INFO			m_EventInfo;
	DWORD				m_dwEvents;												//�o�^���ꂽ�C�x���g�̐�
	A2GMSGCALLBACK		m_MsgCallBack;											//���b�Z�[�W�R�[���o�b�N�e�[�u��
	A2GEVCALLBACK		m_EventCallBacks[THREAD_MAX_EVENTS];					//�C�x���g�R�[���o�b�N�e�[�u��
	A2GEVTOUTCALLBACK	m_EventTOutCallBacks;									//�C�x���g�^�C���A�E�g�R�[���o�b�N�e�[�u��
	A2GOBJTHREADPROCS	m_A2gObjTheradProcs;									//�X���b�h�R�[���o�b�N��`
	LPVOID				m_pObjThreadProcs;										//�X���b�h�R�[���o�b�N���̃C���X�^���X
	DWORD				m_dwThreadLoopCycle;									//�X���b�h���[�v����
};
