#include "stdafx.h"
#include "H1EUSB.h"
#include "H1EUSB_Com.h"
#include "H1EUSB_Dbg.h"
/********************************************************************************************/
/* CH1USBCom�N���X�v���O����																*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20120110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/********************************************************************************************/
/********************************************************************************************/
/*[���\�b�h��] CH1USBCom�N���X�̃R���X�g���N�^												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CH1USBCom(PH1ECOMOBJEP pH1EComObjEP)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CH1USBCom::CH1USBCom(PH1ECOMOBJEP pH1EComObjEP)
{
	//�����o�[�ϐ��̏�����
	m_hCom	= 0;
	m_hTEnd	= 0;
	memset( m_byCurrentWorkKey, 0, sizeof(m_byCurrentWorkKey) ) ;	// �Í���
	memset( m_uKttWork, 0, sizeof(m_uKttWork) ) ;					// �g�匮
	m_bCamelliaEnabled = FALSE ;									// �Í��ʐM��Ԃ��ۂ���\���t���O
	m_pwEnableBank = 0;
	m_wEnableBankCount = 0;
	m_iLastError = H1USB_OK;

	//�G���h�|�C���g����o�^
	m_H1EComObjEP	 = *pH1EComObjEP;

	//�ϐ��̏�����
	ZeroMemory(m_cFwVersion, sizeof(m_cFwVersion));
	ZeroMemory(m_cSerialNum, sizeof(m_cSerialNum));

	//�e���v���[�g���j�b�g�e�[�u���̏�����
	SetTempU(NULL, 0, TRUE);

	//�g�����U�N�V�������t���OOFF
	m_bTransEnable = FALSE;

}
/********************************************************************************************/
/*[���\�b�h��] CH1USBCom�N���X�̃f�X�g���N�^												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		~CH1USBCom(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CH1USBCom::~CH1USBCom(void)
{
	if(m_pwEnableBank){
		delete[] m_pwEnableBank;
	}
	if(m_hCom){
		CloseHandle(m_hCom);
	}
	if(m_hTEnd){
		CloseHandle(m_hTEnd);
	}
}




/********************************************************************************************/
/*[���\�b�h��] �G���h�|�C���g�����X�^�[�g													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		StartEndPoint(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�G���h�|�C���g����o�^���āA�G���h�|�C���g�̋@�\���J�n����				*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::StartEndPoint(void)
{
A2GDBG_OUTSTRING(_T("StartEndPoint() Entry\n"));
	int		iResult;
	iResult = H1USB_OK;

	//��M�X���b�h����̃R�[���o�b�N���[�`����o�^����B
	A2GOBJTHREADPROCS	A2objThreadProcs;
	A2objThreadProcs.A2GObjThreadBefor	= OnThreadBefor;			//�X���b�h���[�v�O
	A2objThreadProcs.A2GObjOnThread		= OnThread;					//�X���b�h���b�Z�[�W���[�v
	A2objThreadProcs.A2GObjThreadAfter	= OnThreadAfter;			//�X���b�h���[�v��
	CA2GObjThread::SetThreadProcs(this, &A2objThreadProcs);

	m_hTEnd = CreateEvent(NULL, TRUE, FALSE, NULL);					//�g�����U�N�V�����I���C�x���g�̍쐬

	//�ʐM�C�x���g�̐ݒ�
	CA2GObjThread::SetMessageProc(OnComMessage, this);
	iResult = CA2GObjThread::CreateThread(THREAD_PRIORITY_NORMAL);

A2GDBG_OUTSTRING(_T("StartEndPoint() Exit\n"));
	return iResult;
}

/********************************************************************************************/
/*[���\�b�h��] �G���h�|�C���g�����I��														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		StopEndPoint(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�G���h�|�C���g�̋@�\���~����												*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::StopEndPoint(void)
{
A2GDBG_OUTSTRING(_T("StopEndPoint() Entry\n"));

	//�G���h�|�C���g�X���b�h���I������B
	CA2GObjThread::EndThread();


A2GDBG_OUTSTRING(_T("StopEndPoint() Exit\n"));
	return H1USB_OK;
}

/********************************************************************************************/
/*[���\�b�h��] COM�|�[�g�I�[�v��															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		Open(LPCTSTR pComPort, LPDWORD pdwError)									*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::Open(LPCTSTR pComPort, LPDWORD pdwError)
{
	int			iReturn;
	BYTE		*bp = NULL ;
	HANDLE		hCom;
	iReturn		= H1USB_OK;
	*pdwError	= 0;
	DWORD		dwError;
	CString		strComPort;

	strComPort	= _T("\\\\.\\");
	strComPort	+= pComPort;
	if(m_hCom){
		//���Ƀ|�[�g���I�[�v������Ă���Έ�x�N���[�Y����B
		Close(&dwError);
	}

	// COM�|�[�g���I�[�v��
	hCom = CreateFile(strComPort,	GENERIC_READ|GENERIC_WRITE, 0, NULL, 
						OPEN_EXISTING, 0, NULL ) ;
	if ( hCom == INVALID_HANDLE_VALUE ) {
		// �I�[�v���Ɏ��s�����ꍇ
		*pdwError = GetLastError();
		return H1USB_ERR_COMOPEN;
	}
	m_hCom = hCom;

	//DCB�\���̃Z�b�g
	DCB dcb;
	memset(&dcb, 0, sizeof(DCB));
	dcb.DCBlength	= sizeof(DCB);
	dcb.fBinary		= 1;
	dcb.fParity		= 1;
	dcb.BaudRate	= CBR_57600;
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

/********************************************************************************************/
/*[���\�b�h��] COM�N���[�Y																	*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		Close(LPDWORD pdwError)														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		COM�|�[�g���N���[�Y����														*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::Close(LPDWORD pdwError)
{
	int		iReturn;
	iReturn		= H1USB_OK;
	*pdwError	= 0;

	if(m_hCom){
		CloseHandle(m_hCom);
		m_hCom = 0;
	}
	m_hCom = 0;
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �L���o���N�Z�b�g																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		SetEnableBank(WORD* pwEnableBank, WORD wEnableBankCount)					*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�o���N�F�؎��̃o���N�i���o�[��o�^����										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::SetEnableBank(WORD* pwEnableBank, WORD wEnableBankCount)
{
	int iReturn;
	WORD w;

	iReturn		= H1USB_OK;

	if(m_pwEnableBank){
		//�ȑO���ɔz�񂪓o�^����Ă���̂ŁA��x�폜
		delete[] m_pwEnableBank;
	}
	//�w�肳�ꂽ�z�񕪗L���o���N�̔z����m��
	m_pwEnableBank = new WORD[wEnableBankCount];

	//�L���o���N�̔z��ƌ����R�s�[
	for(w=0; w<wEnableBankCount; w++){
		m_pwEnableBank[w] = pwEnableBank[w];
	}
	m_wEnableBankCount = wEnableBankCount;
	return iReturn;
}


/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g���Z�b�g														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		SetTempU(PTEMPU pTempU, UINT uiCounts, BOOL bInit)							*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g�Ǘ����Ɏw�肳�ꂽ�e���v���[�g�����Z�b�g����B				*/
/*				bInit:TRUE�ł���΁A�e���v���[�g�Ǘ����̏��������s���Ă���Z�b�g����		*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::SetTempU(PTEMPU pTempU, UINT uiCounts, BOOL bInit)
{
	int iReturn;
	UINT ui;
	UINT uiMaxCount;

	iReturn		= H1USB_OK;
	if(bInit){
		//�������t���O���Z�b�g����Ă���
		m_uiTempUs = 0;								//�Z�b�g����Ă���e���v���[�g���̌����N���A
		ZeroMemory(m_TempU, sizeof(m_TempU));		//�e���v���[�g�Ǘ�����������
	}

	//�w�肳�ꂽ�e���v���[�g�����Z�b�g����
	uiMaxCount = MAX_NINSHO_TEMPS - m_uiTempUs;
	if((pTempU)&&(uiCounts)){
		if(uiCounts <= uiMaxCount){
			for(ui=0; ui<uiCounts; ui++){
				m_TempU[m_uiTempUs] = pTempU[ui];
				m_uiTempUs++;
			}
		}
		else{
			//�ő吔�ɒB���Ă���
			iReturn = H1USB_ERR_BIGGER;
		}
	}
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g���擾															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		GetTempU(PTEMPU pTempU)														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�ŐV�̃e���v���[�g���擾����												*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::GetTempU(PTEMPU pTempU)
{
	int iReturn;
	iReturn = H1USB_OK;

	if(m_uiTempUs > 0){
		*pTempU = m_TempU[m_uiTempUs-1];
	}
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �g�����U�N�V����																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		TransToDevice(UINT uiMsg, WPARAM wParam, LPARAM lParam)						*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�o�C�X�Ƀg�����U�N�V�����������J�n���A�I����҂�							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CH1USBCom::TransToDevice(UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int iReturn;
	DWORD dwWaitObject;

	//�I���C�x���g�����Z�b�g
	ResetEvent(m_hTEnd);

	iReturn = H1USB_OK;

	m_bTransEnable = TRUE;

	//�X���b�h�̃��b�Z�[�W���[�v�փg�����U�N�V�������b�Z�[�W���|�X�g
	::PostThreadMessage(m_uEPThreadID, uiMsg, wParam, lParam);

	//�X���b�h�̃g�����U�N�V���������̏I����҂�
	dwWaitObject = WaitForSingleObject(m_hTEnd, 15000);
	if(dwWaitObject == WAIT_OBJECT_0){
		//�g�����U�N�V��������I��
		iReturn = m_iLastError;
	}
	else{
		//�^�C���A�E�g
		iReturn = H1USB_ERR_TRANSTOUT;
	}

	m_bTransEnable = FALSE;
	return iReturn;
}

/********************************************************************************************/
/* ��{�N���X����̃R�[���o�b�N�X�G���g���[													*/
/********************************************************************************************/
/********************************************************************************************/
/*[���\�b�h��] �X���b�h���b�Z�[�W�������[�v�O����											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		LRESULT OnThreadBefor(LPVOID pObj)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		��������																	*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���F0																	*/
/********************************************************************************************/
LRESULT CALLBACK CH1USBCom::OnThreadBefor(LPVOID pObj)
{
A2GDBG_OUTSTRING(_T("OnThreadBefor() Entry\n"));


A2GDBG_OUTSTRING(_T("OnThreadBefor() Exit\n"));
	return 0;
}


