// H2ECtrlAtl.cpp : CH2ECtrlAtl ��ʵ��
#include "stdafx.h"
#include "H2ECtrlAtl.h"
#include "ftd2xx.h"

/////////////////FTDI D2XX///////////////////////
int CH2ECtrlAtl::DeviceInit()
{

}
	



///////////////// Commmon Part

//************************************
// Method:    GeneralTransaction
// FullName:  CH2ECtrlAtl::GeneralTransaction
// Access:    private 
// Returns:   int
// Qualifier:
// Parameter: UINT uiTransID
// Parameter: BYTE byParam
// Parameter: LPVOID pOption
// Parameter: WORD wOptionLen
// Parameter: UINT uiMsgID
// Parameter: UINT uiMode
//************************************
int CH2ECtrlAtl::GeneralTransaction(UINT uiTransID, BYTE byParam, LPVOID pOption,  WORD wOptionLen, UINT uiMsgID, UINT uiMode)
{
	int iReturn;
	DWORD	dwError;

	dwError = 0;

	//Send command to device.
	iReturn = TransToDevice(uiTransID, byParam, pOption, wOptionLen, &dwError);

	return iReturn;
}

// 
//************************************
// Method:    ����Javascript������ָ��λ�õ�����Ԫ��ֵ
// FullName:  CH2ECtrlAtl::SetArrayNumberOfIndex
// Access:    public 
// Returns:   HRESULT
// Qualifier:
// Parameter: IDispatch * pDisp
// Parameter: int index
// Parameter: BYTE * pTemplete
//************************************
HRESULT CH2ECtrlAtl::SetArrayNumberOfIndex(IDispatch* pDisp, int index, BYTE* pTemplete)
{
	CComVariant varName(index, VT_I4);
	DISPID dispId;
	CComVariant varValue;
	HRESULT hr = 0;
	varName.ChangeType(VT_BSTR); // �������±�תΪ�����ͣ��Խ���GetIDsOfNames
	hr = pDisp->GetIDsOfNames
		(IID_NULL, &varName.bstrVal, 1, LOCALE_USER_DEFAULT, &dispId);
	if (FAILED(hr)) 
		return hr;

	DISPID dispidPut = DISPID_PROPERTYPUT;             // put����
	DISPPARAMS dispparams;
	dispparams.rgvarg = new VARIANTARG[1];                    // ��ʼ��rgvarg
	dispparams.rgvarg[0].vt = VT_UI1;                                      // ��������
	dispparams.rgvarg[0].bVal = *(pTemplete + index);                                  // ����ֵ
	dispparams.cArgs = 1;                                                       // ��������
	dispparams.cNamedArgs = 1;                                             // ��������
	dispparams.rgdispidNamedArgs = &dispidPut;         // ����DispId������������������put����

	hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams, NULL, NULL, NULL);
	return hr;
}
HRESULT CH2ECtrlAtl::SetWordArrayNumberOfIndex(IDispatch* pDisp, int index, WORD* pTemplete)
{
	CComVariant varName(index, VT_I4);
	DISPID dispId;
	CComVariant varValue;
	HRESULT hr = 0;
	varName.ChangeType(VT_BSTR); // �������±�תΪ�����ͣ��Խ���GetIDsOfNames
	hr = pDisp->GetIDsOfNames
		(IID_NULL, &varName.bstrVal, 1, LOCALE_USER_DEFAULT, &dispId);
	if (FAILED(hr)) 
		return hr;

	DISPID dispidPut = DISPID_PROPERTYPUT;             // put����
	DISPPARAMS dispparams;
	dispparams.rgvarg = new VARIANTARG[1];                    // ��ʼ��rgvarg
	dispparams.rgvarg[0].vt = VT_UI2;                                      // ��������
	dispparams.rgvarg[0].uiVal = *(pTemplete + index);                                  // ����ֵ
	dispparams.cArgs = 1;                                                       // ��������
	dispparams.cNamedArgs = 1;                                             // ��������
	dispparams.rgdispidNamedArgs = &dispidPut;         // ����DispId������������������put����

	hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT, &dispparams, NULL, NULL, NULL);
	return hr;
}
// 

