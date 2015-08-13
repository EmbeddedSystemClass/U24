/*********************************************************************************
 * COPYRIGHT Qisda WR24 2013
 *
 * Description: .h file of CAutoMMIBBIndicatorLED object.
 * FileName   : ColorPatternTest.h
 * Version    : $Revision:   1.0  $
 * Author     : CY.Lee
 * Time       : 2013/03/04
 * History    :
 *  NO      Date        Updated by					Content of change
 *  1    2013/03/04     CY.Lee            Create for Boston project MMI test
 ********************************************************************************/

#ifndef _C_Indicator_LED_TEST_H_
#define _C_Indicator_LED_TEST_H_

#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "../MMITestItem/opencv/cv.h"
#include "../MMITestItem/opencv/highgui.h"

class CAutoMMIBBIndicatorLED : public ITestItem
{
RF_DECLARE_DYNCREATE(CAutoMMIBBIndicatorLED)

// Data members
public:
protected:
private:


// Member functions
public:
	CAutoMMIBBIndicatorLED() {}
	~CAutoMMIBBIndicatorLED() {}

	virtual bool InitData(std::map<std::string, std::string>& paramMap);

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool LEDPatternTest();
	bool CaptureColor(char* sz_ledColor, char *sz_patternIndicatorLED);
	bool Analysis(char *Color, int Threshold);
	bool CaptureCapKey();
	bool LoadOpenCVDLL();

	typedef int (__cdecl * PFcvSetCaptureProperty)( CvCapture* capture, int property_id, double value );
	typedef CvCapture* (__cdecl * PFcvCreateCameraCapture)( int index );
	typedef int (__cdecl * PFcvGrabFrame)( CvCapture* capture );
	typedef IplImage* (__cdecl * PFcvRetrieveFrame)( CvCapture* capture, int streamIdx);
	typedef int (__cdecl * PFcvSaveImage)( const char* filename, const CvArr* image, const int* params);
	typedef void (__cdecl * PFcvReleaseCapture)( CvCapture** capture );

	// CVAPI(double) cvGetCaptureProperty( CvCapture* capture, int property_id );
	typedef double (__cdecl * PFcvGetCaptureProperty)( CvCapture* capture, int property_id );
	//cvSetCaptureProperty

	PFcvCreateCameraCapture CFcvCreateCameraCapturey;
	PFcvSetCaptureProperty CFcvSetCaptureProperty;
	PFcvGetCaptureProperty CFcvGetCaptureProperty;
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
	CString prop_log;
};


#endif // End of #ifndef _C_Indicator_LED_TEST_H_