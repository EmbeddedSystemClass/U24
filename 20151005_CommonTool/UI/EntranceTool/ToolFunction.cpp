#include "StdAfx.h"
#include "ToolFunction.h"
#include "io.h"

#pragma comment(lib, "Version.lib")

CToolFunction::CToolFunction(void)
{
	this->m_strEntranceFixTag = "";
	this->m_strEntranceVersion = "";
	this->m_strFImageFixTag = "";
	this->m_strFImageList = "";
	this->m_strFImageVersion = "";
	this->m_strOImageVersion = "";
	this->m_strProcess = "";
	this->m_strProject = "";
	this->m_strSegment = "";
	this->m_strSO = "";
	this->m_strSOModel = "";
	this->m_strSOPartNo = "";
	this->m_strSOType = "";
	this->m_strToolVersion = "";

}

CToolFunction::~CToolFunction(void)
{
}

bool CToolFunction::GetSOInfoFromDB(char* chSOID, char* szErrorCode, char* szErrorMsg)
{
	bool bRes = false;

	this->m_strSO = chSOID;

	char szToolv[100] = {0};
	char szFV[100] = {0};
	char szOV[100] ={0};
	char szEV[100] = {0};
	char szFF[10] = {0};
	char szEF[10] = {0};
	char szModel[50] ={0};
	char szPartNo[50] = {0};
	char szSOType[50] ={0};
	char sz_Qty[50] ={0};
	char sz_CRtype[50] = {0};
	char sz_segment[50] ={0};

	m_Clog.WriteLog(_T("����SO�ڱ�SO_INFO�еļ�¼ -- ��ʼ!"));
	if(ConnectDB())
	{
		bRes = QuerySO_Info_All_V5(chSOID, szModel, szPartNo, szSoType, szToolv, szFV, szOV, szEV, sz_Qty, sz_CRtype, sz_segment,szEF, szFF, szErrorCode, szErrorMsg);
		CloseDB();
		if(b_res)
		{
			m_Clog.WriteLog(_T("����SO�ڱ�SO_INFO�еļ�¼ -- �ɹ�!"));
			m_strSOModel = szModel;
			m_strToolVersion = szToolv;
			m_strFImageVersion = szFV;
			m_strOImageVersion = szOV;
			m_strEntranceVersion = szEV;
			m_strFImageFixTag = szFF;
			m_strEntranceFixTag = szEF;
			m_strSoType = sz_CRtype;
			m_strSegment = sz_segment;
			m_strSOPartNo = szPartNo;
		}
		else
		{
			m_Clog.WriteLog(_T("����SO�ڱ�SO_INFO�еļ�¼ -- ʧ��! -- ��ѯSO ʧ��!"), szErrorCode, szErrorMsg);
		}
	}
	else
	{
		strcpy(szErrorCode, _T("ET000001"));
		strcpy(szErrorMsg, _T("�������ݿ�ʧ��!"));
		m_Clog.WriteLog(_T("����SO�ڱ�SO_INFO�еļ�¼ -- ʧ��! -- ����DB ʧ��!"));
	}
	m_Clog.WriteLog(_T("����SO�ڱ�SO_INFO�еļ�¼ -- ����!"));
	return bRes;
}

bool CToolFunction::CheckModel(char* chModel , char* szErrorCode , char* szErrorMsg)
{
	m_Clog.WriteLog(_T("���Model Name ��ʼ!"));
	bool bRes = false;
	if(m_strSOModel.empty())
	{
		strcpy_s(szErrorCode, _T("ET000101"));
		strcpy_s(szErrorMsg, _T("������Model NameΪ�գ���Ч����!"));
		m_Clog.WriteLog(_T("������Model NameΪ�գ���Ч����!"));
	}
	else
	{
		if(m_strSOModel.compare(chModel) == 0) //Same
		{
			bRes = true;
			m_Clog.WriteLog(_T("Model Name UIѡ��͹����е�һ��! ")+m_strSOModel);
		}
		else
		{
			strcpy(szErrorCode, _T("ET000100"));
			strcpy(szErrorMsg, _T("Model Name UIѡ��͹����еĲ�һ��!"));
			m_Clog.WriteLog(_T("Model Name UIѡ��͹����еĲ�һ��! ����Ϊ��")+m_strSOModel, szErrorCode, szErrorMsg);
		}
	}
	m_Clog.WriteLog(_T("���Model Name ����!"));
	return bRes;
}