/********************************************************************************************/
/*[���\�b�h��] �X���b�h���[�v���������														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		LRESULT OnThread(LPVOID pObj)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�����T�[�r�X�w���̃R�[���o�b�N�X�^�u										*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���F0																	*/
/********************************************************************************************/
LRESULT CALLBACK CH1USBCom::OnThread(LPVOID pObj)
{
A2GDBG_OUTSTRING(_T("OnThread() Entry\n"));
	CH1USBCom* pH1USBCom  = (CH1USBCom*)pObj;

	int		iReturn;
	DWORD	dwStat;

	//�f�o�C�X���^�b�`�Z���T�[�̏�Ԓʒm�i�񓯊��j�����������`�F�b�N����
	iReturn = pH1USBCom->CheckASyncData(&dwStat);
	if(iReturn == H1USB_NOTIFY){
		//�ʒm���������̂Ŕ񓯊���M�̒ʒm���_�C�A���O�ɃR�[���o�b�N����
		pH1USBCom->m_H1EComObjEP.H1ECallBacks.OnTuchNotification(dwStat, pH1USBCom->m_H1EComObjEP.pObj);
	}

A2GDBG_OUTSTRING(_T("OnThread() Exit\n"));
	return 0;
}



/********************************************************************************************/
/*[���\�b�h��] �X���b�h���b�Z�[�W�������[�v�㏈��											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		LRESULT OnThreadAfter(LPVOID pObj)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		��������																	*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���F0																	*/
/********************************************************************************************/
LRESULT CALLBACK CH1USBCom::OnThreadAfter(LPVOID pObj)
{
A2GDBG_OUTSTRING(_T("OnThreadAfter() Entry\n"));



A2GDBG_OUTSTRING(_T("OnThreadAfter() Exit\n"));

	return 0;
}

/********************************************************************************************/
/*[���\�b�h��] �񓯊��f�[�^�`�F�b�N����														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int CheckASyncData(LPDWORD pdwState)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�񓯊��̎�M�f�[�^�ɂ��A�^�b�`�Z���T�[�̏�Ԃ��`�F�b�N����				*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���F0																	*/
/********************************************************************************************/
int CH1USBCom::CheckASyncData(LPDWORD pdwState)
{
	int			iReturn;
	DWORD		dwRecvSize;
	BYTE		recv[2048] ;								// ��M�p
	BYTEWORD	bw;	

	iReturn = H1USB_OK;

	//�񓯊��ł̃f�[�^��M�`�F�b�N
	dwRecvSize = getBuffSize() ;								// ��M�f�[�^�o�b�t�@�̃o�C�g���擾

	if(dwRecvSize > 0){
		memset( recv, 0, sizeof(recv) ) ;
		Sleep(10);
		dwRecvSize = getBuffSize() ;							// ��M�f�[�^�o�b�t�@�̃o�C�g���擾
		// ��M����
		if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	
			// ReadFile���s
			iReturn = H1USB_ERR_READFILE;
		}
		else {
			iReturn = H1USB_NOTIFY;
			bw.byte[0] = recv[2];
			bw.byte[1] = recv[1];
			if ((recv[0] == 0x00) && (bw.word == 2) && (recv[3] == 0x74) ){
				//�^�b�`�Z���T�[�̏�Ԃɕω���������
				if( recv[4] == 0x01){
					//�^�b�`�Z���T�[ON
					*pdwState = H1USB_STATUS_TUCH_ON;
				}
				else{
					//�^�b�`�Z���T�[OFF
					*pdwState = H1USB_STATUS_TUCH_OFF;
				}
			}
			else{
				//�T�[�r�X�O�̃f�[�^����M����
				*pdwState = H1USB_STATUS_INVDATA;
			}
		}
	}
	return iReturn;
}


/********************************************************************************************/
/*[���\�b�h��] ���b�Z�[�W����																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int OnComMessage(UINT uiMsg, WPARAM wParam, LPARAM lParam, LPVOID pObj)		*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�X���b�h�����̃��b�Z�[�W�R�[���o�b�N����									*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FTHREAD_CBCODE_NORMAL												*/
/********************************************************************************************/
LRESULT CALLBACK CH1USBCom::OnComMessage(UINT uiMsg, WPARAM wParam, LPARAM lParam, LPVOID pObj)
{
A2GDBG_OUTSTRING(_T("OnComMessage() Entry\n"));
	CH1USBCom* pH1USBCom  = (CH1USBCom*)pObj;
	BYTE	byMatchMemNo;
	WORD	wMachBank;
	TEMPU	tempu;

	int		iReturn;

	iReturn = H1USB_OK;

	//�g�����U�N�V����ID�ɂ��A�������R�[������
	switch(uiMsg){
		case H1USB_COM_SCRAMBLE:	iReturn = pH1USBCom->TransScranmble();								break;
		case H1USB_COM_GREENLED_ON:	iReturn = pH1USBCom->TransLedGreen();								break;
		case H1USB_COM_REDLED_ON:	iReturn = pH1USBCom->TransLedRed();									break;
		case H1USB_COM_GRLED_ON:	iReturn = pH1USBCom->TransLedGR();									break;
		case H1USB_COM_LED_OFF:		iReturn = pH1USBCom->TransLedOff();									break;
		case H1USB_COM_TUCH_NOTIFY:	iReturn = pH1USBCom->TransTuchNotify(wParam);						break;
		case H1USB_COM_GETFVER:		iReturn = pH1USBCom->TransGetFwVersion(pH1USBCom->m_cFwVersion);	break;
		case H1USB_COM_GETSNUM:		iReturn = pH1USBCom->TransGetSerialNum(pH1USBCom->m_cSerialNum);	break;
		case H1USB_COM_BUZSTART:	iReturn = pH1USBCom->TransBUZStart();								break;
		case H1USB_COM_BUZSTOP:		iReturn = pH1USBCom->TransBUZStop();								break;
		case H1USB_COM_SECLOW:		iReturn = pH1USBCom->TransSetSecurity(0x01);						break;
		case H1USB_COM_SECMIDLOW:	iReturn = pH1USBCom->TransSetSecurity(0x02);						break;
		case H1USB_COM_SECMIDLE:	iReturn = pH1USBCom->TransSetSecurity(0x03);						break;
		case H1USB_COM_SECMIDHG:	iReturn = pH1USBCom->TransSetSecurity(0x04);						break;
		case H1USB_COM_SECHIHGH:	iReturn = pH1USBCom->TransSetSecurity(0x05);						break;
		case H1USB_COM_NINSHO:		iReturn = pH1USBCom->TransNinsho(wParam, lParam, &byMatchMemNo);	break;
		case H1USB_COM_NINSHO_BANK:
			if(pH1USBCom->m_pwEnableBank){
				iReturn = pH1USBCom->TransNinshoBank(pH1USBCom->m_pwEnableBank, 
											pH1USBCom->m_wEnableBankCount, &byMatchMemNo, &wMachBank);
			}
			break;
		case H1USB_COM_NINSHO_TEMP:
			iReturn = pH1USBCom->TransNinshoWithTemplate(pH1USBCom->m_TempU, pH1USBCom->m_uiTempUs, &byMatchMemNo);			break;
		case H1USB_COM_GETTEMPLATE:			
			iReturn = pH1USBCom->TransGetTemplate(&tempu);
			if(iReturn == H1USB_OK){
				iReturn = pH1USBCom->SetTempU(&tempu, 1, TRUE);
			}			
			break;
		case H1USB_COM_CHANGE_BANK: iReturn = pH1USBCom->TransChangeBank((WORD)wParam); 				break;
		case H1USB_COM_SETTEMPLATE:	iReturn = pH1USBCom->TransSetTemplate(pH1USBCom->m_TempU);			break;
		case H1USB_COM_DELTEMPLATE:	iReturn = pH1USBCom->TransDelTemplate((int)wParam);					break;
		case H1USB_COM_HWRESET:		iReturn = pH1USBCom->TransHardWare();								break;
		default: break;

	}

	//�G���[�Z�b�g
	pH1USBCom->m_iLastError = iReturn;
	//�I���C�x���g���Z�b�g����
	SetEvent(pH1USBCom->m_hTEnd);

A2GDBG_OUTSTRING(_T("OnComMessage() Exit\n"));

	return THREAD_CBCODE_NORMAL;

}

