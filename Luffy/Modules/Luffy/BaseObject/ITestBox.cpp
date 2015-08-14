#include "stdafx.h"
#include "ITestBox.h"


GP_IMPLEMENT_DYNAMIC(ITestBox)


std::string ITestBox::g_strRFInputPort = "";
std::string ITestBox::g_strRFOutputPort = "";
int ITestBox::g_iMode = 0;
