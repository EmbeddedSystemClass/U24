#include "stdafx.h"
#include "QcomDiagModule.h"
#include "..\RequestManager\Request\CameraRequest.h"
#include<atlimage.h>

/*****************************************************************************/
//
//  CQcomDiagModule  Class member
//
/*****************************************************************************/

bool CQcomDiagModule::Camera_WolfRequest (BYTE index)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	bool bStatus = false;
	CRequestPtr ptRequest = new CWolfCameraRequest (bStatus, index);	
	bRes = Execute(ptRequest);
	if (bRes)
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool CQcomDiagModule::Camera_WolfAFStatus ()
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	bool bStatus = false;

	CRequestPtr ptRequest = new CWolfCameraAFStatusRequest (bStatus);	
	bRes = Execute(ptRequest);
	if (bRes)
	{
		bRes = bStatus;
	}

	if(bRes == false)
		PopLastLog();

	return bRes;
}

bool  CQcomDiagModule::Camera_DrawPoint (CString FilePath,CString InputPoints)
{
	//point = x1,y1,radius1;x2,y2,radius;x3...

	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CImage image;
	image.Load(FilePath);
	CString newPath = FilePath;
	newPath.Append("_tmp.jpg");

	int parse, dx, dy, dradius, firstP;
	CString parsedPointL, parsedPoint = InputPoints;

	int x, y;

	while(!parsedPoint.IsEmpty())
	{
		firstP = parsedPoint.Find(';');

		if(firstP != -1)
		{
			parsedPointL = parsedPoint.Left(firstP);
			parsedPoint = parsedPoint.Right(parsedPoint.GetLength()-firstP-1);
			
		}
		else
		{	parsedPointL = parsedPoint;
			parsedPoint = _T("");
		}

		parse = parsedPointL.Find(_T(","));
		dx= _ttoi(parsedPointL.Left(parse));
		CString right = parsedPointL.Right(parsedPointL.GetLength()-parse-1);

		parse = right.Find(_T(","));
		dy = _ttoi(right.Left(parse));
		
		dradius = _ttoi(right.Right(right.GetLength()-parse-1));


		for( x=dx-(dradius/2); x< dx+(dradius/2); x++)
		{	
			for( y=dy-(dradius/2); y< dy + (dradius/2); y++)
			{
				image.SetPixel(x,y,RGB(0, 255,0));
			}
		}
	
	}

	image.Save(newPath);
	return (access(newPath, 0) == 0);

}


/*****************************************************************************/
//
//  export for SPU 
//
/*****************************************************************************/
//1, 5
QCOM_DIAG_API bool Camera_SendKeyEvent_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CAMERA_KEY_EVENT);
}
//2
QCOM_DIAG_API bool Camera_SendTouchEvent_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CAMERA_TOUCH_EVENT);
}

QCOM_DIAG_API bool Camera_SendTouchCenterEvent_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CAMERA_TOUCH_CENTER_EVENT);
}
//3, per 2sec
QCOM_DIAG_API bool Camera_CheckPreviewStatus_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CHECK_PREVIEW_STATUS);
}
//4
QCOM_DIAG_API bool Camera_SendFocusEvent_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CAMERA_FOCUS_EVENT);
}
//6
QCOM_DIAG_API bool Camera_CheckSnapshotStatus_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CHECK_SNAPSHOT_STATUS);
}
//7
QCOM_DIAG_API bool Camera_SendBackKeyEvent_SPU (const char* szPort) // to exit camera app
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CAMERA_BACKKEY_EVENT);
}
//8, 10sec to 9
QCOM_DIAG_API bool Camera_CopyPictureFromWMFStoEFS_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CAMERA_COPY_FROM_WMFS_TO_EFS);
}
//9, per 2sec
QCOM_DIAG_API bool Camera_CopyPictureFromWMFStoEFSStatus_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfRequest(CAMERA_COPY_FROM_WMFS_TO_EFS_STATUS);
}
//11
QCOM_DIAG_API bool Camera_AFStatus_SPU (const char* szPort)
{
	return CQcomDiagModule(atoi(szPort)).Camera_WolfAFStatus();
}

QCOM_DIAG_API bool Camera_DrawPoint_SPU (const char* szPort, char* szfilePath, char* szPoint)
{
	CQcomDiagModule diag (atoi(szPort));
	//DWORD dwSize;
	bool ret = diag.Camera_DrawPoint (szfilePath, szPoint);
	//sprintf (szSize, "%u", dwSize);

	return ret;
}