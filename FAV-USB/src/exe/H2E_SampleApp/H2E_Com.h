#pragma once
#include "afx.h"
#include "H2E.h"


#define H2E_SENDBUFF_MAX	2048
#define H2E_RECVBUFF_MAX	2048


#define H2E_CMD_NOOPT			0x0000
#define H2E_CMD_1BYTE2WORD		0x0001
#define H2E_CMD_1ADD4N			0x0002
#define H2E_CMD_1ADD2N			0x0003

#define H2E_SPEED_57600			0x00000001
#define H2E_SPEED_19200			0x00000002


class CH2ECom;
typedef int (CH2ECom::*H2E_MAKEBUFF_PROC)(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOptionData, WORD wOptionLen, DWORD* pdwSendLength);
typedef int (CH2ECom::*H2E_ANALYSBUFF_PROC)(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen);

class CH2EInterface
{
public:
	BYTE					m_byCmd;
	WORD					m_wLength;
	WORD					m_wOpe;
	H2E_MAKEBUFF_PROC		m_makeProc;
	H2E_ANALYSBUFF_PROC		m_analysProc;

	CH2EInterface(BYTE bycom, WORD wLength, WORD wOpe, H2E_MAKEBUFF_PROC makeProc, H2E_ANALYSBUFF_PROC analysProc)
	{
		m_byCmd			= bycom;
		m_wLength		= wLength;
		m_wOpe			= wOpe;
		m_makeProc		= makeProc;
		m_analysProc	= analysProc;
	}

};

class CH2ECom
{
public:
	CH2ECom();
	virtual ~CH2ECom(void);
	int Open(LPCTSTR pComPort,  UINT uiMode, LPDWORD pdwError);
	int Close(LPDWORD pdwError);
	int SensingLine(LPDWORD pdwTuchStatus);
	int TransToDevice(HANDLE hWnd, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, DWORD* pdwError);
	int GetDeviceInfo(CString strFwVersion, CString strSerialNum);
	int GetTempU(PTEMPU pTempU);
	int EnumComPort(HWND hWnd, CString* pstrPortName, LPDWORD pdwCounts);

	BYTE	m_Result;
	WORD	m_RecvLength;
	BYTE	m_DevErrCode;
	char	m_cFwVersion[50];
	char	m_cSerialNum[50];
	BYTE	m_byEnroll_QLevel;
	WORD	m_wEnroll_Group;
	WORD	m_wEnroll_Template;
	DWORD	m_dwTotalTemps;
	BYTE	m_byVerifLevel;
	BYTE	m_byVerifIndex;
	WORD	m_wVerifGroup;
	WORD	m_wVerifTemp;

private:
	CH2EInterface* m_pH2EInterface[20]; 
	int sendCommand(UINT uiSendID, DWORD dwSendLen, DWORD* pdwError);
	int recvResponse(UINT uiSendID, BYTE byOpt,  LPVOID* pSerData, WORD* pwSerLength, DWORD* pdwError);
	int makeNop(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOptionData, WORD wOptionLen, DWORD* pdwSendLength);
	int makeStCmd(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOptionData, WORD wOptionLen, DWORD* pdwSendLength);
	int makeWriteTemplate(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOptionData, WORD wOptionLen, DWORD* pdwSendLength);
	int anlyStRes(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen);
	int anlyGetDevInfo(LPVOID pObj, UINT uiSendID, BYTE byParam,LPVOID pOption, WORD wOptionLen,  LPVOID pRcvData, WORD wRecvLen);
	int anlyGetTempInfo(LPVOID pObj, UINT uiSendID, BYTE byParam,LPVOID pOption, WORD wOptionLen,  LPVOID pRcvData, WORD wRecvLen);
	int anlyEnroll(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen);
	int anlyVerification(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen);
	int anlyReadTemplate(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen);

	HANDLE	m_hCom;
	BYTE*	m_pSendBuffer;
	BYTE*	m_pRecvBuffer;
	PTEMPLIST m_pTempList;
	TEMPU	m_TempU;


};
