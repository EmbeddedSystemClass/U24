
#include "StdAfx.h"
#include "Filename_resolution.h"
#include <tchar.h> 

#include "io.h"

Filename_resolution::Filename_resolution(void)
{
	m_stra_SWEP.RemoveAll();
	m_stra_SWFP.RemoveAll();
	m_stra_TLEU.RemoveAll();
	m_stra_TLFT.RemoveAll();
	m_stra_TLCS.RemoveAll();
	m_stra_SDCT.RemoveAll();
	m_stra_QCNP.RemoveAll();
	m_stra_PLMN.RemoveAll();
	m_stra_SIML.RemoveAll();
	m_stra_APPV.RemoveAll();

	m_stra_SWEP_value.RemoveAll();
	m_stra_SWFP_value.RemoveAll();
	m_stra_TLEU_value.RemoveAll();
	m_stra_TLFT_value.RemoveAll();
	m_stra_TLCS_value.RemoveAll();
	m_stra_SDCT_value.RemoveAll();
	m_stra_QCNP_value.RemoveAll();
	m_stra_PLMN_value.RemoveAll();
	m_stra_SIML_value.RemoveAll();
	m_stra_APPV_value.RemoveAll();

	m_i_SWEP = 0;
	m_i_SWFP = 0;
	m_i_TLEU = 0;
	m_i_TLFT = 0;
	m_i_TLCS = 0;
	m_i_SDCT = 0;
	m_i_QCNP = 0;
	m_i_PLMN = 0;
	m_i_SIML = 0;
	m_i_APPV = 0;
	m_stra[0] = _T("SWEP");
	m_stra[1] = _T("SWFP");
	m_stra[2] = _T("TLEU");
	m_stra[3] = _T("TLFT");
	m_stra[4] = _T("TLCS");
	m_stra[5] = _T("SDCT");
	m_stra[6] = _T("QCNP");
	m_stra[7] = _T("PLMN");
	m_stra[8] = _T("SIML");
	m_stra[9] = _T("APPV");

	for(int i = 0; i < 10; i++)
	{
		m_szbcheck[i] = false;
		m_stra_version[i].RemoveAll();
		m_stra_name[i].RemoveAll();
	}

}

Filename_resolution::~Filename_resolution(void)
{
}

/******************************************************************************
* function name:  GetFilename(m_c_filename)                                  
* Parameters   :  OUT char* filename 
* Summary      :  get filename
* Return       :  True query success.False query fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/12           First version                            
*****************************************************************************/
bool Filename_resolution::GetFilename(TCHAR* Filename)
{
	bool b_res = false;

	return b_res;

}

