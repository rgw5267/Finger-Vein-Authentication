#include "H2E_Number.h"

CNumber::CNumber(void)
{
}

CNumber::~CNumber(void)
{
}

BOOL CNumber::ConvertBankNummber(CString strBank, UINT* puiBank)
{
	UINT uiBank;
	int iSize;
	int iReturn;

	strBank.MakeUpper();
	iSize = strBank.GetLength();
	if(iSize < 1){
		return FALSE;
	}
	iReturn = sscanf_s(strBank, "%x", &uiBank);
	if(iReturn == 0){
		return FALSE;
	}
	if(uiBank>0x7FFF){
		return FALSE;
	}
	*puiBank = uiBank;
	return TRUE;
}

BOOL CNumber::ConvertTemlateNummber(CString strTemp, UINT* puiTemp)
{
	UINT uiTemp;
	int iSize;
	int iReturn;

	strTemp.MakeUpper();

	iSize = strTemp.GetLength();
	if(iSize < 1){
		return FALSE;
	}
	iReturn = sscanf_s(strTemp, "%x", &uiTemp);
	if(iReturn == 0){
		return FALSE;
	}
	if(uiTemp>0xFFFF){
		return FALSE;
	}
	*puiTemp = uiTemp;
	return TRUE;
}

