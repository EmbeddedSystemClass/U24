// NeptuneGeneralController.cpp: implementation of the CNeptuneGeneralController class.
//
//////////////////////////////////////////////////////////////////////

//============================================================================//
//
//  Copyright (c) 2006, All Rights Reserved
//  BENQ Corporation.
//  Hsinchu, Taiwan, R.O.C.
//
//  The information contained herein is the exclusive property of
//  BENQ Corporation and shall not be distributed, reproduced, or
//  disclosed in whole or in part without prior written permission
//  of BENQ Corporation.
//
//============================================================================//
//
//  File Name:      NeptuneGeneralController.cpp
//  Description:
//      Base class of neptune PC Control Object
//
//  Version: 1.0
//
//
//  History:
//    2006/9/01 Yu Teng Tung
//      1) Initial
//    2006/9/06 Yu Teng Tung
//      1) Add debug function and related property: OutputArray
//    2009/9/30 Mick Lin
//      1. remove OutputArray
 
#include "stdafx.h"
#include "NeptuneCtrl.h"
#include "NeptuneGeneralController.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// 2006-09-01 YTT
//Let the class be a AbstractFactory Ideally(as Prototype or Sigleton)
//Add General Control Function as Virtual Function 
//Implelemt Module-Independent function inside the class (such as debug function)



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNeptuneGeneralController::CNeptuneGeneralController()
{
	//GetModule Path and store to member variable
}

CNeptuneGeneralController::~CNeptuneGeneralController()
{
}

