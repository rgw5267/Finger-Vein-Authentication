#pragma once
#include "afx.h"
#include "H1EUSB_Thread.h"
#include "Camellia.h"	// �Í����Ŏg�p
#include "H1EUSB.h"
/********************************************************************************************/
/* CH1USBCom�N���X�w�b�_																	*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20080110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
#define MAX_NINSHO_TEMPS				6					//�e���v���[�g�t�F�؂̍ő吔
#define H1USB_STATUS_TUCH_ON			0x0001				//�^�b�`�Z���T�[���n�m���
#define H1USB_STATUS_TUCH_OFF			0x0002				//�^�b�`�Z���T�[���n�e�e���
#define H1USB_STATUS_INVDATA			0xFFFF				//�\�����Ȃ��f�[�^����M����

//�R�[���o�b�N�G���g���[��`
typedef LRESULT (CALLBACK* H1ECALBACK_TUCH_NTOTIFICATION)(DWORD dwStatus, LPVOID pObj);	//�^�b�`�Z���T�[��Ԓʒm
typedef LRESULT (CALLBACK* H1ECALBACK_SENNOTIFI)(DWORD dwStatus, LPVOID pObj);			//���M�I���ʒm


//�R�[���o�b�N�G���g���[�\����
typedef struct _h1ecallbacks
{
	H1ECALBACK_TUCH_NTOTIFICATION			OnTuchNotification;		//�^�b�`�Z���T�[���ʒm��
	H1ECALBACK_SENNOTIFI					OnSendEnd;				//���M�I���ʒm	
} H1ECALLBACKS, *PH1ECALLBACKS;

//�G���h�|�C���g���iH1ECOMOBJEP�\���́j
typedef struct _h1ecomobjep
{
	LPVOID			pObj;				//�R�[���o�b�N��̃I�u�W�F�N�g
	H1ECALLBACKS	H1ECallBacks;		//�R�[���o�b�N�G���g���[		
} H1ECOMOBJEP, *PH1ECOMOBJEP;


class CH1USBCom :
	public CA2GObjThread
{
public:
	CH1USBCom(PH1ECOMOBJEP pH1EComObjEP);														//�{�N���X�̃R���X�g���N�^
	virtual ~CH1USBCom(void);																	//�{�N���X�̃f�X�g���N�^
	int Open(LPCTSTR pComPort, LPDWORD pdwError);												//COM�|�[�g�I�[�v��
	int Close(LPDWORD pdwError);																//COM�|�[�g�N���[�Y
	int StartEndPoint(void);																	//�G���h�|�C���g�̋@�\�J�n
	int StopEndPoint(void);																		//�G���h�|�C���g�̋@�\��~
	int TransToDevice(UINT uiMsg, WPARAM wParam, LPARAM lParam);								//�g�����U�N�V�������C������
	int SetEnableBank(WORD* pwEnableBank, WORD wEnableBankCount);								//�L���ȃo���N�̔z����Z�b�g����
	int SetTempU(PTEMPU pTempU, UINT uiCounts, BOOL bInit);										//�e���v���[�g�����Z�b�g����
	int GetTempU(PTEMPU pTempU);																//�e���v���[�g�����擾����
	char m_cFwVersion[50];																		//�t�@�[���E�F�A�o�[�W�����i�[�z��
	char m_cSerialNum[50];																		//�V���A���i���o�[�i�[�z��
	WORD* m_pwEnableBank;																		//�o���N�̔z��̃|�C���^
	WORD m_wEnableBankCount;																	//�L���ȃo���N��
	HANDLE			m_hCom;																		//COM�|�[�g�n���h��


private:
	static LRESULT CALLBACK OnThreadBefor(LPVOID pObj);											//�X���b�h�O�����̃R�[���o�b�N����
	static LRESULT CALLBACK OnThread(LPVOID pObj);												//�X���b�h������R�[���o�b�N����
	static LRESULT CALLBACK OnThreadAfter(LPVOID pObj);											//�X���b�h�㏈���̃R�[���o�b�N����
	static LRESULT CALLBACK OnComMessage(UINT uiMsg, WPARAM wParam, LPARAM lParam, LPVOID pObj);//�g�����U�N�V�������b�Z�[�W����

	int decodeMasterKey512( const BYTE *, BYTE * ) ;											// 512�o�C�g�̗�������Í��L�[�𐶐�
	int TransScranmble(void);																	//�Í����R�}���h�g�����U�N�V����
	int TransLedGreen(void);																	//��LED�g�����U�N�V����
	int TransLedRed(void);																		//��LED�g�����U�N�V����
	int TransLedGR(void);																		//�΁{��LED�g�����U�N�V����
	int TransLedOff(void);																		//LED�����g�����U�N�V����
	int TransTuchNotify(BYTE byParam);																//�^�b�`�Z���T�[��Ԓʒm���g�����U�N�V����
	int TransGetFwVersion( char *pFwVersion );													//�t�@�[���E�F�A�o�[�W�����擾�g�����U�N�V����
	int TransGetSerialNum( char *pSerialNum );													//�V���A���i���o�[�擾�g�����U�N�V����
	int TransBUZStart( void );																	//�u�U�[���J�n�g�����U�N�V����
	int TransBUZStop( void );																	//�u�U�[����~�g�����U�N�V����
	int TransSetSecurity(BYTE byMode);															//�Z�L�����e�B���x���ݒ�g�����U�N�V����
	int TransNinsho( const BYTE byNinshoMode, const int nMemNo, BYTE *pMatchMemNo );			//�P�F�P�F�؃g�����U�N�V����
	int TransNinshoBank( const WORD wEnableBank[], const WORD wEnableBankCnt, BYTE *pMatchMemNo, WORD *pMatchBank );	//�o���N�F�؃g�����U�N�V����
	int TransNinshoWithTemplate(const PTEMPU pTempU, const BYTE byCount, BYTE *pMatchNum);		//�e���v���[�g�t�F�؃g�����U�N�V����
	int TransGetTemplate(PTEMPU pTempU);														//�e���v���[�g�擾�g�����U�N�V����
	int TransChangeBank( const WORD wBankNo);													//�o���N�ύX�g�����U�N�V����
	int TransSetTemplate(PTEMPU pTempU);														//�e���v���[�g�o�^�g�����U�N�V����
	int TransDelTemplate(const int nMemNo);														//�e���v���[�g�폜�g�����U�N�V����
	int TransHardWare( void );																	//�n�[�h�E�F�A���Z�b�g�g�����U�N�V����
	int CheckASyncData(LPDWORD pdwState);														//�F�؎��̓������X�|���X�擾
	int ledAllOff(void);
	int ledOnOff( const BYTE byColor, const BYTE byOnOff );
	int ledStartBlink( const BYTE byOffTime, const BYTE byOnTime);
	int ledBlink( const BYTE byOffTime, const BYTE byOnTime );
	int beepOK( void );
	int beepNG( void );
	int beepCancel( void );
	int beepSound( const BYTE byPattern, const BYTE byCount );

	BOOL sendComm( const BYTE *, const DWORD ) ;										//�R�}���h��H1E-USB�ɑ��M(�m�[�}�����)
	BOOL sendCommEnc( const BYTE *, DWORD ) ;											//�R�}���h��H1E-USB�ɑ��M(�Í��ʐM���)
	DWORD getBuffSize( void ) ;															//��M�o�b�t�@�̃o�C�g����ԋp
	BOOL recvComm( BYTE *, const DWORD ) ;												//H1E-USB����f�[�^����M����(�m�[�}�����)
	BOOL recvCommEnc( BYTE *, DWORD ) ;													//H1E-USB����f�[�^����M����(�Í��ʐM���)
	BYTE m_byCurrentWorkKey[16] ;														//�Í���
	BOOL			m_bCamelliaEnabled ;												//�Í��ʐM��Ԃ��ۂ���\���t���O(TRUE: �Í��ʐM���, FALSE:�m�[�}�����)
	KEY_TABLE_TYPE	m_uKttWork ;														//�g�匮
	H1ECOMOBJEP		m_H1EComObjEP;														//�G���h�|�C���g���iH1ECOMOBJEP�\���́j
	HANDLE			m_hTEnd;															//�g�����U�N�V�����I���C�x���g�n���h��
	TEMPU			m_TempU[MAX_NINSHO_TEMPS];											//�e���v���[�g���j�b�g�e�[�u��
	UINT			m_uiTempUs;															//�e���v���[�g���j�b�g�̌�
	int				m_iLastError;														//�G���[�i�[�ϐ�
	BOOL			m_bTransEnable;														//�g�����U�N�V������


};
