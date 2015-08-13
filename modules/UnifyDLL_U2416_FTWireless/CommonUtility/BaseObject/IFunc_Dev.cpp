/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .cpp file of IFunc_Dev object.
 * FileName   : IFunc_Dev.cpp
 * Version    : $Revision:   1.0  $
 * Author     : Gilpin.Xi
 * Time       : 2011/03/23
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/03/23      Gilpin.Xi             Create.
 ********************************************************************************/

#include "stdafx.h"
#include "IFunc_Dev.h"

int IFunc_Dev::m_iCMUMode = 1;
int IFunc_Dev::m_iGPIBCardAddress = 0;
int IFunc_Dev::m_h_Base = -1;