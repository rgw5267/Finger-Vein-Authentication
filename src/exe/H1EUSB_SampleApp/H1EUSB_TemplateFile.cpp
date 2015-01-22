#include "stdafx.h"
#include "H1EUSB_TemplateFile.h"
#include <direct.h>
/********************************************************************************************/
/* CTemplateFileクラスプログラム															*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20120110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/********************************************************************************************/
/********************************************************************************************/
/*[メソッド名] CTemplateFileクラスのコンストラクタ											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		CA2GObjThread(void)															*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		コンストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
/********************************************************************************************/
CTemplateFile::CTemplateFile(void)
{
	//メンバー変数を初期化
	m_pTPFile = new CFile;
	m_pTPFex = new CFileException;

	//テンプレートリストを確保する。
	m_pTempList = new TEMPLIST[MAX_TEMPLATE_TOTAL];
	//テンプレートファイルの格納フォルダーを作成する
	MakeTemplateDirectory();
	//管理テーブルを作成する
	MakeMangeTable();

}


/********************************************************************************************/
/*[メソッド名] CTemplateFileクラスのデストラクタ											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		~CTemplateFile(void)														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		デストラクタ処理															*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		なし																		*/
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
/*[メソッド名] テンプレートファイルオープン													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int Open(int iBankNum)														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレートファイルをオープンする											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::Open(int iBankNum)
{
	int iReturn;
	iReturn	= H1USB_OK;

	CString strTPFName;

	//オープンするパスを作成
	strTPFName.Format(_T("%s\\BANK%04X.tpf"), m_strTenplatePath, iBankNum);

	//ファイルオープン
	if(!m_pTPFile->Open(strTPFName, CFile::modeReadWrite, m_pTPFex)){
		//ファイルのオープンに失敗した
		return H1USB_ERR_TEMPFILE_OPEN;
	}
	return iReturn;
}


/********************************************************************************************/
/*[メソッド名] テンプレートファイルリード													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int Read(PTEMPU pTempU, UINT uiTemps)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		指定された個数分テンプレート情報をリードする								*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
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
		//読み込んだデータ数に整合性がない
		iReturn = H1USB_ERR_TEMPFILE_NON;
	}
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] テンプレートファイルライト													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int Write(PTEMPU pTempU, UINT uiTemps)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		指定された個数分テンプレート情報をライトする								*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
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
/*[メソッド名] テンプレートファイルラクローズ												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int Close(PTEMPU pTempU, UINT uiTemps)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレートファイルをクローズする											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::Close()
{
	int iReturn;
	iReturn	= H1USB_OK;
	if(m_pTPFile){
		//フラッシュしてからクローズする
		m_pTPFile->Flush();
		m_pTPFile->Close();
	}
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] テンプレートを追加する														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int AddTemplate(int iBankNum, PTEMPU pTempU)								*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		指定されたバンクにテンプレート情報を追加する								*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::AddTemplate(int iBankNum, PTEMPU pTempU)
{
	int iReturn;
	iReturn	= H1USB_OK;
	TEMPU tempu;

	CString strTPFName;
	//オープンするテンプレートファイルのパスを作成する
	strTPFName.Format(_T("%s\\BANK%04X.tpf"), m_strTenplatePath, iBankNum);
	CFile cf;
	CFileException ex;

	//ファイルオープン
	if(!cf.Open(strTPFName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite, &ex)){
		//テンプレートファイルオープン失敗
		return H1USB_ERR_TEMPFILE_OPEN;
	}

	ULONGLONG ull;
	LONGLONG ll;

	ull = cf.GetLength();
	while(ull > 0){
		cf.Read(&tempu, sizeof(TEMPU));
		if(tempu.uiTmpNum == pTempU->uiTmpNum){
			//既に登録されたテンプレート番号が存在した場合は上書き
			ll = 0 - (LONGLONG)sizeof(TEMPU);
			//ファイルポインタを一つのテンプレート情報分戻す
			cf.Seek(ll, CFile::current);
			break;
		}
		ull -= sizeof(TEMPU);
	}
	//ライト処理
	cf.Write(LPVOID(pTempU), sizeof(TEMPU));

	//クローズ処理
	cf.Close();

	//テンプレート管理テーブルを編集する
	iReturn = AddTemplateList(iBankNum, pTempU);
	
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] テンプレートを取得する														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int GetTemplate(int iBankNum, int iTempNum, PTEMPU pTempU)					*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		指定されたバンク番号、テンプレート番号のテンプレートファイルを取得する		*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::GetTemplate(int iBankNum, int iTempNum, PTEMPU pTempU)
{
	int iReturn;
	iReturn	= H1USB_OK;
	TEMPU tempU;

	//指定されたバンクのテンプレートファイルをオープンする
	iReturn = Open(iBankNum);
	if(iReturn==H1USB_OK){
		while(1){
			//テンプレート情報を一つリードする
			iReturn = Read(&tempU, 1);
			if(iReturn != H1USB_OK){
				break;				
			}
			if(tempU.uiTmpNum == iTempNum){
				//指定されたテンプレート番号を発見した
				*pTempU = tempU;
				break;
			}

		}
	}
	//ファイルをクローズする
	Close();
	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] テンプレートコピー															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int CopyBank0000(int iCount)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		0x0000バンクのテンプレートを指定されたバンク数コピーする					*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::CopyBank0000(int iCount)
{
	int iReturn;
	int i;
	iReturn = H1USB_OK;

	//バンク0000h以外のファイルを削除する
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

	//指定個数分ファイルをコピーする
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
/*[メソッド名]	メッセージ優先実行															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		void DoEvents()																*/
/*																							*/
/*[内容]		キューに溜まっているメッセージを優先的に実行させる							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
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
/*[メソッド名] テンプレート情報削除															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int DeleteOneTemplate(int iBankNum, int iTempNum)							*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		一つのテンプレート情報を削除する											*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::DeleteOneTemplate(int iBankNum, int iTempNum)
{
	int iReturn;
	ULONGLONG ullLength;
	PTEMPU pTempU;	
	UINT uiTemps;
	TEMPU deleteTempU;

	iReturn = H1USB_OK;

	//指定されたバンクのテンプレートファイルをオープン
	iReturn = Open(iBankNum);
	if(iReturn != H1USB_OK){
		return iReturn;
	}
	UINT ui;
	ullLength = m_pTPFile->GetLength();
	//登録済みのテンプレート情報をファイルサイズより算出する
	uiTemps  = (UINT)ullLength/sizeof(TEMPU);
	pTempU = 0;
	if(uiTemps > 0){
		pTempU = new TEMPU[uiTemps];
		//全てのテンプレートをリードする
		iReturn = Read(pTempU, uiTemps);
		if(iReturn == H1USB_OK){
			for(ui=0; ui<uiTemps; ui++){
				if(pTempU[ui].uiTmpNum == iTempNum){
					deleteTempU = pTempU[ui];
					//指定されたテンプレートを発見した
					//delTempOne(pTempU, uiTemps-ui);				//指定のテンプレートを削除してコンデンスする
					delTempOne(&pTempU[ui], uiTemps-ui);				//指定のテンプレートを削除してコンデンスする
					m_pTPFile->SetLength(0);
					iReturn = Write(pTempU, uiTemps - 1);		
					break;
				}
			}
		
		}
	}
	//テンプレートファイルをクローズする
	Close();
	if(pTempU){
		delete[] pTempU;
	}
	if(uiTemps == 1){
		DeleteBank(iBankNum);
	}

	if(iReturn == H1USB_OK){
		//管理テーブルを編集する
		iReturn =  DeleteTemplateList(iBankNum, &deleteTempU);
	}

	return iReturn;
}


/********************************************************************************************/
/*[メソッド名] テンプレート管理情報削除														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int delTempOne(PTEMPU pTempU, UINT uiTemps)									*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		一つのテンプレート管理情報を削除しコンデンスする							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
void CTemplateFile::delTempOne(PTEMPU pTempU, UINT uiTemps)
{
	UINT uiCondens;
	UINT ui;
	if(uiTemps < 1){
		return;
	}

	uiCondens = uiTemps -1 ;			//コンデンスする回数をセット
	for(ui=0; ui<uiCondens; ui++){
		pTempU[ui] = pTempU[ui+1];
	}
}


/********************************************************************************************/
/*[メソッド名] テンプレートファイル削除														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int DeleteBank(int iBankNum)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		指定されたバンクのテンプレートファイルを削除する							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::DeleteBank(int iBankNum)
{
	int iReturn;
	BOOL bReturn;
	iReturn = H1USB_OK;

	CString strDelFile;
	//削除するテンプレートファイルのパスを作成する
	strDelFile.Format(_T("%s\\BANK%04X.tpf"), m_strTenplatePath, iBankNum);
	bReturn = DeleteFile(strDelFile);
	if(bReturn){
		//管理テーブルを編集する
		iReturn =  DeleteTemplateList(iBankNum, NULL);
	}
	else{
		iReturn = H1USB_ERR_TEMPFILE_DEL;
	}

	return iReturn;
}


/********************************************************************************************/
/*[メソッド名] 全テンプレートファイル削除													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int DeleteAll()																*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		全てのバンクのテンプレートファイルを削除する								*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::DeleteAll()
{
	int iReturn;
	iReturn = H1USB_OK;

	CString strFind;
	strFind = m_strTenplatePath + _T("\\BANK*.tpf");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	//テンプレートファイルを検索しながら削除する
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
		//発見したテンプレートファイルを削除
		DeleteFile(strDelFile);
	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);

	//管理テーブルを編集する
	iReturn =  DeleteTemplateList(-1, NULL);

	return iReturn;

}

/********************************************************************************************/
/*[メソッド名] 全テンプレートファイルコピー													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		intCopyAllTempFiles(CString strSrcDir, CString strDstDir)					*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		全てのテンプレートファイルを指定されたフォルダーへ							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
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
/*[メソッド名] 全テンプレートファイル保存ー													*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int SaveTemplates(HWND hWnd)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		フォルダー選択画面を表示して全てのテンプレートファイルをコピーする			*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
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
/*[メソッド名] 全テンプレートファイルオープン												*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int OpenTemplates(HWND hWnd)												*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		保存されたテンプレートファイルを作業エリア内にコピーする					*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
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
		//管理テーブルを作成する
		iReturn = MakeMangeTable();
	}

	return iReturn;
}

/********************************************************************************************/
/*[メソッド名] フォルダー取得																*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		CString GetDirectoryName(HWND hWnd)											*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		シェルのダイアログを表示し、選択されたフォルダーを取得する					*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		フォルダーの絶対パス														*/
/********************************************************************************************/
CString CTemplateFile::GetDirectoryName(HWND hWnd)
{
	CString strPath;
	CString strWork;


	 BROWSEINFO      bi;
    LPSTR           lpBuffer;
    LPITEMIDLIST    pidlRoot;      // ブラウズのルートPIDL
    LPITEMIDLIST    pidlBrowse;    // ユーザーが選択したPIDL
    LPMALLOC        lpMalloc = NULL;

	strPath.Empty();
    HRESULT hr = SHGetMalloc(&lpMalloc);
    if(FAILED(hr)) return strPath;

    // ブラウズ情報受け取りバッファ領域の確保
    if ((lpBuffer = (LPSTR) lpMalloc->Alloc(_MAX_PATH)) == NULL) {
        return strPath;
    }
	
    // ダイアログ表示時のルートフォルダのPIDLを取得
    // ※以下はデスクトップをルートとしている。デスクトップをルートとする
    //   場合は、単に bi.pidlRoot に０を設定するだけでもよい。その他の特
    //   殊フォルダをルートとする事もできる。詳細はSHGetSpecialFolderLoca
    //   tionのヘルプを参照の事。
	HRESULT hResult;

	hResult = SHGetSpecialFolderLocation(hWnd, CSIDL_DRIVES, &pidlRoot);
	if (FAILED(hResult)){
		lpMalloc->Free(lpBuffer);
        return strPath;
    }
    // BROWSEINFO構造体の初期値設定
    // ※BROWSEINFO構造体の各メンバの詳細説明もヘルプを参照
    bi.hwndOwner = hWnd;
    bi.pidlRoot = pidlRoot;
    bi.pszDisplayName = lpBuffer;
    bi.lpszTitle = "Select Folder";
    bi.ulFlags = 0;
    bi.lpfn = 0;
    bi.lParam = 0;
    // フォルダ選択ダイアログの表示 
    pidlBrowse = SHBrowseForFolder(&bi);
    if (pidlBrowse != NULL) {  
        // PIDL形式の戻り値のファイルシステムのパスに変換
        if (SHGetPathFromIDList(pidlBrowse, lpBuffer)) {
            // 取得成功
			strPath = lpBuffer;
			strWork = strPath.Right(1);
			if(strWork == _T("\\")){
				strPath.Delete(strPath.GetLength()-1, 1);
			}
        }
        // SHBrowseForFolderの戻り値PIDLを解放
        lpMalloc->Free(pidlBrowse);
    }
    // クリーンアップ処理
    lpMalloc->Free(pidlRoot); 
    lpMalloc->Free(lpBuffer);
    lpMalloc->Release();

	return strPath;

}