//************************************
// Method:    ��ȡJavascript������ָ��λ�õ�����Ԫ��ֵ
// FullName:  CH2ECtrlAtl::GetArrayNumberOfIndex
// Access:    public 
// Returns:   HRESULT
// Qualifier:
// Parameter: IDispatch * pDisp
// Parameter: int index
// Parameter: BYTE * pTemplete
//************************************
HRESULT CH2ECtrlAtl::GetArrayNumberOfIndex(IDispatch* pDisp, int index, BYTE * pTemplete)
{
	CComVariant varName(index, VT_I4);   // �����±�
	DISPPARAMS noArgs = {NULL, NULL, 0, 0};
	DISPID dispId;
	VARIANT varValue;
	HRESULT hr = 0;       
	varName.ChangeType(VT_BSTR);         // �������±�תΪ�����ͣ��Խ���GetIDsOfNames
	//
	// ��ȡͨ���±��������Ĺ��̣���������������dispId��
	//
	hr = pDisp->GetIDsOfNames(IID_NULL, &varName.bstrVal, 1, LOCALE_USER_DEFAULT, &dispId);
	if (FAILED(hr))
		return hr;
	//
	// ����COM���̣�����ָ���±�����Ԫ�أ�����dispId ��Ԫ��ֵ������varValue��
	//
	hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET , &noArgs, &varValue, NULL, NULL);
	if (SUCCEEDED(hr))
	{
		*(pTemplete + index) = varValue.bVal;    // ������Ԫ�ذ�BYTE����ȡ��
		return hr;
	}
	else
	{
		return hr;
	}
}
HRESULT CH2ECtrlAtl::GetWordArrayNumberOfIndex(IDispatch* pDisp, int index, WORD * pTemplete)
{
	CComVariant varName(index, VT_I4);   // �����±�
	DISPPARAMS noArgs = {NULL, NULL, 0, 0};
	DISPID dispId;
	VARIANT varValue;
	HRESULT hr = 0;       
	varName.ChangeType(VT_BSTR);         // �������±�תΪ�����ͣ��Խ���GetIDsOfNames
	//
	// ��ȡͨ���±��������Ĺ��̣���������������dispId��
	//
	hr = pDisp->GetIDsOfNames(IID_NULL, &varName.bstrVal, 1, LOCALE_USER_DEFAULT, &dispId);
	if (FAILED(hr))
		return hr;
	//
	// ����COM���̣�����ָ���±�����Ԫ�أ�����dispId ��Ԫ��ֵ������varValue��
	//
	hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET , &noArgs, &varValue, NULL, NULL);
	if (SUCCEEDED(hr))
	{
		*(pTemplete + index) = varValue.uiVal;    // ������Ԫ�ذ�WORD����ȡ��
		return hr;
	}
	else
	{
		return hr;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// CH2ECtrlAtl
//////////////////////////////////////////////////////////////////////////////////////////

//************************************
// Method:    OpenCom
// FullName:  CH2ECtrlAtl::OpenCom
// Access:    public 
// Returns:   STDMETHODIMP
// Qualifier:
// Parameter: int nIdx
// Parameter: UINT uiSpeed
// Parameter: LONG * retval
//			  0������
//			  -200�����ڴ򿪴���
//			  -201�����ڹرմ���
//************************************
#if FTDI_VCP
STDMETHODIMP CH2ECtrlAtl::OpenCom(int nIdx, UINT uiSpeed, LONG* retval)
#elif FTDI_D2XX
STDMETHODIMP CH2ECtrlAtl::OpenCom(LONG* retval)
#endif
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	// TODO: �ڴ����ʵ�ִ���
	int iReturn;

	iReturn = H2E_OK;

	iReturn = Open();

	*retval = iReturn;		//�򿪴��ڳɹ�


	return S_OK;
}