/******************************************************************************
* function name:  ReadINI(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                                  
* Parameters   :  
* Summary      :  get INI data
* Return       :  True success.False fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/13           First version                            
*****************************************************************************/
bool Filename_resolution::ReadINI(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;
	m_stra_SWEP.RemoveAll();
	m_stra_SWFP.RemoveAll();
	m_stra_TLEU.RemoveAll();
	m_stra_TLFT.RemoveAll();
	m_stra_TLCS.RemoveAll();
	m_stra_SDCT.RemoveAll();
	m_stra_QCNP.RemoveAll();
	m_stra_PLMN.RemoveAll();
	m_stra_SIML.RemoveAll();
	m_stra_APPV.RemoveAll();
	m_i_SWEP = 0;
	m_i_SWFP = 0;
	m_i_TLEU = 0;
	m_i_TLFT = 0;
	m_i_TLCS = 0;
	m_i_SDCT = 0;
	m_i_QCNP = 0;
	m_i_PLMN = 0;
	m_i_SIML = 0;
	m_i_APPV = 0;



	/* 检查ini文件是否存在 */
	int i_existentResult = 0;
	i_existentResult = _taccess(INIPASH, 0);
	if(i_existentResult != 0) 
	{
		//InsertStringIntoList(_T("C001 - .\\INI\\setup.ini is not existent.\r\n"));
		sz_ErrCode = _T("ET0038");
		sz_ErrMsg = _T("配置文件不存在, config file was not exist");
		return false;
	} 
	
	m_i_SWEP = GetPrivateProfileInt(_T("Setting"),_T("SWEP"),0,INIPASH);
//	m_i_SWEP = GetPrivateProfileInt("Setting","SWEP",0,"D:\\INI\\config.ini");
	m_i_SWFP = GetPrivateProfileInt(_T("Setting"),_T("SWFP"),0,INIPASH);
	m_i_TLEU = GetPrivateProfileInt(_T("Setting"),_T("TLEU"),0,INIPASH);
	m_i_TLFT = GetPrivateProfileInt(_T("Setting"),_T("TLFT"),0,INIPASH);
	m_i_TLCS = GetPrivateProfileInt(_T("Setting"),_T("TLCS"),0,INIPASH);
	m_i_SDCT = GetPrivateProfileInt(_T("Setting"),_T("SDCT"),0,INIPASH);
	m_i_QCNP = GetPrivateProfileInt(_T("Setting"),_T("QCNP"),0,INIPASH);
	m_i_PLMN = GetPrivateProfileInt(_T("Setting"),_T("PLMN"),0,INIPASH);
	m_i_SIML = GetPrivateProfileInt(_T("Setting"),_T("SIML"),0,INIPASH);
	m_i_APPV = GetPrivateProfileInt(_T("Setting"),_T("APPV"),0,INIPASH);

	TCHAR c_temp[6];
	memset(c_temp,0,6);
	TCHAR c_Key[3];
	memset(c_Key,0,sizeof(c_Key));
	for(int i = 0; i < m_i_SWEP; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, "%02d", i + 1);
		n_check = GetPrivateProfileString(_T("SWEP"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0039");
		    sz_ErrMsg = _T("配置文件SWEP选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[0].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_SWFP; i++)
	{
		int n_check = 0;
		sprintf_s(c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("SWFP"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0040");
		    sz_ErrMsg = _T("配置文件SWFP选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[1].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_TLEU; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("TLEU"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0041");
		    sz_ErrMsg = _T("配置文件TLEU选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[2].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_TLFT; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("TLFT"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0042");
		    sz_ErrMsg = _T("配置文件TLFT选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[3].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_TLCS; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("TLCS"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0043");
		    sz_ErrMsg = _T("配置文件SWEP选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[4].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_SDCT; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("SDCT"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0044");
		    sz_ErrMsg = _T("配置文件SDCT选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[5].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_QCNP; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("QCNP"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0045");
		    sz_ErrMsg = _T("配置文件QCNP选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[6].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_PLMN; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("PLMN"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0046");
		    sz_ErrMsg = _T("配置文件PLMN选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[7].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_SIML; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("SIML"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0047");
		    sz_ErrMsg = _T("配置文件SIML选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[8].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	for(int i = 0; i < m_i_APPV; i++)
	{
		int n_check = 0;
		sprintf_s (c_Key, _T("%02d"), i + 1);
		n_check = GetPrivateProfileString(_T("APPV"),c_Key,_T(""),c_temp,sizeof(c_temp),INIPASH);
		if(n_check == 0)
		{
			sz_ErrCode = _T("ET0048");
		    sz_ErrMsg = _T("配置文件APPV选项个数和总数不一致,config file has a error");
		}
		else
		{
			m_stra_value[9].Add(c_temp);
		}
		memset(c_temp,0,6);
	}

	return b_res;
}



/******************************************************************************
* function name:  Getfileattributes(TCHAR* Filename, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                                
* Parameters   :  OUT char* filename 
* Summary      :  get filename and version by "_"
* Return       :  True query success.False query fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/12           First version                            
*****************************************************************************/
//SWEP_PRJMGORAB1A_SWVRv01.05.03.r_VART03_BLDN2435_REGNLA_SECUUnsign_CNTRTW_SWRTShipment
bool Filename_resolution::Getfileattributes(TCHAR* Filename, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;
	int i_position = 0;		   // The position of the character in string 
	int i_strLength = 0;       // The length of sz_filename

	if(Filename == NULL)
	{
		return false;
	}
	CString sz_filename = Filename;
	CString sz_filetemp = _T("");
	sz_filename = sz_filename.TrimLeft();
	sz_filename = sz_filename.TrimRight();
	m_stra_fileattributes.RemoveAll();

	while(b_res)
	{
		i_position = sz_filename.FindOneOf(_T("_"));
		if(i_position == -1) {
			// The source string did not have delimiter
			m_stra_fileattributes.Add(sz_filename);		
			break;
		}
		else
		{
			sz_filetemp = sz_filename.Left(i_position);
			m_stra_fileattributes.Add(sz_filetemp);		
			sz_filename = sz_filename.Mid(i_position + 1,sz_filename.GetLength() - i_position);
			b_res = true;
	}
	}


	return b_res;
}



/******************************************************************************
* function name:  Getfileversion(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                              
* Parameters   :  OUT TCHAR* filename 
* Summary      :  分析每个字段的name和value,m_szbcheck[10]用来记录该字段是否存在值
                  将这些字段的信息保存在m_stra_version[n_section](value),m_stra_name[n_section](name)
				  m_szbcheck[20], True表明该字段有值， False 该字段无值
* Return       :  True success.False fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/13           First version                            
*****************************************************************************/
bool Filename_resolution::Getfileversion(TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = true;
	bool b_check = false;
	int n_section = 0;//Record the section of this file 
	int n_filenum = m_stra_fileattributes.GetCount();
    
		for(int k = 0; k < n_filenum; k++)
		{
			if(b_check)
				break;
			for( int i = 0; i < 10; i++)
			{
				if(m_stra[k].FindOneOf(m_stra[i]) >= 0)
				{
					n_section = i;
					m_szbcheck[n_section] = true;
					b_check = true;
					break;
				}
			}
		}

	//m_stra_value 表明文件中所含的name和value
	int n_num = m_stra_value[n_section].GetCount();
	//int n_total = 

	m_stra_name[n_section].Add(m_stra_fileattributes.GetAt(0));
	m_stra_version[n_section].Add(_T("No value"));
	for(int j = 1; j< n_filenum; j++)
	  for(int i = 0; i< n_num; i++)
		{
			//m_stra_fileattributes 文件中的所有字段信息
			//m_stra_value[n_section] 字段中所包含的信息
			int n_location = m_stra_fileattributes.GetAt(j).Find(m_stra_value[n_section].GetAt(i));
			if(n_location >= 0)
			{
				CString sz_temp = m_stra_fileattributes.GetAt(j);
				m_stra_name[n_section].Add(sz_temp.Left(4));
				sz_temp = sz_temp.Mid(n_location + 4,sz_temp.GetLength());
				m_stra_version[n_section].Add(sz_temp);
				break;
				
				//m_szbcheck[i] = true;
			}

			if(j == (n_filenum - 1) && i == (n_num - 1) && !m_szbcheck[n_num - 1])
			{
				sz_ErrCode = _T("C003");
				sz_ErrMsg = _T("路径中的文件没有出现在配置文件中");
				return false;
			}
		}

	return b_res;
}



/******************************************************************************
* function name:  GetSWversion(TCHAR* sz_version, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                           
* Parameters   :  OUT char* sz_version software version 
* Summary      :  根据m_stra_version[n_section](value),m_stra_name[n_section](name)和m_szbcheck[10]
                  获取SW 版本
* Return       :  True success.False fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/13           First version                            
*****************************************************************************/
bool Filename_resolution::GetSWversion(TCHAR* sz_version, TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res =false;
    //int n_num = 
	for(int i = 0; i < 10; i++)
	{
		if(m_szbcheck[i])
		{
			int n_num = m_stra_name[i].GetCount();
			for(int j = 0; j < n_num; j++)
			{
				CString sz_temp = m_stra_name[i].GetAt(j);
				if(sz_temp.Find(_T("SWVR")) >= 0)
				{
					CString sz_version_get = m_stra_version[i].GetAt(j);
					int n_length = sz_version_get.GetLength();
					for(int k = 0; k < n_length; k++)
					{
					  //sz_version = sz_version_get;
						sz_version[k] =  sz_version_get[k];
					}
					sz_version[n_length] = _T('\0');
					b_res = true;
				}
			}
		}
	}

	return b_res;
}



/******************************************************************************
* function name:  GetModule(TCHAR* sz_model,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)                          
* Parameters   :  OUT TCHAR* sz_model mobile model 
* Summary      :  根据m_stra_version[n_section](value),m_stra_name[n_section](name)和m_szbcheck[n_section]
                  获取 model 信息(m_szbcheck[n_section]为True时，该n_section及为当前文件名所对应的项)
* Return       :  True success.False fail  
* Version      :  Author            Date                 Abstract                      
* 1.0          :  Tom Li          2011/07/14           First version                            
*****************************************************************************/
bool Filename_resolution::GetModule(TCHAR* sz_model,TCHAR* sz_ErrCode, TCHAR* sz_ErrMsg)
{
	bool b_res = false;

	for(int i = 0; i < 10; i++)
	{
		if(m_szbcheck[i])
		{
			int n_num = m_stra_name[i].GetCount();
			for(int j = 0; j < n_num; j++)
			{
				CString sz_temp = m_stra_name[i].GetAt(j);
				if(sz_temp.MakeUpper().Find(_T("PRJM")) >= 0)
				{
					CString sz_model_get = m_stra_version[i].GetAt(j);
					int n_length = sz_model_get.GetLength();
					for(int k = 0; k < n_length; k++)
					{
					  //sz_version = sz_version_get;
						sz_model[k] =  sz_model_get[k];
					}
					sz_model[n_length] = '\0';
					b_res = true;
				}
			}
		}
	}

	return b_res;
}