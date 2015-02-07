#include "stdafx.h"
#include "H1EUSB_MsgGuide.h"
/********************************************************************************************/
/* CMsgGuide�N���X�v���O����																*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20120110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/********************************************************************************************/
CMsgGuide::CMsgGuide(void)
{
}

CMsgGuide::~CMsgGuide(void)
{
}

//�e���v���[�g���M���b�Z�[�W�쐬
CString CMsgGuide::SendFormatMessage(UINT uiBankNum, UINT uiTemNum, UINT uiSendCounts, UINT uiLists)
{
	CString strMessage;
	CString strWork1;
	CString strWork2;

	strWork1.FormatMessage(IDS_SEND_TEMPLATES);
	strWork2.FormatMessage(IDS_SEND_INFO, uiBankNum, uiTemNum, uiSendCounts, uiLists);
	strMessage = strWork1 + _T("\r\n") + strWork2;
	return strMessage;
}

//�e���v���[�g���M�I�����b�Z�[�W�쐬
CString CMsgGuide::SendCompFormatMessage(UINT uiBankNum, UINT uiTemNum, UINT uiSendCounts, UINT uiLists, int iReturn, DWORD dwError)
{
	CString strMessage;
	CString strWork1;
	CString strWork2;
	CString strWork3;

	strWork1.FormatMessage(IDS_SEND_COMPLETE);
	strWork2.FormatMessage(IDS_SEND_INFO, uiBankNum, uiTemNum, uiSendCounts, uiLists);
	strMessage = strWork1 + _T("\r\n") + strWork2;
	if(dwError != 0x0000){
		strMessage += _T("\r\n") + ErrorFormatMessage(iReturn, dwError);
	}
	return strMessage;
}

//�G�����[���I�����b�Z�[�W�쐬
CString CMsgGuide::EnrollFormatMessage(UINT uiBankNum, UINT uiTemNumt, int iReturn, DWORD dwError)
{
	CString strMessage;
	CString strWork1;
	CString strWork2;
	CString strWork3;

	strWork1.FormatMessage(IDS_ENROLL_COMPLETE);
	strWork2.FormatMessage(IDS_BANKTEMP_INFO, uiBankNum, uiTemNumt);
	strMessage = strWork1 + _T("\r\n") + strWork2;
	if(dwError != 0x0000){
		strMessage += _T("\r\n") + ErrorFormatMessage(iReturn, dwError);
	}
	return strMessage;
}

//�G�����[���ُ�I�����b�Z�[�W�쐬
CString CMsgGuide::EnrollNGMessage()
{
	CString strMessage;
	strMessage.FormatMessage(IDS_ENROLL_NG);

	return strMessage;
}


//�G���[�K�C�_���X�쐬
CString CMsgGuide::ErrorFormatMessage(int iReturn, DWORD dwError)
{
	CString strMessage;

	strMessage.FormatMessage(IDS_ERR_INFO, iReturn, dwError);
	return strMessage;
}

//�o���N�ԍ����̓K�C�_���X�쐬
CString CMsgGuide::GuideBankNummberFormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_GUIDE_BANK);
	return strMessage;
}

//�e���v���[�g�ԍ����̓K�C�_���X�쐬
CString CMsgGuide::GuideTemplateNummberFormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_GUIDE_TEMP);
	return strMessage;
}

//�e���v���[�g�R�s�[�K�C�_���X�쐬
CString CMsgGuide::GuideCopyNummberFormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_GUIDE_COPY);
	return strMessage;
}

//�e���v���[�g�ԍ����̓K�C�_���X�쐬
CString CMsgGuide::GuideNotExistBank0000FormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_NOTEXIST_BANK0000);
	return strMessage;
}

//�w�������Ă����������b�Z�[�W�쐬
CString CMsgGuide::GuidePlaceFingerFormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_GUIDE_PLACE_FINGER);
	return strMessage;
}