/*====================================================================*/
/* �����T�v  �F�Í�����Ԃɂ���
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransScranmble()
{

	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[1024] ;	// ���M�p
	BYTE recv[1024] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;
	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	BYTE *bp = NULL ;


	send[0] = 0x1F ;	// �Í��L�[���M�R�}���h
	send[1] = 0x02 ;	// �㑱�f�[�^�̃o�C�g��(512�o�C�g)
	send[2] = 0x00 ;	// �V
	

	// �Í��L�[�̌��ƂȂ�512�o�C�g�̗����𐶐����Asend[3]�`send[514]�ɃZ�b�g
	int i = 0 ;
	for ( bp = send + 3, i = 0; i < 512; bp++, i++ ) {

		*bp = rand() ;
	}

	dwSendSize = 3 + 512 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// �t���O�̍X�V
	m_bCamelliaEnabled = TRUE ;	// TRUE: �Í��ʐM���

	// 512�o�C�g�̗�������Í��L�[(m_byCurrentWorkKey)�𐶐�����
	decodeMasterKey512( send + 3, m_byCurrentWorkKey ) ;

	// �Í��L�[�����Ɋg�匮(m_uKttWork)�𐶐�(�Í����E�������s���O�ɕK���g�匮�𐶐����邱��)
	// ��x�g�匮�𐶐�����΁A�������ňÍ����E�������s������A�ēx�g�匮�̐������s���K�v�͂Ȃ�
	Camellia_Ekeygen( 128, m_byCurrentWorkKey, m_uKttWork ) ;

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
		//MessageBox( "��M�f�[�^�����݂��Ȃ����߁A�������I�����܂�", TITLE, MB_ICONEXCLAMATION ) ;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_READFILE; 
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��

		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;
}

/*====================================================================*/
/* �����T�v  �F��LED��_��������
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransLedGreen( void )
{	
	int iReturn;
	iReturn = H1USB_OK;

	// ��LED, ��LED������
	iReturn = ledAllOff();
	if ( iReturn != H1USB_OK ) {
		return iReturn ;
	}

	// ��LED�_��
	iReturn = ledOnOff( 0x02, 0x01 ) ;

	return iReturn ;
}

/*====================================================================*/
/* �����T�v  �F��LED��_��������
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransLedRed( void )
{
	int iReturn;
	iReturn = H1USB_OK;

	// ��LED, ��LED������
	iReturn = ledAllOff();
	if ( iReturn != H1USB_OK ) {
		return iReturn ;
	}

	// ��LED�_��
	iReturn = ledOnOff( 0x01, 0x01 ) ;

	return iReturn;
}
/*====================================================================*/
/* �����T�v  �F��LED��_��������
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransLedGR( void )
{
	int iReturn;
	iReturn = H1USB_OK;

	// ��LED, ��LED������
	iReturn = ledAllOff();
	if ( iReturn != H1USB_OK ) {
		return iReturn ;
	}

	// ��LED�_��
	iReturn = ledOnOff( 0x03, 0x01 ) ;

	return iReturn ;
}


/*====================================================================*/
/* �����T�v  �FLED��S�ď���������
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransLedOff( void )
{

	// ��LED, ��LED������
	return ledOnOff( 0x03, 0x00 ) ;

}




/*====================================================================*/
/* �����T�v  �F�^�b�`�Z���T�[�̒ʒm���󂯎��悤�ɐݒ肷��B
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransTuchNotify(BYTE byParam)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x28 ;	// �^�b�`�Z���T�[����R�}���h
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x01 ;	// �V
	send[3] = byParam ;	// �^�b�`�Z���T�[�̏�Ԃ��ω��������Ƃ����X�|���X�����ݒ�

	dwSendSize = 4 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_WRITEFILE;
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;
}

/*====================================================================*/
/* �����T�v  �F�t�@�[���E�F�A�̃o�[�W��������ԋp����
/* ����      �FpFwVersion	�t�@�[���E�F�A�̃o�[�W�������
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransGetFwVersion( char *pFwVersion )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x14 ;		// ���擾�R�}���h
	send[1] = 0x00 ;		// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x01 ;		// �V
	send[3] = 0x00 ;		// �t�@�[���E�F�A�̃o�[�W���������擾

	dwSendSize = 4 ;		// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	//��M�f�[�^�m�F����(V1.10---2011/5/27)
	//16�o�C�g�̕�����������M�f�[�^����S�̂̎�M�f�[�^���v�Z���Ă���A
	//�S�̂̎�M�f�[�^����16�o�C�g���������������̃f�[�^����M���ĕ���������B
	//begin
	DWORD dwRecvSizeRest = 0;//�㑱�̎c��o�C�g��
	DWORD dwRecvSizeAll = 0;//���X�|���X�f�[�^�S�̂̒���

	BYTE *pRecv = recv ;	// recv�ւ̃|�C���^
	DWORD dwTmplTotalSize = 0 ;	// ��M�e���v���[�g�f�[�^�̍��v�T�C�Y

	// �S�f�[�^����M���邩�A�G���[�R�[�h�����^�[������܂őҋ@����
	for ( ; ; ) {

		if(dwTmplTotalSize == 0){

			while (dwRecvSize < 16) {
				// ��M�o�b�t�@�̃o�C�g�����擾
				dwRecvSize = getBuffSize() ;
			}
			// ��M����
			if ( recvCommEnc( pRecv, 16 ) == FALSE ) {	// ReadFile���s
				return H1USB_ERR_READFILE;
			}
			else {	// ReadFile����
				pRecv += 16 ;	// ��M�����������|�C���^��i�߂�
				dwTmplTotalSize += 16 ;	// ��M�f�[�^���v�T�C�Y(dwTmplTotalSize)�ɉ��Z
				dwRecvSizeAll = ((recv[1]*256+recv[2]+18) & 0xFFFFFFF0);// ��M�f�[�^�S�̃T�C�Y
				dwRecvSizeRest = dwRecvSizeAll-16;
				if ( recv[0] != 0x00 ) { // �ُ�I��
					return H1USB_ERR_RESPONS;
				}
			}
		}
		

		// ��M�o�b�t�@�̃o�C�g�����擾
		dwRecvSize = getBuffSize() ;

		// ��M�f�[�^������ꍇ
		if ( dwRecvSize > 0 ) {
			// ��M����
			if ( recvCommEnc( pRecv, 16 ) == FALSE ) {	// ReadFile���s
				return H1USB_ERR_READFILE;
			}

			// ��M�f�[�^���v�T�C�Y(dwTmplTotalSize)�ɉ��Z
			dwTmplTotalSize += 16 ;
			pRecv += 16 ;	// ��M�����������|�C���^��i�߂�

			// �S�f�[�^����M�����烋�[�v�𔲂���
			if ( dwTmplTotalSize == dwRecvSizeAll ) {

				break ;
			}
			// �܂��S�e���v���[�g�f�[�^����M������Ă��Ȃ�
			else {

				// �������s
			}
		}
	}
	//��M�f�[�^�m�F����(V1.10---2011/5/27)
	//end

	// �t�@�[���E�F�A�̃o�[�W���������R�s�[
	memcpy( pFwVersion, recv + 3, 14 ) ;
	pFwVersion[14] = '\0' ;
	return iReturn;

}

/*====================================================================*/
/* �����T�v  �F�V���A���ԍ���ԋp����
/* ����      �FpSerialNum	�V���A���ԍ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransGetSerialNum( char *pSerialNum )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x14 ;		// ���擾�R�}���h
	send[1] = 0x00 ;		// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x01 ;		// �V
	send[3] = 0x01 ;		// �V���A���ԍ����擾

	dwSendSize = 4 ;		// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		//MessageBox( "��M�f�[�^�����݂��Ȃ����߁A�������I�����܂�", TITLE, MB_ICONEXCLAMATION ) ;
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFile����

		if ( recv[0] != 0x00 ) { // �ُ�I��
			return H1USB_ERR_RESPONS;
		}
		else { // ����I��
			// �V���A���ԍ������R�s�[
			memcpy( pSerialNum, recv + 3, 12 ) ;
			pSerialNum[12] = '\0' ;
		}
	}
	return iReturn;

}

/*====================================================================*/
/* �����T�v  �F��LED��_��������
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransBUZStart( void )
{	
	int iReturn;
	iReturn = H1USB_OK;

	iReturn = beepSound(0XFF, 0);

	return iReturn;
}

/*====================================================================*/
/* �����T�v  �F��LED��_��������
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransBUZStop( void )
{
	int iReturn;
	iReturn = H1USB_OK;

	iReturn = beepSound(0, 0);

	return iReturn;
}

/*====================================================================*/
/* �����T�v  �F�^�b�`�Z���T�[�̒ʒm���󂯎��悤�ɐݒ肷��B
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransSetSecurity(BYTE byMode)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x20 ;	// �^�b�`�Z���T�[����R�}���h
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x01 ;	// �V
	send[3] = byMode ;	// �^�b�`�Z���T�[�̏�Ԃ��ω��������Ƃ����X�|���X�����ݒ�

	dwSendSize = 4 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;
}

/*====================================================================*/
/* �����T�v  �F1:1�F�؂܂���1:N�F�؂��s��
/* ����      �FbyNinshoMode	0x00=>1:N�F��, 0x01=>1:1�F��
/*             nMemNo		�ƍ�����e���v���[�g�������ԍ�(1:N�F�؂̏ꍇ�͔C��)
/*             pMatchMemNo	��v�����e���v���[�g�������ԍ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransNinsho( const BYTE byNinshoMode, const int nMemNo, BYTE *pMatchMemNo )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[2048] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x13 ;		// �F�؎w���R�}���h
	send[1] = 0x00 ;		// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x02 ;		// �V
	send[3] = byNinshoMode ;// �F�؃��[�h����уI�v�V����
	send[4] = nMemNo ;		// �e���v���[�g�ԍ�(1:N�F�؂̏ꍇ�͔C��)

	dwSendSize = 5 ;		// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		iReturn = H1USB_ERR_WRITEFILE;
		goto ERR_TREAT ;

	}

	// �f�[�^����M����܂őҋ@����
	for ( ; ; ) {
		dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
		// 16�o�C�g�̎�M�f�[�^������΃��[�v�𔲂���
		if ( 16 == dwRecvSize ) {
			break ;
		}
		else{
			if(!m_bTransEnable){
				iReturn = H1USB_ERR_TRANSTOUT;
				goto ERR_TREAT ;
			}
		}
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		iReturn = H1USB_ERR_READFILE;
		goto ERR_TREAT ;

	}
	else {	// ReadFile����
		if ( recv[0] != 0x00 ) { // �F�؎��s
			// �B�e���_�C�A���O�̃L�����Z���{�^�����������ꂽ�ꍇ
			if ( recv[3] == 0x10 ) {
				// ��LED�_��
				TransLedGreen() ;				
				// Beep��(�L�����Z��)��炷
				beepCancel() ;
				// ���b�Z�[�W�\��
				return H1USB_CANCEL;
			}
			// ��L�ȊO�̓G���[
			else {
				iReturn = H1USB_ERR_RESPONS;
				goto ERR_TREAT ;
			}
		}
		else { // �F�ؐ���
			// ��v�����e���v���[�g�������ԍ����R�s�[
			*pMatchMemNo = recv[3] ;
		}
	}
	// ��LED�_��
	TransLedGreen() ;	
	// Beep��(����)��炷
	beepOK() ;
	return iReturn;

ERR_TREAT:
	// ��LED�_��
	TransLedRed() ;
	// Beep��(�ُ�)��炷
	beepNG() ;
	return iReturn;

}

/*====================================================================*/
/* �����T�v  �F�o���N�ԔF�؂��s��
/* ����      �FwEnableBank		�F�؂̑ΏۂƂ���o���N�ԍ��̔z��
/*             wEnableBankCnt	�F�؂̑ΏۂƂ���o���N�̐�
/*             pMatchMemNo		��v�����e���v���[�g�������ԍ�
/*             pMatchBank		��v�����e���v���[�g�����݂���o���N�ԍ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransNinshoBank( const WORD wEnableBank[], const WORD wEnableBankCnt, BYTE *pMatchMemNo, WORD *pMatchBank )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[8192] ;	// ���M�p
	BYTE recv[2048] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x18 ;	// �F�؎w���R�}���h(�o���N�ԔF��)
	send[1] = (BYTE)( ((1 + 2 * wEnableBankCnt) & 0xFF00) >> 8 ) ;			// �㑱�f�[�^�̃o�C�g��(�F�؂ŗp����o���N�̐��ɂ���ĕς��)
	send[2] = (BYTE)( (1 + 2 * wEnableBankCnt) & 0x00FF ) ;	// �V
	send[3] = 0x00 ;	// �I�v�V�����r�b�g
	
	// �F�؂ŗp����o���N�ԍ����Asend[4]�ȍ~�Ɋi�[����(2�o�C�g��1�o���N�ԍ�)
	//for ( WORD i = 0; i < wEnableBankCnt; i++ ) {
	//	send[2*(i+2)] = (BYTE)( (wEnableBank[i] & 0xFF00) >> 8 ) ;	
	//	send[2*(i+2)+1] = (BYTE)(wEnableBank[i] & 0x00FF ) ;
	//}
	WORD w;
	BYTEWORD bw;
	for(w=0; w<wEnableBankCnt; w++)
	{
		bw.word = wEnableBank[w];
		send[4+w*2]		=  	bw.byte[1];
		send[4+w*2+1]	=	bw.byte[0];
	}



	dwSendSize = 4 + 2 * wEnableBankCnt ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��(�F�؂ŗp����o���N�̐��ɂ���ĕς��)
	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		iReturn = iReturn;
		goto ERR_TREAT ;

	}
	// �f�[�^����M����܂őҋ@����
	for ( ; ; ) {
		dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
		// 16�o�C�g�̎�M�f�[�^������΃��[�v�𔲂���
		if ( 16 <= dwRecvSize ) {
			break ;
		}
		else{
			if(!m_bTransEnable){
				iReturn = H1USB_ERR_TRANSTOUT;
				goto ERR_TREAT ;
			}
		}

	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		iReturn = H1USB_ERR_READFILE;
		goto ERR_TREAT ;
	}

	else {	// ReadFile����
		if ( recv[0] != 0x00 ) { // �F�؎��s
			// �B�e���_�C�A���O�̃L�����Z���{�^�����������ꂽ�ꍇ
			if ( recv[3] == 0x10 ) {
				// ��LED�_��
				TransLedGreen() ;				
				// Beep��(�L�����Z��)��炷
				beepCancel() ;
				// ���b�Z�[�W�\��
				return H1USB_CANCEL;

			}
			// ��L�ȊO�̓G���[
			else {
				iReturn = H1USB_ERR_RESPONS;
				goto ERR_TREAT ;
			}
		}
		else { // �F�ؐ���
			// ��v�����e���v���[�g�������ԍ����R�s�[
			*pMatchMemNo = recv[3] ;
			// �o���N�ԍ����R�s�[
			*pMatchBank = ( recv[4] << 8 ) | recv[5] ;
		}
	}

	// ��LED�_��
	TransLedGreen() ;
	
	// Beep��(����)��炷
	beepOK() ;

	return iReturn ;

ERR_TREAT:

	// ��LED�_��
	TransLedRed() ;
	// Beep��(�ُ�)��炷
	beepNG() ;
	// �G���[���b�Z�[�W�\��
	//	MessageBox( cMsg, TITLE, MB_ICONSTOP ) ;
	return iReturn ;

}

/*====================================================================*/
/* �����T�v  �F�e���v���[�g�t���F�؂��s��
/* ����      �FpTempU			�e���v���[�g���j�b�g�\���̂̔z��
/*             byCount			�e���v���[�g���j�b�g�\���̂̔z��̐�
/*             pMatchNum		��v�����e���v���[�g�������ԍ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransNinshoWithTemplate(const PTEMPU pTempU, const BYTE byCount, BYTE *pMatchNum)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE* pSend;
	BYTE recv[2048] ;	// ��M�p
	UINT uiSendBuffSize;
	UINT uiSendLength;
	UINT uiTemplateSize;
	BYTEWORD bw;
	int i;
	uiTemplateSize = 536*byCount;
	uiSendLength = uiTemplateSize + 8;
	uiSendBuffSize = uiSendLength + 8;
	bw.word = uiTemplateSize + 5;
	pSend = new BYTE[uiSendBuffSize];
	ZeroMemory(pSend, uiSendBuffSize);
	memset( recv, 0, sizeof(recv) ) ;
	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	// ���M�f�[�^���Z�b�g
	pSend[0] = 0x19 ;		// �F�؎w���R�}���h(�e���v���[�g�t��)
	pSend[1] = bw.byte[1] ;	// �㑱�f�[�^�̃o�C�g��
	pSend[2] = bw.byte[0] ;	// �V
	pSend[3] = 0x08 ;			// ���M��̃e���v���[�g�������ԍ�(0x00�`0x63)
	// �e���v���[�g�f�[�^��send[4]�`send[539]�ɃR�s�[
	for(i=0; i<byCount; i++){
		memcpy(&pSend[4+i*536], pTempU[i].byTemplate, 536);
	}

	// �`�F�b�N�T���̌v�Z
	DWORD dwCheckSum = 0 ;	// �`�F�b�N�T��
	DWORD *pd = ( DWORD * )pSend ;

	for ( UINT ui = 0; ui < (uiTemplateSize+4)/4; ui++, pd++ ) {
		dwCheckSum ^= *pd ;
	}

	// �`�F�b�N�T����send[540]�`send[543]�ɃR�s�[
	*pd = dwCheckSum ;

	dwSendSize = uiSendLength ;		// ���M�f�[�^�T�C�Y�̃o�C�g��

	// ���M����
	if ( sendCommEnc( pSend, dwSendSize ) == FALSE ) {
		iReturn = H1USB_ERR_WRITEFILE;
		goto ERR_TREAT ;
	}

	for ( ; ; ) {
		dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
		// 16�o�C�g�̎�M�f�[�^������΃��[�v�𔲂���
		if ( 16 == dwRecvSize ) {
			break ;
		}
		else{
			if(!m_bTransEnable){
				iReturn = H1USB_ERR_TRANSTOUT;
				goto ERR_TREAT ;
			}
		}
	}


	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		iReturn = H1USB_ERR_READFILE;
		goto ERR_TREAT ;

	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) {
			iReturn = H1USB_ERR_RESPONS;
			goto ERR_TREAT ;
		}
	}
	// ��LED�_��
	TransLedGreen() ;
	
	// Beep��(����)��炷
	beepOK() ;

	if(pSend){
		delete[] pSend;
	}
	return iReturn;

ERR_TREAT:

	// ��LED�_��
	TransLedRed() ;
	// Beep��(�ُ�)��炷
	beepNG() ;

	if(pSend){
		delete[] pSend;
	}

	return iReturn;
}


/*====================================================================*/
/* �����T�v  �F�e���v���[�g���B�e���AH1E-USB�����M����
/* ����      �FpTemplate	�e���v���[�g
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransGetTemplate(PTEMPU pTempU)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[640] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;
	BYTE *pRecv = recv ;	// recv�ւ̃|�C���^
	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwTmplTotalSize = 0 ;	// ��M�e���v���[�g�f�[�^�̍��v�T�C�Y

	// ���M�f�[�^���Z�b�g
	send[0] = 0x15 ;	// �e���v���[�g��M�R�}���h(H1E-USB => �z�X�g�@)
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x00 ;	// �V

	dwSendSize = 3 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		iReturn = H1USB_ERR_WRITEFILE;
		goto ERR_TREAT ;
	}
	//��M�f�[�^�m�F����(V1.10---2011/5/27)
	//16�o�C�g�̕�����������M�f�[�^����S�̂̎�M�f�[�^���v�Z���Ă���A
	//�S�̂̎�M�f�[�^����16�o�C�g���������������̃f�[�^����M���ĕ���������B
	//begin
	DWORD dwRecvSizeRest = 0;//�㑱�̎c��o�C�g��
	DWORD dwRecvSizeAll = 0;//���X�|���X�f�[�^�S�̂̒���
	// �S�e���v���[�g�f�[�^(544�o�C�g)����M���邩�A�G���[�R�[�h�����^�[������܂őҋ@����
	for ( ; ; ) {
		if(dwTmplTotalSize == 0){
			while (dwRecvSize < 16) {
				// �B�e���_�C�A���O�̃L�����Z���{�^���������ꂽ�ꍇ
				//if ( m_pDlg->IsCancel() ) {
				//	cancel() ;
				//}
				// ��M�o�b�t�@�̃o�C�g�����擾
				dwRecvSize = getBuffSize() ;
				// ���b�Z�[�W�|���v
				//messagePump() ;
			}
			// ��M����
			if ( recvCommEnc( pRecv, 16 ) == FALSE ) {	// ReadFile���s
				iReturn = H1USB_ERR_READFILE;
				goto ERR_TREAT ;
			}
			else {	// ReadFile����				
				pRecv += 16 ;	// ��M�����������|�C���^��i�߂�
				dwTmplTotalSize += 16 ;	// ��M�f�[�^���v�T�C�Y(dwTmplTotalSize)�ɉ��Z
				dwRecvSizeAll = ((recv[1]*256+recv[2]+18) & 0xFFFFFFF0);// ��M�f�[�^�S�̃T�C�Y
				dwRecvSizeRest = dwRecvSizeAll-16;
				if ( recv[0] != 0 ) {	// �ُ�I��
					// �B�e���_�C�A���O�̃L�����Z���{�^�����������ꂽ�ꍇ
					if ( recv[3] == 0x10 ) {
						// ��LED�_��
						TransLedGreen() ;						
						// Beep��(�L�����Z��)��炷
						beepCancel() ;
						// ���b�Z�[�W�\��
						return H1USB_CANCEL;

					}
					// ��L�ȊO�̓G���[
					else {
						// �_�C�A���O�����
					iReturn = H1USB_ERR_RESPONS;
						goto ERR_TREAT ;
					}
				}
			}
		}
		

		// ��M�o�b�t�@�̃o�C�g�����擾
		dwRecvSize = getBuffSize() ;

		// ��M�f�[�^������ꍇ
		if ( dwRecvSize > 0 ) {
			// ��M����
			if ( recvCommEnc( pRecv, 16 ) == FALSE ) {	// ReadFile���s
				iReturn = H1USB_ERR_READFILE;
				goto ERR_TREAT ;
			}
			// ��M�f�[�^���v�T�C�Y(dwTmplTotalSize)�ɉ��Z
			dwTmplTotalSize += 16 ;
			pRecv += 16 ;	// ��M�����������|�C���^��i�߂�
			// �S�e���v���[�g�f�[�^(544�o�C�g)����M�����烋�[�v�𔲂���
			if ( dwTmplTotalSize == dwRecvSizeAll ) {
				break ;
			}
			// �܂��S�e���v���[�g�f�[�^����M������Ă��Ȃ�
			else {
				// �������s
			}
		}
	}
	//��M�f�[�^�m�F����(V1.10---2011/5/27)
	//end

	// H1E-USB�����M�����f�[�^�񂩂�`�F�b�N�T�����v�Z����
	DWORD dwCheckSum = 0 ;
	DWORD *pd = ( DWORD * )recv ;
	for ( int i = 0; i < 540/4; i++, pd++ ) {
		dwCheckSum ^= *pd ;
	}
	// H1E-USB�����M�����`�F�b�N�T���Ɠ����ł��邩�m�F
	if ( *pd != dwCheckSum ) {
		goto ERR_TREAT ;
	}
	// ��M�����e���v���[�g�� pTemplate �ɃR�s�[
	SYSTEMTIME st;
	GetLocalTime(&st);
	pTempU->uiTmpNum = 0xFF;
	pTempU->time = st;
	memcpy( pTempU->byTemplate, recv+4, 536 ) ;

	// ��LED�_��
	TransLedGreen() ;	
	// Beep��(����)��炷
	beepOK() ;
	return iReturn;

ERR_TREAT:

	// �`�F�b�N�T���G���[���N�����ꍇ�A�f�[�^�S�̂���M���؂�Ă��Ȃ��\��������
	// ����M�o�b�t�@�Ƀf�[�^���c���Ă���ƌ㑱�����Ɏx������������߁A����M�o�b�t�@�����������Ă���
	dwRecvSize = getBuffSize() ;	// ��M�o�b�t�@�̃o�C�g�����擾
	if ( dwRecvSize != 0 ) {
		// ����M�o�b�t�@�̏�����
		PurgeComm( m_hCom, PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR ) ;
	}
	// ��LED�_��
	TransLedRed() ;
	// Beep��(�ُ�)��炷
	beepNG() ;
	// �G���[���b�Z�[�W�\��
	//MessageBox( cMsg, TITLE, MB_ICONSTOP ) ;	
	return iReturn ;

}


/*====================================================================*/
/* �����T�v  �F�e���v���[�g�i�[�o���N��؂�ւ���
/* ����      �FwBankNo	�ؑ֐�̃o���N�ԍ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransChangeBank( const WORD wBankNo )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;
	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	// ���M�f�[�^���Z�b�g
	send[0] = 0x1C ;	// �e���v���[�g�i�[�o���N�w��R�}���h
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x02 ;	// �V
	send[3] = (BYTE)(( wBankNo & 0xFF00 ) >> 8) ;	// �o���N�ԍ�(��ʃo�C�g)
	send[4] = (BYTE)( wBankNo & 0x00FF ) ;		// �o���N�ԍ�(���ʃo�C�g)
	dwSendSize = 5 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}
	
	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;

}


/*====================================================================*/
/* �����T�v  �F�e���v���[�g���w�肵���������ɑ��M����
/* ����      �FpTemplate	�e���v���[�g
/*             nMemNo		���M��̃e���v���[�g�������ԍ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransSetTemplate(PTEMPU pTempU)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[640] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;
	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	// ���M�f�[�^���Z�b�g
	send[0] = 0x12 ;			// �e���v���[�g���M�R�}���h(�z�X�g�@ => H1E-USB)
	send[1] = 0x02 ;			// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x1D ;			// �V
	send[3] = pTempU->uiTmpNum;	// ���M��̃e���v���[�g�������ԍ�(0x00�`0x63)
	// �e���v���[�g�f�[�^��send[4]�`send[539]�ɃR�s�[
	memcpy( send + 4, pTempU->byTemplate, 536 ) ;	
	// �`�F�b�N�T���̌v�Z
	DWORD dwCheckSum = 0 ;	// �`�F�b�N�T��
	DWORD *pd = ( DWORD * )send ;
	for ( int i = 0; i < 540/4; i++, pd++ ) {
		dwCheckSum ^= *pd ;
	}
	// �`�F�b�N�T����send[540]�`send[543]�ɃR�s�[
	*pd = dwCheckSum ;
	dwSendSize = 544 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}
	return iReturn;

}


/*====================================================================*/
/* �����T�v  �FH1E-USB����e���v���[�g���폜����
/* ����      �FnMemNo	�e���v���[�g�ԍ�
                          0�`99 : �w�肵���e���v���[�g�ԍ�����폜
						  -1    : ���݂̃o���N�̑S�e���v���[�g���폜
                          -2    : �S�o���N�̑S�e���v���[�g���폜
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransDelTemplate(const int nMemNo)
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x16 ;	// �e���v���[�g�폜�R�}���h
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x01 ;	// �V

	// ���݂̃o���N�̎w�肵���e���v���[�g�ԍ�����폜
	if ( 0x00 <= nMemNo && nMemNo <= 0x63 ) {
		send[3] = nMemNo ;	// �e���v���[�g�ԍ�
	}
	else {

		// ���݂̃o���N�̑S�e���v���[�g���폜
		if ( nMemNo == -1 ) {
			send[3] = 0xFF ;	// �e���v���[�g�ԍ�
		}
		// �S�o���N�̑S�e���v���[�g���폜
		else if ( nMemNo == -2 ) {
			send[3] = 0xFE ;	// �e���v���[�g�ԍ�
		}
		// ��L�ȊO
		else {

		}
	}

	dwSendSize = 4 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		iReturn = H1USB_ERR_READFILE;
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) { 
			return H1USB_ERR_RESPONS;
		}
	}

	return iReturn;

}

/*====================================================================*/
/* �����T�v  �F�n�[�h�E�F�A�����Z�b�g����
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::TransHardWare( void )
{
	int iReturn;
	iReturn = H1USB_OK;



	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;


	DWORD dwRecvSize;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	// ���M�f�[�^���Z�b�g
	send[0] = 0x17 ;	// �n�[�h�E�F�A���Z�b�g�R�}���h
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x00 ;	// �V
	dwSendSize = 3 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	Sleep(200);
	m_bCamelliaEnabled = FALSE;
	do{
		if(!m_bTransEnable){
			iReturn = H1USB_ERR_TRANSTOUT;
			break;
		}

	// ��M�o�b�t�@�̃o�C�g�����擾
		dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
		recvCommEnc( recv, dwRecvSize );

	}while(dwRecvSize > 0);



	return iReturn;

}


/*====================================================================*/
/* �����T�v  �FLED��S�ď���������
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::ledAllOff( void )
{

	// ��LED, ��LED������
	return ledOnOff( 0x03, 0x00 ) ;

}

/*====================================================================*/
/* �����T�v  �FLED(��)�̓_�ł��J�n����
/* ����      �FbyOffTime	�_�Ŏ��̏�������(10ms�P�ʂŎw��)
/*             byOnTime		�_�Ŏ��̓_������(10ms�P�ʂŎw��)
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::ledStartBlink( const BYTE byOffTime, const BYTE byOnTime )
{
	int iReturn;

	iReturn = H1USB_OK;

	// ��LED, ��LED������
	iReturn = ledAllOff();
	if(iReturn != H1USB_OK){
		return iReturn;
	}
	// ��LED�_��
	iReturn = ledBlink( byOffTime, byOnTime ) ;

	return iReturn;

}

/*====================================================================*/
/* �����T�v  �FLED��_���A�܂��͏���������
/* ����      �FbyColor	0x01:��, 0x02:��, 0x01:�ԂƗ�
/*             byOnOff	0x00:����, 0x01:�_��
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::ledOnOff( const BYTE byColor, const BYTE byOnOff )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x11 ;	// LED����R�}���h
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x02 ;	// �V
	send[3] = byColor ; // 0x01:��, 0x02:��, 0x01:�ԂƗ�
	send[4] = byOnOff ;	// 0x00:����, 0x01:�_��

	dwSendSize = 5 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}

	return iReturn;
}

/*====================================================================*/
/* �����T�v  �FLED(��)�̓_�ł�����
/* ����      �FbyOffTime	�_�Ŏ��̏�������(10ms�P�ʂŎw��)
/*             byOnTime		�_�Ŏ��̓_������(10ms�P�ʂŎw��)
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::ledBlink( const BYTE byOffTime, const BYTE byOnTime )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x11 ;	// LED����R�}���h
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x04 ;	// �V
	send[3] = 0x02 ;	// 0x02:��
	send[4] = 0x02 ;	// 0x02:�_��
	send[5] = byOffTime ;	// �_�Ŏ��̏�������(10ms�P�ʂŎw��)
	send[6] = byOnTime ;	// �_�Ŏ��̓_������(10ms�P�ʂŎw��)

	dwSendSize = 7 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}

	return iReturn;
}



/*====================================================================*/
/* �����T�v  �F����I������Beep����炷
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::beepOK( void )
{

	// �s�E�s �Ɩ炷
	return beepSound( 0x00, 0x02 ) ;

}

/*====================================================================*/
/* �����T�v  �F�ُ�I������Beep����炷
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::beepNG( void )
{

	// �s�E�s�[ �Ɩ炷
	return beepSound( 0x40, 0x02 ) ;

}

/*====================================================================*/
/* �����T�v  �F�L�����Z������Beep����炷
/* ����      �F�Ȃ�
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::beepCancel( void )
{

	// �s�[ �Ɩ炷
	return beepSound( 0x80, 0x01 ) ;

}

/*====================================================================*/
/* �����T�v  �F�w�肳�ꂽ�p�^�[����Beep����炷
/* ����      �FbyPattern	���p�^�[��(�r�b�g�̒l => 0:�Z����, 1:������)
/*             byCount		���p�^�[����(�擪���牽�r�b�g���炷����1�`8�Ŏw��)
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
int CH1USBCom::beepSound( const BYTE byPattern, const BYTE byCount )
{
	int iReturn;
	iReturn = H1USB_OK;

	BYTE send[128] ;	// ���M�p
	BYTE recv[128] ;	// ��M�p
	memset( send, 0, sizeof(send) ) ;
	memset( recv, 0, sizeof(recv) ) ;

	DWORD dwSendSize = 0 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��
	DWORD dwRecvSize = 0 ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M�f�[�^���Z�b�g
	send[0] = 0x10 ;	// �u�U�[����R�}���h
	send[1] = 0x00 ;	// �㑱�f�[�^�̃o�C�g��
	send[2] = 0x03 ;	// �V
	if((byPattern == 0) && (byCount == 0)){
		send[3] = 0x00 ;	// 0x00:����~
	}
	else{
		if(byCount == 0){
			send[3] = 0x01 ;	// 0x01:���J�n
		}
		else{
			send[3] = 0x02 ;	// 0x02:���p�^�[���ɏ]���Ė��J�n
		}
	}
	send[4] = byPattern ;	// ���p�^�[��(�r�b�g�̒l => 0:�Z����, 1:������)
	send[5] = byCount ;		// ���p�^�[����(�擪���牽�r�b�g���炷����1�`8�Ŏw��)

	dwSendSize = 6 ;	// ���M�f�[�^�o�b�t�@�̃o�C�g��

	// ���M����
	if ( sendCommEnc( send, dwSendSize ) == FALSE ) {
		return H1USB_ERR_WRITEFILE;
	}

	// ��M�o�b�t�@�̃o�C�g�����擾
	dwRecvSize = getBuffSize() ;	// ��M�f�[�^�o�b�t�@�̃o�C�g��
	if ( dwRecvSize == 0 ) {
		return H1USB_ERR_RDATA_NON;
	}

	// ��M����
	if ( recvCommEnc( recv, dwRecvSize ) == FALSE ) {	// ReadFile���s
		return H1USB_ERR_READFILE;
	}
	else {	// ReadFile�ɂ͐����������A�R�}���h���ُ�I��
		if ( recv[0] != 0x00 ) {
			return H1USB_ERR_RESPONS;
		}
	}

	return iReturn;
}








/*====================================================================*/
/* �����T�v  �F�R�}���h��H1E-USB�ɑ��M����(�m�[�}�����)
/* ����      �FpComm	�f�[�^�o�b�t�@
/*             dwSize	���M����f�[�^�o�b�t�@�̃o�C�g��
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
BOOL CH1USBCom::sendComm( const BYTE *pComm, const DWORD dwSize )
{

	//----- ���M���� -----//
	DWORD dwLen = 0 ;

	BOOL bRet = WriteFile(
		m_hCom,	// �f�o�C�X�̃n���h��
		pComm,	// �f�[�^�o�b�t�@
		dwSize,	// �������ݑΏۂ̃o�C�g��
		&dwLen,	// �������񂾃o�C�g��
		NULL	// �I�[�o�[���b�v�\���̂̃o�b�t�@
		) ;

	// WriteFile()�����ClearCommError()���Ăяo���ƁAComStat.cbInQue��
	// ���΂���0�ɂȂ�(��M�o�b�t�@�̃o�C�g�����擾�o���Ȃ�)���߁ASleep������
	Sleep(100) ;

	return bRet ;
}

/*====================================================================*/
/* �����T�v  �F�R�}���h��H1E-USB�ɑ��M����(�Í��ʐM���)
/* ����      �FpComm	�f�[�^�o�b�t�@
/*             dwSize	���M����f�[�^�o�b�t�@�̃o�C�g��
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
BOOL CH1USBCom::sendCommEnc( const BYTE *pComm, DWORD dwSize )
{

	// �Í��ʐM��Ԃł͂Ȃ��ꍇ
	if ( m_bCamelliaEnabled == FALSE ) {

		return sendComm( pComm, dwSize) ;
	}

	//----- �Í������� -----//
	// dwSize�ɉ�����16�o�C�g�̔{���̗̈���m�ۂ���
	// (dwSize��1�`16�̏ꍇ:16�o�C�g, 17�`32�̏ꍇ:32�o�C�g �c)
	BYTE *pBuff = NULL ;
	pBuff = (BYTE *)malloc( dwSize = (dwSize + 15) & ~15 ) ;
	memset( pBuff, 0, dwSize ) ;

	// 16�o�C�g����pComm�̓��e���Í�������pBuff�Ɋi�[����
	for ( DWORD i = 0; i < dwSize; i+=16 ) {
		Camellia_EncryptBlock(
			128,		// ����
			pComm + i,	// �����̃|�C���^
			m_uKttWork,	// �g�匮
			pBuff + i	// �Í������ꂽ���̃|�C���^
			) ;
	}

	//----- ���M���� -----//
	DWORD dwLen = 0 ;
	
	BOOL bRet = WriteFile(
			m_hCom,	// �f�o�C�X�̃n���h��
			pBuff,	// �Í������ꂽ�f�[�^�o�b�t�@
			dwSize,	// �Í������ꂽ�������ݑΏۂ̃o�C�g��
			&dwLen,	// �������񂾃o�C�g��
			NULL	// �I�[�o�[���b�v�\���̂̃o�b�t�@
			) ;

	// �m�ۂ������������������
	free( pBuff ) ;

	// WriteFile()�����ClearCommError()���Ăяo���ƁAComStat.cbInQue��
	// ���΂���0�ɂȂ�(��M�o�b�t�@�̃o�C�g�����擾�o���Ȃ�)���߁ASleep������
	Sleep(100) ;

	return bRet ;

}

/*====================================================================*/
/* �����T�v  �FH1E-USB����f�[�^����M����(�m�[�}�����)
/* ����      �FpComm	�f�[�^�o�b�t�@
/*             dwSize	��M����f�[�^�o�b�t�@�̃o�C�g��
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
BOOL CH1USBCom::recvComm( BYTE *pComm, const DWORD dwSize )
{

	//----- ��M���� -----//
	DWORD dwLen = 0 ;

	return ReadFile(
		m_hCom,	// �f�o�C�X�̃n���h��
		pComm,	// �f�[�^�o�b�t�@
		dwSize,	// �ǂݎ��Ώۂ̃o�C�g��
		&dwLen,	// �ǂݎ�����o�C�g��
		NULL	// �I�[�o�[���b�v�\���̂̃o�b�t�@
		) ;

}

/*====================================================================*/
/* �����T�v  �FH1E-USB����f�[�^����M����(�Í��ʐM���)
/* ����      �FpComm	�f�[�^�o�b�t�@
/*             dwSize	��M����f�[�^�o�b�t�@�̃o�C�g��
/* �߂�l    �F����:0�ȊO�A���s:0
/*====================================================================*/
BOOL CH1USBCom::recvCommEnc( BYTE *pComm, DWORD dwSize )
{

	// �Í��ʐM��Ԃł͂Ȃ��ꍇ
	if ( m_bCamelliaEnabled == FALSE ) {

		return recvComm( pComm, dwSize) ;
	}

	//----- ��M���� -----//
	// dwSize�ɉ�����16�o�C�g�̔{���̗̈���m�ۂ���
	// (dwSize��1�`16�̏ꍇ:16�o�C�g, 17�`32�̏ꍇ:32�o�C�g �c)
	BYTE *pBuff = NULL ;
	pBuff = (BYTE *)malloc( dwSize = (dwSize + 15) & ~15 ) ;
	memset( pBuff, 0, dwSize ) ;

	DWORD dwLen = 0 ;

	BOOL bRet = ReadFile(
			m_hCom,	// �f�o�C�X�̃n���h��
			pBuff,	// �Í������ꂽ�f�[�^�o�b�t�@
			dwSize,	// �Í������ꂽ�ǂݎ��Ώۂ̃o�C�g��
			&dwLen,	// �ǂݎ�����o�C�g��
			NULL	// �I�[�o�[���b�v�\���̂̃o�b�t�@
 			) ;

	if ( bRet == FALSE ) {
		goto EXIT_TREAT ;
	}

	//�ǂݎ��Ώۂ̃o�C�g���Ɠǂݎ�����o�C�g�����قȂ�ꍇ�͎�M�G���[�Ƃ���B(V1.10---2011/5/25)
	//begin
	if ( dwSize != dwLen ) {

		return FALSE;
	}
	//�ǂݎ��Ώۂ̃o�C�g���Ɠǂݎ�����o�C�g�����قȂ�ꍇ�͎�M�G���[�Ƃ���B(V1.10---2011/5/25)
	//end

	//----- �������� -----//
	// 16�o�C�g����pBuff�̓��e�𕜍�����pComm�Ɋi�[����
	for ( DWORD i = 0; i < dwSize; i+=16 ) {
		Camellia_DecryptBlock(
			128,		// ����
			pBuff + i,	// �Í����̃|�C���^
			m_uKttWork, // �g�匮
			pComm + i	// �������ꂽ���̃|�C���^
			) ;
	}

EXIT_TREAT:

	// �m�ۂ������������������
	free( pBuff ) ;

	return bRet ;

}

