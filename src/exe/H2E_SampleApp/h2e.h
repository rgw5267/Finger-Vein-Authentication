#pragma once

typedef union _byteword
{
	BYTE	byte[2];
	WORD	word;
} BYTEWORD, *PBYTEWORD;
typedef union _worddword
{
	BYTE	byte[4];
	WORD	word[2];
	DWORD	dword;
}WORDDWORD, *PWORDDWORD;

#define MAX_TEMPLATE_TOTAL		400				//max tenmplates
#define MAX_TEMPLATE_LEN		1072			//template data byte size.

//template info structure.
typedef struct _tempu
{
	UINT		uiGroupNum;						//group nummber
	UINT		uiTmpNum;						//template nummber
	UINT		uiQLevel;						//quality level
	SYSTEMTIME	time;							//enrolled time
	BYTE		byTemplate[MAX_TEMPLATE_LEN];	//template data
	DWORD		dwAddSum;						//sumcheck data(add)
	DWORD		dwXorSum;						//sumcheck data(xor)

}TEMPU, *PTEMPU;

//template manage table
typedef struct _templist
{
	UINT		uiGroupNum;						//group nummber
	UINT		uiTempNum;						//template nummber
	UINT		uiQLevel;						//enrolled level
	SYSTEMTIME	time;							//create level
}TEMPLIST, *PTEMPLIST;

//template manage structure
typedef struct _tempInfo
{
	DWORD		dwTemplates;					//enrolled templates
	PTEMPLIST	pTempList;						//template nmanage table's pointer
} TEMPINFO, *PTEMPINFO;


//Finite State Machine
#define SYSTEM_STARTUP					1			//startup state
#define SYSTEM_DEVICE_OFF				2			//non connect state
#define SYSTEM_DEVICE_ON				3			//connected device state
#define SYSTEM_DEVICE_ERR				4			//error state



//Error Code Define;
#define H2E_OK							0			//success, normal end.
#define H2E_NOTIFY						1			//notification
#define H2E_CANCEL						2			//cancel operation
#define H2E_TAKEEND						3			//taked finger vein.

#define H2E_ERR_TRANSTOUT				-10			//transaction error（timeout）
#define H2E_ERR_TSPOLTOUT				-11			//Wait touch sensot TimeOut
#define H2E_ERR_BIGGER					-100		//nummber bigger
#define H2E_ERR_INPNUMMBER				-101		//invalid parameter
#define H2E_ERR_MAKEBUFFER				-102		//making buffer error
#define H2E_ERR_ANLYSBUFFER				-103		//analysis buffer error


#define H2E_ERR_COMOPEN					-200		//COM port open error
#define H2E_ERR_COMCLOSE				-201		//COM port close error
#define H2E_ERR_READFILE				-202		//COM port read error
#define H2E_ERR_WRITEFILE				-203		//COM port write error
#define H2E_ERR_RDATA_NON				-204		//received data non
#define H2E_ERR_RESPONS					-205		//received invalid data
#define H2E_ERR_CCOMERR					-206		//COM port error
#define H2E_ERR_ADDSUM					-207		//sumcheck error(add)
#define H2E_ERR_XORSUM					-208		//sumcheck error(xor)
#define H2E_ERR_COMNOTOPEN				-250		//COM port not open


#define H2E_ERR_TEMP_NOTOPEN			-500		//template oblect not open
#define H2E_ERR_TEMPFILE_OPEN			-501		//template file not open
#define H2E_ERR_TEMPFILE_NON			-502		//template file not exist
#define H2E_ERR_TEMPFILE_COPY			-503		//template file copy error
#define H2E_ERR_TEMPFILE_DEL			-504		//template file delete error

//コマンド
#define H2E_COM_SENDNOP						0			//No Opration
#define H2E_COM_GETDEVINFO					1			//#1 Get Infomation
#define H2E_COM_HWRESET						2			//#2 Hard ware Reset
#define H2E_COM_PASS_DRVER_CAN				3			//#3 PASS_DRVER Pin Cancel
#define H2E_COM_TMP_ENROLL					4			//#4 Template Enroll
#define H2E_COM_TMP_DELETE					5			//#5 Template Delete
#define H2E_COM_TMP_G_DELETE				6			//#6 Template Delete(Group)
#define H2E_COM_TMP_TMP_INIT				7			//#7 Template Initial
#define H2E_COM_VER1TO1						8			//#8 Verifcation 1:1
#define H2E_COM_VER_G_1TON					9			//#9 Verifcation 1:N(Group)
#define H2E_COM_VER_1TON					10			//#10 Verifcation 1:N(Template)
#define H2E_COM_GET_TMP_INFO				11			//#11 Get Template Infomation
#define H2E_COM_READ_TMP					12			//#12 Read Template
#define H2E_COM_WRITE_TMPC2					13			//#13 Wtite Template(C2h)
#define H2E_COM_WRITE_TMPC3					14			//#13 Wtite Template(C3h)
#define H2E_COM_SET_TMP						15			//#14 Set Template ID
#define H2E_COM_CHANGE_TMP					16			//#15 Change Template ID
#define H2E_COM_GET_FROM_COND				17			//#16 Get FllashROM Condition
#define H2E_COM_SET_SECURITY				18			//#17 Set Security Level
#define H2E_COM_SET_TAKE_MODE				19			//#18 Set Mode


//Tuch Sensor Status
#define H2E_TOUCH_OUT1						0x0001		//TUCH_OUT1 line active  
#define H2E_TOUCH_OUT2						0x0002		//TUCH_OUT2 line active
 