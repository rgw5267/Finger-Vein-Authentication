#pragma once
/********************************************************************************************/
/* CA2GObjThreadクラスヘッダ																*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20080110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
#define THREAD_END_TIMEOUTS			5000			//スレッド終了待ちタイムアウト値(msec)
#define THREAD_ENDCODE_NORMAL		0x00000000		//スレッド正常終了				
#define THREAD_ENDCODE_FORCED		0x80000000		//スレッド強制終了
#define THREAD_MAX_EVENTS			8				//スレッド内で処理するイベントの最大数
#define THREAD_EVENTS_CYCLE			5				//イベントループ周期(msec)

//スレッド
#define THREAD_CBCODE_NORMAL		0x00000000		//正常終了
#define THREAD_CBCODE_E_PENDING		0x00000001		//エラーをペンディング
#define THREAD_CBCODE_E_STOP		0x00010000		//スレッド停止を指示
#define THREAD_CBCODE_E_FORCED		0x80000000		//スレッドの強制終了を指示


//メッセージループコールバック関数の定義
typedef LRESULT (CALLBACK* A2GOBJ_THREAD_PROC)(LPVOID pObj);
typedef struct _a2gobjthreadprocs
{
	A2GOBJ_THREAD_PROC	A2GObjThreadBefor;
	A2GOBJ_THREAD_PROC	A2GObjOnThread;
	A2GOBJ_THREAD_PROC	A2GObjThreadAfter;
}A2GOBJTHREADPROCS, *PA2GOBJTHREADPROCS;

//OSイベント・OSメッセージでのコールバック関数の定義
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
	int SetMessageProc(A2GOBJ_MSG_PROC A2gMsgProc, LPVOID pObj);									//メッセージPROCの登録
	int SetEventProc(HANDLE hEvent, HANDLE hDevice, A2GOBJ_EVENT_PROC A2gEventProc, LPVOID pObj);	//イベントPROCの登録
	int SetEventTOutProc(A2GOBJ_EVENT_TIMEOUT_PROC A2gEventTOutProc, LPVOID pObj);					//イベントタイムアウトPROCの登録
	int SetThreadCycle(DWORD dwCycle);																//スレッド周期セット
	int SetHandle(HANDLE hHandle);																	//監視するハンドルをセットする
	int CreateThread(int nPriority);																//スレッド起動
	int EndThread(void);																			//スレッド終了

protected:
	HANDLE	m_hEPThread;														//エンドポイントスレッドハンドル
	UINT	m_uEPThreadID;														//エンドポイントスレッドハンドル

private:
	static	UINT WINAPI A2GObjThreadStab(void* pParam);							//スレッドスタブ
	int		A2GObjThreadBefor(void);											//スレッド前処理
	UINT	A2GObjThread(void);													//スレッドの実体
	int		A2GObjOnThread(LPBOOL pOnEvnet);									//スレッド処理
	int		A2GObjThreadAfter(void);											//スレッド後処理
	void	CA2GObjInitialize(void);

	EVENT_INFO			m_EventInfo;
	DWORD				m_dwEvents;												//登録されたイベントの数
	A2GMSGCALLBACK		m_MsgCallBack;											//メッセージコールバックテーブル
	A2GEVCALLBACK		m_EventCallBacks[THREAD_MAX_EVENTS];					//イベントコールバックテーブル
	A2GEVTOUTCALLBACK	m_EventTOutCallBacks;									//イベントタイムアウトコールバックテーブル
	A2GOBJTHREADPROCS	m_A2gObjTheradProcs;									//スレッドコールバック定義
	LPVOID				m_pObjThreadProcs;										//スレッドコールバック側のインスタンス
	DWORD				m_dwThreadLoopCycle;									//スレッドループ周期
};
