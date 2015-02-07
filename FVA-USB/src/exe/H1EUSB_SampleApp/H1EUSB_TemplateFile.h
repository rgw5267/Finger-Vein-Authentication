#pragma once
#include "afx.h"
#include "H1EUSB.h"
/********************************************************************************************/
/* CTemplateFileクラスヘッダ																*/
/*------------------------------------------------------------------------------------------*/
/*[作成]		<作成者><作成日><バージョン‐リビジョン>									*/
/*				<白戸><20080110>　Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[修正来歴]	<修正者><修正日><バージョン‐リビジョン><障害管理番号><修正内容>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
//バンクごとのテンプレート管理用の構造体
typedef struct _bankinfo
{
	UINT		uiBankNum;					//バンク№
	UINT		uiCounts;					//登録されているテンプレート数
}BANKINFO, *PBANKINFO;

//すべてのテンプレート管理用の構造体
typedef struct _templist
{
	UINT	uiBankNum;
	UINT	uiTempNum;
	SYSTEMTIME	time;						//作成された時間
}TEMPLIST, *PTEMPLIST;


class CTemplateFile :
	public CObject
{
public:
	CTemplateFile(void);										//コンストラクタ
	virtual ~CTemplateFile(void);								//デストラクタ
	int Open(int iBankNum);										//テンプレートファイルオープン
	int Read(PTEMPU pTempU, UINT uiTemps);						//テンプレートファイルリード
	int Write(PTEMPU pTempU, UINT uiTemps);						//テンプレートファイルライト
	int Close();												//テンプレートファイルクローズ
	int AddTemplate(int iBankNum, PTEMPU pTempU);				//指定されたバンクファイルにテンプレートを追加する
	int GetTemplate(int iBankNum, int iTempNum, PTEMPU pTempU);	//指定されたバンクからテンプレートを削除する
	int CopyBank0000(int iCOunt);								//バンク０のテンプレートをコピーする
	int DeleteOneTemplate(int iBankNum, int iTempNum);			//バンク内の一つのテンプレートを削除する
	int DeleteBank(int iBankNum);								//指定されたバンクを削除する
	int DeleteAll();											//すべてのテンプレートを削除する
	int SaveTemplates(HWND hWnd);								//テンプレートファイルをセーブする
	int OpenTemplates(HWND hWnd);								//テンプレートファイルをセーブする

	PTEMPLIST m_pTempList;										//テンプレート管理情報のポインタ
	UINT m_uiBanks;												//存在するバンク数
	PBANKINFO m_pBankInfo;										//バンク管理情報のポインター
	UINT m_uiTotalTemplates;									//全体のテンプレート数
	CString m_strTenplatePath;									//格納されているテンプレートのフォルダー

private:
	int MakeTemplateDirectory();								//テンプレートファイル格納フォルダー作成
	int CountTemplate(PBANKINFO pBankInfo);						//テンプレートカウント
	CFile* m_pTPFile;											//ファイルオブジェクトのポインタ
	CFileException* m_pTPFex;									//エクセプションオブジェクトのポインタ
	void delTempOne(PTEMPU pTempU, UINT uiTemps);				//テンプレート管理情報から、テンプレートを削除
	CString GetDirectoryName(HWND hWnd);						//フォルダーパス取得
	int CopyAllTempFiles(CString strSrcDir, CString strDstDir);	//全ファイルコピー
	int AddTemplateList(int iBankNum, PTEMPU pTempU);			//テンプレート情報にテンプレートを追加する
	int DeleteTemplateList(int iBankNum, PTEMPU pTempU);		//テンプレート情報よりテンプレートを削除
	int MakeMangeTable();										//テンプレート管理情報を作成
	void DoEvents();											//呼び出し側のキューにあるメッセージを優先実行
};