/*====================================================================*/
/* �����T�v  �F��M�o�b�t�@�̃o�C�g����ԋp����
/* ����      �F�Ȃ�
/* �߂�l    �F��M�o�b�t�@�̃o�C�g��
/*====================================================================*/
/* ReadFile()�Ŏw�肷��o�C�g�����A���ۂɓǍ��ރo�C�g���ƈ�v���Ă��Ȃ���
   ��M�҂��Ŏ~�܂��Ă��܂����߁A�{�֐��Ŏ�M�o�b�t�@�̃T�C�Y���擾���Ă��� */
DWORD CH1USBCom::getBuffSize( void )
{
	DWORD dwErrors = 0 ;
	DWORD dwCount = 0 ;
	COMSTAT ComStat ;
	memset( &ComStat, 0, sizeof(COMSTAT) ) ;
	
	ClearCommError(
		m_hCom,		// �ʐM�f�o�C�X�̃n���h��
		&dwErrors,	// �G���[�R�[�h���󂯎��ϐ��ւ̃|�C���^
		&ComStat	// �ʐM��ԃo�b�t�@�ւ̃|�C���^
		) ;
	
	return ComStat.cbInQue ;

}

/*====================================================================*/
/* �����T�v  �F512�o�C�g�̗�������Í��L�[�𐶐�����
/*  step1 4096�r�b�g(512�o�C�g)�̃r�b�g�A�h���X�ϊ��ɂ��A�r�b�g������
/*  step2 32��16�o�C�g�u���b�N�Ř_�����Z
/* ����      �Fsrc	512�o�C�g�̗���
/*             dst	�X�N�����u������������16�o�C�g�̃f�[�^
/* �߂�l    �F����:0
/*====================================================================*/
// H1E-USB�R�}���h�C���^�[�t�F�[�X�d�l��(�X�N�����u���Í��L�[��)�����p
int CH1USBCom::decodeMasterKey512( const BYTE *src, BYTE *dst )
{

	DWORD _work[512/4], srcAddr, dstAddr ;
	BYTE *work, *sp, *bp, *cp, *dp, cmd ;
	int i, j, k ;
	
	work = (BYTE*)_work ;
	for ( i = 0; i < sizeof(_work) / 4; i++ ) {  // ��Ɨ̈�̏�����
		_work[i] = 0 ;
	}
	
	// �r�b�g�A�h���X�ϊ�
	for ( srcAddr = 0; srcAddr < 4096; srcAddr++ ) {

		dstAddr = 0 ;

		ADDRCONV( srcAddr, dstAddr, 0,  5 ) ; 
		ADDRCONV( srcAddr, dstAddr, 1, 10 ) ; 
		ADDRCONV( srcAddr, dstAddr, 2,  8 ) ; 
		ADDRCONV( srcAddr, dstAddr, 3,  6 ) ; 
		ADDRCONV( srcAddr, dstAddr, 4,  7 ) ; 
		ADDRCONV( srcAddr, dstAddr, 5,  4 ) ; 
		ADDRCONV( srcAddr, dstAddr, 6, 11 ) ; 
		ADDRCONV( srcAddr, dstAddr, 7,  2 ) ; 
		ADDRCONV( srcAddr, dstAddr, 8,  3 ) ; 
		ADDRCONV( srcAddr, dstAddr, 9,  1 ) ; 
		ADDRCONV( srcAddr, dstAddr,10,  0 ) ;
		ADDRCONV( srcAddr, dstAddr,11,  9 ) ;

		if ( src[srcAddr/8] & ( (BYTE)0x80 >> ( srcAddr&7 ) ) ) {
			work[dstAddr/8] |= (BYTE)0x80 >> ( dstAddr&7 ) ;
		}
	}

	// �_�����Z
	bp = work + 16 ;
	for ( i = 0; i < 10; i++, bp += 16*3 ) {
		
		cp = bp ;
		dp = bp + 16 ;
		for ( j = 0; j < 2; j++ ) {
			
			sp = work ;
			for ( k = 0; k < 16; k++, dp++, sp++ ) {
			
				// �_�����Z�R�}���h�̎��o��
				if ( k & 1 ) {
					cmd = cp[k/2] & 0x0F ;
					cp++ ;
				}
				else {
					cmd = cp[k/2] >> 4 ;
				}

				// �_�����Z�̎��{(���Z���ʂ̉��ʃo�C�g�����ӂɑ��)
				switch( cmd ) {
					case 0x00:
						*sp = *sp ^ *dp ;
						break ;

					case 0x01:
						*sp = *sp + *dp ;
						break ;

					case 0x02:
						*sp = *sp - *dp ;
						break ;

					case 0x03:
						*sp = ( *sp << 1 ) | ( *sp & 0x80 ) ? 0x01 : 0x00 ;
						break ;

					case 0x04:
						*sp = ( *sp >> 1 ) | ( *sp & 0x01 ) ? 0x80 : 0x00 ;
						break ;

					case 0x05:
						#pragma warning( push ) // �����̌x����Ԃ�ۑ�
						#pragma warning( disable:4244 ) // 4244�Ԃ̌x����}��
						
						*sp = (DWORD)*sp * (DWORD)*dp ;
						
						#pragma warning( pop ) // �ۑ������x����Ԃ𕜌�
						break ;

					case 0x06:
						*sp = ~*sp ^ *dp ;
						break ;

					case 0x07:
						*sp = ~*dp ^ *sp ;
						break ;

					case 0x08:
						*sp = ~*sp ;
						break ;

					case 0x09:
						*sp = ( *sp<<4 ) | ( *dp>>4 ) ;
						break ;

					case 0x0A:
						*sp = ( *sp>>4 ) | ( *dp<<4 ) ;
						break ;

					case 0x0B:
						*sp = ( *sp<<4 ) | ( *dp>>4 ) ;
						break ;

					case 0x0C:
						*sp = *sp << ( *dp&3 ) ;
						break ;

					case 0x0D:
						*sp = *sp >> ( *dp>>6 ) ;
						break ;

					case 0x0E:
						*sp = *dp << ( *sp&3 ) ;
						break ;

					case 0x0F:
						*sp = *dp >> ( *sp >> 6 ) ;
						break ;
				
				} // end switch

			} // end for 

		} // end for 

	} // end for 

	for ( i = 0; i < 16; i++ ) {
		work[i] = dp[i] ^ work[i] ;
	}

	memcpy(dst, work, 16) ;

	return 0 ;
}