//************************************
// Method:    Enroll
// FullName:  CH2ECtrlAtl::Enroll
// Access:    public 
// Returns:   STDMETHODIMP
// Qualifier:
// Parameter: WORD wBank
// Parameter: UINT nIdx
// Parameter: UINT uiTimes
// Parameter: LONG bTouchInfo
// Parameter: LONG * retval
//            0������
//            -101:�����������
//            -206:���ڴ���
//************************************
STDMETHODIMP CH2ECtrlAtl::Enroll(WORD wBank, WORD wIdx, UINT uiTimes, LONG lTouchInfo, LONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WORD wOption[2];
	BYTE byParam;
	UINT uiCounts;
	UINT uiFixTimes;
	BOOL bCheck;

	if(uiTimes > 3)
	{
		*retval = H2E_ERR_INPNUMMBER;
		return S_FALSE;		//ָ��ɨ�����Ϊ2��3��
	}
	if(lTouchInfo == H2E_OK)
	{
		bCheck = FALSE;
	}
	else if(lTouchInfo == H2E_ERR_TSPOLTOUT && uiTimes == 1)	//�ڶ���ɨ��ָ����ʱ��ʱ
	{
		bCheck = TRUE;	
	}
	else
	{
		*retval = H2E_ERR_CCOMERR;
		return S_FALSE;
	}

	*retval = H2E_OK;
	byParam = 0x84 | uiTimes;
	uiFixTimes = uiTimes + 1;
	uiCounts = 2;
	if(bCheck){
		uiCounts = 1;
		if(uiTimes > 0){
			byParam = 0xFF;
			uiFixTimes = uiTimes;
		}
	}
	wOption[0] = wBank;
	wOption[1] = wIdx;

	//Send Enroll Command.
	*retval= GeneralTransaction
		(H2E_COM_TMP_ENROLL, byParam, wOption, sizeof(wOption), MSGGUID_SEND_ENROLL, H2E_COMMENT_UOP);
	if(*retval < H2E_OK){
		return S_FALSE;
	}
	if(*retval == H2E_TAKEEND){
		//Get response from device.
		*retval = GeneralTransaction
			(H2E_COM_SENDNOP, 0x00, wOption, sizeof(wOption), MSGGUID_NODISP, H2E_COMMENT_UOP);


		//Display "releace finger message"

	}
	if(*retval < H2E_OK){
		return S_FALSE;
	}

	return S_OK;
}


//************************************
// Method:    TouchSensorState
// FullName:  CH2ECtrlAtl::TouchSensorState
// Access:    public 
// Returns:   STDMETHODIMP
// Qualifier:
// Parameter: BYTE bTouchMode
// Parameter: LONG * retval
//			  0:����
//			  -11:������������ʱ
//************************************
STDMETHODIMP CH2ECtrlAtl::TouchSensorState(BYTE bTouchMode, LONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���
	int iTimeOut;
	*retval = H2E_OK;	//����ֵ

	BYTE bTouchInfo;		//Touch������״̬

	bTouchInfo = 0;

	iTimeOut = 0;
	while(bTouchMode != bTouchInfo){
		*retval = SensingLine(&bTouchInfo);
		if(*retval == H2E_ERR_COMNOTOPEN)
		{
			break;
		}
		Sleep(100);
		iTimeOut++;
		if(iTimeOut > 50){
			*retval = H2E_ERR_TSPOLTOUT;
			break;
		}
	}

	return S_OK;
}


//************************************
// Method:    Verify1_1
// FullName:  CH2ECtrlAtl::Verify1_1
// Access:    public 
// Returns:   STDMETHODIMP
// Qualifier:
// Parameter: WORD wBank
// Parameter: UINT nIdx
// Parameter: INT * iMatchMemNo
//************************************
STDMETHODIMP CH2ECtrlAtl::Verify1_1(WORD wBank, WORD wIdx, INT* iMatchMemNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���
	int iReturn;
	WORD wOption[2];
	wOption[0] = wBank;
	wOption[1] = wIdx;
	//Send verification command(1:1)
	iReturn = GeneralTransaction
		(H2E_COM_VER1TO1, 0x80, wOption, sizeof(wOption), MSGGUID_SEND_VERIF_11, H2E_COMMENT_UOP);
	if(iReturn == H2E_OK)
	{
		*iMatchMemNo = wIdx;
	}
	else
	{
		*iMatchMemNo = H2E_ERR_VERIFY;
		return S_FALSE;
	}
	GeneralTransaction
		(H2E_COM_PASS_DRVER_CAN, 0x00, 0x0000, 2, MSGGUID_SEND_PASS_DRIVER, H2E_COMMENT_UOP);
	return S_OK;
}