//�R�s�[�I�����b�Z�[�W�쐬
CString CMsgGuide::CompleteCopyFormatMessage(int iCounts, int iError, DWORD dwError)
{
	CString strMessage;
	CString strWork1;
	CString strWork2;

	strWork1.FormatMessage(IDS_COPY_COMPLETE, iCounts);
	strWork2.FormatMessage(IDS_ERR_INFO, iError, dwError);
	strMessage = strWork1;
	if(dwError != 0){
		strMessage += _T("\r\n") + strWork2;
	}
	return strMessage;
}

//�e���v���[�g�폜�I�����b�Z�[�W�쐬
CString CMsgGuide::CompleteDelFormatMessage(UINT uiBankNum, UINT uiTemNumt, int iReturn, DWORD dwError)
{
	CString strMessage;
	CString strWork1;
	CString strWork2;
	CString strWork3;

	strWork1.FormatMessage(IDS_DEL_COMPLETE_TEMP);
	strWork2.FormatMessage(IDS_BANKTEMP_INFO, uiBankNum, uiTemNumt);
	strMessage = strWork1 + _T("\r\n") + strWork2;
	if(dwError != 0){
		strMessage += _T("\r\n") + ErrorFormatMessage(iReturn, dwError);
	}
	return strMessage;
}

//�o���N�폜�I�����b�Z�[�W�쐬
CString CMsgGuide::CompleteDelBankFormatMessage(UINT uiBankNum, int iReturn, DWORD dwError)
{
	CString strMessage;

	strMessage.FormatMessage(IDS_DEL_COMPLETE_BANK, uiBankNum);
	if(dwError != 0){
		strMessage += _T("\r\n") + ErrorFormatMessage(iReturn, dwError);
	}
	return strMessage;
}


//�S�e���v���[�g�폜�I�����b�Z�[�W�쐬
CString CMsgGuide::CompleteDelAllFormatMessage(int iReturn, DWORD dwError)
{
	CString strMessage;

	strMessage.FormatMessage(IDS_DEL_COMPLETE_ALL);
	if(dwError != 0){
		strMessage += _T("\r\n") + ErrorFormatMessage(iReturn, dwError);
	}
	return strMessage;
}

//�e���v���[�g�R�s�[���b�Z�[�W�쐬
CString CMsgGuide::CopyFileFormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_COPY_TEMPFILE);
	return strMessage;
}

//�e���v���[�g�F�؃��b�Z�[�W�쐬
CString CMsgGuide::VerificationFormatMessage(UINT uiBankNum, UINT uiTemNumt, int iReturn, DWORD dwError)
{
	CString strMessage;
	CString strWork1;
	CString strWork2;

	strWork1.FormatMessage(IDS_VERIF_SUCCESS);
	strWork2.FormatMessage(IDS_BANKTEMP_INFO, uiBankNum, uiTemNumt);
	strMessage = strWork1 + _T("\r\n") + strWork2;
	if(dwError != 0x0000){
		strMessage += _T("\r\n") + ErrorFormatMessage(iReturn, dwError);
	}
	return strMessage;
}

//�o���N�F�؃��b�Z�[�W�쐬
CString CMsgGuide::VerifBankFormatMessage(UINT uiBankNum, int iReturn, DWORD dwError)
{
	CString strMessage;
	CString strWork1;
	CString strWork2;

	strWork1.FormatMessage(IDS_VERIF_SUCCESS);
	strWork2.FormatMessage(IDS_BANKINFO, uiBankNum);
	strMessage = strWork1 + _T("\r\n") + strWork2;
	if(dwError != 0x0000){
		strMessage += _T("\r\n") + ErrorFormatMessage(iReturn, dwError);
	}
	return strMessage;
}
//�o���N�ԔF�؃��b�Z�[�W�쐬
CString CMsgGuide::VerifAcrossBKFormatMessage(UINT uiSBankNum, UINT uiEBankNum, int iReturn, DWORD dwError)
{
	CString strMessage;
	CString strWork1;
	CString strWork2;

	strWork1.FormatMessage(IDS_VERIF_SUCCESS);
	strWork2.FormatMessage(IDS_ACROSBKINFO, uiSBankNum, uiEBankNum);
	strMessage = strWork1 + _T("\r\n") + strWork2;
	if(dwError != 0x0000){
		strMessage += _T("\r\n") + ErrorFormatMessage(iReturn, dwError);
	}
	return strMessage;
}
//�F�ؐ���I�����b�Z�[�W�쐬
CString CMsgGuide::VerificationOKFormatMessage()
{
	CString strMessage;
	strMessage.FormatMessage(IDS_VERIF_SUCCESS);

	return strMessage;
}

