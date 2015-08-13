#ifndef __CMDMODULE_BASE_H__
#define __CMDMODULE_BASE_H__

#include "CmdBase.h"

class CCmdModule_Base
{
public:
	CCmdModule_Base(){};

	virtual ~CCmdModule_Base(){};

	bool ExeDiag(CCmdPtr &Cmd)
	{
		bool ret = true;

		if( !this->IsConnected() )
			ret = this->ConnectDevice();

		if( ret )
		{
			ret = this->PreExe() &&
				  this->ExeCommand(Cmd) &&
				  this->PostExe();
		}
		
		return ret;
	};
	
	virtual bool ConnectDevice() = 0;
	virtual bool DisConnectDevice() = 0;
	virtual bool IsConnected() = 0;

protected:
	virtual bool PreExe() = 0;
	virtual bool ExeCommand(CCmdPtr &Cmd) = 0;
	virtual bool PostExe() = 0;
};

#endif