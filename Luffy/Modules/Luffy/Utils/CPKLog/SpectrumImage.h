#ifndef _SPECTRUM_IMAGE_H
#define _SPECTRUM_IMAGE_H


#include <string>
#include "Utils/StringUtil.h"


typedef struct
{
	double x;
	double y;
} MYPOINT;


class CSpectrumImage
{
public:
	CSpectrumImage();
	~CSpectrumImage();

	void SetFrequencyRange (const double start, const double center); // X range
	//void SetPSDRange (const int start, const int stop); // Y range
	void SetMask (int count, MYPOINT *pPointArray, COLORREF maskColor = RGB(255,0,0));
	void SetResult(int count, MYPOINT *pPointArray);
	void SaveImage (LPCTSTR fileName, const bool bAddDate = true);
	std::string GetCurrentFilename() {return m_CurrentFile;}
	static string GetTimeStamp();

protected:

	int    m_MaskCount;
	MYPOINT *pMaskArray;
	COLORREF m_MaskColor;
	std::string m_CurrentFile;

	int    m_ResultCount;
	MYPOINT  *pResultArray;

	double m_CentralFreq;
	double m_StartFreq; // in MHz
	int    m_YStart;
	int    m_YEnd;


	void DrawInnerSquareBackground (BYTE* pImage);
	void DrawYGrid (BYTE* pImage, CDC *pDC);
	void DrawXGrid (BYTE* pImage, CDC *pDC);
	void DrawMask  (BYTE* pImage, CDC *pDC);
	void DrawResult(BYTE* pImage, CDC *pDC);
	bool WriteToFile (LPCTSTR fileName, BITMAPINFO *pBitmapInfo, BYTE* pImage, const bool bAddDate = true);

};



#endif