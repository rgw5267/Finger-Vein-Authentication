#pragma once 
//#include "afx.h"
//#include "resource.h"		// メイン シンボル
//#include "H2E.h"

//Send Command Message Guidance id.
#define MSGGUID_SEND_GETDEVINFO		1				//Get deviee infomation.
#define MSGGUID_SEND_HWRESET		2				//Hardware reset.
#define MSGGUID_SEND_PASS_DRIVER	3				//Relase PASS_DRIVE.
#define MSGGUID_SEND_ENROLL			4				//Enroll 
#define MSGGUID_SEND_DELTMP			5				//Delete one of template.
#define MSGGUID_SEND_DELTMP_GROUP	6				//Delete Group template.
#define MSGGUID_SEND_DELTMP_ALL		7				//Delete All Template.
#define MSGGUID_SEND_VERIF_11		8				//One to one Verification.
#define MSGGUID_SEND_VERIF_1N_GROUP	9				//Group template Verification.
#define MSGGUID_SEND_VERIF_1N_ALL	10				//All template Verification.
#define MSGGUID_SEND_GET_TMPINFO	11				//Get template infomation.
#define MSGGUID_SEND_READ_TMP		12				//Read Template.
#define MSGGUID_SEND_WRITE_TMP		13				//Write template.
#define MSGGUID_SEND_SETID			14				//Set id.
#define MSGGUID_SEND_CHANGE_ID		15				//Change id.
#define MSGGUID_SEND_CHECK_FLASH	16				//Check flash ROM
#define MSGGUID_SEND_SET_SECLEVEL	17				//Set security level.
#define MSGGUID_SEND_TAKEMODE		18				//Set take mode.

#define MSGGUID_NODISP				999				//Not dispalay message.


//class CMsgGuide
//{
//public:
//	CMsgGuide(void);
//	virtual ~CMsgGuide(void);
//	CString GuideBankNummberFormatMessage();
//	CString GuideTemplateNummberFormatMessage();
//	CString GuideCopyNummberFormatMessage();
//	CString GuidePlaceFingerFormatMessage(BOOL bPlace, int iCounts, int iTotalCounts);
//	CString SendGeneralFormatMessage(UINT uiMsgID, UINT uiSubID);
//	CString VerificationOKFormatMessage();
//	CString VerificationNGMessage();
//	CString EnrollNGMessage();
//	CString EndAppSettingMessage(CString strCom);
//};
