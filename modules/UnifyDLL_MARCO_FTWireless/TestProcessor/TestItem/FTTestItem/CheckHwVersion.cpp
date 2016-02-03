/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .cpp file of CCheckHwVersion object.
 * FileName   : CheckHwVersion.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Evonne.Hsieh
 * Time       : 2013/06/05
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/06/05    Evonne.Hsieh            Create for Boston BB test
 ********************************************************************************/

#include "stdafx.h"
#include "CheckHwVersion.h"
#include <map>


RF_IMPLEMENT_DYNCREATE(CCheckHwVersion)


bool CCheckHwVersion::PreRun()
{
	m_strResult = "-";
	m_strItemCode = "-";
	m_strErrorCode = CommErr_Check_HW_ID_Fail;
	m_strBand = "-";
	m_strChannel = "-";
	m_strUpper = "-";
	m_strLower = "-";
	m_strMeasured = "";
	m_strUnit = "-";
	m_strMessage = "";

	return true;
}

bool CCheckHwVersion::Run()
{
	return CheckHwVersion();
}

bool CCheckHwVersion::PostRun()
{
	return true;
}

bool CCheckHwVersion::InitData(std::map<std::string, std::string>& paramMap)
{
	if (paramMap.find(std::string("HWVersion")) != paramMap.end())
	{
		m_str_HwVersion = paramMap[std::string("HWVersion")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get HW version for class 'CCheckHwVersion'"));
		return false;
	}

	if (paramMap.find(std::string("CheckStation")) != paramMap.end())
	{
		m_str_CheckStation = paramMap[std::string("CheckStation")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get CheckStation for class 'CCheckHwVersion'"));
		return false;
	}

	if (paramMap.find(std::string("ModelName")) != paramMap.end())
	{
		m_str_ModelName = paramMap[std::string("ModelName")];
	}
	else
	{
		TraceLog(MSG_INFO, _T("Failed to Get ModelName for class 'CCheckHwVersion'"));
		return false;
	}

	return true;
}

bool CCheckHwVersion::CheckHwVersion()
{
	bool b_res = true;
	char sz_HwVersion[64] = {0};
	std::string str_msg;

	//[A][Hardware variant] ¡V FTD command 17-1 (NFC chip id)
	char sz_InData[FTD_BUF_SIZE] = {0};
	char sz_OutData[FTD_BUF_SIZE] = {0};
	if (m_str_ModelName.compare("Saporro") != 0)
	{
		if (!(b_res = m_pIPhone->FTD_NFCid(m_nFtdPort, m_nFtdTimeOut, sz_InData, sz_OutData)))
		{
			str_msg = _T("Read NFC chip id from mobile fail");
			TraceLog(MSG_INFO, str_msg);
		}
	}
	/* Boston checks NFC or Non-NFC */
	/* Saporro checks hardware variant*/
	if (b_res)
	{
		if (m_str_ModelName.compare("Saporro") != 0)
		{
			if (strncmp(sz_OutData, "0,0", 3) == 0)
			{
				// Non-NFC 0,0
				memcpy(sz_HwVersion, "2", 1);
			}
			else if (strncmp(sz_OutData, "81,177", 6) == 0)
			{
				// NFC 81,177
				memcpy(sz_HwVersion, "1", 1);
			}
		}
		else
		{
			memcpy(sz_HwVersion, "1", 1);
		}

		//[B][Hardware PCB Stage] ¡V FTD command 21-2 (Board id)
		//[C][Major HW Stage Serial Number] -- FTD command 21-2
		//[D][Minor HW Stage Serial Number] ¡VFTD command 21-2
		memset(sz_OutData, 0, FTD_BUF_SIZE);
		if (!(b_res = m_pIPhone->FTD_BoardADC(m_nFtdPort, m_nFtdTimeOut, sz_InData, sz_OutData)))
		{
			str_msg = _T("Read board id from mobile fail");
			TraceLog(MSG_INFO, str_msg);
		}
		if (b_res)
		{
			std::map<UINT32, string> StageBoardIdMap;
			StageBoardIdMap[0x100000]	= "EVB0_1";
			StageBoardIdMap[0x101000]	= "EVB1_1";
			StageBoardIdMap[0x102000]	= "EVB2_1";
			StageBoardIdMap[0x200000]	= "EVT0_1";
			StageBoardIdMap[0x201000]	= "EVT1_1";
			StageBoardIdMap[0x01201000] = "EVT1_1";
			StageBoardIdMap[0x201020]	= "EVT1_2";
			StageBoardIdMap[0x201030]	= "EVT1_3";
			StageBoardIdMap[0x202000]	= "EVT2_1";
			StageBoardIdMap[0x01202000]	= "EVT2_1";
			StageBoardIdMap[0x202020]	= "EVT2_2";
			StageBoardIdMap[0x202030]	= "EVT2_3";
			StageBoardIdMap[0x300000]	= "DVT0_1";
			StageBoardIdMap[0x01300000]	= "DVT0_1";
			StageBoardIdMap[0x301000]	= "DVT1_1";
			StageBoardIdMap[0x400000]	= "PVT0_1";
			StageBoardIdMap[0x7fffffff] = "unknown";

			std::map<string, char> StageMap;
			StageMap["EVB"] = 'B';
			StageMap["EVT"] = 'E';
			StageMap["DVT"] = 'D';
			StageMap["PVT"] = 'P';
			StageMap["MP"] = 'M';
			StageMap["unk"] = 'U';

			CString sBoardId = sz_OutData;
			int nBoardId = atoi(sBoardId);
			std::map<UINT32, string>::iterator it_board_id = StageBoardIdMap.begin();
			for (it_board_id = StageBoardIdMap.begin(); it_board_id != StageBoardIdMap.end(); ++it_board_id)
			{
				if (it_board_id->first == nBoardId && it_board_id != StageBoardIdMap.end())
				{
					std::string PcbStage = it_board_id->second;
					std::string stage = PcbStage.substr(0, 3);
					std::map<string, char>::iterator it_stage = StageMap.begin();
					for (it_stage = StageMap.begin(); it_stage != StageMap.end(); ++it_stage)
					{
						if (strcmp(it_stage->first.c_str(), stage.c_str()) == 0)
						{
							char sz_HwStageType[10] = {0};
							memcpy(sz_HwStageType, &it_stage->second, sizeof(it_stage->second));
							strcat(sz_HwVersion, sz_HwStageType);
							break;
						}
					}
					// extract major & mimor HW stage serial number
					std::string major = PcbStage.substr(3, 1);
					std::string minor = PcbStage.substr(5, 1);
					strcat(sz_HwVersion, major.c_str());
					strcat(sz_HwVersion, minor.c_str());
					break;
				}
			}


			//[E][DDR Memory] ¡V FTD command 3-2
			/************************************
			*SAMSUNG_DDR 0x1,
			*ELPIDA_DDR 0x3,
			*HYNIX_DDR 0x6,
			*MICRON_DDR 0xff,
			*UNKNOWN_DDR_VENDOR 0x7fffffff,
			*************************************/
			memset(sz_OutData, 0, FTD_BUF_SIZE);
			if (!(b_res = m_pIPhone->FTD_MCPddrID(m_nFtdPort, m_nFtdTimeOut, sz_InData, sz_OutData)))
			{
				str_msg = _T("Read ddr id from mobile fail");
				TraceLog(MSG_INFO, str_msg);
			}
			if (b_res)
			{
				std::map<UINT32, char> DDRMap;
				DDRMap[0x7fffffff] = '0';
				DDRMap[0xff] = '1';
				DDRMap[0x6] = '2';
				DDRMap[0x3] = '3';

				CString sDdrId = sz_OutData;
				int nDdrId = atoi(sDdrId);
				std::map<UINT32, char>::iterator it_ddr = DDRMap.begin();
				for (it_ddr = DDRMap.begin(); it_ddr != DDRMap.end(); ++it_ddr)
				{
					if (it_ddr->first == nDdrId)
					{
						char sz_HwDdrType[10] = {0};
						memcpy(sz_HwDdrType, &it_ddr->second, sizeof(it_ddr->second));
						strcat(sz_HwVersion, sz_HwDdrType);
						break;
					}
				}

				//[F][eMMC] ¡V FTD command 3-1
				/************************************
				*SANDISK_EMMC 0x45,
				*KINGSTON_EMMC 0x70,
				*SAMSUNG_EMMC 0x15,
				*MICRON_EMMC 0xFE,
				*UNKNOWN_DDR_VENDOR 0x7fffffff,
				*************************************/
				memset(sz_OutData, 0, FTD_BUF_SIZE);
				if (!(b_res = m_pIPhone->FTD_MCPflashID(m_nFtdPort, m_nFtdTimeOut, sz_InData, sz_OutData)))
				{
					str_msg = _T("Read flash id from mobile fail");
					TraceLog(MSG_INFO, str_msg);
				}
				if (b_res)
				{
					std::map<UINT32, char> FlashMap;
					if (m_str_ModelName.compare("Saporro") != 0)
					{
						FlashMap[0x7fffffff] = '0';
						FlashMap[0xFE] = '1';
						FlashMap[0x70] = '2';
						FlashMap[0x45] = '3';
						FlashMap[0x15] = '4';
					}
					else
					{
						FlashMap[0x7fffffff] = '0';
						FlashMap[0x15] = '1';
						FlashMap[0x15] = '1';
						FlashMap[0x45] = '3';
					}

					CString sFlashId = sz_OutData;
					int nFlashId = atoi(sFlashId);
					std::map<UINT32, char>::iterator it_flash = FlashMap.begin();
					for (it_flash = FlashMap.begin(); it_flash != FlashMap.end(); ++it_flash)
					{
						if (it_flash->first == nFlashId)
						{
							char sz_HwFlashType[10] = {0};
							memcpy(sz_HwFlashType, &it_flash->second, sizeof(it_flash->second));
							strcat(sz_HwVersion, sz_HwFlashType);
							break;
						}
					}

					// distinguish T-line & A-line
					if (strstr(m_str_CheckStation.c_str(), "T") == NULL)
					{
						//[G][Camera Module] ¡V FTD command 6-1
						memset(sz_OutData, 0, FTD_BUF_SIZE);
						if (!(b_res = m_pIPhone->FTD_CAMReadID(m_nFtdPort, m_nFtdTimeOut, "front", sz_OutData)))
						{
							str_msg = _T("Read camera front id from mobile fail");
							TraceLog(MSG_INFO, str_msg);
						}
						if (b_res)
						{
							char sz_CameraModule[FTD_BUF_SIZE] = {0};
							memcpy(sz_CameraModule, sz_OutData, strlen(sz_OutData));
							strcat(sz_CameraModule, ",");
							memset(sz_OutData, 0, FTD_BUF_SIZE);
							if (!(b_res = m_pIPhone->FTD_CAMReadID(m_nFtdPort, m_nFtdTimeOut, "back", sz_OutData)))
							{
								str_msg = _T("Read camera back id from mobile fail");
								TraceLog(MSG_INFO, str_msg);
							}
							if (b_res)
							{
								strcat(sz_CameraModule, sz_OutData);

								bool b_isCameraModuleFound = false;
								std::map<string, char> CameraModuleMap;
								CameraModuleMap["9345,34853,0"] = '1';
								CameraModuleMap["9814,34853,0"] = '2';
								std::map<string, char>::iterator it_camera = CameraModuleMap.begin();
								for (it_camera = CameraModuleMap.begin(); it_camera != CameraModuleMap.end(); ++it_camera)
								{
									if (strcmp(it_camera->first.c_str(), sz_CameraModule) == 0)
									{
										char sz_HwCameraType[10] = {0};
										memcpy(sz_HwCameraType, &it_camera->second, sizeof(it_camera->second));
										strcat(sz_HwVersion, sz_HwCameraType);
										b_isCameraModuleFound = true;
										break;
									}
								}
								if (!b_isCameraModuleFound)
								{
									strcat(sz_HwVersion, "0");
								}

								//[H][TP+LCM] ¡V FTD command 4-10
								memset(sz_OutData, 0, FTD_BUF_SIZE);
								if (!(b_res = m_pIPhone->FTD_TouchModel(m_nFtdPort, m_nFtdTimeOut, sz_InData, sz_OutData)))
								{
									str_msg = _T("Read touch model from mobile fail");
									TraceLog(MSG_INFO, str_msg);
									b_res = false;
								}
								if (b_res)
								{
									if (strcmp(sz_OutData, "JTOUCH") == 0)
									{
										strcat(sz_HwVersion, "1");
									}
								}
							}
						}
					}
				}
			}
		}
	}

	m_strMeasured = sz_HwVersion;
	if (b_res)
	{
		//compare with XML file
		b_res = (strstr(sz_HwVersion, m_str_HwVersion.c_str()) != NULL);
	}

	/* test result */
	if(b_res)
	{
		m_strResult = "PASS";
		m_strErrorCode = "-";
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, _T("Check HW version PASS!"));
		SetPICSData(_T("CHECK_HW_VERSION"), m_strMeasured);
	}
	else
	{
		m_strResult = "FAIL";
		str_msg += _T("version = ");
		str_msg += m_str_HwVersion;
		m_strMessage = str_msg;
		TraceLog(MSG_INFO, _T("Check HW version FAIL!"));
		SetPICSData(_T("CHECK_HW_VERSION"), m_strMeasured);
	}
	FactoryLog();

	return b_res;
}

string CCheckHwVersion::ltrim(const string &str)
{
	size_t at = str.find_first_not_of(" \t\r\n\0\a\b\f\v");
	return at == string::npos ? str : str.substr(at);
}

string CCheckHwVersion::rtrim(const string &str)
{
	size_t at = str.find_last_not_of(" \t\r\n\0\a\b\f\v");
	return at == string::npos ? str : str.substr(0, at+1);
}

string CCheckHwVersion::trim(const string &str0)
{
	string str = str0;
	size_t at2 = str.find_last_not_of(" \t\r\n\0\a\b\f\v");
	size_t at1 = str.find_first_not_of(" \t\r\n\0\a\b\f\v");
	if (at2 != string::npos) str.erase(at2+1);
	if (at1 != string::npos) str.erase(0,at1);
	return str;
}