#include "stdafx.h"
#include "WP7Request.h"

namespace PLATFORM_TOOL
{

	CPVKRequest::CPVKRequest()
		: CDiagRequestBase (_T("CPVKRequest"), 30000, false)
	{
		//75 250 100 00
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 100, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	CWolfWritePVKRequest::CWolfWritePVKRequest(int &nStatus, int& nIsBegin, int& nIsMore, int& nCheckSum, int& nDataLength, TCHAR* pData)
		: CDiagRequestBase (_T("CWolfWritePVKRequest"))
		, m_nStatus (nStatus)
	{
		CBufferPtr ptBufferSend;
		BYTE req[521];// 10 + 512 (Data Max Length)

		// = {75, 250, 119, 00, Begin, More, Checksum, DataLength, DataLength, Data};
		req[0] = (BYTE)75;
		req[1] = (BYTE)250;
		req[2] = (BYTE)119;
		req[3] = (BYTE)00;
		req[4] = (BYTE)nIsBegin;
		req[5] = (BYTE)nIsMore;
		req[6] = (BYTE)nCheckSum;
		req[7] = (BYTE)(nDataLength & 0xFF);
		req[8] = (BYTE)((nDataLength >> 8) & 0xFF);
	
		memcpy(&req[9], pData, nDataLength);
		
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
		
		if(( nIsBegin == 1) && (nIsMore == 0))//the data length < 512, remember add for data length >512
			nStatus = 99;// if it's the last data to be send, and it doesn't response after send data
	}

	void CWolfWritePVKRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[5] = {0};
		Safememcopy((void*)&response, 5,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		
		m_nStatus = response[4];
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWolfPVKisWriteRequest::CWolfPVKisWriteRequest (bool &bWritten)
		: CDiagRequestBase  (_T("CWolfPVKisWriteRequest"), 5000, true)
		, m_bWritten (bWritten)
	{
		//75 250 121 0
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 137};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfPVKisWriteRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		//75 250 121 0 * : 1 = success
		BYTE response[5] = {0};
		Safememcopy((void*)&response, 5,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_bWritten = (response[4] == 1);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CDeletePVKRequest::CDeletePVKRequest()
		: CDiagRequestBase (_T("CDeletePVKRequest"), 30000, false)
	{
		//75 250 99 00
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 99, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}


	CWolfClearPVKRequest::CWolfClearPVKRequest (bool &bSuccess)
		: CDiagRequestBase  (_T("CWolfClearPVKRequest"), 30000, true)
		, m_bSuccess (bSuccess)
	{
		//75 250 121 0
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 121, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfClearPVKRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		//75 250 121 0 * : 1 = success
		BYTE response[5] = {0};
		Safememcopy((void*)&response, 5,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
	
		m_bSuccess = (response[4] == 1);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CCheckPVKRequest::CCheckPVKRequest (int &status)
		: CDiagRequestBase  (_T("CCheckPVKRequest"), 30000, true)
		, m_nStatus (status)
	{
		m_nStatus = -1;

		//75 250 98 00
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 98, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CCheckPVKRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		//[U 02:34:42] Chenc Tai: 4B FA 62 00 00 00 E7 AB
		//[U 02:34:59] Chenc Tai: bytesuccess00

		BYTE response[6] = {0};
		Safememcopy((void*)&response, 6,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		if (response[0] != 0x13)
		{
			m_nStatus = response[4];
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWolfCheckPVKRequest::CWolfCheckPVKRequest (int &status)
		: CDiagRequestBase  (_T("CWolfCheckPVKRequest"), 30000, true)
		, m_nStatus (status)
	{
		m_nStatus = -1;

		//75 250 120 00
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 120, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfCheckPVKRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{

		//75 250 120 0 * * 0 0
		//ERROR_SUCCESS: 0
		//ERROR_FILE_NOT_FOUND: 2
		//ERROR_ACCESS_DENIED: 5
		//ERROR_NOT_ENOUGH_MEMORY: 8
		//ERROR_HANDLE_DISK_FULL: 39
		//ERROR_OPEN_FAILED: 110
		//ERROR_DISK_OPERATION_FAILED: 1127

		BYTE response[8] = {0};
		Safememcopy((void*)&response, 8,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_nStatus = response[5];
		m_nStatus <<= 8;
		m_nStatus += response[4];
		
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWolfBackUpPVKRequest::CWolfBackUpPVKRequest (int &status)
		: CDiagRequestBase  (_T("CWolfBackUpPVKRequest"), 30000, true)
		, m_nStatus (status)
	{
		m_nStatus = -1;

		//75 250 125 00
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 125, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfBackUpPVKRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{

		//75 250 125 0 * * * *
		//ERROR_SUCCESS: 0
		//ERROR_WRITE_FAULT: 29
		//ERROR_OPEN_FAILED: 110

		BYTE response[8] = {0};
		Safememcopy((void*)&response, 8,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_nStatus = response[7];
		m_nStatus <<= 8;
		m_nStatus += response[6];
		m_nStatus <<= 8;
		m_nStatus += response[5];
		m_nStatus <<= 8;
		m_nStatus += response[4];

		

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWolfRestorePVKRequest::CWolfRestorePVKRequest (int &status)
		: CDiagRequestBase  (_T("CWolfRestorePVKRequest"), 30000, true)
		, m_nStatus (status)
	{
		m_nStatus = -1;

		//75 250 126 00
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 126, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfRestorePVKRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{

		//75 250 126 0 * * * *
		//ERROR_SUCCESS: 0
		//ERROR_WRITE_FAULT: 29
		//ERROR_OPEN_FAILED: 110

		BYTE response[8] = {0};
		Safememcopy((void*)&response, 8,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_nStatus = response[7];
		m_nStatus <<= 8;
		m_nStatus += response[6];
		m_nStatus <<= 8;
		m_nStatus += response[5];
		m_nStatus <<= 8;
		m_nStatus += response[4];

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CLeopardMasterClearRequest::CLeopardMasterClearRequest(bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardMasterClearRequest"))
		, m_bSuccess (bSuccess)
	{
		//request:  75 250 101 00
		//response: 75 250 101 00 01 00
		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 250, 101, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CLeopardMasterClearRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[6] = {0};
		Safememcopy((void*)&response, 6,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_bSuccess = (response[4] == 1);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CWolfResetRequest::CWolfResetRequest(bool &bSuccess)
		: CDiagRequestBase (_T("CWolfResetRequest"))
		, m_bSuccess (bSuccess)
	{
		//request:  75 250 90 0 1 0
		//response: none
		CBufferPtr ptBufferSend;
		BYTE req[6] = {75, 250, 90, 0, 1, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWolfResetRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		// No Response
		m_bSuccess = true ;
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	
//-----------------------------------------------------------------	
// notes from SW about Leopard HW version;
//-----------------------------------------------------------------
//
//
//ABCDE= 11001 EVB variant 1, mcp=hynix.
//ABCDE= 22022  EVT2-2 variant 2, mcp= Toshiba.
//ABCDE= 21311  MP variant 2, mcp= hynix.
//                                   
//// variantVer
//typedef enum {                     
//  VARIANT_1 = 1, // EU variant     
//  VARIANT_2 = 2, // US variant     
//  VARIANT_3 = 3, // AWS variant    
//  VARIANT_UNKNOWN = 255,           
//} variant_t;                       
//
//// memVer
//typedef enum {                     
//  MCP_HYNIX   =1,                  
//  MCP_TOSHIBA =2,                  
//  MCP_SAMSUNG =3,                  
//  MCP_UNKNOWN = 255,               
//} mem_t;                           
//
//// HWVer
//typedef enum {                     
//  HW_ID_EVT, // E                  
//  HW_ID_DVT, // D                  
//  HW_ID_PVT, // P                  
//  HW_ID_MP, // A                   
//  HW_ID_UNKNOWN = 255,             
//} hw_id_t;                         
//
//// HWMinorVer
//typedef enum {                     
//  HW_ID_EVB_0 = 0,                
//  HW_ID_EVT_1 = 1,                 
//  HW_ID_EVT_2 = 2,                 
//  HW_ID_EVT_3 = 3,                 
//  HW_ID_EVT_4 = 4,                 
//  HW_ID_EVT_5 = 5,                 
//  HW_ID_EVT_UNKNOWN = 255,         
//} hw_id_minor_t;  
//
////pilotRun
//typedef enum {                     
//  PILOTRUN_ID_1 = 1,               
//  PILOTRUN_ID_2 = 2,               
//  PILOTRUN_ID_3 = 3,               
//  PILOTRUN_ID_UNKNOWN = 255,       
//} pilotrun_id_t;   //
//typedef struct {                   
//  variant_t variant_id; // A       
//  mem_t mem_id; // B               
//  hw_id_t hw_id; // C              
//  hw_id_minor_t hw_id_minor; // D  
//  pilotrun_id_t pilotrun_id; // E  
//  uint32 adcValue;                 
//  uint8 gpioin;                    
//} hwid_data;                       
//
//void qisda_get_hwid(hwid_data * rsp)
	
	CLeopardHWversionRequest::CLeopardHWversionRequest(int& variant, int& mem, int& evt, int& evtMinor, int& pilotRun)
		: CDiagRequestBase (_T("CLeopardHWversionRequest"))
		, m_varient (variant)
		, m_mem (mem)
		, m_evt (evt)
		, m_evtMinor (evtMinor)
		, m_pilotRun (pilotRun)
	{
		m_varient = -1;
		m_mem = -1;
		m_evt = -1;
		m_evtMinor = -1;
		m_pilotRun = -1;

		BYTE szRAWCommand[] = {75, 9, 12, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardHWversionRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			BYTE cmd_code;
			BYTE subsys_id;
			short mobile_id;
			BYTE  variantVer;
			BYTE  memVer;
			BYTE  HWVer; //EVT1, EVT2,...
			BYTE  HWMinorVer;
			BYTE  pilotRun;

		} diag_leopard_HWver_response;
		#pragma pack(pop)

		diag_leopard_HWver_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		bool bCommandSuccess = resp.cmd_code == 75;
		
		if (bCommandSuccess)
		{
			m_varient = resp.variantVer;
			m_mem = resp.memVer;
			m_evt = resp.HWVer;
			m_evtMinor = resp.HWMinorVer;
			m_pilotRun = resp.pilotRun;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CLeopardSWversionRequest::CLeopardSWversionRequest (std::string &SWver)
		: CDiagRequestBase (_T("CLeopardSWversionRequest"))
		, m_SWversion (SWver)
	{
		m_SWversion = "";

		BYTE szRAWCommand[] = {75, 250, 0x5b, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardSWversionRequest::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		#pragma pack (push, 1)
		typedef struct
		{
			BYTE  cmd_code;
			BYTE  subsys_id;
			short mobile_id;
			WORD  FW_type;
			BYTE  FW_ver [19];
			WORD  Result;
		} diag_leopard_SWver_response;
		#pragma pack(pop)

		diag_leopard_SWver_response resp = {0};
		Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		char szBuffer[38];
		memset(szBuffer, 0, 38);
		memcpy(szBuffer, resp.FW_ver, 15);
		m_SWversion = szBuffer;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	CWolfHWversionRequest::CWolfHWversionRequest(int& nVariant, int& nPCBStage, int& nMajorSN, int& nMinorSN, int& nMem, int& neMMC, int& nTouchPanel, int& nModule)
		: CDiagRequestBase (_T("CWolfHWversionRequest"))
		, m_Variant (nVariant)
		, m_PCBStage (nPCBStage)
		, m_MajorSN (nMajorSN)
		, m_MinorSN (nMinorSN)
		, m_Mem (nMem)
		, m_eMMC (neMMC)
		, m_TouchPanel (nTouchPanel)
		, m_Module (nModule)
	{
		m_Variant = -1;
		m_PCBStage = -1;
		m_MajorSN = -1;
		m_MinorSN = -1;
		m_Mem = -1;
		m_eMMC = -1;
		m_TouchPanel = -1;
		m_Module = -1;

		BYTE szRAWCommand[] = {75, 9, 12, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CWolfHWversionRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
#pragma pack (push, 1)

		typedef struct
		{
			BYTE cmd_code;
			BYTE subsys_id;
			short mobile_id;
			BYTE  variantVer;
			BYTE  PCBStage;
			BYTE  MajorSN;
			BYTE  MinorSN;
			BYTE  Mem;
			BYTE  eMMC;
			BYTE  TouchPanel;
			BYTE  Module;

		} diag_wolf_HWver_response;
#pragma pack(pop)

		diag_wolf_HWver_response resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		bool bCommandSuccess = resp.cmd_code == 75;

		if (bCommandSuccess)
		{
			m_Variant = resp.variantVer;
			m_PCBStage = resp.PCBStage;
			m_MajorSN = resp.MajorSN;
			m_MinorSN = resp.MinorSN;
			m_Mem = resp.Mem;
			m_eMMC = resp.eMMC;
			m_TouchPanel = resp.TouchPanel;
			m_Module = resp.Module;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	//request: 75 250 97 00
	//response:75 250 97 00 0x , 0=success, 
	CLeopardSetLocalTime::CLeopardSetLocalTime (bool &bSuccess)
		: CDiagRequestBase (_T("CLeopardSetLocalTime"))
		, m_Success (bSuccess)
	{
		bSuccess = false;
		BYTE szRAWCommand[] = {75, 250, 97, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CLeopardSetLocalTime::OnDataReceiver (CBufferPtr &ptBufferReceived)
	{
		BYTE response[6] = {0};
		Safememcopy((void*)&response, 6,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_Success = (response[4] == 0);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	/*  JY , get target operator
		CMD_CODE	1	Message ID V CMD_CODE is set to 75 for this message
		SUB_SYS_ID	1	Subsystem ID VQisda EM = 250
		Mode ID		2	Mode ID V set to 110  to get MOName value
		Result		1	Success: 1, Fail: 0
		MOName		16	ASCII (Hex), ex: 54 4D 4F 2D 55 53
		*/

	CTargetTargetOperatorRequest::CTargetTargetOperatorRequest(std::string &operatorName)
		:CDiagRequestBase(_T("CTargetTargetOperatorRequest"))
		, m_strOperatorName (operatorName)
	{
		BYTE szRAWCommand[] = {75, 250, 110, 0};
		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CTargetTargetOperatorRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
	
		#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short Mode_ID;
			BYTE result;
			BYTE  DeviceName[16]; //ASCII
		} diag_target_operator_rsp_type;
		#pragma pack(pop)

		diag_target_operator_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		bool bSuccess = resp.result == 1;
		if (bSuccess)
		{
			char szBuffer[16] = {0};
			memcpy (szBuffer, resp.DeviceName, 16);
			m_strOperatorName = szBuffer;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);

	}

	CWP7CITSpecifyFileNameRequest::CWP7CITSpecifyFileNameRequest(int &nStatus, int& nFileNameLength, TCHAR* pFileName)
		: CDiagRequestBase (_T("CWP7CITSpecifyFileNameRequest"))
		, m_nStatus (nStatus)
	{
		CBufferPtr ptBufferSend;
		BYTE req[521];// 10 + 512 (Data Max Length)

		// = {75, 250, 122, 0, nFileNameLength_1, nFileNameLength_2, FileName};
		req[0] = (BYTE)75;
		req[1] = (BYTE)250;
		req[2] = (BYTE)122;
		req[3] = (BYTE)0;

		req[4] = (BYTE)(nFileNameLength & 0xFF);
		req[5] = (BYTE)((nFileNameLength >> 8) & 0xFF);


		memcpy(&req[6], pFileName, nFileNameLength);

		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWP7CITSpecifyFileNameRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[5] = {0};
		Safememcopy((void*)&response, 5,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_nStatus = response[4];

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWP7CITWriteRequest::CWP7CITWriteRequest(int &nStatus, int& nIsBegin, int& nIsMore, int& nDataLength, TCHAR* pData)
		: CDiagRequestBase (_T("CWP7CITWriteRequest"))
		, m_nStatus (nStatus)
	{
		CBufferPtr ptBufferSend;
		BYTE req[519];// 8 + 512 (Data Max Length)

		// = {75, 250, 119, 00, Begin, More, DataLength, DataLength, Data};
		req[0] = (BYTE)75;
		req[1] = (BYTE)250;
		req[2] = (BYTE)123;
		req[3] = (BYTE)0;
		req[4] = (BYTE)nIsBegin;
		req[5] = (BYTE)nIsMore;
		req[6] = (BYTE)(nDataLength & 0xFF);
		req[7] = (BYTE)((nDataLength >> 8) & 0xFF);

		memcpy(&req[8], pData, nDataLength);

		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWP7CITWriteRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[5] = {0};
		Safememcopy((void*)&response, 5,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_nStatus = response[4];

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CWP7CITReadRequest::CWP7CITReadRequest(int &nStatus, int& nIsBegin, int& nIsMore, int& nDataLength, char* pData)
		: CDiagRequestBase (_T("CWP7CITReadRequest"))
		, m_nStatus (nStatus)
		, m_nIsMore (nIsMore)
		, m_nDataLength (nDataLength)
		, m_pData (pData)
	{
		CBufferPtr ptBufferSend;
		BYTE req[5];

		// = {75, 250, 119, 00, Begin, More, DataLength, DataLength, Data};
		req[0] = (BYTE)75;
		req[1] = (BYTE)250;
		req[2] = (BYTE)124;
		req[3] = (BYTE)0;
		req[4] = (BYTE)nIsBegin;

		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CWP7CITReadRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[522]	= {0};
		Safememcopy((void*)&response, 522,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		m_nStatus		= response[4];
		m_nIsMore		= response[6];
		
		m_nDataLength = response[8];
		m_nDataLength <<= 8;
		m_nDataLength += response[7];
				
		/*for(int i=0; i < m_nDataLength; i++)
		{
			*m_pData = response[i+9];
			m_pData++;
		}*/
		memset(m_pData,0,512);
		memcpy_s(m_pData, 512 , &response[9], m_nDataLength);
		
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	CSetupSleepRequest::CSetupSleepRequest(bool &nStatus, int& nTime)
		: CDiagRequestBase (_T("CSetupSleepRequest"))
		, m_nStatus (nStatus)
		, m_nTime (nTime)
	{
		CBufferPtr ptBufferSend;
		BYTE req[8];// 10 + 512 (Data Max Length)

		// = {75, 250, 133, 00, nTime, 0}; nTime: 0~255
		req[0] = (BYTE)75;
		req[1] = (BYTE)250;
		req[2] = (BYTE)133;
		req[3] = (BYTE)0;
		req[4] = (BYTE)nTime;
		req[5] = (BYTE)0;

		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CSetupSleepRequest::OnDataReceiver (CBufferPtr& ptBufferReceived)
	{
		BYTE response[8] = {0};
		Safememcopy((void*)&response, 5,(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());

		if(response[5] == 0)
			m_nStatus = true;
		else
			m_nStatus = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


} //PLATFORM_TOOL