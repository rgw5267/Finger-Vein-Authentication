#pragma once
#include "stdafx.h"

class CNumber
{
public:
	CNumber(void);
	~CNumber(void);

	BOOL ConvertBankNummber(CString strBank, UINT* puiBank);
	BOOL ConvertTemlateNummber(CString strTemp, UINT* puiTemp);

};
