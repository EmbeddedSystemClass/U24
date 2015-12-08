/***************************************************************************
* Copyright (C) : QCS/WR318                                      
* Module name   : Common Define value   
* File name     : Errorcode.h             
* Summary       : define error code                                                
* Version     Author             Date                Abstract                      
* 1.0         Nancy.Yang         2011/07/14          First version  
* 1.1         Luffy.Liu          2011/10/13          modify
****************************************************************************/
#pragma once

/*==================== ErrorCode ==========================*/
//**UI related
#define CommErr_UI_Unknown_Error_From_DLL			_T("UI000000")
#define CommErr_UI_Init_Diag_Fail					_T("UI000001")
#define CommErr_UI_DLL_Load_Fail					_T("UI000002")
#define CommErr_UI_Config_File_Not_Found			_T("UI000003")
#define CommErr_UI_Config_File_Miss_Parameter		_T("UI000004")
#define CommErr_UI_Init_TestItem_Fail				_T("UI000005")
#define CommErr_UI_Run_Thread_Fail				    _T("UI000006")
//UI cim related
#define CommErr_UI_CIM_MobileID_Match_Fail			_T("UI001001")
#define CommErr_UI_CIM_SO_CR_Count_Over_Upper		_T("UI001002")
#define CommErr_UI_CIM_SO_NULL						_T("UI001003")
#define CommErr_UI_CIM_SMT_Picasso_Not_Exist		_T("UI001004")
#define CommErr_UI_CIM_Picasso_Locked				_T("UI001005")
#define CommErr_UI_CIM_Picasso_Status_Invalide		_T("UI001006")
#define CommErr_UI_CIM_Picasso_Match_Phone_Fail		_T("UI001007")
#define CommErr_UI_CIM_Routing_Check_Fail			_T("UI001008")
#define CommErr_UI_CIM_Connect_Fail					_T("UI001009")
#define CommErr_UI_CIM_Picasso_QA_Hold				_T("UI001010")
#define CommErr_UI_CIM_Pics_Clean_Fail				_T("UI001011")
#define CommErr_UI_CIM_Interface_Get_Fail			_T("UI001012")
#define CommErr_UI_CIM_Picasso_Match_IMEI_Fail		_T("UI001013")
#define CommErr_UI_CIM_Tool_Version_Invalide		_T("UI001014")
#define CommErr_UI_CIM_IMEI_Status_Invalide			_T("UI001015")
#define CommErr_UI_CIM_EmployeeID_Not_TS			_T("UI001016")
#define CommErr_UI_CIM_Phone_Init_Not_Record		_T("UI001017")
#define CommErr_UI_CIM_Phone_Reocrd_Not_Init		_T("UI001018")
//UI pvcs related
#define CommErr_UI_PVCS_ParamList_Invalide			_T("UI002001")
#define CommErr_UI_PVCS_PN_Segment_Match_Fail		_T("UI002002")
#define CommErr_UI_PVCS_ImageList_Empty_Fail		_T("UI002003")
//UI input related
#define CommErr_UI_Picasso_Invalide					_T("UI003001")
#define CommErr_UI_Picasso_Not_Input				_T("UI003002")
#define CommErr_UI_Picasso_Duplicated				_T("UI003003")
#define CommErr_UI_Picasso_Match_Phone_Fail			_T("UI003004")
#define CommErr_UI_IMEI_Not_Input					_T("UI003005")
#define CommErr_UI_IMEI_Rule_Check_Fail				_T("UI003006")
#define CommErr_UI_IMEI_First_Part_Fail				_T("UI003007")
#define CommErr_UI_NAL_Not_Input					_T("UI003008")
#define CommErr_UI_NAL_Rule_Check_Fail				_T("UI003009")
#define CommErr_UI_FA_Model_Invalide				_T("UI003010")
#define CommErr_UI_FA_SWVersion_Invalide			_T("UI003011")
#define CommErr_UI_ACER22_Not_Input                 _T("UI003012")
#define CommErr_UI_ACER22_Rule_Check_Fail           _T("UI003013")
#define CommErr_UI_Picasso_Scaned_Fail              _T("UI003014")
#define CommErr_UI_ID_Invalid                       _T("UI003015")
#define CommErr_UI_TAG_Not_Input				_T("UI003016")
//UIDLpage
#define CommErr_UI_DL_Image_Not_Exist				_T("UI004001")
//UIinterface related
#define CommErr_UI_Interface_Prerun_Fail			_T("UI005001")
#define CommErr_UI_Interface_Setparameter_Fail		_T("UI005002")
#define CommErr_UI_Interface_Getparameter_Fail		_T("UI005003")
#define CommErr_UI_Interface_GetFA_Fail				_T("UI005004")
#define CommErr_UI_Interface_SetFA_Fail				_T("UI005005")
#define CommErr_UI_Interface_Begin_Fail				_T("UI005006")
//UI power supply
#define CommErr_UI_PowerSupply_On_Fail				_T("UI006001")
#define CommErr_UI_PowerSupply_Off_Fail				_T("UI006002")
#define CommErr_UI_PowerSupply_Detect_Fail			_T("UI006003")
//UI jig control
#define CommErr_UI_Jig_Open_Fail					_T("UI007001")
#define CommErr_UI_Jig_Close_Fail					_T("UI007002")
#define CommErr_UI_Jig_Com_Open_Fail				_T("UI007003")
#define CommErr_UI_Jig_Data_Send_Fail				_T("UI007004")
//UI relayboard
#define CommErr_UI_RelayBoard_Init_Fail				_T("UI008001")
#define CommErr_UI_RelayBoard_Set_Fail				_T("UI008002")
//UI batch command
#define CommErr_UI_Cmd_Fail							_T("UI009001")
#define CommErr_UI_Cmd_Time_Syn_Fail				_T("UI009002")
//UI accessory related
#define CommErr_UI_Accessory_UsedTimes_Check_Fail	_T("UI010001")
#define CommErr_UI_Accessory_No_Define_MaxTimes		_T("UI010002")
/*==================== ErrorCode ==========================*/