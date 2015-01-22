#pragma once
#include "afx.h"
#include "resource.h"		// メイン シンボル
#include "H1EUSB.h"
/********************************************************************************************/
/* CMsgGuideクラスヘッダ																	*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20080110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/

//トランザクション汎用メッセージ作成用メッセージID
#define MSGGUID_LED_OFF				0
#define MSGGUID_LED_GREEN			1
#define MSGGUID_LED_RED				2
#define MSGGUID_LED_GREENRED		3
#define MSGGUID_SECURITY_LOW		10
#define MSGGUID_SECURITY_MIDLOW		11
#define MSGGUID_SECURITY_MIDDLE		12
#define MSGGUID_SECURITY_MIDHIGH	13
#define MSGGUID_SECURITY_HIGH		14
#define MSGGUID_BUZZER_ON			20
#define MSGGUID_BUZZER_OFF			21
#define MSGGUID_VERIF_11			30
#define MSGGUID_VERIF_1N			31
#define MSGGUID_VERIF_ACBANK		32
#define MSGGUID_VERIF_WTEMP			33
#define MSGGUID_SEND_HWRESET		34
#define MSGGUID_SEND_SCRAMBLE		35
#define MSGGUID_SEND_GETFWVER		36
#define MSGGUID_SEND_GETSERNUM		37
#define MSGGUID_SEND_TSEN_NOTIFY	38
#define MSGGUID_SEND_GETTEMP		39

#define MSGGUID_NODISP				999


class CMsgGuide
{
public:
	CMsgGuide(void);
	virtual ~CMsgGuide(void);
	CString SendFormatMessage(UINT uiBankNum, UINT uiTemNum, UINT uiSendCounts, UINT uiLists);
	CString SendCompFormatMessage(UINT uiBankNum, UINT uiTemNum, UINT uiSendCounts, UINT uiLists, int iReturn, DWORD dwError);
	CString EnrollFormatMessage(UINT uiBankNum, UINT uiTemNumt, int iReturn, DWORD dwError);
	CString ErrorFormatMessage(int iReturn, DWORD dwError);
	CString EnrollNGMessage();
	CString GuideBankNummberFormatMessage();
	CString GuideTemplateNummberFormatMessage();
	CString GuideCopyNummberFormatMessage();
	CString GuideNotExistBank0000FormatMessage();
	CString GuidePlaceFingerFormatMessage();
	CString CompleteCopyFormatMessage();
	CString CompleteCopyFormatMessage(int iCounts, int iError, DWORD dwError);
	CString CompleteDelFormatMessage(UINT uiBankNum, UINT uiTemNumt, int iReturn, DWORD dwError);
	CString CompleteDelBankFormatMessage(UINT uiBankNum, int iReturn, DWORD dwError);
	CString CompleteDelAllFormatMessage(int iReturn, DWORD dwError);
	CString CopyFileFormatMessage();
	CString SendGeneralFormatMessage(int iContorl);
	CString VerificationFormatMessage(UINT uiBankNum, UINT uiTemNumt, int iReturn, DWORD dwError);
	CString VerifBankFormatMessage(UINT uiBankNum, int iReturn, DWORD dwError);
	CString VerifAcrossBKFormatMessage(UINT uiSBankNum, UINT uiEBankNum, int iReturn, DWORD dwError);
	CString VerificationOKFormatMessage();
	CString VerificationNGMessage();
	CString TemplateFullMessage();
	CString EndAppSettingMessage(CString strCom);
	CString ErrorGeneralFormatMessage(int iError);

};
