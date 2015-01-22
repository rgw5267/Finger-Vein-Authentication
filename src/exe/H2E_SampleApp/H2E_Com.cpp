#include "stdafx.h"
#include <winioctl.h>
#include <setupapi.h>
#include "H2E_Com.h"

CH2ECom::CH2ECom(void)
{
	int i;

	//Initialize H2E interface table
	int iLoop = sizeof(m_pH2EInterface)/sizeof(CH2EInterface*);
	for(i=0; i<iLoop; i++)
	{
		m_pH2EInterface[i] = 0;
	}
	//
	//                        CH2EInterface(Command, length, oplation mode, making command method, analysis respons method);
	//
	//Create H2E interface.
	m_pH2EInterface[0]	= new CH2EInterface(0x00, 0x0000, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyEnroll);			//No Opration
	m_pH2EInterface[1]	= new CH2EInterface(0x12, 0x0001, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyGetDevInfo);		//#1 Get Infomation
	m_pH2EInterface[2]	= new CH2EInterface(0x13, 0x0000, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#2 Hard ware Reset
	m_pH2EInterface[3]	= new CH2EInterface(0x14, 0x0000, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#3 PASS_DRVER Pin Cancel
	m_pH2EInterface[4]	= new CH2EInterface(0x21, 0x0005, H2E_CMD_1BYTE2WORD,	&CH2ECom::makeStCmd, &CH2ECom::anlyEnroll);			//#4 Template Enroll
	m_pH2EInterface[5]	= new CH2EInterface(0x16, 0x0005, H2E_CMD_1BYTE2WORD,	&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#5 Template Delete
	m_pH2EInterface[6]	= new CH2EInterface(0x17, 0x0003, H2E_CMD_1BYTE2WORD,	&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#6 Template Delete(Group)
	m_pH2EInterface[7]	= new CH2EInterface(0x18, 0x0002, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#7 Template Initial
	m_pH2EInterface[8]	= new CH2EInterface(0x19, 0xFFFF, H2E_CMD_1ADD4N,		&CH2ECom::makeStCmd, &CH2ECom::anlyVerification);	//#8 Verifcation 1:1
	m_pH2EInterface[9]	= new CH2EInterface(0x1A, 0xFFFF, H2E_CMD_1ADD2N,		&CH2ECom::makeStCmd, &CH2ECom::anlyVerification);	//#9 Verifcation 1:N(Group)
	m_pH2EInterface[10]	= new CH2EInterface(0x1F, 0x0001, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyVerification);	//#10 Verifcation 1:N(Template)
	m_pH2EInterface[11]	= new CH2EInterface(0x1B, 0x0000, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyGetTempInfo);	//#11 Get Template Infomation
	m_pH2EInterface[12]	= new CH2EInterface(0x1C, 0x0005, H2E_CMD_1BYTE2WORD,	&CH2ECom::makeStCmd, &CH2ECom::anlyReadTemplate);	//#12 Read Template
	m_pH2EInterface[13]	= new CH2EInterface(0x1D, 0x043D, H2E_CMD_NOOPT,		&CH2ECom::makeWriteTemplate, &CH2ECom::anlyStRes);	//#13 Wtite Template(C2h)
	m_pH2EInterface[14]	= new CH2EInterface(0x1D, 0x063D, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#13 Wtite Template(C3h)
	m_pH2EInterface[15]	= new CH2EInterface(0x1E, 0x0009, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#14 Set Template ID
	m_pH2EInterface[16]	= new CH2EInterface(0x23, 0x0009, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#15 Change Template ID
	m_pH2EInterface[17]	= new CH2EInterface(0x20, 0x0000, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#16 Get FllashROM Condition
	m_pH2EInterface[18]	= new CH2EInterface(0x22, 0x0001, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#17 Set Security Level
	m_pH2EInterface[19]	= new CH2EInterface(0x24, 0x0001, H2E_CMD_NOOPT,		&CH2ECom::makeStCmd, &CH2ECom::anlyStRes);			//#18 Set Mode

	// initialize a variable
	m_hCom	= 0;									//Com port handle
	m_pTempList = 0;								//template infomation manage table
	m_dwTotalTemps = 0;								//total templates counter
	m_pSendBuffer = new BYTE[H2E_SENDBUFF_MAX];		//create send buffer
	ZeroMemory(m_pSendBuffer, H2E_SENDBUFF_MAX);	
	m_pRecvBuffer = new BYTE[H2E_RECVBUFF_MAX];		//create receive buffer
	ZeroMemory(m_pSendBuffer, H2E_RECVBUFF_MAX);
	ZeroMemory(m_cFwVersion, sizeof(m_cFwVersion));
	ZeroMemory(m_cSerialNum, sizeof(m_cSerialNum));


}

CH2ECom::~CH2ECom(void)
{

	if(m_pSendBuffer){
		delete[] m_pSendBuffer;
	}
	if(m_pRecvBuffer){
		delete[] m_pRecvBuffer;
	}
	if(m_hCom){
		CloseHandle(m_hCom);
	}

	int i;
	int iLoop = sizeof(m_pH2EInterface)/sizeof(CH2EInterface*);
	for(i=0; i<iLoop; i++)
	{
		if(m_pH2EInterface[i]){
			delete m_pH2EInterface[i];
		}
	}
}
//
//COM port open Method
//
int CH2ECom::Open(LPCTSTR pComPort, UINT uiMode, LPDWORD pdwError)
{
	int			iReturn;
	DWORD		dwError;
	HANDLE		hCom;
	CString		strComPort;

	iReturn		= H2E_OK;
	*pdwError	= 0;

	// COM port open
	strComPort	= _T("\\\\.\\");
	strComPort	+= pComPort;
	if(m_hCom){
		Close(&dwError);
	}
	hCom = CreateFile(strComPort,	GENERIC_READ|GENERIC_WRITE, 0, NULL, 
						OPEN_EXISTING, 0, NULL ) ;
	if ( hCom == INVALID_HANDLE_VALUE ) {
		*pdwError = GetLastError();
		return H2E_ERR_COMOPEN;
	}
	m_hCom = hCom;

	//DCB set.
	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
	dcb.DCBlength	= sizeof(DCB);
	dcb.fBinary		= 1;
	dcb.fParity		= 1;
	if(uiMode == H2E_SPEED_57600){
		//57.6Kbps
		dcb.BaudRate	= CBR_57600;
	}
	else{
		//19.2KBps
		dcb.BaudRate	= CBR_19200;
	}
	dcb.ByteSize	= 8;
	dcb.Parity		= ODDPARITY;
	dcb.StopBits	= ONESTOPBIT;
	SetCommState(m_hCom, &dcb);

	COMMTIMEOUTS cts;
	cts.ReadTotalTimeoutConstant	= 10000;
	cts.ReadTotalTimeoutMultiplier	= 0;
	cts.WriteTotalTimeoutConstant	= 10000;
	cts.WriteTotalTimeoutMultiplier	= 0;
	SetCommTimeouts(m_hCom, &cts);


	return iReturn;
}

//
//COM port close Method
//
int CH2ECom::Close(LPDWORD pdwError)
{
	int		iReturn;

	iReturn		= H2E_OK;
	*pdwError	= 0;

	if(m_hCom){
		CloseHandle(m_hCom);
		m_hCom = 0;
		iReturn = H2E_ERR_COMCLOSE;
	}
	m_hCom = 0;
	return iReturn;
}

//
//Sensing tuch sensor state (TUCH_OUT1, TUCH_OUT2) Method
//
//#define MS_CTS_ON           ((DWORD)0x0010)
//#define MS_DSR_ON           ((DWORD)0x0020)
//#define MS_RING_ON          ((DWORD)0x0040)
//#define MS_RLSD_ON          ((DWORD)0x0080)

int CH2ECom::SensingLine(LPDWORD pdwTuchStatus)
{
	int		iReturn;
	DWORD	dwModemStatus;
	DWORD   dwTStatus;
	BOOL	bReturn;

	iReturn		= H2E_OK;
	dwTStatus	= 0;

	if(m_hCom){	
		//get modem state.
		bReturn = GetCommModemStatus(m_hCom, &dwModemStatus);
		if(bReturn){
			if(dwModemStatus & MS_RLSD_ON){
				//TUCH_OUT1 active
				dwTStatus |= H2E_TOUCH_OUT1;
			}
			if(dwModemStatus & MS_RING_ON){
				//TUCH_OUT2 active
				dwTStatus |= H2E_TOUCH_OUT2;
			}
		}
	}
	else{
		iReturn = H2E_ERR_COMNOTOPEN;
	}

	*pdwTuchStatus = dwTStatus;

	return iReturn;
}

//
//Send command to devaice Method
//
int CH2ECom::TransToDevice(HANDLE hWnd, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, DWORD* pdwError)
{
	int					iReturn;
	DWORD	dwSendLen;
	H2E_MAKEBUFF_PROC	makeProc;
	H2E_ANALYSBUFF_PROC	AnlsProc;
	CH2EInterface*		pH2EIfObj;

	pH2EIfObj	= m_pH2EInterface[uiSendID];
	makeProc	= pH2EIfObj->m_makeProc;
	AnlsProc	= pH2EIfObj->m_analysProc;

	if(uiSendID != H2E_COM_SENDNOP){
		//make send data
		iReturn = (this->*makeProc)(pH2EIfObj, uiSendID, byParam, pOption, wOptionLen, &dwSendLen);
		if(iReturn < H2E_OK){
			return iReturn;
		}
		//send command data to device
		iReturn =  sendCommand(uiSendID, dwSendLen, pdwError);
		if(iReturn < H2E_OK){
			return iReturn;
		}

	}
	LPVOID pRecvData;
	WORD	wRecvLength;
	//Receive respons from device
	iReturn = recvResponse(uiSendID, byParam, &pRecvData, &wRecvLength, pdwError);
	if(iReturn < H2E_OK){
		return iReturn;
	}
	//analysis recived data
	iReturn = (this->*AnlsProc)(pH2EIfObj, uiSendID, byParam, pOption, wOptionLen, pRecvData, wRecvLength);

	return iReturn;
}

//
//Send command to device Method
//
int CH2ECom::sendCommand(UINT uiSendID, DWORD dwSendLen, DWORD* pdwError)
{
	int			iReturn;
	BOOL		bRet;
	DWORD		dwWrite;
	DWORD		dwError;
	DWORD		dwRead;
	COMSTAT		ComStat;

	ClearCommError(m_hCom, &dwError, &ComStat);
	if(ComStat.cbInQue > 0){
		//dummy read driver's buffer
		ReadFile(
				m_hCom,
				m_pRecvBuffer,
				ComStat.cbInQue,
				&dwRead,
				NULL
 				) ;
	}

	iReturn = H2E_OK;
	//Send command data
	bRet = WriteFile(
			m_hCom,
			m_pSendBuffer,
			dwSendLen,
			&dwWrite,
			NULL
			);
	if(!bRet){
		iReturn = H2E_ERR_WRITEFILE;
	}
	if(iReturn < H2E_OK){
		*pdwError = GetLastError();
	}
	return iReturn;
}

//
// receive respons from device Method
//
int CH2ECom::recvResponse(UINT uiSendID, BYTE byOpt,  LPVOID* pSerData, WORD* pwSerLength, DWORD* pdwError)
{

	int			iReturn;
	BOOL		bRet;
	DWORD		dwRecvSize;
	DWORD		dwRead;
	COMSTAT		ComStat;
	DWORD		dwError;
	BYTEWORD	bwLength;
	LPVOID		pData;
	int			i;

	iReturn				= H2E_OK;
	*pdwError			= 0;
	dwError				= 0 ;
	bwLength.word		= 0;
	pData				= 0;
	memset(&ComStat, 0, sizeof(COMSTAT)) ;


	m_Result		= 0;
	m_RecvLength	= 0;
	m_DevErrCode	= 0;

	//check arrived respons in driver's buffer
	switch(uiSendID){
		case H2E_COM_TMP_ENROLL:
		case H2E_COM_VER1TO1:
		case H2E_COM_VER_G_1TON:
		case H2E_COM_VER_1TON:
			// enroll and verification respons
			while(ComStat.cbInQue == 0){
				Sleep(100);
				bRet = ClearCommError(m_hCom, &dwError, &ComStat);
			}
			break;
		case H2E_COM_READ_TMP:
			//read template respons
			while(ComStat.cbInQue < 0x43C){
				Sleep(100);
				bRet = ClearCommError(m_hCom, &dwError, &ComStat);
			}
			break;
		default:
			//other respons.
			for(i=0; i<20; i++){
				Sleep(100);
				bRet = ClearCommError(m_hCom, &dwError, &ComStat);
				if(ComStat.cbInQue > 0 ){
					break;
				}
			}
			break;
	}
	if(bRet){
		dwRecvSize = ComStat.cbInQue;
		if ( dwRecvSize > 0 ) {
			// read response
			bRet = ReadFile(
					m_hCom,	
					m_pRecvBuffer,
					dwRecvSize,
					&dwRead,
					NULL
 					) ;
			if(!bRet){
				//Read File Error
				iReturn = H2E_ERR_READFILE;
			}
			m_Result			= m_pRecvBuffer[0];
			bwLength.byte[0]	= m_pRecvBuffer[2];
			bwLength.byte[1]	= m_pRecvBuffer[1];
			m_RecvLength		= bwLength.word;
			if(m_RecvLength > 0){
				m_DevErrCode = m_pRecvBuffer[3];
			}
			pData				= &m_pRecvBuffer[3];
			if(m_pRecvBuffer[0] != 0){	
				iReturn = H2E_ERR_RESPONS;
			}
		}
		else{
			iReturn =  H2E_ERR_RDATA_NON;
		}
	}
	else{
		iReturn = H2E_ERR_CCOMERR;
	}
	if(iReturn < H2E_OK){
		dwError = GetLastError();
	}
	*pSerData		= pData;
	*pwSerLength	= bwLength.word;
	*pdwError		= dwError;
	return iReturn;
}
int CH2ECom::makeNop(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOptionData, WORD wOptionLen, DWORD* pdwSendLength)
{
	int	iReturn;
	iReturn  = H2E_OK;

	return iReturn;

}

//
// make command data (standard commands) Method
//
int CH2ECom::makeStCmd(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOptionData, WORD wOptionLen, DWORD* pdwSendLength)
{
	int				iReturn;
	BYTEWORD		bwDataLen;
	WORD			wOption;
	WORD			wOffset;
	WORD*			pWord;
	BYTEWORD		bw;
	LPBYTE			pbyOptionData;
	CH2EInterface*	pH2EIfObj;
	BYTE			byCmd;


	iReturn = H2E_OK;
	*pdwSendLength	= 0;

	pH2EIfObj		= (CH2EInterface*)pObj;
	wOption			= pH2EIfObj->m_wOpe;
	byCmd			= pH2EIfObj->m_byCmd;
	if(pH2EIfObj->m_wLength == 0xFFFF){
		bwDataLen.word	= wOptionLen + 1;
	}
	else{
		bwDataLen.word	= pH2EIfObj->m_wLength;
	}
	wOffset = 0;
	m_pSendBuffer[wOffset++] = byCmd; 
	m_pSendBuffer[wOffset++] = bwDataLen.byte[1];
	m_pSendBuffer[wOffset++] = bwDataLen.byte[0];
	if(bwDataLen.word > 0){
		int i;
		int iLoop;

		m_pSendBuffer[wOffset++] = byParam;
		iLoop = bwDataLen.word -1;
		switch(wOption){
			case H2E_CMD_NOOPT:
				pbyOptionData = (BYTE*)pOptionData;
				for(i=0; i<iLoop; i++){
					m_pSendBuffer[wOffset++] = pbyOptionData[i];
				}
				break;
			case H2E_CMD_1BYTE2WORD:
				pWord = (WORD*)pOptionData;
				bw.word = pWord[0];
				m_pSendBuffer[wOffset++] = bw.byte[1];
				m_pSendBuffer[wOffset++] = bw.byte[0];
				bw.word = pWord[1];
				m_pSendBuffer[wOffset++] = bw.byte[1];
				m_pSendBuffer[wOffset++] = bw.byte[0];
				break;
			case H2E_CMD_1ADD4N:
			case H2E_CMD_1ADD2N:
				pbyOptionData = (BYTE*)pOptionData;
				iLoop /=2;
				pWord = (WORD*)pOptionData;
				for(i=0; i<iLoop; i++){
					bw.word = pWord[i];
					m_pSendBuffer[wOffset++] = bw.byte[1];
					m_pSendBuffer[wOffset++] = bw.byte[0];
				}
				bwDataLen.word = iLoop*2 + 1;
				m_pSendBuffer[1] = bwDataLen.byte[1];
				m_pSendBuffer[2] = bwDataLen.byte[0];
				break;
			default:
				break;
		}
	}
	*pdwSendLength = (DWORD)wOffset;
	return iReturn;

}

//
//for write template make command data Method
//
int CH2ECom::makeWriteTemplate(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOptionData, WORD wOptionLen, DWORD* pdwSendLength)
{
	int			iReturn;
	PTEMPU		pTempU;	
	WORD		wOffset;
	WORDDWORD	wdw;
	BYTEWORD	bwDataLen;
	BYTEWORD	bw;
	CH2EInterface*	pH2EIfObj;
	BYTE			byCmd;


	iReturn = H2E_OK;
	pTempU = (PTEMPU)pOptionData;
	pH2EIfObj		= (CH2EInterface*)pObj;
	byCmd			= pH2EIfObj->m_byCmd;
	bwDataLen.word	= 0x43d;

	wOffset = 0;
	m_pSendBuffer[wOffset++] = byCmd; 
	m_pSendBuffer[wOffset++] = bwDataLen.byte[1];
	m_pSendBuffer[wOffset++] = bwDataLen.byte[0];
	m_pSendBuffer[wOffset++] = byParam;

	bw.word = pTempU->uiGroupNum;
	m_pSendBuffer[wOffset++] = bw.byte[1];
	m_pSendBuffer[wOffset++] = bw.byte[0];

	bw.word = pTempU->uiTmpNum;
	m_pSendBuffer[wOffset++] = bw.byte[1];
	m_pSendBuffer[wOffset++] = bw.byte[0];

	memcpy(&m_pSendBuffer[wOffset], pTempU->byTemplate, MAX_TEMPLATE_LEN);
	wOffset += MAX_TEMPLATE_LEN;

	wdw.dword = pTempU->dwAddSum;
	m_pSendBuffer[wOffset++] = wdw.byte[3];
	m_pSendBuffer[wOffset++] = wdw.byte[2];
	m_pSendBuffer[wOffset++] = wdw.byte[1];
	m_pSendBuffer[wOffset++] = wdw.byte[0];

	wdw.dword = pTempU->dwXorSum;
	m_pSendBuffer[wOffset++] = wdw.byte[3];
	m_pSendBuffer[wOffset++] = wdw.byte[2];
	m_pSendBuffer[wOffset++] = wdw.byte[1];
	m_pSendBuffer[wOffset++] = wdw.byte[0];

	*pdwSendLength = wOffset;

	return iReturn;
}

//
//analysis respons (standard respons) Method
//
int CH2ECom::anlyStRes(LPVOID pObj, UINT uiSendID, BYTE byParam,  LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen)
{
	int iReturn;
	LPBYTE pRecvData = (LPBYTE)pRcvData;

	iReturn = H2E_OK;
	if(wRecvLen > 1){
		m_DevErrCode = pRecvData[0];
		if(wRecvLen == 1){
			iReturn = H2E_ERR_RESPONS;
		}
		else{
			iReturn = H2E_ERR_ANLYSBUFFER;
		}
	}
	else{
		m_DevErrCode = 0;
	}
	return iReturn;
}

//
//for device infonation analysis respons Method
//
int CH2ECom::anlyGetDevInfo(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen)
{
	int	iReturn;

	iReturn = H2E_ERR_ANLYSBUFFER;
	if(uiSendID == H2E_COM_GETDEVINFO){
		if(byParam == 0x0000){
			memcpy(m_cFwVersion, pRcvData, wRecvLen);
			iReturn = H2E_OK;
		}
		if(byParam == 0x0001){
			memcpy(m_cSerialNum, pRcvData, wRecvLen);
			iReturn = H2E_OK;
		}
	}
	return iReturn;
}

//
//for enroll analysis respons Method
//
int CH2ECom::anlyEnroll(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen)
{
	int	iReturn;
	BYTEWORD bw;
	LPBYTE pSerData = (LPBYTE)pRcvData;

	iReturn = H2E_OK;
	if((uiSendID == H2E_COM_TMP_ENROLL) || (uiSendID == H2E_COM_SENDNOP)){
		switch(wRecvLen){
			case 0x0001:
				if(pSerData[0] == 0xAA){
					iReturn = H2E_TAKEEND;
				}
				break;
			case 0x0002:
				m_byEnroll_QLevel = pSerData[1];
				break;
			case 0x0005:
				m_byEnroll_QLevel = pSerData[0];
				bw.byte[1] = pSerData[1];
				bw.byte[0] = pSerData[2];
				m_wEnroll_Group = bw.word;
				m_byEnroll_QLevel = pSerData[0];
				bw.byte[1] = pSerData[3];
				bw.byte[0] = pSerData[4];
				m_wEnroll_Template = bw.word;
				break;
			default:
				iReturn = H2E_ERR_ANLYSBUFFER;
				break;
		}
	}
	else{
		iReturn = H2E_ERR_ANLYSBUFFER;
	}
	return iReturn;
}

//
//for verification analysis respons Method
//
int CH2ECom::anlyVerification(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen)
{
	int	iReturn;
	BYTEWORD bw;
	LPBYTE pSerData = (LPBYTE)pRcvData;

	iReturn = H2E_OK;
	switch(wRecvLen){
		case 0x0002:
			m_byVerifLevel = pSerData[0];
			m_byVerifIndex = pSerData[1];
			break;
		case 0x0005:
			m_byVerifLevel = pSerData[0];
			bw.byte[1] = pSerData[1];
			bw.byte[0] = pSerData[2];
			m_wVerifGroup = bw.word;
			bw.byte[1] = pSerData[3];
			bw.byte[0] = pSerData[4];
			m_wVerifTemp = bw.word;
			break;
		default:
			iReturn = H2E_ERR_ANLYSBUFFER;
			break;
	}

	return iReturn;
}

//
//for read template analysis respons Method
//

int CH2ECom::anlyReadTemplate(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen)
{
	int	iReturn;
	LPBYTE pSerData = (LPBYTE)pRcvData;
	iReturn = H2E_OK;
	WORD* pWOption = (WORD*)pOption;

	ZeroMemory(&m_TempU, sizeof(m_TempU));
	m_TempU.uiGroupNum = pWOption[0];
	m_TempU.uiTmpNum = pWOption[1];
	m_DevErrCode = pSerData[0];
	memcpy(m_TempU.byTemplate, &pSerData[1], MAX_TEMPLATE_LEN);
	m_TempU.uiQLevel = 0xFF;

	LPVOID pWork = m_TempU.byTemplate;
	LPDWORD pdwSum = (LPDWORD)pWork;
	DWORD dwLoops = MAX_TEMPLATE_LEN/4;
	DWORD dw = 0;
	WORDDWORD wdwAdd;
	WORDDWORD wdwXor;
	WORDDWORD wdwWork;
	WORDDWORD wdwWork2;

	DWORD dwAddSum = 0;
	DWORD dwXorSum = 0;
	for(dw=0; dw<dwLoops; dw++){
		wdwWork.dword = pdwSum[dw];
		wdwWork2.byte[3] = wdwWork.byte[0];
		wdwWork2.byte[2] = wdwWork.byte[1]; 
		wdwWork2.byte[1] = wdwWork.byte[2]; 
		wdwWork2.byte[0] = wdwWork.byte[3];
		dwAddSum += wdwWork2.dword;
		dwXorSum ^= wdwWork2.dword;
	}
	wdwAdd.byte[3] = pSerData[1073];
	wdwAdd.byte[2] = pSerData[1074];
	wdwAdd.byte[1] = pSerData[1075];
	wdwAdd.byte[0] = pSerData[1076];
	wdwXor.byte[3] = pSerData[1077];
	wdwXor.byte[2] = pSerData[1078];
	wdwXor.byte[1] = pSerData[1079];
	wdwXor.byte[0] = pSerData[1080];
	m_TempU.dwAddSum = wdwAdd.dword;
	m_TempU.dwXorSum = wdwXor.dword;

	if(dwAddSum != wdwAdd.dword){
		iReturn = H2E_ERR_ADDSUM;
		return iReturn;
	}
	if(dwXorSum != wdwXor.dword){
		iReturn = H2E_ERR_XORSUM;
		return iReturn;
	}

	return iReturn;
}

//
//for get template infomation analysis respons Method
//
int CH2ECom::anlyGetTempInfo(LPVOID pObj, UINT uiSendID, BYTE byParam, LPVOID pOption, WORD wOptionLen, LPVOID pRcvData, WORD wRecvLen)
{
	int			iReturn;
	BYTEWORD	bwGlen;
	BYTEWORD	bwTlen;
	BYTEWORD	bw;
	int			iOff;
	WORD		dw;
	WORD		dw2;
	WORD*		pGList;
	SYSTEMTIME	st;
	PTEMPINFO	pTempInfo;

	LPBYTE pSerData = (LPBYTE)pRcvData;
	ZeroMemory(&st, sizeof(SYSTEMTIME));
	pTempInfo = (PTEMPINFO)pOption;
	m_pTempList = pTempInfo->pTempList;

	iReturn = H2E_OK;
	iOff = 0;
	pGList = 0;
	if(uiSendID == H2E_COM_GET_TMP_INFO){
		if(pSerData[iOff++] == 0x00){
			bwGlen.byte[1] = pSerData[iOff++];
			bwGlen.byte[0] = pSerData[iOff++];
			if(bwGlen.word > 0){
				//Template Exits
				pGList = new WORD[bwGlen.word];
				for(dw=0; dw<bwGlen.word; dw++){
					bw.byte[1] = pSerData[iOff++];
					bw.byte[0] = pSerData[iOff++];
					pGList[dw] = bw.word;
				}
				m_dwTotalTemps = 0;
				for(dw=0; dw<bwGlen.word; dw++){
					bwTlen.byte[1] = pSerData[iOff++];
					bwTlen.byte[0] = pSerData[iOff++];
					dw2 = 0;
					for(dw2=0; dw2<bwTlen.word; dw2++){
						bw.byte[1] = pSerData[iOff++];
						bw.byte[0] = pSerData[iOff++];
						m_pTempList[m_dwTotalTemps].uiGroupNum = pGList[dw];
						m_pTempList[m_dwTotalTemps].uiTempNum = bw.word;
						m_pTempList[m_dwTotalTemps].time = st;
						m_pTempList[m_dwTotalTemps].uiQLevel = 0xFF;
						m_dwTotalTemps++;
					}
				}
				pTempInfo->dwTemplates = m_dwTotalTemps;
				if(pGList){
					delete[] pGList;
					pGList = 0;
				}
			}
		}
	}
	else{
		iReturn = H2E_ERR_ANLYSBUFFER;
	}

	return iReturn;
}

int CH2ECom::GetTempU(PTEMPU pTempU)
{
	int iReturn;
	iReturn = H2E_OK;

	*pTempU = m_TempU;

	return iReturn;
}


//
//enumerate COM port classes Method
//

int CH2ECom::EnumComPort(HWND hWnd, CString* pstrPortName, LPDWORD pdwCounts)
{
	int iReturn;
	iReturn  = H2E_OK;

	DWORD i;
	BYTE Buffer[256]; 
	DWORD Length = 0;
	SP_DEVINFO_DATA DeviceInfoData = {sizeof(SP_DEVINFO_DATA)};
	HDEVINFO hDevInfo = 0;
 
	hDevInfo = SetupDiGetClassDevs( &GUID_DEVINTERFACE_COMPORT, NULL, hWnd, ( DIGCF_PRESENT | DIGCF_DEVICEINTERFACE ) );
	for(i=0; SetupDiEnumDeviceInfo( hDevInfo, i, &DeviceInfoData ); i++)
	{
		HKEY tmp_key = SetupDiOpenDevRegKey( hDevInfo, &DeviceInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE );
		if( tmp_key){
			DWORD tmp_type = 0;
			DWORD tmp_size = sizeof(Buffer);
			RegQueryValueEx(tmp_key,_T("PortName"),NULL, &tmp_type, Buffer, &tmp_size);
			if(pstrPortName){
				pstrPortName[i] = Buffer;
			}
			RegCloseKey( tmp_key );
		}
	} 
	*pdwCounts = i;
	return iReturn;
}