#pragma once
#include "afx.h"
#include "H1EUSB.h"
/********************************************************************************************/
/* CTemplateFile�N���X�w�b�_																*/
/*------------------------------------------------------------------------------------------*/
/*[�쐬]		<�쐬��><�쐬��><�o�[�W�����]���r�W����>									*/
/*				<����><20080110>�@Initial Version.                                          */
/*------------------------------------------------------------------------------------------*/
/*[�C������]	<�C����><�C����><�o�[�W�����]���r�W����><��Q�Ǘ��ԍ�><�C�����e>	    	*/
/*                                                                          				*/
/*------------------------------------------------------------------------------------------*/
/********************************************************************************************/
//�o���N���Ƃ̃e���v���[�g�Ǘ��p�̍\����
typedef struct _bankinfo
{
	UINT		uiBankNum;					//�o���N��
	UINT		uiCounts;					//�o�^����Ă���e���v���[�g��
}BANKINFO, *PBANKINFO;

//���ׂẴe���v���[�g�Ǘ��p�̍\����
typedef struct _templist
{
	UINT	uiBankNum;
	UINT	uiTempNum;
	SYSTEMTIME	time;						//�쐬���ꂽ����
}TEMPLIST, *PTEMPLIST;


class CTemplateFile :
	public CObject
{
public:
	CTemplateFile(void);										//�R���X�g���N�^
	virtual ~CTemplateFile(void);								//�f�X�g���N�^
	int Open(int iBankNum);										//�e���v���[�g�t�@�C���I�[�v��
	int Read(PTEMPU pTempU, UINT uiTemps);						//�e���v���[�g�t�@�C�����[�h
	int Write(PTEMPU pTempU, UINT uiTemps);						//�e���v���[�g�t�@�C�����C�g
	int Close();												//�e���v���[�g�t�@�C���N���[�Y
	int AddTemplate(int iBankNum, PTEMPU pTempU);				//�w�肳�ꂽ�o���N�t�@�C���Ƀe���v���[�g��ǉ�����
	int GetTemplate(int iBankNum, int iTempNum, PTEMPU pTempU);	//�w�肳�ꂽ�o���N����e���v���[�g���폜����
	int CopyBank0000(int iCOunt);								//�o���N�O�̃e���v���[�g���R�s�[����
	int DeleteOneTemplate(int iBankNum, int iTempNum);			//�o���N���̈�̃e���v���[�g���폜����
	int DeleteBank(int iBankNum);								//�w�肳�ꂽ�o���N���폜����
	int DeleteAll();											//���ׂẴe���v���[�g���폜����
	int SaveTemplates(HWND hWnd);								//�e���v���[�g�t�@�C�����Z�[�u����
	int OpenTemplates(HWND hWnd);								//�e���v���[�g�t�@�C�����Z�[�u����

	PTEMPLIST m_pTempList;										//�e���v���[�g�Ǘ����̃|�C���^
	UINT m_uiBanks;												//���݂���o���N��
	PBANKINFO m_pBankInfo;										//�o���N�Ǘ����̃|�C���^�[
	UINT m_uiTotalTemplates;									//�S�̂̃e���v���[�g��
	CString m_strTenplatePath;									//�i�[����Ă���e���v���[�g�̃t�H���_�[

private:
	int MakeTemplateDirectory();								//�e���v���[�g�t�@�C���i�[�t�H���_�[�쐬
	int CountTemplate(PBANKINFO pBankInfo);						//�e���v���[�g�J�E���g
	CFile* m_pTPFile;											//�t�@�C���I�u�W�F�N�g�̃|�C���^
	CFileException* m_pTPFex;									//�G�N�Z�v�V�����I�u�W�F�N�g�̃|�C���^
	void delTempOne(PTEMPU pTempU, UINT uiTemps);				//�e���v���[�g�Ǘ���񂩂�A�e���v���[�g���폜
	CString GetDirectoryName(HWND hWnd);						//�t�H���_�[�p�X�擾
	int CopyAllTempFiles(CString strSrcDir, CString strDstDir);	//�S�t�@�C���R�s�[
	int AddTemplateList(int iBankNum, PTEMPU pTempU);			//�e���v���[�g���Ƀe���v���[�g��ǉ�����
	int DeleteTemplateList(int iBankNum, PTEMPU pTempU);		//�e���v���[�g�����e���v���[�g���폜
	int MakeMangeTable();										//�e���v���[�g�Ǘ������쐬
	void DoEvents();											//�Ăяo�����̃L���[�ɂ��郁�b�Z�[�W��D����s
};