#pragma once
/********************************************************************************************/
/* �{�A�v���P�[�V�����̃v���W�F�N�g�w�b�_													*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20080110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
typedef union _byteword
{
	BYTE	byte[2];
	WORD	word;
} BYTEWORD, *PBYTEWORD;

typedef union _worddword
{
	BYTE	byte[4];
	WORD	word[2];
	DWORD	dword;
}WORDDWORD, *PWORDDWORD;


#define MAX_TEMPLATE_TOTAL		2000		//�e���v���[�g�̍ő�o�^�\��
#define MAX_TEMPLATE_INBANK		100			//�P�o���N���̍ő�o�^�\��

typedef struct _tempu
{
	UINT		uiTmpNum;					//�e���v���[�g��
	SYSTEMTIME	time;						//�쐬���ꂽ����
	BYTE		byTemplate[536];			//�e���v���[�g
}TEMPU, *PTEMPU;



// �A�h���X�ϊ��}�N��
#define ADDRCONV(s,d,sb,db) if (s & (1<<sb) ) {d |= 1<<db;}

// ���b�Z�[�W�{�b�N�X�̃^�C�g��
#define TITLE	"H1E-USB_SampleApp"	

// H1E-USB �� FriendlyName
#define FRIENDLY_NAME	"Hitachi USB Finger Vein Biometric Authentication Unit"

//��ԑJ��
#define SYSTEM_STARTUP			1					//�N�������
#define SYSTEM_DEVICE_OFF		2					//�f�o�C�X���ڑ����
#define SYSTEM_DEVICE_ON		3					//�f�o�C�X�ڑ����
#define SYSTEM_DEVICE_ERR		4					//�G���[��ԏ��



//Error Code Define;
#define H1USB_OK							0
#define H1USB_NOTIFY						1
#define H1USB_CANCEL						2

#define H1USB_ERR_TRANSTOUT					-10			//�g�����U�N�V�����G���[�i�^�C���A�E�g�j
#define H1USB_ERR_BIGGER					-100		//�p�����[�^�̒l���傫������
#define H1USB_ERR_INPNUMMBER				-101		//���͒l�G���[
#define H1USB_ERR_CREATETHREAD				-120		//�X���b�h�̍쐬�Ɏ��s����
#define H1USB_ERR_MAXEVENTS					-121		//�C�x���g�̍쐬���K��ő���ɒB�����B


#define H1USB_ERR_COMOPEN					-200		//COM�|�[�g�̃I�[�v���G���[
#define H1USB_ERR_COMCLOSE					-201		//COM�|�[�g�̃I�[�v���G���[
#define H1USB_ERR_READFILE					-202		//COM�|�[�g��ReadFile�G���[
#define H1USB_ERR_WRITEFILE					-203		//COM�|�[�g��ReadFile�G���[
#define H1USB_ERR_RDATA_NON					-204		//��M�f�[�^�Ȃ�
#define H1USB_ERR_RESPONS					-205		//�ُ탌�X�|���X����M����


#define H1USB_ERR_TEMP_NOTOPEN				-500		//�e���v���[�g�t�@�C���N���X���I�[�v������Ă��Ȃ�
#define H1USB_ERR_TEMPFILE_OPEN				-501		//�e���v���[�g�t�@�C�����I�[�v���ł��Ȃ�
#define H1USB_ERR_TEMPFILE_NON				-502		//�e���v���[�g�����݂��Ȃ�
#define H1USB_ERR_TEMPFILE_COPY				-503		//�e���v���[�g�t�@�C���̃R�s�[�Ɏ��s����
#define H1USB_ERR_TEMPFILE_DEL				-504		//�e���v���[�g�t�@�C���̃R�s�[�Ɏ��s����


//�ʐM�������b�Z�[�W��`

#define H1USB_COM_SCRAMBLE				WM_USER + 0			//�Í����R�}���h���M
#define H1USB_COM_GREENLED_ON			WM_USER + 1			//�΂k�d�c�n�m
#define H1USB_COM_REDLED_ON				WM_USER + 2			//�Ԃk�d�c�n�m
#define H1USB_COM_GRLED_ON				WM_USER + 3			//�΁{�Ԃk�d�c�n�m
#define H1USB_COM_LED_OFF				WM_USER + 4			//�k�d�c�n�e�e
#define H1USB_COM_TUCH_NOTIFY			WM_USER + 5			//�^�b�`�Z���T�[��Ԓʒm
#define H1USB_COM_GETFVER				WM_USER + 6			//�^�b�`�Z���T�[��Ԓʒm
#define H1USB_COM_GETSNUM				WM_USER + 7			//�^�b�`�Z���T�[��Ԓʒm
#define H1USB_COM_BUZSTART				WM_USER + 8			//�u�U�[���J�n
#define H1USB_COM_BUZSTOP				WM_USER + 9			//�u�U�[���J�n
#define H1USB_COM_SECLOW				WM_USER + 10		//�u�U�[����~
#define H1USB_COM_SECMIDLOW				WM_USER + 11		//�u�U�[����~
#define H1USB_COM_SECMIDLE				WM_USER + 12		//�u�U�[����~
#define H1USB_COM_SECMIDHG				WM_USER + 13		//�u�U�[����~
#define H1USB_COM_SECHIHGH				WM_USER + 14		//�u�U�[����~
#define H1USB_COM_NINSHO				WM_USER + 20		//�F�؎w��
#define H1USB_COM_NINSHO_BANK			WM_USER + 22		//�o���N�ԔF��
#define H1USB_COM_NINSHO_TEMP			WM_USER + 23		//�e���v���[�g�t���F��
#define H1USB_COM_GETTEMPLATE			WM_USER + 24		//�u�U�[����~
#define H1USB_COM_CHANGE_BANK			WM_USER + 25		//�o���N�ύX
#define H1USB_COM_SETTEMPLATE			WM_USER + 26		//�e���v���[�g���M
#define H1USB_COM_DELTEMPLATE			WM_USER + 27		//�e���v���[�g�폜
#define H1USB_COM_HWRESET				WM_USER + 100		//�n�[�h�E�F�A���Z�b�g







