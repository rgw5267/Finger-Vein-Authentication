#include "stdafx.h"
#include "H2E_MsgGuide.h"

CMsgGuide::CMsgGuide(void)
{
}

CMsgGuide::~CMsgGuide(void)
{
}

//
//make group number input guidance 
//
CString CMsgGuide::GuideBankNummberFormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_GUIDE_BANK);
	return strMessage;
}

//
//make template number input guidance
//
CString CMsgGuide::GuideTemplateNummberFormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_GUIDE_TEMP);
	return strMessage;
}

//
//make copy group number input guidance
//
CString CMsgGuide::GuideCopyNummberFormatMessage()
{
	CString strMessage;

	strMessage.FormatMessage(IDS_GUIDE_COPY);
	return strMessage;
}

//
//make please place finger guidance & release finger
//
CString CMsgGuide::GuidePlaceFingerFormatMessage(BOOL bPlace, int iCounts, int iTotalCounts)
{
	CString strMsg1;
	CString strMsg2;

	strMsg2.Empty();
	if(bPlace){
		strMsg1.FormatMessage(IDS_GUIDE_PLACE_FINGER);
	}
	else{
		strMsg1.FormatMessage(IDS_GUIDE_RELEASE_FINGER);
	}
	if(iTotalCounts > 0){
		strMsg2.Format(_T("(%d/%d)"), iCounts, iTotalCounts);
	}

	strMsg1 += strMsg2;

	return strMsg1;
}


//
// make general-purpose guidance
//
CString CMsgGuide::SendGeneralFormatMessage(UINT uiMsgID, UINT uiSubID)
{
	CString strMessage;
	CString strW1;
	CString strW2;

	strW1.Empty();
	strW2.Empty();
	switch(uiMsgID){
		case MSGGUID_SEND_GETDEVINFO:											//Get deviee infomation.
			strW1.FormatMessage(IDS_SEND_GETDEVINFO);
			if(uiSubID == 0)
				strW2.FormatMessage(IDS_SUBID_FVERSION);
			else
				strW2.FormatMessage(IDS_SUBID_SERIAL);											
			break;
		case MSGGUID_SEND_HWRESET:																						break;	//Hardware reset.
		case MSGGUID_SEND_PASS_DRIVER:																					break;	//Relase PASS_DRIVE.
		case MSGGUID_SEND_ENROLL:			strW1.FormatMessage(IDS_SEND_ENROLL);										break;	//Enroll 
		case MSGGUID_SEND_DELTMP:			strW1.FormatMessage(IDS_SEND_DELTEMP);										break;	//Delete one of template.
		case MSGGUID_SEND_DELTMP_GROUP:		strW1.FormatMessage(IDS_SEND_GDELTEMP);										break;	//Delete Group template.
		case MSGGUID_SEND_DELTMP_ALL:		strW1.FormatMessage(IDS_SEND_DELALLTMP);									break;	//Delete All Template.
		case MSGGUID_SEND_VERIF_11:			strW1.FormatMessage(IDS_SEND_VERIF);strW2.FormatMessage(IDS_VERIF_11);		break;	//One to one Verification.
		case MSGGUID_SEND_VERIF_1N_GROUP:	strW1.FormatMessage(IDS_SEND_VERIF);strW2.FormatMessage(IDS_VERIF_1N);		break;	//Group template Verification.
		case MSGGUID_SEND_VERIF_1N_ALL:		strW1.FormatMessage(IDS_SEND_VERIF);strW2.FormatMessage(IDS_VERIF_1N);		break;	//All template Verification.
		case MSGGUID_SEND_GET_TMPINFO:		strW1.FormatMessage(IDS_SEND_GETTEMP_INFO);									break;	//Get template infomation.
		case MSGGUID_SEND_READ_TMP:			strW1.FormatMessage(IDS_SEND_READ_TEMP);																			break;	//Read Template.
		case MSGGUID_SEND_WRITE_TMP:		strW1.FormatMessage(IDS_SEND_WRITE_TEMP);									break;	//Write template.
		case MSGGUID_SEND_SETID:																						break;	//Set id.
		case MSGGUID_SEND_CHANGE_ID:																					break;	//Change id.
		case MSGGUID_SEND_CHECK_FLASH:																					break;	//Check flash ROM
		case MSGGUID_SEND_SET_SECLEVEL:																					break;	//Set security level.
		case MSGGUID_SEND_TAKEMODE:																						break;	//Set take mode.
		default:	break;
	}
	if((!strW1.IsEmpty()) || (!strW2.IsEmpty()))  {
		strMessage = strW1 + strW2 + _T(".");
	}
	return strMessage;
}


//make verification success end message
CString CMsgGuide::VerificationOKFormatMessage()
{
	CString strMessage;
	strMessage.FormatMessage(IDS_VERIF_SUCCESS);

	return strMessage;
}

//make verification abnormal end message
CString CMsgGuide::VerificationNGMessage()
{
	CString strMessage;
	strMessage.FormatMessage(IDS_VERIF_NG);

	return strMessage;
}

//make enroll abnormal end message
CString CMsgGuide::EnrollNGMessage()
{
	CString strMessage;
	strMessage.FormatMessage(IDS_ENROLL_NG);

	return strMessage;
}

//make applicatin and device setting  abnormal end message
CString CMsgGuide::EndAppSettingMessage(CString strCom)
{
	CString strMessage;
	strMessage.FormatMessage(IDS_SETTING_END, strCom);

	return strMessage;
}
