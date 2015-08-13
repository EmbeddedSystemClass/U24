#include "DLLDefine.h"

/******************************************************************************
 * Export/import function prototypes
 ******************************************************************************/

NEPDLLAPI int __stdcall WIFI_Read_MCR(int nPort, int nTimeout,BYTE* OutConfirm, BYTE* OutPeerData);
