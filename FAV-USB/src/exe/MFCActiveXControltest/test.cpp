// test.cpp : Ctest ��ʵ��
#include "stdafx.h"
#include "test.h"


// Ctest


STDMETHODIMP Ctest::OpenCom(int nIdx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ����ʵ�ִ���

	return S_OK;
}


STDMETHODIMP Ctest::OpenCom2(int nIdx, UINT uiSpeed, ULONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retval = 1;
	// TODO: �ڴ����ʵ�ִ���

	return S_OK;
}
