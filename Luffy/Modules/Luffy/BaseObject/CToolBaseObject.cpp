#include "stdafx.h"
#include "CToolBaseObject.h"

CToolRuntimeClass* CToolRuntimeClass::m_pcFirstClass = NULL;

CToolRuntimeClass::CToolRuntimeClass(const std::string& strClassName, LPFN_CREATE_OBJECT  lpfnCreateObject, CToolRuntimeClass* pcNextClass)
{
	m_strClassName = strClassName;
	m_lpfnCreateObject = lpfnCreateObject;
	m_pcNextClass = pcNextClass;
}

CToolBaseObject* CToolRuntimeClass::CreateObject()
{
	CToolBaseObject* pcObject = NULL;

	if (m_lpfnCreateObject)
	{
		pcObject = (*m_lpfnCreateObject)();  
	}

	return pcObject;
}

CToolBaseObject* CToolRuntimeClass::CreateObject(const std::string& strClassName)
{
	CToolRuntimeClass* pcCurrClass = m_pcFirstClass;

	while (pcCurrClass)
	{
		if (strClassName == pcCurrClass->m_strClassName)
			return pcCurrClass->CreateObject();
		pcCurrClass = pcCurrClass->m_pcNextClass;
	}

	return NULL;
}

_TOOL_INIT_CLASS::_TOOL_INIT_CLASS(CToolRuntimeClass* pcNewClass)
{
	pcNewClass->m_pcNextClass = CToolRuntimeClass::m_pcFirstClass;
	CToolRuntimeClass::m_pcFirstClass = pcNewClass;
}

static const std::string g_strCToolBaseObject = "CToolBaseObject";
CToolRuntimeClass CToolBaseObject::classCToolBaseObject(g_strCToolBaseObject, NULL);     //定义了一个CToolRuntimeClass类，类名为classCToolBaseObject并作初始化，CToolBaseObject是？
                                                                                         //初始化类CToolBaseObject中的公共变量classCToolBaseObject
static _TOOL_INIT_CLASS g_init_CToolBaseObject(&CToolBaseObject::classCToolBaseObject);  //定义全局变量g_init_CToolBaseObject，并作初始化；g_init_CToolBaseObject是一个类_TOOL_INIT_CLASS

CToolRuntimeClass* CToolBaseObject::GetRuntimeClass() const
{
	return &CToolBaseObject::classCToolBaseObject;
}
