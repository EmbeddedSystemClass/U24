#include "stdafx.h"
#include "GPSRequest.h"
#include "..\codec\QualcommCRCChecker.h"

namespace PLATFORM_TOOL
{

	/*****************************************************/
	/*	GPS   CLeopardReadIQValueRequest                  */
    /******************************************************/

	CLeopardReadIQValueRequest::CLeopardReadIQValueRequest (bool &bSuccess, WORD &Ivalue, WORD &Qvalue)
		: CDiagRequestBase (_T("CLeopardReadIQValueRequest"))
		, m_bSuccess (bSuccess)
		, m_wIvalue (Ivalue)
		, m_wQvalue (Qvalue)
	{
		// 4B 0D 6F 00

		bSuccess = false;

		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 13, 111, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CLeopardReadIQValueRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code;
			BYTE sybsys_id;
			short Mode_ID;
			WORD Ivalue;
			WORD Qvalue;
		} diag_IQvalue_resp;

		#pragma pack (pop)

		diag_IQvalue_resp resp = {0};
		Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_wIvalue = resp.Ivalue;
		m_wQvalue = resp.Qvalue;
		m_bSuccess = true;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/*****************************************************/
	/*	GPS   CLeopardGetCNOvalueRequest                  */
    /******************************************************/

	CLeopardGetCNOvalueRequest::CLeopardGetCNOvalueRequest (WORD &CNO, bool& bSuccess)
		: CDiagRequestBase (_T("CLeopardGetCNOvalueRequest"))
		, m_bSuccess (bSuccess)
		, m_CNO (CNO)
	{
		// 4B 0D 6E 00
		bSuccess = false;

		CBufferPtr ptBufferSend;
		BYTE req[4] = {75, 13, 0x6E, 0};
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CLeopardGetCNOvalueRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code;
			BYTE sybsys_id;
			short Mode_ID;
			WORD CNO;
		} diag_CNOvalue_resp;

		#pragma pack (pop)