STDMETHODIMP CH2ECtrlAtl::Verify1_N(WORD wBank, INT* iMatchMemNo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���
	int iReturn;
	WORD wOption;
	wOption = wBank;
	//Send verification command(1:N).
	iReturn = GeneralTransaction
		(H2E_COM_VER_G_1TON, 0x80, &wOption, sizeof(wOption), MSGGUID_SEND_VERIF_1N_GROUP, H2E_COMMENT_UOP);
	if(iReturn == H2E_OK)
	{
		*iMatchMemNo = m_wVerifTemp;
	}
	else
	{
		*iMatchMemNo = H2E_ERR_VERIFY;
		return S_FALSE;
	}
	GeneralTransaction
		(H2E_COM_PASS_DRVER_CAN, 0x00, 0x0000, 2, MSGGUID_SEND_PASS_DRIVER, H2E_COMMENT_UOP);
	return S_OK;
}

STDMETHODIMP CH2ECtrlAtl::VerifyAcrossBanks(VARIANT vAcrossBanks, WORD wTotalBankNumbers, VARIANT vMatchedBankAndTemplate, LONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���

	if(wTotalBankNumbers < 0){
		*retval = H2E_ERR_INPNUMMBER;
		return S_FALSE;
	}
	DWORD dwTotalArrayNumbers =  wTotalBankNumbers *2;

	WORD* pwAccrossBanksArray = new WORD[wTotalBankNumbers];
	for(DWORD index = 0; index < dwTotalArrayNumbers; index++)
	{
		GetWordArrayNumberOfIndex(vAcrossBanks.pdispVal, index, pwAccrossBanksArray);
	}
	*retval = GeneralTransaction
		(H2E_COM_VER_G_1TON, 0x80, pwAccrossBanksArray, (WORD)(sizeof(WORD)*wTotalBankNumbers), MSGGUID_SEND_VERIF_1N_GROUP, H2E_COMMENT_UOP);

	if(*retval == H2E_OK){
		WORD wMatchedBankAndTemplate[2];
		wMatchedBankAndTemplate[0] = m_wVerifGroup;
		wMatchedBankAndTemplate[1] = m_wVerifTemp;

		for(int index = 0; index < 2; index++)
		{
			SetWordArrayNumberOfIndex(vMatchedBankAndTemplate.pdispVal, index, wMatchedBankAndTemplate);
		}
		if(pwAccrossBanksArray){
			delete[] pwAccrossBanksArray;
		}
		GeneralTransaction
		(H2E_COM_PASS_DRVER_CAN, 0x00, 0x0000, 2, MSGGUID_SEND_PASS_DRIVER, H2E_COMMENT_UOP);
		
		return S_OK;
	}
	else{
		if(pwAccrossBanksArray){
			delete[] pwAccrossBanksArray;
		}
		return S_FALSE;
	}

}


STDMETHODIMP CH2ECtrlAtl::DeleteTemplete(WORD wBank, WORD wIdx, INT iMode, LONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���
	int iReturn;
	WORD wOption[2];

	if(iMode == 0)	//ָ��Ҫɾ����ģ��ţ�
	{
		wOption[0] = wBank;
		wOption[1] = wIdx;
		iReturn = GeneralTransaction
			(H2E_COM_TMP_DELETE, 0x00, wOption, sizeof(wOption), MSGGUID_SEND_DELTMP, H2E_COMMENT_NON);

	}
	else if (iMode == 1)	//ɾ����ǰBank ��ȫ��ģ�壻
	{
		wOption[0] = wBank;
		//Send delete group command.
		iReturn = GeneralTransaction
			(H2E_COM_TMP_G_DELETE, 0x00, &(wOption[0]), sizeof(wOption[0]), MSGGUID_SEND_DELTMP_GROUP, H2E_COMMENT_NON);

	}
	else if (iMode == 2)	//ɾ��ȫ��Bank ��ȫ��ģ�� ��
	{
		BYTE byOption = 0xC2;
		//Send initial template command.
		iReturn = GeneralTransaction
			(H2E_COM_TMP_TMP_INIT, 0x03, &byOption, 1, MSGGUID_SEND_DELTMP_ALL, H2E_COMMENT_NON);
	}
	if(iReturn != H2E_OK){
		*retval = iReturn;
		return S_FALSE;
	}
	return S_OK;
}


