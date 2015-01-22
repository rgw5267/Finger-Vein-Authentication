#pragma once
#include "afx.h"
#include "H2E.h"

class CTemplateFile :
	public CObject
{
public:
	CTemplateFile(void);
	virtual ~CTemplateFile(void);

	int InitialTemplateLists(PTEMPINFO pTempInfo);
	int Open(int iOPenModeint, CString serFileName);
	int Read(PTEMPU pTempU, UINT uiTemps);
	int Write(PTEMPU pTempU, UINT uiTemps);
	int Close();
	CString GetFileName(int iMode);
	int GetFileSize(CString strFilePath, ULONGLONG* pullFileSize);
	int AddTemplateList(int iBankNum, PTEMPU pTempU);
	int GetTemplateLists(LPDWORD pdwTemplates, PTEMPLIST* pTempList);
	int DeleteOneTemplateList(UINT uiGroup, UINT uiTemp);
	int DeleteGroup(UINT uiGroup);

	PTEMPLIST m_pTempList;
	UINT m_uiBanks;									
	UINT m_uiTotalTemplates;						
	CString m_strTenplatePath;						

private:
	int MakeTemplateDirectory();
	CFile* m_pTPFile;
	CFileException* m_pTPFex;


};