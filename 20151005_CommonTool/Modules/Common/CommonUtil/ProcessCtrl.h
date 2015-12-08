#pragma once

class ProcessCtrl
{
public:
	ProcessCtrl(void);
	~ProcessCtrl(void);
	
	int KILL_PROC_BY_NAME(const char *szToTerminate);
};
