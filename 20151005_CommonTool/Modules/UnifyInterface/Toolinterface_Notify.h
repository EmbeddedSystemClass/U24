#ifndef _I_TOOL_INTERFACE_NOTIFY_H_
#define _I_TOOL_INTERFACE_NOTIFY_H_

#include "Toolinterface.h"

#define     DLL_NEW_TOOLINTERFACE_NOTIFY	"NewToolInterfaceNotify"
#define     DLL_DEL_TOOLINTERFACE_NOTIFY	"DeleteToolInterfaceNotify"

interface IToolInterface_Notify : public IToolInterface, public QSYNC_LIB::INotifyBase_Char
{
public:
	virtual ~IToolInterface_Notify(){};
};

typedef bool (__cdecl *p_fn_newToolInterfaceNotify)   (IToolInterface_Notify**);
typedef bool (__cdecl *p_fn_deleteToolInterfaceNotify)(IToolInterface_Notify*);

#endif//_I_TOOL_INTERFACE_NOTIFY_H_