STDMETHODIMP CH2ECtrlAtl::GetTemplate(WORD wBank, WORD wIdx, VARIANT vTemplate, LONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���
	int iReturn;
	WORD wOption[2];
	BYTE		byTemplate[1080];
	PTEMPU pTempu = new TEMPU;

	wOption[0] = wBank;
	wOption[1] = wIdx;

	//Send Read Template Command.
	iReturn = GeneralTransaction
		(H2E_COM_READ_TMP, 0x00, wOption, sizeof(wOption), MSGGUID_SEND_READ_TMP, H2E_COMMENT_NON);
	if(iReturn != H2E_OK){
		*retval = iReturn;
		return S_FALSE;
	}
	GetTempU(pTempu);
	memmove(byTemplate, pTempu->byTemplate, 1072);

	byTemplate[1072] = (BYTE)(pTempu->dwAddSum & 0x000000FF);
	byTemplate[1073] = (BYTE)((pTempu->dwAddSum & 0x0000FF00) >> 8);
	byTemplate[1074] = (BYTE)((pTempu->dwAddSum & 0x00FF0000) >> 16);
	byTemplate[1075] = (BYTE)((pTempu->dwAddSum & 0xFF000000) >> 24);

	byTemplate[1076] = (BYTE)(pTempu->dwXorSum & 0x000000FF);
	byTemplate[1077] = (BYTE)((pTempu->dwXorSum & 0x0000FF00) >> 8);
	byTemplate[1078] = (BYTE)((pTempu->dwXorSum & 0x00FF0000) >> 16);
	byTemplate[1079] = (BYTE)((pTempu->dwXorSum & 0xFF000000) >> 24);

	//����ģ�嵽������
	for(int index = 0; index < 1080; index++)
	{
		SetArrayNumberOfIndex(vTemplate.pdispVal, index, byTemplate);
	}
	if(pTempu){
		delete[] pTempu;
	}

	return S_OK;
}


STDMETHODIMP CH2ECtrlAtl::SetTemplate(WORD wBank, WORD wIdx, VARIANT vTemplate, LONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: �ڴ����ʵ�ִ���
	int iReturn;
	WORD wOption[2];
	BYTE byTemplate[1080];	// ģ��
	PTEMPU pTempU = new TEMPU;
	DWORD dwAddSum,dwXorSum;
	wOption[0] = wBank;
	wOption[1] = wIdx;

	memset( byTemplate, 0, sizeof(byTemplate) ) ;

	//��ô����ģ��
	for(int index = 0; index < 1080; index++)
	{
		GetArrayNumberOfIndex(vTemplate.pdispVal, index, byTemplate);
	}

	memmove(pTempU->byTemplate, byTemplate, 1072);

	dwAddSum = 			
		(byTemplate[1075] << 24) |				
		(byTemplate[1074] << 16) | 				
		(byTemplate[1073] << 8) | 		
		byTemplate[1072];

	dwXorSum = 
		(byTemplate[1079] << 24) |
		(byTemplate[1078] << 16) | 
		(byTemplate[1077] << 8) | 
		byTemplate[1076];	

	pTempU->dwAddSum = dwAddSum;
	pTempU->dwXorSum = dwXorSum;
	pTempU->uiGroupNum = wBank;
	pTempU->uiTmpNum = wIdx;
	pTempU->uiQLevel = 0xff;

	//Send delete template command.
	iReturn = GeneralTransaction
		(H2E_COM_TMP_DELETE, 0x00, wOption, sizeof(wOption), MSGGUID_SEND_DELTMP, H2E_COMMENT_NON);
	if(iReturn != H2E_OK){
		*retval = iReturn;
		return S_FALSE;
	}


	//Send write template command
	iReturn = GeneralTransaction
		(H2E_COM_WRITE_TMPC2, 0x00, pTempU, sizeof(TEMPU), MSGGUID_SEND_WRITE_TMP, H2E_COMMENT_NON);
	if(iReturn != H2E_OK){
		*retval = iReturn;
		return S_FALSE;
	}

	if(pTempU){
		delete[] pTempU;
	}

	*retval = iReturn;
	return S_OK;
}

