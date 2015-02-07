// test.cpp : Ctest 的实现
#include "stdafx.h"
#include "test.h"


// Ctest


STDMETHODIMP Ctest::OpenCom(int nIdx)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加实现代码

	return S_OK;
}


STDMETHODIMP Ctest::OpenCom2(int nIdx, UINT uiSpeed, ULONG* retval)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*retval = 1;
	// TODO: 在此添加实现代码

	return S_OK;
}