/********************************************************************************************/
/*[メソッド名] テンプレートファイルフォルダーの作成											*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int MakeTemplateDirectory()													*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレートを格納する作業用のフォルダーを作成する							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::MakeTemplateDirectory()
{
	int iReturn;
	iReturn	= H1USB_OK;
	TCHAR tcExePath[MAX_PATH];
	int	iOffset;

	CWinApp* pApp = AfxGetApp();
	HMODULE hModule = GetModuleHandle(pApp->m_pszExeName);
	//このモジュールの所在の絶対パスを取得する
	GetModuleFileName(hModule, tcExePath, sizeof(tcExePath));
	m_strTenplatePath = tcExePath;
	iOffset = m_strTenplatePath.ReverseFind(_T('\\'));
	m_strTenplatePath.Delete(iOffset, MAX_PATH);
	//このモジュールの配下に作業用フォルダーを作成する
	m_strTenplatePath += _T("\\template");
	_tmkdir(m_strTenplatePath);

	return iReturn;
}



/********************************************************************************************/
/*[メソッド名] テンプレート管理情報にテンプレート情報を追加する								*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int AddTemplateList(int iBankNum, PTEMPU pTempU)							*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレート管理情報にテンプレート情報を追加し、バンク管理情報も更新する	*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::AddTemplateList(int iBankNum, PTEMPU pTempU)
{
	int iReturn;
	UINT ui;

	iReturn = H1USB_OK;
	//現在のリストに指定されたリストが存在するかをチェックする
	for(ui=0; ui<m_uiTotalTemplates; ui++){
		if((m_pTempList[ui].uiBankNum == iBankNum )
			&& (m_pTempList[ui].uiTempNum == pTempU->uiTmpNum)){
			//既に既存のリストに存在する
			m_pTempList[ui].time = pTempU->time;	//時刻のみを入れ替える
			break;
		}
	}
	if(ui>=m_uiTotalTemplates){
		//現在のリストには存在しなかったので追加する
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
				//新しいバンクなので構造体を末尾に追する
				m_uiBanks++;
				PBANKINFO pNewBankInfo = new BANKINFO[m_uiBanks];
				ZeroMemory(pNewBankInfo, sizeof(BANKINFO)*(m_uiBanks));
				memcpy(pNewBankInfo, m_pBankInfo, sizeof(BANKINFO)*(m_uiBanks-1));
				pNewBankInfo[m_uiBanks-1].uiBankNum = iBankNum;
				pNewBankInfo[m_uiBanks-1].uiCounts = 1;
				//新しい構造体の配列と交換する
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
/*[メソッド名] テンプレート管理情報からテンプレート情報を削除する							*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int DeleteTemplateList(int iBankNum, PTEMPU pTempU)							*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレート管理情報からテンプレート情報を削除し、バンク管理情報も更新する	*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
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
		//すべてのテンプレートを削除する
		ZeroMemory(m_pTempList, sizeof(TEMPLIST)*MAX_TEMPLATE_TOTAL);
		m_uiTotalTemplates = 0;
		if(m_pBankInfo){
			delete[] m_pBankInfo;
			m_pBankInfo = 0;
		}
		m_uiBanks = 0;
		return iReturn;
	}
	//バンク番号とテンプレート番号により削除する
	for(ui=0; ui<m_uiTotalTemplates; ui){
		bDelete = FALSE;
		if(m_pTempList[ui].uiBankNum == iBankNum){
			//指定のバンクが存在する
			if(pTempU){
				if(m_pTempList[ui].uiTempNum == pTempU->uiTmpNum){
					bDelete = TRUE;		
				}
			}
			else{
				bDelete = TRUE;	
			}
			if(bDelete){
				//テンプレートをコンデンスする
				uiCondens = m_uiTotalTemplates - ui -1;
				for(uii=0; uii<uiCondens; uii++){
					m_pTempList[ui+uii] = m_pTempList[ui+uii+1];
				}
				m_uiTotalTemplates--;
				//バンク情報の編集
				pBankInfo = 0;
				for(uii=0; uii<m_uiBanks; uii++){
					if(m_pBankInfo[uii].uiBankNum == iBankNum){
						if(m_pBankInfo[uii].uiCounts > 0){
							m_pBankInfo[uii].uiCounts--;
							if(m_pBankInfo[uii].uiCounts == 0){
								//バンク内のテンプレートが存在しない
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
//		//すべてのテンプレートを削除する
//		ZeroMemory(m_pTempList, sizeof(TEMPLIST)*MAX_TEMPLATE_TOTAL);
//		m_uiTotalTemplates = 0;
//		if(m_pBankInfo){
//			delete[] m_pBankInfo;
//			m_pBankInfo = 0;
//		}
//		m_uiBanks = 0;
//		return iReturn;
//	}
//	//バンク番号とテンプレート番号により削除する
//	for(ui=0; ui<m_uiTotalTemplates; ui){
//		bDelete = FALSE;
//		if(m_pTempList[ui].uiBankNum == iBankNum){
//			//指定のバンクが存在する
//			if(pTempU){
//				if(m_pTempList[ui].uiTempNum == pTempU->uiTmpNum){
//					bDelete = TRUE;		
//				}
//			}
//			else{
//				bDelete = TRUE;	
//			}
//			if(bDelete){
//				//テンプレートをコンデンスする
//				uiCondens = m_uiTotalTemplates - ui -1;
//				for(uii=0; uii<uiCondens; uii++){
//					m_pTempList[ui+uii] = m_pTempList[ui+uii+1];
//				}
//				m_uiTotalTemplates--;
//				//バンク情報の編集
//				pBankInfo = 0;
//				for(uii=0; uii<m_uiBanks; uii++){
//					if(m_pBankInfo[ui].uiBankNum == iBankNum){
//						if(m_pBankInfo[ui].uiCounts > 0){
//							m_pBankInfo[ui].uiCounts--;
//							if(m_pBankInfo[ui].uiCounts == 0){
//								//バンク内のテンプレートが存在しない
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
/*[メソッド名] テンプレート管理情報作成														*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int MakeMangeTable()														*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレート管理情報とバンク管理委情報を作成する							*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
/********************************************************************************************/
int CTemplateFile::MakeMangeTable()
{
	int iReturn;

	iReturn = H1USB_OK;
	m_uiBanks = 0;						//存在するバンク数
	m_pBankInfo = 0;					//バンク管理情報のポインター
	m_uiTotalTemplates = 0;				//全体のテンプレート数

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
/*[メソッド名] テンプレートカウント															*/
/*------------------------------------------------------------------------------------------*/
/*[宣言]		int CountTemplate(PBANKINFO pBankInfo)										*/
/*																							*/
/*------------------------------------------------------------------------------------------*/
/*[内容]		テンプレートファイルを検索し、テンプレートの数をバンク単位にカウントする	*/
/*------------------------------------------------------------------------------------------*/
/*[戻り値]		正常終了：H1USB_OK															*/
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
				//指定のテンプレートファイルが存在しない
				Close();
				return  iReturn;
			}
			while(1){
				iReturn = Read(&TempU,1);
				if(iReturn != H1USB_OK){
					//読み込むデータが存在しない
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