		diag_CNOvalue_resp resp = {0};
		Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_CNO = resp.CNO;
		m_bSuccess = true;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);	
	}



	/******************************************************/
	/*	GPS   CGPS_SA_RF_VERIF_MODE_SWITCHRequest         */
    /******************************************************/

	CGPS_SA_RF_VERIF_MODE_SWITCHRequest::CGPS_SA_RF_VERIF_MODE_SWITCHRequest (bool bEnter, bool &bSuccess)
		: m_bSuccess (bSuccess)
	{
		bSuccess = false;

		//4B 0D 65 00 14 01 00
		BYTE ByteEnterMode = bEnter ? 0 : 1;
		BYTE req[7] = {75, 13, 101, 0, 20, 1, ByteEnterMode};

		CBufferPtr ptBufferSend;
		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CGPS_SA_RF_VERIF_MODE_SWITCHRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code; 
			BYTE subsys_id;
			short Mode_ID;
			BYTE CGPS_command_ID;
			BYTE status;
		} diag_gps_sa_rf_IO_rsp;
		#pragma pack(pop)

		diag_gps_sa_rf_IO_rsp resp = {75, 13, 101, 23, 0};
		Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		if (resp.status == 0)
				m_bSuccess = true;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/******************************************************/
	/*	GPS   CReadIQValueRequest                         */
    /******************************************************/

	CReadIQValueRequest::CReadIQValueRequest(int& nIValue,int& nQValue,bool& bSuccess)
		:CDiagRequestBase(_T("CReadIQValueRequest"))
		,m_nIValue(nIValue)
		,m_nQValue(nQValue)
		,m_bSuccess(bSuccess)
	{

		/*CMD_CODE              1 	Message ID C CMD_CODE is set to 0x4b for this message
		SUB_SYS_ID	            1   Subsystem ID C SUBSYS_ID is set to 0x0d for GPS command
		Subsystem Command Code	2	0x71 0x00 is set for Amp I/Q test*/

		BYTE szRAWCommand[] = {75, 13, 113, 0};

		CBufferPtr ptBufferSend;
		Encode(szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CReadIQValueRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{
		/*CMD_CODE              1 	Message ID C CMD_CODE is set to 0x4b for this message
		SUB_SYS_ID	            1   Subsystem ID C SUBSYS_ID is set to 0x0d for GPS command
		Subsystem Command Code	2	0x71 0x00 is set for Amp I/Q test
		wAmp_I	2	Amp I Value, Little Endian
		wAmp_Q	2	Amp Q Value, Little Endian
		pass	1	1, for pass; 0, for fail*/
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short subsys_id1;
			short IValue;
			short QValue;
			BYTE RESULT;
		} diag_iqvalue_rsp_type;
#pragma pack(pop)

		diag_iqvalue_rsp_type resp = {0};
		Safememcopy((void*)&resp,sizeof(resp),(void*)ptBufferReceived->GetData(),ptBufferReceived->GetLength());
		m_nIValue = resp.IValue;
		m_nQValue = resp.QValue;
		m_bSuccess = (resp.RESULT == 1 && resp.command_code == 0x4B);
		assert(m_bSuccess);

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/*****************************************************/
	/*	GPS   CGPS_SV_TRACKRequest                        */
    /******************************************************/

	CGPS_SV_TRACKRequest::CGPS_SV_TRACKRequest (BYTE svNumber, BYTE cmd, bool &bSuccess)
		: CDiagRequestBase (_T("CGPS_SV_TRACKRequest"))
		, m_bSuccess (bSuccess)
	{
		//4B 0D 65 00 17 01 sv cmd 00 00 00 00
		//sv: 0 to 32
		//cmd:	1 - Start_Clear_All
		//		2 - Start_No_Clear
		//		3 - Stop_Clear_All
		//		4 - Stop_No_Clear
		//		5 - Start_Clear_All_No_Bit_Edge
		//		6 - Start_No_Clear_No_Bit_Edge

		CBufferPtr ptBufferSend;

		BYTE req[] = {75, 13, 101, 0, 23, 1, svNumber, cmd, 0, 0, 0, 0};
		Encode (req, 12, ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CGPS_SV_TRACKRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code; 
			BYTE subsys_id;
			short Mode_ID;
			BYTE CGPS_command_ID;
			BYTE status;
		} diag_gps_SVtrack_IO_rsp;
		#pragma pack(pop)

		diag_gps_SVtrack_IO_rsp resp;
		Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		if (resp.status == 0)
			m_bSuccess = true;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	/*****************************************************/
	/*	GPS   CEnableGPSLog                              */
    /******************************************************/

	#pragma pack(push, 1)
	typedef struct
	{
		BYTE  cmd_code;
		BYTE  reserved[3];
		DWORD operation; //0-disable log, 1-retrieve ID range, 3-set log mask, 4-get log mask

		//logging range status
		DWORD equip_ID;
		DWORD num_items;
		
	} logConfig_req;
	#pragma pack(pop)

	CEnableGPSLog::CEnableGPSLog(bool bEnable, DWORD packetNo, bool &bSuccess)
		: CDiagRequestBase (_T("CEnableGPSLog"))
		, m_bSuccess (bSuccess)
	{
		//// dont know what is this, dont have enough time to clarify
		//// It is from Aaron
		//BYTE req[] = {
		//	0x73,0,0,0,3,0,0,0,1,0,
		//	0,0,0xEA,4,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0x00,0,0,0,0,0,2,0,0,0, // the important mask is here!
		//	0x00,0,0,0,0,0,0,0,0,0,
		//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		const DWORD num_items = 0x4EA;

		int size = (num_items+7)/8 + sizeof(logConfig_req);
		BYTE *req = new BYTE [size];
		memset (req, 0, size);

		logConfig_req *pReq = (logConfig_req*) req;
		pReq->cmd_code = 0x73;
		pReq->operation = 3; //set log mask
		pReq->equip_ID = 1;
		pReq->num_items = num_items;

		BYTE* pMask = req + sizeof(logConfig_req);

		packetNo -= 0x1000;
		int index = packetNo / 8;
		BYTE mask = 1 << (packetNo%8);
		pMask [index] |= mask;
		if (! bEnable)
		{
			pMask [index] &= ~mask;
		}


		CBufferPtr ptBufferSend;
		Encode (req, size, ptBufferSend);
		SetSendData (ptBufferSend);
		delete [] req;
	}

	void CEnableGPSLog::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		ptBufferReceived->GetData();
		DWORD dwLen = ptBufferReceived->GetLength();
		m_bSuccess = true; //??

		CDiagRequestBase::OnDataReceiver (ptBufferReceived);
	}








	/******************************************************/
	/*	GPS   CGPSEnterStandaloneRFRequest                */
    /******************************************************/


	#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code; 
			BYTE subsys_id;
			short Mode_ID;
			BYTE CGPS_command_ID;
			BYTE status;
		} diag_gps_sa_rf_IO_rsp;

		typedef struct
		{
			BYTE  Version;
			DWORD Fcount;	//receiver's ms count value
			WORD  GPSweek;	//GPS week number
			DWORD GPSmsec;	//GPs millisecond into the weak
			float TimeBias;	//(User time - GPS time) in ms, float data type
			float TimeUnc;	//clock time uncertainty
			float FreqBias; //clock freq bias in ms
			float FreqUnc;	//clock freq uncertainty in ms
			BYTE  SVSnumber;//(0 to 16)
		} log_code_1371_header;

		typedef struct
		{
			BYTE  SV_ID;		//satellite ID
			BYTE  ObsState;		//observation state
			BYTE  ObsCount;		//count ofobservations, both success and failure
			BYTE  GoodObsCount;	//count of good observation
			BYTE  FilterN;		//carrier-to-code filtering, number of filter stages
			WORD  CNO;			//carrier-to-noise in a 1 Hz bandwidth, range is 0-655.35
			WORD  Latency;
			BYTE  Pre_int;		//Pre-det interval in ms
			WORD  Post_int;		//Number of post-dets
			DWORD Unfilt_ms;	//Unfiltered measurement ms in user time
			DWORD Unfilt_sm;	//Unfiltered measurement fractional ms in user time;
			float Unfilt_time_unc;
			float Unfilt_speed; //Unfiltered SV speed in ms
			float Unfilt_speed_unc;
			DWORD MeasStatus;	// measurement status flag
			BYTE  MiscStatus;	
			DWORD MultipathEst; //Multipath estimate
			float AziRad;		//Azimuth of SV
			float ElevRad;		//Elevation of SV
			long Carrier_Phase_Cycles;
			WORD Carrier_Phase_Fract;
			BYTE CycleSlipCount;
			BYTE Reserved[3];
		} log_SV;

		typedef struct
		{
			BYTE  Version;
			DWORD Fcount;	//receiver's ms count value
			BYTE  PLL_chan_and_lock_status;
			WORD  BP_Mean_I;
			WORD  BP_Mean_Q;
			WORD  BP_Ampl_I;
			WORD  BP_Ampl_Q;
			DWORD BP_Rotator_Freq;
			WORD  BP_Rotator_delta_freq;
			BYTE  Reserved[36];

		} LOG_CONVERGED_GPS_RF_STATUS_REPORT_C; //packet 0x1372

		typedef struct
		{
			BYTE cmdCode; //0x10
			BYTE more;
			WORD length;

			WORD logLength;
			WORD logCode;
			BYTE TimeStamp[8];
			union 
			{
				log_SV logSV;
				LOG_CONVERGED_GPS_RF_STATUS_REPORT_C gps_rf_status;
			} logItem;

		} logresponse;
	#pragma pack(pop)

	CGPSEnterStandaloneRFRequest::CGPSEnterStandaloneRFRequest (BYTE ByteEnterMode, DWORD packetNo, bool& bSuccess, int &status, void *pOutput, DWORD dwBufferSize, bool bWaitLog) //EnterMode 0:enter, 1:exit
		: CDiagRequestBase (_T("CGPSEnterStandaloneRFRequest"))
		, m_bSuccess(bSuccess)
		, m_bWaitLog (bWaitLog)
		, m_status (status)
		, m_counter (0)
		, m_packetNo (packetNo)
		, m_bFirstCommandResponseSuccess(false)
		, m_ptTempBufferReceived (new CBuffer)
		, m_packet1372 ((packet_0x1372_structure*)pOutput)
	{
		
		//4B 0D 65 00 14 01 00

		#pragma pack(push, 1)
		typedef struct
		{
			BYTE command_code; 
			BYTE subsys_id; 
			short Mode_ID; 
			BYTE CGPS_command_ID;
			BYTE version; 
			BYTE flag;
		} diag_gps_sa_rf_IO_req;
		#pragma pack(pop)


		status = 1;

		if (ByteEnterMode > 1) //dont know how to implement
			ByteEnterMode = 1;
		m_CmdType = ByteEnterMode;


		//#ifdef _USE_LOGPACKET
		if (0x1372 == m_packetNo)
		{
			if (m_packet1372)
				SetTimeout ((int)(m_packet1372->packetCount * 1.5 * 1000));
		}
		//#endif 


		CBufferPtr ptBufferSend;
		diag_gps_sa_rf_IO_req req = {75, 13, 101, 20, 1, ByteEnterMode};
		Encode ((BYTE*)&req, sizeof(diag_gps_sa_rf_IO_req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CGPSEnterStandaloneRFRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{

		if (! m_bWaitLog)
		{
			diag_gps_sa_rf_IO_rsp resp = {75, 13, 101, 20, 0};
			Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			if (resp.status == 0)
			{
				m_status = 1;
				m_bSuccess = true;
			}
			else
			{
				m_status = 0;
				m_bSuccess = false;
			}
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CGPSEnterStandaloneRFRequest::IsCompletePacket (CBufferPtr &ptBufferReceived)
	{
		if (m_bWaitLog)
		{
			m_ptTempBufferReceived->Append(ptBufferReceived);

			bool bRes = false;
			char *szBuffer = const_cast<char *>(m_ptTempBufferReceived->GetData());
			int nTotalLen = m_ptTempBufferReceived->GetLength();		
			int nszBufferIndex = 0;
			bool bFind7E = false;
			do
			{
				bFind7E = false;
				char *szNewBuffer = szBuffer + nszBufferIndex;
				for(; nszBufferIndex < nTotalLen ; nszBufferIndex++)
				{
					if(szBuffer[nszBufferIndex] == 0x7E)
					{
						bFind7E = true;
						break;
					}
				}
				nszBufferIndex+=1;

				if(bFind7E)
				{
					if(szNewBuffer[0] == 0x60)
					{//It's Event Message, ingore it
					}
					else if(szNewBuffer[0] == 0x15)
					{//It's Hello Message, ingore it
					}
					else if (szNewBuffer[0] == 0x79)
					{
						break;  //debug log, ingore it, sunlight
					}
					else if(szNewBuffer[0] == 0x13 && szNewBuffer[1] == 0x61)
					{//It's Hello Message, ingore it
					}
					else
					{
						QualcommCRCChecker CRCChecker(false);
						for ( DWORD j = 0 ; ((szNewBuffer - szBuffer) + j) <= m_ptTempBufferReceived->GetLength(); j ++)
						{
							int nRxLen = CRCChecker.Push(szNewBuffer[j]);
							if ( nRxLen > 0)
							{
								bRes = true;
								ptBufferReceived = new CBuffer(nRxLen, (const char*)CRCChecker.GetBuffer());
								m_ptTempBufferReceived->PopFront(j + 1);
								break;
							}
						};
						break;
					}
				}
			}
			while(bFind7E);

			bool bIsCompletePacket = false;
			if(m_bFirstCommandResponseSuccess == false)
			{
				diag_gps_sa_rf_IO_rsp resp = {75, 13, 101, 20, 0};
				if (bRes && memcmp (ptBufferReceived->GetData(), &resp, sizeof(resp)-1) == 0) //dont compare the "status" field
				{
					m_bFirstCommandResponseSuccess = true;

					// this is the response, check for status
					diag_gps_sa_rf_IO_rsp* pResponse = (diag_gps_sa_rf_IO_rsp*)ptBufferReceived->GetData();
					if (pResponse->status == 0)
					{
						m_bSuccess = true;

						if ((1 == m_CmdType) ||		//stop command
							(0x1372 != m_packetNo)) // other than packet 0x1372, currently we only parse packet 0x1372
						{
							bIsCompletePacket = true;
						}						
					}
					else
					{
						bIsCompletePacket = true;
						m_status = 0; // IO error
					}
				}
			}
			else
			{//Fist response success
				
				if (bRes && ptBufferReceived->GetData()[0] == 0x10 ) // this is the log we want
				{
					logresponse *pLogResp = (logresponse*) ptBufferReceived->GetData();
					if ((0x1372 == pLogResp->logCode) && m_packet1372)
					{
						WORD Ivalue = pLogResp->logItem.gps_rf_status.BP_Ampl_I;
						WORD Qvalue = pLogResp->logItem.gps_rf_status.BP_Ampl_Q;
						m_packet1372->Ivalue = Ivalue;
						m_packet1372->Qvalue = Qvalue;

						long sum = Ivalue + Qvalue;
						if (sum >= m_packet1372->Lobound && sum <= m_packet1372->Upbound)
						{
							if (++m_counter >= m_packet1372->packetCount)
							{
								bIsCompletePacket = true;
							}
						}
						else
						{
							m_status = 2; // value error
							m_bSuccess = false;
							bIsCompletePacket = true;
						}

					}
				}
			}
			return bIsCompletePacket;
		}
		else
		{
			return CDiagRequestBase::IsCompletePacket (ptBufferReceived);

		}
	}

	/*****************************************************/
	/*	GPS   CGPS_SV_TRACKRequest by LOG                 */
    /******************************************************/

	#pragma pack(push, 1)
	typedef struct
	{
		BYTE command_code; 
		BYTE subsys_id; 
		short Mode_ID; 
		BYTE CGPS_command_ID;
		BYTE version; 
		BYTE SV; //satellite PRN [0..32]
		BYTE command_ID;
		BYTE Reserved[4];
	} diag_gps_SVtrack_IO_req;

	typedef struct
	{
		BYTE command_code; 
		BYTE subsys_id;
		short Mode_ID;
		BYTE CGPS_command_ID;
		BYTE status;
	} diag_gps_SVtrack_IO_rsp;

	#pragma pack(pop)

	CGPSSVTrack_LogRequest::CGPSSVTrack_LogRequest(BYTE ByteSV, int CmdType, bool &bSuccess, WORD wThreshold, WORD &lastCNO, bool bWaitLog)
		: CDiagRequestBase (_T("CGPSSVTrack_LogRequest"), 30000)
		, m_wThreshold (wThreshold)
		, m_bSuccess(bSuccess)
		, m_bWaitLog (bWaitLog)
		, m_CmdType (CmdType)
		, m_lastCNO (lastCNO)
		, m_ptTempBufferReceived(new CBuffer)
	{
		//4B 0D 65 00 17 01 00 01 00 00 00 00

		m_bSuccess = false;

		if (ByteSV > 32) //dont know how to implement
			ByteSV = 0;

		if (0 == m_wThreshold)
			m_wThreshold = 2500; //default

		BYTE commandID = CmdType;
	
		CBufferPtr ptBufferSend;
		diag_gps_SVtrack_IO_req req = {75, 13, 101, 23, 1, ByteSV, commandID, 0, 0, 0, 0};
		Encode ((BYTE*)&req, sizeof(diag_gps_SVtrack_IO_req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CGPSSVTrack_LogRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		if (! m_bWaitLog)
		{
			diag_gps_SVtrack_IO_rsp resp = {75, 13, 101, 23, 0};
			Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
			if (resp.status == 0)
				m_bSuccess = true;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}

	bool CGPSSVTrack_LogRequest::IsCompletePacket (CBufferPtr &ptBufferReceived)
	{	
		if (m_bWaitLog)
		{
			BYTE* pPtr;
			bool bReturn = false;

			m_ptTempBufferReceived->Append(ptBufferReceived);
			diag_gps_SVtrack_IO_rsp resp = {75, 13, 101, 23, 0};

			while (1)
			{
				// we are expecting the response package (diag_gps_SVtrack_IO_rsp structure) + 
				// n blocks of log data.

				pPtr = (BYTE*) m_ptTempBufferReceived->GetData();
				DWORD dwTotalLen = m_ptTempBufferReceived->GetLength();
				if (dwTotalLen == 0)
					break;

				//find 0x7E
				DWORD size7E = 0;
				while (pPtr[size7E] != 0x7E)
				{
					if (++size7E >= dwTotalLen)
						break;
				}
				++size7E;
				
				
				int rxLen = 0;
				QualcommCRCChecker CRCChecker (false);
				for (DWORD i=0; i<size7E; ++i)
				{
					rxLen = CRCChecker.Push (*pPtr++);
				}

				if (rxLen > 0) // this is a complete response
				{
					CBufferPtr ptBufferTemp = new CBuffer(rxLen, (const char*)CRCChecker.GetBuffer());
					pPtr = (BYTE*)ptBufferTemp->GetData();

					if (memcmp (pPtr, &resp, sizeof(diag_gps_SVtrack_IO_rsp)-1) == 0) //dont compare the "status" field
					{
						// this is the response, check for status
						diag_gps_SVtrack_IO_rsp* pResponse = (diag_gps_SVtrack_IO_rsp*)pPtr;
						if (pResponse->status == 0)
						{
							m_bSuccess = true;
							if (3==m_CmdType || 4==m_CmdType) //stop command
							{
								bReturn = true;
								break;
							}
						}
						else
						{
							bReturn = true; // to end IsCompletePacket 
							break;
						}
					}
					else if (pPtr[0] == 0x10) // this is the log we want
					{
						logresponse *pLogResp = (logresponse*) pPtr; 
						if (pLogResp->logCode == 0x1371)//check for 1371 packet here
						{
							pPtr += 16;// shift 16 byte
							// 28 byte header
							int SVScount = ((log_code_1371_header*)pPtr)->SVSnumber;
							pPtr += sizeof(log_code_1371_header);

							// only analyze the first SV
							log_SV *pSVlog = (log_SV*)pPtr;
							m_lastCNO = pSVlog->CNO;
							if (pSVlog->CNO >= m_wThreshold)
							{
								bReturn = true;// to end IsCompletePacket
								break;
							}
						}
					}

					// we dont need the response anymore after analyzing
					m_ptTempBufferReceived->PopFront (size7E);
				}
				else
				{
					break; //not a complete packet, maybe next call.
				}


			} //while (1);

			return bReturn;
		}
		else
		{
			return CDiagRequestBase::IsCompletePacket (ptBufferReceived);
		}
	}


	/******************************************************/
	/*	GPS   CGPSTrackingSwitchRequest                   */
	/******************************************************/

	CGPSTrackingSwitchRequest::CGPSTrackingSwitchRequest(bool &bSuccess, bool bOn)
		: CDiagRequestBase(_T("CGPSTrackingSwitchRequest"))
		, m_bSuccess(bSuccess)
	{
		// 75 13 112 0 switch

		m_bSuccess = false;

		CBufferPtr ptBufferSend;

		BYTE req[5] = {75, 13, 112, 0, 0};

		if (bOn)
			req[4] = 1;

		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGPSTrackingSwitchRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		// 75 13 112 0 result

		BYTE resp[5] = {75, 13, 112, 0, 0};

		Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (resp[4] == 1)
			m_bSuccess = true;
		else
			m_bSuccess = false;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}


	/******************************************************/
	/*	GPS   CGPSPollingTTFFCN0Request                   */
	/******************************************************/

	CGPSPollingTTFFCN0Request::CGPSPollingTTFFCN0Request(bool &bSuccess, int& iTTFF, int& iCN0)
		: CDiagRequestBase(_T("CGPSPollingTTFFCN0Request"))
		, m_bSuccess(bSuccess)
		, m_iTTFF(iTTFF)
		, m_iCN0(iCN0)
	{
		// 75 13 113 0

		m_bSuccess = false;

		CBufferPtr ptBufferSend;

		BYTE req[4] = {75, 13, 113, 0};

		Encode(req, sizeof(req), ptBufferSend);
		SetSendData(ptBufferSend);
	}

	void CGPSPollingTTFFCN0Request::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		// 75 13 113 0 result TTFF[0] TTFF[1] TTFF[2] TTFF[3] CN0[0] CN0[1]

		BYTE resp[11] = {75, 13, 113, 0, 0, 0, 0, 0, 0, 0, 0};

		Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());

		if (resp[4] == 1)
		{
			m_bSuccess = true;

			m_iTTFF = resp[5] + resp[6] * 256 + resp[7] * 256 * 256 + resp[8] * 256 * 256 * 256;
			m_iCN0 = resp[9] + resp[10] * 256;
		}
		else
		{
			m_bSuccess = false;

			m_iTTFF = 0;
			m_iCN0 = resp[9] + resp[10] * 256;
		}

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}



	/******************************************************/
	/*	GNSS RF Dev Multichannel Tracking command         */
	/******************************************************/

	CGPS_RF_Dev_Multichannel_TrackingRequest::CGPS_RF_Dev_Multichannel_TrackingRequest(bool &bSuccess, int nCommand)
		: CDiagRequestBase(_T("CGPS_RF_Dev_Multichannel_TrackingRequest"))
		, m_bSuccess(bSuccess)
	{
		m_bSuccess = false;

		//Single SV Track #1 (clear start)
		//Sendrawrequest 0x4B 0x0D 0x65 0x00 27 50 0 0 1 0 1 1 1 1 0

		CBufferPtr ptBufferSend;
		BYTE req[15] = {75, 13, 101, 0, 27, 50, 0, 0, 1, 0, 1, 1, 1, 1, 0};
		req[14] = nCommand;

		Encode (req, sizeof(req), ptBufferSend);
		SetSendData (ptBufferSend);
	}

	void CGPS_RF_Dev_Multichannel_TrackingRequest::OnDataReceiver(CBufferPtr &ptBufferReceived)
	{
		BYTE resp[6] = {75, 13, 101, 0, 27, 0};

		// byte[5] = 0 success, 1 failed
		Safememcopy((void*)&resp, sizeof(resp), (void*)ptBufferReceived->GetData(), ptBufferReceived->GetLength());
		m_bSuccess =  (0 == resp[5]);
		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}









} // End of namespace PLATFORM_TOOL