bool CToolFunction::CheckVRCMStatus(char* szErrorCode, char* szErrorMsg)
{
	bool bRes = false;
	m_Clog.WriteLog(_T("����PART NUM , IMAGE , TOOL VERSION �ڱ�VRCM�еļ�¼��ͬʱ�鿴statusֵ -- ��ʼ!"));

	char sz_status[10]={0};
	char sz_record_status[10]={0};
	if(VMS_ConnectDB())
	{
		bRes = VMS_QueryImagePTToolVersionStatus(m_strSOModel.c_str(), m_strSOPartNo.c_str(), m_strFImageVersion.c_str(),m_strOImageVersion.c_str(),m_strToolVersion.c_str(),sz_status, sz_record_status, szErrorCode, szErrorMsg);
		VMS_CloseDB();
		if(bRes)
		{
			m_Clog.WriteLog(_T("����PART NUM , IMAGE , TOOL VERSION �ڱ�VRCM�еļ�¼��ͬʱ�鿴statusֵ -- �ɹ�!"));
			if (strcmp(sz_status,"1")!=0 || strcmp(sz_record_status,"2")!=0)
			{
				strcpy(sz_ErrCode,_T("ET000102"));
				strcpy(sz_ErrMsg,_T("��VRCM�����ҵ���Ӧ�ļ�¼����statusֵ��Ϊ1��Record_status��Ϊ2���޷���������"));
				bRes = false;
			}
		}
		else
		{
			if (strcmp(sz_ErrCode,"NoRecord")==0)
			{   
				memset(sz_ErrCode,0,ERRORNUMBER);
				m_Clog.WriteLog("No record in VRCM, skip checking VRCM");
				bRes = true;
			}
			else
				m_Clog.WriteLog(_T("����PART NUM , IMAGE , TOOL VERSION �ڱ�VRCM�еļ�¼��ͬʱ�鿴statusֵ -- ʧ��! ��ѯʧ��"),szErrorCode,szErrorMsg);
		}			
	}
	else
	{
		strcpy(szErrorCode, _T("ET000001"));
		strcpy(szErrorMsg, _T("�������ݿ�ʧ��!"));
		m_Clog.WriteLog(_T("����PART NUM , IMAGE , TOOL VERSION �ڱ�VRCM�еļ�¼��ͬʱ�鿴statusֵ -- ʧ��! --����DBʧ��"),szErrorCode,szErrorMsg);
	}
	m_Clog.WriteLog(_T("����PART NUM , IMAGE , TOOL VERSION �ڱ�VRCM�еļ�¼��ͬʱ�鿴statusֵ -- ����!"));

	if(bRes && (m_strSegment == _T("FA"))) 
	{
		m_Clog.WriteLog(_T("FA����ʹ�õ�Factory Image����PN/ToolV -- ��ʼ!"));
		if(VMS_ConnectDB())
		{
			b_res = VMS_QueryVRCMRImageList(m_strSOModel.c_str(), m_strSOPartNo.c_str(),m_strToolVersion.c_str(), m_strFImageVersion.c_str(), szErrorCode,szErrorMsg);
			VMS_CloseDB();
			if(bRes)
			{
				m_Clog.WriteLog(_T("FA����ʹ�õ�Factory Image����PN/ToolV -- �ɹ�!"));
				m_strFImageList = szFV;
				m_Clog.WriteLog(_T("FactoryImage List : " ) + m_strFImageList);
			}
			else
			{
				m_Clog.WriteLog(_T("FA����ʹ�õ�Factory Image����PN/ToolV -- ʧ��! ��ѯʧ��"), szErrorCode,szErrorMsg);
			}			
		}
		else
		{
			strcpy(szErrorCode, _T("ET000001"));
			strcpy(szErrorMsg, _T("�������ݿ�ʧ��!"));
			m_Clog.WriteLog(_T("FA����ʹ�õ�Factory Image����PN/ToolV -- ʧ��! --����DBʧ��"), szErrorCode,szErrorMsg);
			bRes = false;
		}
		m_Clog.WriteLog(_T("FA����ʹ�õ�Factory Image����PN/ToolV -- ����!"));
	}	
	return bRes;
}

