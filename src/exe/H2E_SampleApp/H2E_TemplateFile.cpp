#include "stdafx.h"
#include "H2E_TemplateFile.h"
#include <direct.h>


CTemplateFile::CTemplateFile(void)
{
	//Initialize member's variables.

	m_pTPFile = new CFile;								//create CFile objec.t
	m_pTPFex = new CFileException;						//ctreate CFile exeption
	m_uiTotalTemplates = 0;								//total templates counter

	m_pTempList = new TEMPLIST[MAX_TEMPLATE_TOTAL];		//create template infomation manage table
	MakeTemplateDirectory();							//making template folder.


}

CTemplateFile::~CTemplateFile(void)
{
	if(m_pTPFex){
		delete m_pTPFex;
	}
	if(m_pTPFile)	{
		delete m_pTPFile;
	}

	if(m_pTempList){
		delete[] m_pTempList;
	}


}

//
//initialaize template infomation manage table
//
int CTemplateFile::InitialTemplateLists(PTEMPINFO pTempInfo)
{
	int iReturn;
	iReturn = H2E_OK;
	DWORD dw;;

	if(pTempInfo->dwTemplates == 0){
		ZeroMemory(m_pTempList, sizeof(TEMPLIST)*MAX_TEMPLATE_TOTAL);
	}
	else{
		if(pTempInfo->pTempList){
			if(pTempInfo->pTempList != m_pTempList){
				for(dw=0; dw<pTempInfo->dwTemplates; dw++){
					m_pTempList[dw] = pTempInfo->pTempList[dw];
				}
			}
		}
	}
	m_uiTotalTemplates = pTempInfo->dwTemplates;
	return iReturn;
}

//
//get template infomation manage table 
//
int CTemplateFile::GetTemplateLists(LPDWORD pdwTemplates, PTEMPLIST* pTempList)
{
	int iReturn;

	iReturn = H2E_OK;

	*pTempList = m_pTempList;
	*pdwTemplates = m_uiTotalTemplates;

	return iReturn;
}

//
//add template infomation to manage table
//
int CTemplateFile::AddTemplateList(int iBankNum, PTEMPU pTempU)
{
	int iReturn;
	UINT ui;

	iReturn = H2E_OK;
	//check the existence of the template
	for(ui=0; ui<m_uiTotalTemplates; ui++){
		if((m_pTempList[ui].uiGroupNum == iBankNum )
			&& (m_pTempList[ui].uiTempNum == pTempU->uiTmpNum)){
			//alreadytemplate exisit. update template infomation
			m_pTempList[ui].uiQLevel = pTempU->uiQLevel;	//Quality Level
			m_pTempList[ui].time = pTempU->time;			//Time
			break;
		}
	}
	if(ui>=m_uiTotalTemplates){
		//The template does not exist, insert template infomation
		if(m_uiTotalTemplates<MAX_TEMPLATE_TOTAL){
			m_pTempList[m_uiTotalTemplates].uiGroupNum = iBankNum;
			m_pTempList[m_uiTotalTemplates].uiTempNum = pTempU->uiTmpNum;
			m_pTempList[m_uiTotalTemplates].uiQLevel= pTempU->uiQLevel;
			m_pTempList[m_uiTotalTemplates].time = pTempU->time;
			m_uiTotalTemplates++;
		}
	}
	return iReturn;
}

//
//delete one of template infomation form manage table
//
int CTemplateFile::DeleteOneTemplateList(UINT uiGroup, UINT uiTemp)
{
	int iReturn;
	iReturn = H2E_OK;
	UINT uiCondens;
	UINT ui;
	UINT ui2;

	if(m_uiTotalTemplates > 0){
		//check the existence of the template
		for(ui=0; ui<m_uiTotalTemplates; ui++){
			if((m_pTempList[ui].uiGroupNum == uiGroup)
				&&(m_pTempList[ui].uiTempNum == uiTemp)){
					m_uiTotalTemplates;
					//template exist
					break;
			}
		}
		if(m_uiTotalTemplates > ui){
			//condens template infomation table
			uiCondens = m_uiTotalTemplates - ui - 1 ;	
			for(ui2=0; ui2<uiCondens; ui2++){
				m_pTempList[ui] = m_pTempList[ui+1];
				ui++;
			}
		}
		m_uiTotalTemplates--;
	}
	return iReturn;
}

