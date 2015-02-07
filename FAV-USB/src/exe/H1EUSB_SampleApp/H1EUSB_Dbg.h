/********************************************************************************************/
/* AL2P ATFRAME Local Link Protocol                                                         */
/*------------------------------------------------------------------------------------------*/
/* A2GObj.dll A2GObject Application Interface												*/
/*------------------------------------------------------------------------------------------*/
/*  Copyright (C) Office M&M 2008 Co,Ltd. ALL rights reserved.	                            */
/********************************************************************************************/
#pragma once
//#include "AL2P.h"
#include <shlwapi.h>
/********************************************************************************************/
/* AL2P Debug Header																		*/
/*------------------------------------------------------------------------------------------*/
/*[çÏê¨]		<çÏê¨é“><çÏê¨ì˙><ÉoÅ[ÉWÉáÉìÅ]ÉäÉrÉWÉáÉì>									*/
/*				<îíåÀ><20080110>Å@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[èCê≥óàó]	<èCê≥é“><èCê≥ì˙><ÉoÅ[ÉWÉáÉìÅ]ÉäÉrÉWÉáÉì><è·äQä«óùî‘çÜ><èCê≥ì‡óe>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/*  Copyright (C) Office M&M 2008 Co,Ltd. ALL rights reserved.	                            */
/********************************************************************************************/
#define A2G_DEBUG

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)

#define A2GOBJ_DEBUG
inline void A2GDebugString(LPCTSTR pDebug)
{
#ifdef A2GOBJ_DEBUG
	OutputDebugString(pDebug);
#endif
}

inline void FormatLogOut(LPCTSTR format, ...)
{
#ifdef A2GOBJ_DEBUG
	va_list argptr;
	va_start(argptr, format);

	DWORD bufLen = _vsctprintf(format, argptr) + 1;
	LPTSTR pBuf = new TCHAR[bufLen];
	int ret = _vsntprintf_s(pBuf, bufLen, bufLen-1, format, argptr);
	OutputDebugString(pBuf);
	delete[] pBuf;
	va_end(argptr);
#endif

}


inline void AL2PDbgString(LPCTSTR format, ...)
{
#ifdef A2G_DEBUG
	va_list argptr;
	va_start(argptr, format);
	DWORD bufLen = _vsctprintf(format, argptr) + 16;
	LPTSTR pBuf = new TCHAR[bufLen];
	int ret = _vsntprintf_s(pBuf, bufLen, bufLen-1, format, argptr);
	if(ret >0){
		OutputDebugString(pBuf);
	}
	delete[] pBuf;
	va_end(argptr);
#endif
}

#if defined(A2G_DEBUG)
#define A2GDBG_OUTSTRING(fmt, ...) \
	AL2PDbgString(_T("DBG>>%s(); L=#%d "),__WFUNCTION__,__LINE__); \
	AL2PDbgString(fmt, __VA_ARGS__); \
	OutputDebugString(_T(":\n")); \
	OutputDebugString(_T(":\n"))
#else
#define A2GDBG_OUTSTRING(fmt, ...)
#endif

/*
#if defined(A2G_DEBUG)
#define A2GDBG_OUTSTRING(fmt, ...) \
	AL2PDbgString(_T("DBG>>%s L=%d %s()\n"),__WFILE__,__LINE__,__WFUNCTION__); \
	AL2PDbgString(fmt, __VA_ARGS__)
#else
#define A2GDBG_OUTSTRING(fmt, ...)
#endif
*/