bool CToolFunction::CheckVesionStatus(char* szErrorCode, char* szErrorMsg)
{
	bool bRes = false;

	char m_sz_Status_factory[50] ={0};
	char m_sz_Status_tool[50] ={0};
	char m_sz_Status_OS[50] ={0};
	char m_sz_StartTime_tool[50] ={0};
	char m_sz_StartTime_factory[50] ={0};
	char m_sz_StartTime_OS[50] ={0};
	char m_sz_EndTime_tool[50] ={0};
	char m_sz_EndTime_factory[50] ={0};
	char m_sz_EndTime_OS[50] ={0};

	m_Clog.WriteLog(_T("���汾״̬�͹���״̬ -- ��ʼ"));
	if(m_strFImageVersion.compare(_T("")) != 0)
	{
		if(VMS_ConnectDB())
		{
			bRes = VMS_QueryFactoryImageStatus(m_strSOModel.c_str(),m_strFImageVersion.c_str(),m_sz_Status_factory,m_sz_StartTime_factory,m_sz_EndTime_factory,szErrorCode,szErrorMsg);
			VMS_CloseDB();
			if(!bRes)
			{   
				m_Clog.WriteLog(_T("VMSϵͳ��ѯfactoryImage����fail"),szErrorCode,szErrorMsg);
			}
		}
		else
		{
			bRes = false;
			strcpy(szErrorCode, _T("ET000001"));
			strcpy(szErrorMsg, _T("�������ݿ�ʧ��!"));
			m_Clog.WriteLog(_T("Connect VMS Server fail\n����VMS ���ݿ�ʧ��"),szErrorCode,szErrorMsg);
		}
	}
	if(bRes && (m_strToolVersion.compare(_T("")) != 0))
	{
		if(VMS_ConnectDB())
		{
			bRes = VMS_QueryToolStatus(m_strSOModel.c_str(),m_strToolVersion.c_str(),m_sz_Status_tool,m_sz_StartTime_tool,m_sz_EndTime_tool,szErrorCode,szErrorMsg);
			VMS_CloseDB();
			if(!bRes)
			{   
				m_Clog.WriteLog(_T("VMSϵͳ��ѯToolVersion Status����fail"),szErrorCode,szErrorMsg);
				return false;
			}
		}
		else
		{
			bRes = false;
			strcpy(szErrorCode, _T("ET000001"));
			strcpy(szErrorMsg, _T("�������ݿ�ʧ��!"));
			m_Clog.WriteLog(_T("Connect VMS Server fail\n����VMS ���ݿ�ʧ��"),szErrorCode,szErrorMsg);
		}
		
	}
	if(bRes && (m_strSegment.compare(_T("PACKAGE")) == 0) && (m_strOImageVersion.compare(_T("")) != 0) && (m_strProcess.compare(_T("RE_DL")) != 0))
	{
		char *pi;
		char sz_count[10]={0};
		char sz_9Jinfo[4096]={0};

		if(VMS_ConnectDB())
		{
			bRes = VMS_QuerySQNinfo_All(m_strSOModel.c_str(),m_strSOPartNo.c_str(),m_strOImageVersion.c_str(),sz_count,sz_9Jinfo,szErrorCode,szErrorMsg);
			VMS_CloseDB();
			if(bRes)
			{
				pi = strstr(sz_9Jinfo,"SQN_release_status");
				for (int i=0;i<25;i++)
				{  
					if(i==19)
						sz_info[0]=*(pi+i);

				}				
			}
			else
			{
				m_Clog.WriteLog(_T("VMSϵͳ��ѯSQN����fail"),szErrorCode,szErrorMsg);
			}
		}
		else
		{
			bRes = false;
			strcpy(szErrorCode, _T("ET000001"));
			strcpy(szErrorMsg, _T("�������ݿ�ʧ��!"));
			m_Clog.WriteLog(_T("Connect VMS Server fail\n����VMS ���ݿ�ʧ��"),szErrorCode,szErrorMsg);
		}

	}

	if(b_res)
	{
		if(m_strSOType.IsEmpty())
		{   
			m_Clog.WriteLog(_T("CRTypeΪ��, normal SO"),sz_ErrCode,sz_ErrMsg);
			if((!m_strToolVersion.IsEmpty() && strcmp(m_sz_Status_tool, "1") != 0) || (!m_strFImageVersion.IsEmpty() && strcmp(m_sz_Status_factory, "1") != 0))
			{
				strcpy_s(sz_ErrCode,ERRORNUMBER,_T("ET0101"));
				strcpy_s(sz_ErrMsg,ERRORNUMBER,_T("CRType is empty,means it's normal SO, Tool or Factory Image Status is not 'R'\n CRTypeΪ��,����������Tool��Factory Image Status״̬��ΪR"));
				return false;
			}
			if(!m_strOImageVersion.IsEmpty()&&(strProcess != _T("RE_DL")))
			{
				if(strcmp(m_sz_Status_OS,"2") != 0)
				{
					strcpy_s(sz_ErrCode,ERRORNUMBER,_T("ET0104"));
					strcpy_s(sz_ErrMsg,ERRORNUMBER,_T("CRType is empty,and user image status is not 'R' CRTypeΪ��,��������,and user image status״̬��ΪR"));
					return false;
				}
			}
		}
		else
		{   
			
			if(m_strSOType.MakeUpper() == _T("C") || m_strSOType.MakeUpper() == _T("B")|| m_strSOType.MakeUpper() == _T("D"))
			{   
				m_Clog.WriteLog(_T("CRTypeΪC��B��D"),sz_ErrCode,sz_ErrMsg);
				m_Clog.WriteLog(m_str_CRType,sz_ErrCode,sz_ErrMsg);
			}
			else
			{   
				m_Clog.WriteLog(_T("δ����� CRType"),sz_ErrCode,sz_ErrMsg);
				strcpy_s(sz_ErrCode,ERRORNUMBER,_T("ET0100"));
				strcpy_s(sz_ErrMsg,ERRORNUMBER,_T("SOType status is fault\n��SO Typeδ����")));
				return false;
			}
		}
	}
	m_Clog.WriteLog(_T("���汾״̬�͹���״̬ -- ����"));

	return bRes;
}