//�F�؈ُ�I�����b�Z�[�W�쐬
CString CMsgGuide::VerificationNGMessage()
{
	CString strMessage;
	strMessage.FormatMessage(IDS_VERIF_NG);

	return strMessage;
}

//�e���v���[�g�t�����b�Z�[�W�쐬
CString CMsgGuide::TemplateFullMessage()
{
	CString strMessage;
	strMessage.FormatMessage(IDS_TEMPFULL);

	return strMessage;
}

//�Z�b�e�B���O�I�����b�Z�[�W�쐬
CString CMsgGuide::EndAppSettingMessage(CString strCom)
{
	CString strMessage;
	strMessage.FormatMessage(IDS_SETTING_END, strCom);

	return strMessage;
}




//�g�����U�N�V�����ėp���b�Z�[�W�쐬
CString CMsgGuide::SendGeneralFormatMessage(int iContorl)
{
			CString strMessage;
			CString strW1;
			CString strW2;

			strW2.Empty();
	
	switch(iContorl){
		case MSGGUID_LED_OFF:			strW1.FormatMessage(IDS_SEND_LED);strW2.FormatMessage(IDS_LED_OFF);					break;
		case MSGGUID_LED_GREEN:			strW1.FormatMessage(IDS_SEND_LED);strW2.FormatMessage(IDS_LED_GREEN);				break;
		case MSGGUID_LED_RED:			strW1.FormatMessage(IDS_SEND_LED);strW2.FormatMessage(IDS_LED_RED);					break;
		case MSGGUID_LED_GREENRED:		strW1.FormatMessage(IDS_SEND_LED);strW2.FormatMessage(IDS_LED_GREENRED);			break;
		case MSGGUID_SECURITY_LOW:		strW1.FormatMessage(IDS_SEND_SEC);strW2.FormatMessage(IDS_SEC_LOW);					break;
		case MSGGUID_SECURITY_MIDLOW:	strW1.FormatMessage(IDS_SEND_SEC);strW2.FormatMessage(IDS_SEC_MIDLOW);				break;
		case MSGGUID_SECURITY_MIDDLE:	strW1.FormatMessage(IDS_SEND_SEC);strW2.FormatMessage(IDS_SEC_MID);					break;
		case MSGGUID_SECURITY_MIDHIGH:	strW1.FormatMessage(IDS_SEND_SEC);strW2.FormatMessage(IDS_SEC_MIDHIGH);				break;
		case MSGGUID_SECURITY_HIGH:		strW1.FormatMessage(IDS_SEND_SEC);strW2.FormatMessage(IDS_SEC_HIGH);				break;
		case MSGGUID_BUZZER_ON:			strW1.FormatMessage(IDS_SEND_BUZ);strW2.FormatMessage(IDS_BUZ_ON);					break;
		case MSGGUID_BUZZER_OFF:		strW1.FormatMessage(IDS_SEND_BUZ);strW2.FormatMessage(IDS_BUZ_OFF);					break;
		case MSGGUID_VERIF_11:			strW1.FormatMessage(IDS_SEND_VERIF);strW2.FormatMessage(IDS_VERIF_11);				break;
		case MSGGUID_VERIF_1N:			strW1.FormatMessage(IDS_SEND_VERIF);strW2.FormatMessage(IDS_VERIF_1N);				break;
		case MSGGUID_VERIF_ACBANK:		strW1.FormatMessage(IDS_SEND_VERIF);strW2.FormatMessage(IDS_VERIF_ACBANK);			break;
		case MSGGUID_VERIF_WTEMP:		strW1.FormatMessage(IDS_SEND_VERIF);strW2.FormatMessage(IDS_VERIF_WTEMP);			break;
		case MSGGUID_SEND_HWRESET:		strW1.FormatMessage(IDS_SEND_HWRESET);												break;
		case MSGGUID_SEND_SCRAMBLE:		strW1.FormatMessage(IDS_SEND_SCRAMBLE);												break;
		case MSGGUID_SEND_GETFWVER:		strW1.FormatMessage(IDS_SEND_DEVINFO);strW2.FormatMessage(IDS_FW_VERSION);			break;
		case MSGGUID_SEND_GETSERNUM:	strW1.FormatMessage(IDS_SEND_DEVINFO);strW2.FormatMessage(IDS_SER_NUMMBER);			break;
		case MSGGUID_SEND_TSEN_NOTIFY:	strW1.FormatMessage(IDS_SND_TSENNOTFY);strW2.FormatMessage(IDS_TSENNOTYFY_ENABLE);	break;
		case MSGGUID_SEND_GETTEMP:		strW1.FormatMessage(IDS_SEND_GETTEMP);												break;


		default:						strW1.Empty(); strW2.Empty();			break;
	}
	strMessage = strW1 + strW2 + _T(".");
	return strMessage;
}




