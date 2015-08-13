/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of CColorPatternTest object.
 * FileName   : ColorPatternTest.h
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2013/03/04
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/03/04     CY.Lee            Create for Boston project MMI test
 ********************************************************************************/

#ifndef _C_COLOR_PATTERN_TEST_H_
#define _C_COLOR_PATTERN_TEST_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"

class CColorPatternTest : public ITestItem
{
RF_DECLARE_DYNCREATE(CColorPatternTest)

// Data members
public:
protected:
private:


// Member functions
public:
	CColorPatternTest() {}
	~CColorPatternTest() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool ColorPatternTest();
	bool CaptureColor(char *Color);
	bool Analysis(char *Color, int Threshold);
	bool CaptureCapKey();
	bool LoadOpenCVDLL();

	typedef int (__cdecl * PFcvSetCaptureProperty)( CvCapture* capture, int property_id, double value );
	typedef CvCapture* (__cdecl * PFcvCreateCameraCapture)( int index );
	typedef int (__cdecl * PFcvGrabFrame)( CvCapture* capture );
	typedef IplImage* (__cdecl * PFcvRetrieveFrame)( CvCapture* capture, int streamIdx);
	typedef int (__cdecl * PFcvSaveImage)( const char* filename, const CvArr* image, const int* params);
	typedef void (__cdecl * PFcvReleaseCapture)( CvCapture** capture );

	PFcvCreateCameraCapture CFcvCreateCameraCapturey;
	PFcvSetCaptureProperty CFcvSetCaptureProperty;
	PFcvReleaseCapture CFcvReleaseCapture;
	PFcvSaveImage CFcvSaveImage;
	PFcvGrabFrame CFcvGrabFrame;
	PFcvRetrieveFrame CFcvRetrieveFrame;

	CvCapture *m_capture;
	IplImage *m_pImg;
	HINSTANCE m_hHighgui;

	int m_iCameraDelay;
	int m_iRedThreshold;
	int m_iGreenThreshold;
	int m_iBlueThreshold;
	double d_Prop_Brightness;
	double d_Prop_CONTRAST;
	std::string m_strBackLight;
};


#endif // End of #ifndef _C_COLOR_PATTERN_TEST_H_