/*********************************************************************************
 * COPYRIGHT Qisda WGC18 2011
 *
 * Description: .h file of CTouchPanelProc object.
 * FileName   : CTouchPanelProc.h
 * Version    : $Revision:   1.2  $
 * Author     : Gilpin.Xi
 * Time       : 2011/04/18
 * History    :
 *  NO      Date        Updated by        Content of change
 *  1    2011/04/18      Gilpin.Xi             Create.
 ********************************************************************************/

#ifndef _C_Touch_Panel_Proc_H_
#define _C_Touch_Panel_Proc_H_


#include "../../../CommonUtility/BaseObject/ITestItem.h"
#include "../../../TouchLineDlg.h"
#include "../../../TouchCircleDlg.h"
#include "../../../TouchGraphDlg.h"
#include "../../../TouchParallelDlg.h"
#include "../../../TouchPointDlg.h"
#include "../../../TouchCapkeyDlg.h"


/* Orange Touch related track file name */
#define OPENSHORT_TEST_FILE				_T("OpenShort_Test.rle")
#define LINEARITY_TEST_FILE				_T("Linearity_Test.rle")
#define MUITI_TEST_FILE					_T("Multi_Test.rle")


class CTouchPanelProc : public ITestItem
{
RF_DECLARE_DYNCREATE(CTouchPanelProc)


// Data members
public:
	CTouchLineDlg *pTouchLineDlg;
	CTouchCircleDlg *pTouchCircleDlg;
	CTouchGraphDlg *pTouchGraphDlg;
	CTouchParallelDlg *pTouchParallelDlg;
	CTouchPointDlg *pTouchPointDlg;
	CTouchCapkeyDlg *pTouchCapkeyDlg;

protected:
private:
	//parameters
    int m_iDisplayRate;
	int m_i_DisplayGap_ms;		//Delay ms between every test pattern display gap
	int m_iPanelHPixel;//水平像素点
	int m_iPanelVPixel;//垂直像素点
	int m_iBlockLenght;//TouchPanel Block长度（水平像素点）
	int m_iBlockWidth; //TouchPanel Block宽度（垂直像素点）

	int m_iTouchCircleCount;//Touch Circle的个数
	int m_iDistance;//Touch Circle之间的间隔距离

	int TimeOut;

	/* show picture use */
	bool m_b_ShowTrack;
	std::string m_str_TrackFilePath;	

	IntVtr m_vTouchItem;//Touch测试项的顺序
	int m_iCurrentTouchItem;

	CWinThread *TouchLineThreadHandle;
	CWinThread *TouchCircleThreadHandle;
	CWinThread *TouchGraphThreadHandle;
	CWinThread *TouchParallelThreadHandle;
	CWinThread *TouchPointThreadHandle;
	CWinThread *TouchCapkeyThreadHandle;

	bool m_bStopThread;
	bool m_bStopTouchTest;

// Member functions
public:
	CTouchPanelProc();
	~CTouchPanelProc();

	virtual bool InitData(std::map<std::string, std::string>& paramMap);
	virtual bool InitData();

	virtual bool PreRun();
	virtual bool Run();
	virtual bool PostRun();

protected:
private:
	bool TouchPanelTest();
	bool TouchCapkeyTest();
	bool CopyTouchTrackPictureToMobile();

	//Diagonal Line
    static UINT AfxTouchLineDlgThread(LPVOID pParam);
	bool CloseTouchLineDlgThread();
	bool InitialDiagonalLineDlg();
	bool RunDiagonalLineProcess();
	bool EndDiagonalLineDlg();

	//Circle-Touch
	static UINT AfxTouchCircleDlgThread(LPVOID pParam);
	bool CloseTouchCircleDlgThread();
	bool InitialCircleTouchDlg();
	bool RunCircleTouchProcess();
	bool EndCircleTouchDlg();

	//Multi-Touch(Graph-8 lines)
    static UINT AfxTouchGraphDlgThread(LPVOID pParam);
	bool CloseTouchGraphDlgThread();
	bool InitialMultiTouchDlg();
	bool RunMultiTouchProcess();
	bool EndMultiTouchDlg();

	//Multi-Touch(2 Parallel lines)
	static UINT AfxTouchParallelDlgThread(LPVOID pParam);
	bool CloseTouchParallelDlgThread();
	bool InitialParallelTouchDlg();
	bool RunParallelTouchProcess();
	bool EndParallelTouchDlg();

	//Multi points Touch
    static UINT AfxTouchPointDlgThread(LPVOID pParam);
	bool CloseTouchPointDlgThread();
	bool InitialTouchPointDlg();
	bool RunTouchPointProcess();
	bool EndTouchPointDlg();

	//Capkey Touch Test
	static UINT AfxTouchCapkeyDlgThread(LPVOID pParam);
	bool CloseTouchCapkeyDlgThread();
	bool InitialTouchCapkeyDlg();
	bool RunTouchCapkeyProcess();
	bool EndTouchCapkeyDlg();
};

#endif // End of #ifndef _C_Touch_Panel_Proc_H_