CString CMsgGuide::ErrorGeneralFormatMessage(int iError)
{
	CString strMessage;
	CString strW1;

	strW1.Empty();

	switch(iError){
		case H1USB_ERR_TRANSTOUT:				strW1.FormatMessage(IDS_ERR_G_TRANTOUT);		break;
		//case H1USB_ERR_BIGGER:					strW1.FormatMessage(IDS_ERR_G_TRANTOUT);	break;
		//case H1USB_ERR_INPNUMMBER:				strW1.FormatMessage(IDS_ERR_G_TRANTOUT);	break;
		//case H1USB_ERR_CREATETHREAD:			strW1.FormatMessage(IDS_ERR_G_TRANTOUT);		break;
		//case H1USB_ERR_MAXEVENTS:				strW1.FormatMessage(IDS_ERR_G_TRANTOUT);		break;
		//case H1USB_ERR_COMOPEN:					strW1.FormatMessage(IDS_ERR_G_TRANTOUT);	break;
		//case H1USB_ERR_COMCLOSE:				strW1.FormatMessage(IDS_ERR_G_TRANTOUT);		break;
		case H1USB_ERR_READFILE:				strW1.FormatMessage(IDS_ERR_G_READFILE);		break;
		case H1USB_ERR_WRITEFILE:				strW1.FormatMessage(IDS_ERR_G_WRITEFILE);		break;
		case H1USB_ERR_RDATA_NON:				strW1.FormatMessage(IDS_ERR_G_RECVDATANON);		break;
		case H1USB_ERR_RESPONS:					strW1.FormatMessage(IDS_ERR_G_INVALIDRESP);		break;
		case H1USB_ERR_TEMP_NOTOPEN:			strW1.FormatMessage(IDS_ERR_G_TMPFILE_NOTOPEN);	break;
		case H1USB_ERR_TEMPFILE_NON:			strW1.FormatMessage(IDS_ERR_G_TEMPNON);			break;
		case H1USB_ERR_TEMPFILE_COPY:			strW1.FormatMessage(IDS_ERR_G_TEMPCOPY);		break;
		case H1USB_ERR_TEMPFILE_DEL:			strW1.FormatMessage(IDS_ERR_G_TEMPDELETE);		break;
		default:
			break;
	}
	strMessage.FormatMessage(IDS_ERR_GENMSG, iError, strW1);
	return strMessage;
}