//
//delete Group  form manage table
//
int CTemplateFile::DeleteGroup(UINT uiGroup)
{
	int iReturn;
	iReturn = H2E_OK;
	UINT ui;

	for(ui=0; ui<m_uiTotalTemplates; ){
		if(m_pTempList[ui].uiGroupNum == uiGroup){
			DeleteOneTemplateList(uiGroup, m_pTempList[ui].uiTempNum);
		}
		else{
			ui++;
		}
	}
	return iReturn;
}

//
// make "template" folder
//
int CTemplateFile::MakeTemplateDirectory()
{
	int iReturn;
	iReturn	= H2E_OK;
	TCHAR tcExePath[MAX_PATH];
	int	iOffset;

	CWinApp* pApp = AfxGetApp();
	HMODULE hModule = GetModuleHandle(pApp->m_pszExeName);
	GetModuleFileName(hModule, tcExePath, sizeof(tcExePath));
	m_strTenplatePath = tcExePath;
	iOffset = m_strTenplatePath.ReverseFind(_T('\\'));
	m_strTenplatePath.Delete(iOffset, MAX_PATH);
	m_strTenplatePath += _T("\\template");
	_tmkdir(m_strTenplatePath);

	return iReturn;
}

//
// get open and save template file name
//
CString CTemplateFile::GetFileName(BOOL bMode)
{
	CString strFilename;
	SYSTEMTIME st;
	GetLocalTime(&st);

	CString strDefaultName;
	if(bMode){
		//strDefaultName = m_strTenplatePath + _T("\\");			//\¨XP‚ÅŠJ‚©‚È‚¢ 2012.02.15
		strDefaultName = m_strTenplatePath + _T("\\*.tp2");			//*.tp2‚Æ‚·‚é 2012.02.15
	}
	else{
		strDefaultName.Format(_T("%s\\H2E%04d%02d%02d_%02d%02d.tp2"), 
							m_strTenplatePath, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	}
	strFilename.Empty();
	//Open comon open & save dialog
	CFileDialog dlg(bMode, _T("tp2"), strDefaultName, OFN_HIDEREADONLY | OFN_CREATEPROMPT,
		_T("H2E Template files (*.tp2)|*.tp2|All Files (*.*)|*.*||"));

	if(dlg.DoModal() == IDOK){
		strFilename = dlg.GetPathName();
	}

    
    return strFilename;

}

//
// Get template file size
//
int CTemplateFile::GetFileSize(CString strFilePath, ULONGLONG* pullFileSize)
{
	int iReturn;

	iReturn = H2E_OK;

	iReturn = Open(0, strFilePath);
	if(iReturn != H2E_OK){
		return iReturn;
	}
	*pullFileSize = m_pTPFile->GetLength();
	Close();
	return iReturn;
}

//
//Open template file.
//
int CTemplateFile::Open(int iOpenModeint, CString serFileName)
{
	int iReturn;
	iReturn	= H2E_OK;
	UINT uiMode = CFile::modeReadWrite;
	if(iOpenModeint){
		uiMode |= CFile::modeCreate;
	}

	if(!m_pTPFile->Open(serFileName, uiMode, m_pTPFex)){
		return H2E_ERR_TEMPFILE_OPEN;
	}
	return iReturn;
}

//
//Read template file.
//
int CTemplateFile::Read(PTEMPU pTempU, UINT uiTemps)
{
	int iReturn;
	iReturn	= H2E_OK;
	UINT uiCount;
	UINT uiReadCount;

	uiReadCount = uiTemps*sizeof(TEMPU);
	uiCount = m_pTPFile->Read(pTempU, uiReadCount);
	if((uiCount == 0) || (uiCount < sizeof(TEMPU))){
		iReturn = H2E_ERR_TEMPFILE_NON;
	}
	return iReturn;
}

//
//Write template file
//
int CTemplateFile::Write(PTEMPU pTempU, UINT uiTemps)
{
	int iReturn;
	iReturn	= H2E_OK;
	UINT uiWriteCount;

	uiWriteCount = uiTemps*sizeof(TEMPU);
	if(uiWriteCount>0){
		m_pTPFile->Write(pTempU, uiWriteCount);
	}
	return iReturn;
}

//
//Close template file
//
int CTemplateFile::Close()
{
	int iReturn;
	iReturn	= H2E_OK;
	if(m_pTPFile){
		m_pTPFile->Flush();
		m_pTPFile->Close();
	}
	return iReturn;
}

