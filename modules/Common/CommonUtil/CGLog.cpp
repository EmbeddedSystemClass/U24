#include "stdafx.h"
#include "CGLog.h"

bool CGLog::m_Initialized = false;

//Glog
CGLog::CGLog():m_strGLogPath("")
{
}

CGLog::~CGLog()
{
	if(this->m_Initialized)
	{
		google::ShutdownGoogleLogging();
		this->m_Initialized = false;
	}
}

void CGLog::InitializeGLog(const char *szLogPath)
{
	//if(this->m_Initialized)
	//	return;

	if(0 == m_strGLogPath.compare("")){
		char chModule[MAX_PATH];
		GetModuleFileNameA (NULL, chModule, MAX_PATH);
		char* pPtr = strrchr (chModule, '\\');

		pPtr++;

		google::InitGoogleLogging(pPtr);

		if (0 == strcmp(szLogPath,"")){
			// use default log path
			this->m_strGLogPath = DEFAULT_LOG_DIR;
		} else {
			this->m_strGLogPath.assign(szLogPath) ;
		}

		if (this->m_strGLogPath.find (":\\") == this->m_strGLogPath.npos){
			// not find means this is not absolute path
			*pPtr = 0;
			this->m_strGLogPath.insert (0, chModule);
		}

		// CreateDirectory if not exist
		CString dirName(this->m_strGLogPath.c_str());
		if (! CreateDirectory (dirName, NULL)){
			if (GetLastError() != ERROR_ALREADY_EXISTS){
				this->m_strGLogPath = "";
			}
		}

		this->m_Initialized = true;
	}
}

void CGLog::SetGLogDestination_Error(const char *szErrLogName)
{
	std::string strFullPath = this->m_strGLogPath + szErrLogName;
	google::SetLogDestination (google::_ERROR, strFullPath.c_str());
}

void CGLog::SetGLogDestination_Warning(const char *szWarLogName)
{
	std::string strFullPath = this->m_strGLogPath + szWarLogName;
	google::SetLogDestination (google::_WARNING, strFullPath.c_str());
}

void CGLog::SetGLogDestination_Info(const char *szInfoLogName)
{
	std::string strFullPath = this->m_strGLogPath + szInfoLogName;
	google::SetLogDestination (google::_INFO, strFullPath.c_str());
}

void CGLog::SetGLogDestination_All(const char *szErrLogName, const char *szWarLogName, const char *szInfoLogName)
{
	this->SetGLogDestination_Error(szErrLogName);
	this->SetGLogDestination_Warning(szWarLogName);
	this->SetGLogDestination_Info(szInfoLogName);
}

void CGLog::Log(const int Severity, const char *szLogMsg)
{
	switch(Severity){
	case __ERROR:
		LOG(_ERROR) << szLogMsg;
		break;
	case __WARNING:
		LOG(_WARNING) << szLogMsg;
		break;
	case __INFO:
		LOG(_INFO) << szLogMsg;
		break;
	default:
		LOG(_ERROR) << szLogMsg;
		break;
	}
}

void CGLog::ShutdownGLog()
{
	google::ShutdownGoogleLogging();
}