#include "stdafx.h"
#include "H1EUSB_TemplateFile.h"
#include <direct.h>
/********************************************************************************************/
/* CTemplateFile�N���X�v���O����															*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20120110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/********************************************************************************************/
/********************************************************************************************/
/*[���\�b�h��] CTemplateFile�N���X�̃R���X�g���N�^											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CA2GObjThread(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�R���X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CTemplateFile::CTemplateFile(void)
{
	//�����o�[�ϐ���������
	m_pTPFile = new CFile;
	m_pTPFex = new CFileException;

	//�e���v���[�g���X�g���m�ۂ���B
	m_pTempList = new TEMPLIST[MAX_TEMPLATE_TOTAL];
	//�e���v���[�g�t�@�C���̊i�[�t�H���_�[���쐬����
	MakeTemplateDirectory();
	//�Ǘ��e�[�u�����쐬����
	MakeMangeTable();

}


/********************************************************************************************/
/*[���\�b�h��] CTemplateFile�N���X�̃f�X�g���N�^											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		~CTemplateFile(void)														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�f�X�g���N�^����															*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�Ȃ�																		*/
/********************************************************************************************/
CTemplateFile::~CTemplateFile(void)
{
	
	if(m_pTPFex){
		delete m_pTPFex;
	}
	if(m_pTPFile)	{
		delete m_pTPFile;
	}

	if(m_pBankInfo){
		delete[] m_pBankInfo;
	}
	if(m_pTempList){
		delete[] m_pTempList;
	}

}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�t�@�C���I�[�v��													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int Open(int iBankNum)														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g�t�@�C�����I�[�v������											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::Open(int iBankNum)
{
	int iReturn;
	iReturn	= H1USB_OK;

	CString strTPFName;

	//�I�[�v������p�X���쐬
	strTPFName.Format(_T("%s\\BANK%04X.tpf"), m_strTenplatePath, iBankNum);

	//�t�@�C���I�[�v��
	if(!m_pTPFile->Open(strTPFName, CFile::modeReadWrite, m_pTPFex)){
		//�t�@�C���̃I�[�v���Ɏ��s����
		return H1USB_ERR_TEMPFILE_OPEN;
	}
	return iReturn;
}


/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�t�@�C�����[�h													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int Read(PTEMPU pTempU, UINT uiTemps)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�w�肳�ꂽ�����e���v���[�g�������[�h����								*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::Read(PTEMPU pTempU, UINT uiTemps)
{
	int iReturn;
	iReturn	= H1USB_OK;
	UINT uiCount;
	UINT uiReadCount;

	uiReadCount = uiTemps*sizeof(TEMPU);
	uiCount = m_pTPFile->Read(pTempU, uiReadCount);
	if((uiCount == 0) || (uiCount < sizeof(TEMPU))){
		//�ǂݍ��񂾃f�[�^���ɐ��������Ȃ�
		iReturn = H1USB_ERR_TEMPFILE_NON;
	}
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�t�@�C�����C�g													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int Write(PTEMPU pTempU, UINT uiTemps)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�w�肳�ꂽ�����e���v���[�g�������C�g����								*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::Write(PTEMPU pTempU, UINT uiTemps)
{
	int iReturn;
	iReturn	= H1USB_OK;
	UINT uiWriteCount;

	uiWriteCount = uiTemps*sizeof(TEMPU);
	if(uiWriteCount>0){
		m_pTPFile->Write(pTempU, uiWriteCount);
	}
	return iReturn;
}


/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�t�@�C�����N���[�Y												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int Close(PTEMPU pTempU, UINT uiTemps)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g�t�@�C�����N���[�Y����											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::Close()
{
	int iReturn;
	iReturn	= H1USB_OK;
	if(m_pTPFile){
		//�t���b�V�����Ă���N���[�Y����
		m_pTPFile->Flush();
		m_pTPFile->Close();
	}
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g��ǉ�����														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int AddTemplate(int iBankNum, PTEMPU pTempU)								*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�w�肳�ꂽ�o���N�Ƀe���v���[�g����ǉ�����								*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::AddTemplate(int iBankNum, PTEMPU pTempU)
{
	int iReturn;
	iReturn	= H1USB_OK;
	TEMPU tempu;

	CString strTPFName;
	//�I�[�v������e���v���[�g�t�@�C���̃p�X���쐬����
	strTPFName.Format(_T("%s\\BANK%04X.tpf"), m_strTenplatePath, iBankNum);
	CFile cf;
	CFileException ex;

	//�t�@�C���I�[�v��
	if(!cf.Open(strTPFName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite, &ex)){
		//�e���v���[�g�t�@�C���I�[�v�����s
		return H1USB_ERR_TEMPFILE_OPEN;
	}

	ULONGLONG ull;
	LONGLONG ll;

	ull = cf.GetLength();
	while(ull > 0){
		cf.Read(&tempu, sizeof(TEMPU));
		if(tempu.uiTmpNum == pTempU->uiTmpNum){
			//���ɓo�^���ꂽ�e���v���[�g�ԍ������݂����ꍇ�͏㏑��
			ll = 0 - (LONGLONG)sizeof(TEMPU);
			//�t�@�C���|�C���^����̃e���v���[�g��񕪖߂�
			cf.Seek(ll, CFile::current);
			break;
		}
		ull -= sizeof(TEMPU);
	}
	//���C�g����
	cf.Write(LPVOID(pTempU), sizeof(TEMPU));

	//�N���[�Y����
	cf.Close();

	//�e���v���[�g�Ǘ��e�[�u����ҏW����
	iReturn = AddTemplateList(iBankNum, pTempU);
	
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g���擾����														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int GetTemplate(int iBankNum, int iTempNum, PTEMPU pTempU)					*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�w�肳�ꂽ�o���N�ԍ��A�e���v���[�g�ԍ��̃e���v���[�g�t�@�C�����擾����		*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::GetTemplate(int iBankNum, int iTempNum, PTEMPU pTempU)
{
	int iReturn;
	iReturn	= H1USB_OK;
	TEMPU tempU;

	//�w�肳�ꂽ�o���N�̃e���v���[�g�t�@�C�����I�[�v������
	iReturn = Open(iBankNum);
	if(iReturn==H1USB_OK){
		while(1){
			//�e���v���[�g��������[�h����
			iReturn = Read(&tempU, 1);
			if(iReturn != H1USB_OK){
				break;				
			}
			if(tempU.uiTmpNum == iTempNum){
				//�w�肳�ꂽ�e���v���[�g�ԍ��𔭌�����
				*pTempU = tempU;
				break;
			}

		}
	}
	//�t�@�C�����N���[�Y����
	Close();
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�R�s�[															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int CopyBank0000(int iCount)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		0x0000�o���N�̃e���v���[�g���w�肳�ꂽ�o���N���R�s�[����					*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::CopyBank0000(int iCount)
{
	int iReturn;
	int i;
	iReturn = H1USB_OK;

	//�o���N0000h�ȊO�̃t�@�C�����폜����
	CString strFind;
	strFind = m_strTenplatePath + _T("\\BANK*.tpf");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strFind, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE){
		return -1;

	}

	CString strWork;
	CString strDelFile;
	do{
		strWork = FindFileData.cFileName;
		strWork.MakeUpper();
		if(strWork != _T("BANK0000.TPF")){			
			strDelFile = m_strTenplatePath + _T("\\") + strWork;
			DeleteFile(strDelFile);
			DoEvents();
		}
	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);

	//�w������t�@�C�����R�s�[����
	CString strSrcFileName;
	CString strDstFileName;
	BOOL bReturn;

	strSrcFileName = m_strTenplatePath + _T("\\BANK0000.tpf");
	for(i=0; i<iCount; i++){
		strDstFileName.Format(_T("%s\\BANK%04X.tpf"), m_strTenplatePath, (i+1));
		bReturn = CopyFile(strSrcFileName, strDstFileName, FALSE);
		if(!bReturn){
			iReturn = H1USB_ERR_TEMPFILE_COPY;
			break;
		}
		DoEvents();
	}
	if(iReturn == H1USB_OK){
		iReturn = MakeMangeTable();
	}

	return iReturn;
}
/********************************************************************************************/
/*[���\�b�h��]	���b�Z�[�W�D����s															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		void DoEvents()																*/
/*																							*/
/*[���e]		�L���[�ɗ��܂��Ă��郁�b�Z�[�W��D��I�Ɏ��s������							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
void CTemplateFile::DoEvents()
{
	MSG msg;
	while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)){
		if (!AfxGetThread()->PumpMessage()){
			return;
		}
	}
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g���폜															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int DeleteOneTemplate(int iBankNum, int iTempNum)							*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		��̃e���v���[�g�����폜����											*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::DeleteOneTemplate(int iBankNum, int iTempNum)
{
	int iReturn;
	ULONGLONG ullLength;
	PTEMPU pTempU;	
	UINT uiTemps;
	TEMPU deleteTempU;

	iReturn = H1USB_OK;

	//�w�肳�ꂽ�o���N�̃e���v���[�g�t�@�C�����I�[�v��
	iReturn = Open(iBankNum);
	if(iReturn != H1USB_OK){
		return iReturn;
	}
	UINT ui;
	ullLength = m_pTPFile->GetLength();
	//�o�^�ς݂̃e���v���[�g�����t�@�C���T�C�Y���Z�o����
	uiTemps  = (UINT)ullLength/sizeof(TEMPU);
	pTempU = 0;
	if(uiTemps > 0){
		pTempU = new TEMPU[uiTemps];
		//�S�Ẵe���v���[�g�����[�h����
		iReturn = Read(pTempU, uiTemps);
		if(iReturn == H1USB_OK){
			for(ui=0; ui<uiTemps; ui++){
				if(pTempU[ui].uiTmpNum == iTempNum){
					deleteTempU = pTempU[ui];
					//�w�肳�ꂽ�e���v���[�g�𔭌�����
					//delTempOne(pTempU, uiTemps-ui);				//�w��̃e���v���[�g���폜���ăR���f���X����
					delTempOne(&pTempU[ui], uiTemps-ui);				//�w��̃e���v���[�g���폜���ăR���f���X����
					m_pTPFile->SetLength(0);
					iReturn = Write(pTempU, uiTemps - 1);		
					break;
				}
			}
		
		}
	}
	//�e���v���[�g�t�@�C�����N���[�Y����
	Close();
	if(pTempU){
		delete[] pTempU;
	}
	if(uiTemps == 1){
		DeleteBank(iBankNum);
	}

	if(iReturn == H1USB_OK){
		//�Ǘ��e�[�u����ҏW����
		iReturn =  DeleteTemplateList(iBankNum, &deleteTempU);
	}

	return iReturn;
}


/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�Ǘ����폜														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int delTempOne(PTEMPU pTempU, UINT uiTemps)									*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		��̃e���v���[�g�Ǘ������폜���R���f���X����							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
void CTemplateFile::delTempOne(PTEMPU pTempU, UINT uiTemps)
{
	UINT uiCondens;
	UINT ui;
	if(uiTemps < 1){
		return;
	}

	uiCondens = uiTemps -1 ;			//�R���f���X����񐔂��Z�b�g
	for(ui=0; ui<uiCondens; ui++){
		pTempU[ui] = pTempU[ui+1];
	}
}


/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�t�@�C���폜														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int DeleteBank(int iBankNum)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�w�肳�ꂽ�o���N�̃e���v���[�g�t�@�C�����폜����							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::DeleteBank(int iBankNum)
{
	int iReturn;
	BOOL bReturn;
	iReturn = H1USB_OK;

	CString strDelFile;
	//�폜����e���v���[�g�t�@�C���̃p�X���쐬����
	strDelFile.Format(_T("%s\\BANK%04X.tpf"), m_strTenplatePath, iBankNum);
	bReturn = DeleteFile(strDelFile);
	if(bReturn){
		//�Ǘ��e�[�u����ҏW����
		iReturn =  DeleteTemplateList(iBankNum, NULL);
	}
	else{
		iReturn = H1USB_ERR_TEMPFILE_DEL;
	}

	return iReturn;
}


/********************************************************************************************/
/*[���\�b�h��] �S�e���v���[�g�t�@�C���폜													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int DeleteAll()																*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�S�Ẵo���N�̃e���v���[�g�t�@�C�����폜����								*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::DeleteAll()
{
	int iReturn;
	iReturn = H1USB_OK;

	CString strFind;
	strFind = m_strTenplatePath + _T("\\BANK*.tpf");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	//�e���v���[�g�t�@�C�����������Ȃ���폜����
	hFind = FindFirstFile(strFind, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE){
		return -1;

	}

	CString strWork;
	CString strDelFile;
	do{
		strWork = FindFileData.cFileName;
		strWork.MakeUpper();
		strDelFile = m_strTenplatePath + _T("\\") + strWork;
		//���������e���v���[�g�t�@�C�����폜
		DeleteFile(strDelFile);
	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);

	//�Ǘ��e�[�u����ҏW����
	iReturn =  DeleteTemplateList(-1, NULL);

	return iReturn;

}

/********************************************************************************************/
/*[���\�b�h��] �S�e���v���[�g�t�@�C���R�s�[													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		intCopyAllTempFiles(CString strSrcDir, CString strDstDir)					*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�S�Ẵe���v���[�g�t�@�C�����w�肳�ꂽ�t�H���_�[��							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::CopyAllTempFiles(CString strSrcDir, CString strDstDir)
{
	int iReturn;
	iReturn = H1USB_OK;

	CString strFind;
	strFind = strSrcDir + _T("\\BANK*.tpf");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strFind, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE){
		return -1;

	}

	CString strWork;
	CString strSrcFile;
	CString strDstFile;
	BOOL bReturn;
	do{
		strWork = FindFileData.cFileName;
		strSrcFile = strSrcDir + _T("\\") + FindFileData.cFileName;
		strDstFile = strDstDir + _T("\\") + FindFileData.cFileName;
		bReturn = CopyFile(strSrcFile, strDstFile, FALSE);
		if(!bReturn){
			iReturn = H1USB_ERR_TEMPFILE_COPY;
			break;
		}

	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);

	return iReturn;
}




/********************************************************************************************/
/*[���\�b�h��] �S�e���v���[�g�t�@�C���ۑ��[													*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int SaveTemplates(HWND hWnd)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�t�H���_�[�I����ʂ�\�����đS�Ẵe���v���[�g�t�@�C�����R�s�[����			*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::SaveTemplates(HWND hWnd)
{
	int iReturn;
	CString strDirectory;

	iReturn = H1USB_OK;
	strDirectory = GetDirectoryName(hWnd);
	if(!strDirectory.IsEmpty()){
		iReturn = CopyAllTempFiles(m_strTenplatePath, strDirectory);

	}

	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �S�e���v���[�g�t�@�C���I�[�v��												*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int OpenTemplates(HWND hWnd)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�ۑ����ꂽ�e���v���[�g�t�@�C������ƃG���A���ɃR�s�[����					*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::OpenTemplates(HWND hWnd)
{
	int iReturn;
	CString strDirectory;

	iReturn = H1USB_OK;
	strDirectory = GetDirectoryName(hWnd);
	if(!strDirectory.IsEmpty()){
		iReturn = CopyAllTempFiles(strDirectory, m_strTenplatePath);
	}

	if(iReturn == H1USB_OK){
		//�Ǘ��e�[�u�����쐬����
		iReturn = MakeMangeTable();
	}

	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �t�H���_�[�擾																*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		CString GetDirectoryName(HWND hWnd)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�V�F���̃_�C�A���O��\�����A�I�����ꂽ�t�H���_�[���擾����					*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		�t�H���_�[�̐�΃p�X														*/
/********************************************************************************************/
CString CTemplateFile::GetDirectoryName(HWND hWnd)
{
	CString strPath;
	CString strWork;


	 BROWSEINFO      bi;
    LPSTR           lpBuffer;
    LPITEMIDLIST    pidlRoot;      // �u���E�Y�̃��[�gPIDL
    LPITEMIDLIST    pidlBrowse;    // ���[�U�[���I������PIDL
    LPMALLOC        lpMalloc = NULL;

	strPath.Empty();
    HRESULT hr = SHGetMalloc(&lpMalloc);
    if(FAILED(hr)) return strPath;

    // �u���E�Y���󂯎��o�b�t�@�̈�̊m��
    if ((lpBuffer = (LPSTR) lpMalloc->Alloc(_MAX_PATH)) == NULL) {
        return strPath;
    }
	
    // �_�C�A���O�\�����̃��[�g�t�H���_��PIDL���擾
    // ���ȉ��̓f�X�N�g�b�v�����[�g�Ƃ��Ă���B�f�X�N�g�b�v�����[�g�Ƃ���
    //   �ꍇ�́A�P�� bi.pidlRoot �ɂO��ݒ肷�邾���ł��悢�B���̑��̓�
    //   ��t�H���_�����[�g�Ƃ��鎖���ł���B�ڍׂ�SHGetSpecialFolderLoca
    //   tion�̃w���v���Q�Ƃ̎��B
	HRESULT hResult;

	hResult = SHGetSpecialFolderLocation(hWnd, CSIDL_DRIVES, &pidlRoot);
	if (FAILED(hResult)){
		lpMalloc->Free(lpBuffer);
        return strPath;
    }
    // BROWSEINFO�\���̂̏����l�ݒ�
    // ��BROWSEINFO�\���̂̊e�����o�̏ڍא������w���v���Q��
    bi.hwndOwner = hWnd;
    bi.pidlRoot = pidlRoot;
    bi.pszDisplayName = lpBuffer;
    bi.lpszTitle = "Select Folder";
    bi.ulFlags = 0;
    bi.lpfn = 0;
    bi.lParam = 0;
    // �t�H���_�I���_�C�A���O�̕\�� 
    pidlBrowse = SHBrowseForFolder(&bi);
    if (pidlBrowse != NULL) {  
        // PIDL�`���̖߂�l�̃t�@�C���V�X�e���̃p�X�ɕϊ�
        if (SHGetPathFromIDList(pidlBrowse, lpBuffer)) {
            // �擾����
			strPath = lpBuffer;
			strWork = strPath.Right(1);
			if(strWork == _T("\\")){
				strPath.Delete(strPath.GetLength()-1, 1);
			}
        }
        // SHBrowseForFolder�̖߂�lPIDL�����
        lpMalloc->Free(pidlBrowse);
    }
    // �N���[���A�b�v����
    lpMalloc->Free(pidlRoot); 
    lpMalloc->Free(lpBuffer);
    lpMalloc->Release();

	return strPath;

}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�t�@�C���t�H���_�[�̍쐬											*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int MakeTemplateDirectory()													*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g���i�[�����Ɨp�̃t�H���_�[���쐬����							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::MakeTemplateDirectory()
{
	int iReturn;
	iReturn	= H1USB_OK;
	TCHAR tcExePath[MAX_PATH];
	int	iOffset;

	CWinApp* pApp = AfxGetApp();
	HMODULE hModule = GetModuleHandle(pApp->m_pszExeName);
	//���̃��W���[���̏��݂̐�΃p�X���擾����
	GetModuleFileName(hModule, tcExePath, sizeof(tcExePath));
	m_strTenplatePath = tcExePath;
	iOffset = m_strTenplatePath.ReverseFind(_T('\\'));
	m_strTenplatePath.Delete(iOffset, MAX_PATH);
	//���̃��W���[���̔z���ɍ�Ɨp�t�H���_�[���쐬����
	m_strTenplatePath += _T("\\template");
	_tmkdir(m_strTenplatePath);

	return iReturn;
}



/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�Ǘ����Ƀe���v���[�g����ǉ�����								*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int AddTemplateList(int iBankNum, PTEMPU pTempU)							*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g�Ǘ����Ƀe���v���[�g����ǉ����A�o���N�Ǘ������X�V����	*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::AddTemplateList(int iBankNum, PTEMPU pTempU)
{
	int iReturn;
	UINT ui;

	iReturn = H1USB_OK;
	//���݂̃��X�g�Ɏw�肳�ꂽ���X�g�����݂��邩���`�F�b�N����
	for(ui=0; ui<m_uiTotalTemplates; ui++){
		if((m_pTempList[ui].uiBankNum == iBankNum )
			&& (m_pTempList[ui].uiTempNum == pTempU->uiTmpNum)){
			//���Ɋ����̃��X�g�ɑ��݂���
			m_pTempList[ui].time = pTempU->time;	//�����݂̂����ւ���
			break;
		}
	}
	if(ui>=m_uiTotalTemplates){
		//���݂̃��X�g�ɂ͑��݂��Ȃ������̂Œǉ�����
		if(m_uiTotalTemplates<MAX_TEMPLATE_TOTAL){
			m_pTempList[m_uiTotalTemplates].uiBankNum = iBankNum;
			m_pTempList[m_uiTotalTemplates].uiTempNum = pTempU->uiTmpNum;
			m_pTempList[m_uiTotalTemplates].time = pTempU->time;
			m_uiTotalTemplates++;
			for(ui=0; ui<m_uiBanks; ui++){
				if(m_pBankInfo[ui].uiBankNum == iBankNum){
					m_pBankInfo[ui].uiCounts++;
					break;
				}
			}
			if(ui>=m_uiBanks){
				//�V�����o���N�Ȃ̂ō\���̂𖖔��ɒǂ���
				m_uiBanks++;
				PBANKINFO pNewBankInfo = new BANKINFO[m_uiBanks];
				ZeroMemory(pNewBankInfo, sizeof(BANKINFO)*(m_uiBanks));
				memcpy(pNewBankInfo, m_pBankInfo, sizeof(BANKINFO)*(m_uiBanks-1));
				pNewBankInfo[m_uiBanks-1].uiBankNum = iBankNum;
				pNewBankInfo[m_uiBanks-1].uiCounts = 1;
				//�V�����\���̂̔z��ƌ�������
				if(m_pBankInfo){
					delete[] m_pBankInfo;
				}
				m_pBankInfo = pNewBankInfo;
			}

		}
	}
	return iReturn;
}
/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�Ǘ���񂩂�e���v���[�g�����폜����							*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int DeleteTemplateList(int iBankNum, PTEMPU pTempU)							*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g�Ǘ���񂩂�e���v���[�g�����폜���A�o���N�Ǘ������X�V����	*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::DeleteTemplateList(int iBankNum, PTEMPU pTempU)
{
	int iReturn;
	UINT ui;
	UINT uii;
	UINT uiii;
	UINT uiCondens;
	BOOL bDelete;
	PBANKINFO pBankInfo;

	iReturn = H1USB_OK;
	if(m_uiTotalTemplates == 0){
		return iReturn;
	}
	if(iBankNum < 0){
		//���ׂẴe���v���[�g���폜����
		ZeroMemory(m_pTempList, sizeof(TEMPLIST)*MAX_TEMPLATE_TOTAL);
		m_uiTotalTemplates = 0;
		if(m_pBankInfo){
			delete[] m_pBankInfo;
			m_pBankInfo = 0;
		}
		m_uiBanks = 0;
		return iReturn;
	}
	//�o���N�ԍ��ƃe���v���[�g�ԍ��ɂ��폜����
	for(ui=0; ui<m_uiTotalTemplates; ui){
		bDelete = FALSE;
		if(m_pTempList[ui].uiBankNum == iBankNum){
			//�w��̃o���N�����݂���
			if(pTempU){
				if(m_pTempList[ui].uiTempNum == pTempU->uiTmpNum){
					bDelete = TRUE;		
				}
			}
			else{
				bDelete = TRUE;	
			}
			if(bDelete){
				//�e���v���[�g���R���f���X����
				uiCondens = m_uiTotalTemplates - ui -1;
				for(uii=0; uii<uiCondens; uii++){
					m_pTempList[ui+uii] = m_pTempList[ui+uii+1];
				}
				m_uiTotalTemplates--;
				//�o���N���̕ҏW
				pBankInfo = 0;
				for(uii=0; uii<m_uiBanks; uii++){
					if(m_pBankInfo[uii].uiBankNum == iBankNum){
						if(m_pBankInfo[uii].uiCounts > 0){
							m_pBankInfo[uii].uiCounts--;
							if(m_pBankInfo[uii].uiCounts == 0){
								//�o���N���̃e���v���[�g�����݂��Ȃ�
								uiCondens = m_uiBanks - uii - 1;
								if(uiCondens > 0){
									pBankInfo = new BANKINFO[m_uiBanks-1];
									for(uiii=0; uiii<uiCondens; uiii++){
										m_pBankInfo[uii] = m_pBankInfo[uii+1];
									}
									memcpy(pBankInfo, m_pBankInfo, sizeof(BANKINFO)*(m_uiBanks-1));
									delete[] m_pBankInfo;
									m_pBankInfo = pBankInfo;
								}
								m_uiBanks--;
							}
						}
					}
				}
			}
		}
		if(!bDelete){
			ui++;
		}
	}
	return iReturn;
}
//int CTemplateFile::DeleteTemplateList(int iBankNum, PTEMPU pTempU)
//{
//	int iReturn;
//	UINT ui;
//	UINT uii;
//	UINT uiii;
//	UINT uiCondens;
//	BOOL bDelete;
//	PBANKINFO pBankInfo;
//
//	iReturn = H1USB_OK;
//	if(m_uiTotalTemplates == 0){
//		return iReturn;
//	}
//	if(iBankNum < 0){
//		//���ׂẴe���v���[�g���폜����
//		ZeroMemory(m_pTempList, sizeof(TEMPLIST)*MAX_TEMPLATE_TOTAL);
//		m_uiTotalTemplates = 0;
//		if(m_pBankInfo){
//			delete[] m_pBankInfo;
//			m_pBankInfo = 0;
//		}
//		m_uiBanks = 0;
//		return iReturn;
//	}
//	//�o���N�ԍ��ƃe���v���[�g�ԍ��ɂ��폜����
//	for(ui=0; ui<m_uiTotalTemplates; ui){
//		bDelete = FALSE;
//		if(m_pTempList[ui].uiBankNum == iBankNum){
//			//�w��̃o���N�����݂���
//			if(pTempU){
//				if(m_pTempList[ui].uiTempNum == pTempU->uiTmpNum){
//					bDelete = TRUE;		
//				}
//			}
//			else{
//				bDelete = TRUE;	
//			}
//			if(bDelete){
//				//�e���v���[�g���R���f���X����
//				uiCondens = m_uiTotalTemplates - ui -1;
//				for(uii=0; uii<uiCondens; uii++){
//					m_pTempList[ui+uii] = m_pTempList[ui+uii+1];
//				}
//				m_uiTotalTemplates--;
//				//�o���N���̕ҏW
//				pBankInfo = 0;
//				for(uii=0; uii<m_uiBanks; uii++){
//					if(m_pBankInfo[ui].uiBankNum == iBankNum){
//						if(m_pBankInfo[ui].uiCounts > 0){
//							m_pBankInfo[ui].uiCounts--;
//							if(m_pBankInfo[ui].uiCounts == 0){
//								//�o���N���̃e���v���[�g�����݂��Ȃ�
//								uiCondens = m_uiBanks - uii - 1;
//								if(uiCondens > 0){
//									pBankInfo = new BANKINFO[m_uiBanks-1];
//									for(uiii=0; uiii<uiCondens; uiii++){
//										m_pBankInfo[ui] = m_pBankInfo[ui+1];
//									}
//									memcpy(pBankInfo, m_pBankInfo, sizeof(BANKINFO)*(m_uiBanks-1));
//									delete[] m_pBankInfo;
//									m_pBankInfo = pBankInfo;
//									m_uiBanks--;
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//		if(!bDelete){
//			ui++;
//		}
//	}
//	return iReturn;
//}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�Ǘ����쐬														*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int MakeMangeTable()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g�Ǘ����ƃo���N�Ǘ��Ϗ����쐬����							*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::MakeMangeTable()
{
	int iReturn;

	iReturn = H1USB_OK;
	m_uiBanks = 0;						//���݂���o���N��
	m_pBankInfo = 0;					//�o���N�Ǘ����̃|�C���^�[
	m_uiTotalTemplates = 0;				//�S�̂̃e���v���[�g��

	ZeroMemory(m_pTempList, sizeof(TEMPLIST)*MAX_TEMPLATE_TOTAL);

	int iBanks;
	if(m_pBankInfo){
		delete[] m_pBankInfo;
		m_uiBanks = 0;

	}
	iBanks = CountTemplate(NULL);
	if(iBanks > 0){
		m_uiBanks = iBanks;
		m_pBankInfo = new BANKINFO[m_uiBanks];
		m_uiBanks = CountTemplate(m_pBankInfo);
	}
	return iReturn;
}

/********************************************************************************************/
/*[���\�b�h��] �e���v���[�g�J�E���g															*/
/*------------------------------------------------------------------------------------------*/
/*[�錾]		int CountTemplate(PBANKINFO pBankInfo)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[���e]		�e���v���[�g�t�@�C�����������A�e���v���[�g�̐����o���N�P�ʂɃJ�E���g����	*/
/*------------------------------------------------------------------------------------------*/
/*[�߂�l]		����I���FH1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::CountTemplate(PBANKINFO pBankInfo)
{
	int iReturn;
	iReturn	= H1USB_OK;
	CString strFind;
	CString strTPFName;

	TEMPU TempU;
	UINT uiBank;
	m_uiBanks = 0;
	m_uiTotalTemplates = 0;

	strFind = m_strTenplatePath + _T("\\BANK*.tpf");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(strFind, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE){

		return -1;

	}
	CString strBankNum;
	do{
		m_uiBanks++;
		if(pBankInfo){
			strTPFName = FindFileData.cFileName;
			strBankNum = strTPFName.Mid(4,4);
			_stscanf(strBankNum, "%x", &uiBank);
			pBankInfo[m_uiBanks-1].uiBankNum = uiBank;
			pBankInfo[m_uiBanks-1].uiCounts = 0;
			iReturn = Open(uiBank);
			if(iReturn != H1USB_OK){
				//�w��̃e���v���[�g�t�@�C�������݂��Ȃ�
				Close();
				return  iReturn;
			}
			while(1){
				iReturn = Read(&TempU,1);
				if(iReturn != H1USB_OK){
					//�ǂݍ��ރf�[�^�����݂��Ȃ�
					break;
				}
				pBankInfo[m_uiBanks-1].uiCounts++;
				m_pTempList[m_uiTotalTemplates].uiBankNum = uiBank;
				m_pTempList[m_uiTotalTemplates].uiTempNum = TempU.uiTmpNum;
				m_pTempList[m_uiTotalTemplates].time = TempU.time;
				m_uiTotalTemplates++;
			}
			Close();
		}

	}while(FindNextFile(hFind, &FindFileData));

	FindClose(hFind);
	return m_uiBanks;
}
