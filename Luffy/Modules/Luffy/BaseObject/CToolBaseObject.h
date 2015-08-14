#ifndef _C_TOOL_BASE_OBJECT_H_
#define _C_TOOL_BASE_OBJECT_H_


#include <string>
#include <vector>
#include <map>


class CToolRuntimeClass;
class _TOOL_INIT_CLASS;
class CToolBaseObject;

#define GP_RUNTIME_CLASS(class_name)																						\
	(&class_name::class##class_name)

#define GP_DECLARE_DYNAMIC(class_name)																						\
	public:																													\
	static CToolRuntimeClass class##class_name;																			\
	static class_name* GetObject(std::map<std::string, CToolBaseObject*>& cObjectMap, const std::string& strName);		\
	virtual CToolRuntimeClass* GetRuntimeClass() const;																	\
	virtual void Release();																								\
	typedef bool (class_name::*FPClassFunc)();																			\
	std::map<std::string, FPClassFunc> m_mapClassFunc;																	\
	FPClassFunc	m_fpMainFunc;																							\
	bool SetMainFunc(const std::string& strMainFunc);


#define GP_DECLARE_DYNCREATE(class_name)																					\
	GP_DECLARE_DYNAMIC(class_name)																							\
	static CToolBaseObject* CreateObject();


#define _GP_IMPLEMENT_RUNTIME_CLASS(class_name, lpfnCreateObject)															\
	static const std::string g_str##class_name = #class_name;																\
	CToolRuntimeClass class_name::class##class_name(g_str##class_name, lpfnCreateObject, NULL);								\
	static _TOOL_INIT_CLASS g_init_##class_name(&class_name::class##class_name);											\
	CToolRuntimeClass* class_name::GetRuntimeClass() const																	\
	{																														\
	return &class_name::class##class_name;																				\
	}																														\
	void class_name::Release()																								\
	{																														\
	delete this;																										\
	}																														\
	class_name* class_name::GetObject(std::map<std::string, CToolBaseObject*>& cObjectMap, const std::string& strName)		\
	{																														\
	std::map<std::string, CToolBaseObject*>::iterator itr = cObjectMap.find(strName);									\
	if (CToolBaseObject** ppcObject = &(itr->second))																	\
		{																													\
		return dynamic_cast<class_name*>(*ppcObject);																	\
		}																													\
		return NULL;																										\
	}																														\
	bool class_name::SetMainFunc(const std::string& strMainFunc)															\
	{																														\
	std::map<std::string, FPClassFunc>::iterator itrMainFunc = m_mapClassFunc.find(strMainFunc);						\
	if (itrMainFunc != m_mapClassFunc.end())																			\
		{																													\
		m_fpMainFunc = (*itrMainFunc).second;																			\
		}																													\
		else																												\
		{																													\
		return false;																									\
		}																													\
		return true;																										\
	}


#define GP_IMPLEMENT_DYNAMIC(class_name)																					\
	_GP_IMPLEMENT_RUNTIME_CLASS(class_name, NULL)


#define GP_IMPLEMENT_DYNCREATE(class_name)																					\
	CToolBaseObject* class_name::CreateObject()																				\
	{																														\
	return new class_name;																								\
	}																														\
	_GP_IMPLEMENT_RUNTIME_CLASS(class_name, class_name::CreateObject)


class CToolRuntimeClass
{
	// Data members
public:
	typedef CToolBaseObject* (*LPFN_CREATE_OBJECT)(void);

	std::string m_strClassName;
	LPFN_CREATE_OBJECT m_lpfnCreateObject;
	CToolRuntimeClass* m_pcNextClass;				// Next node
	static CToolRuntimeClass* m_pcFirstClass;		// First node

protected:
private:


	// Member functions
public:
	CToolRuntimeClass(const std::string& strClassName, LPFN_CREATE_OBJECT lpfnCreateObject, CToolRuntimeClass* pcNextClass = NULL);

	CToolBaseObject* CreateObject();
	CToolBaseObject* CreateObject(const std::string& strClassName);

protected:
private:
};


class _TOOL_INIT_CLASS
{
	// Data members
public:
protected:
private:


	// Member functions
public:
	_TOOL_INIT_CLASS(CToolRuntimeClass* pcNewClass);

protected:
private:
};


class CToolBaseObject
{
	// Data members
public:
	static CToolRuntimeClass classCToolBaseObject;
protected:
private:


	// Member functions
public:
	virtual CToolRuntimeClass* GetRuntimeClass() const;
	virtual void Release() { delete this; }
protected:
private:
};


#endif // End of #ifndef _C_TOOL_BASE_OBJECT_H_
