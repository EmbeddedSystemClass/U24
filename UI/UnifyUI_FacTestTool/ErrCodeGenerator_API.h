/***************************************************************************
* Copyright (C) : QCS/WR318                                      
* Module name   : Error code generator API 
* File name     : ErrCodeGenerator_API.h             
* Summary       : Error code generator API.                                                  
* Version     Author             Date                 Abstract                      
* 1.0         Nancy.Yang          2012/04/23          First version  
****************************************************************************/

#pragma once

typedef bool (*pQueryErrorCode)(char* sz_errInput, char* sz_genErrcode, char* sz_genErrMsg, char* sz_errMessage,bool &b_inCIM);
typedef bool (*pMapCSV)(char* sz_errMessage);