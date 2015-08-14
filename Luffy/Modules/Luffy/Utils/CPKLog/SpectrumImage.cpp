#include "stdafx.h"
#include "SpectrumImage.h"
#include <atlimage.h>

#define INNER_PIXNUM		480
#define INNER_LINENUM		400
#define PIXNUM				560 //must be multiple of 4, otherwise bitmap need padding! I dont write the padding
#define LINENUM				450


using namespace std;
using namespace StringUtility;


CSpectrumImage::CSpectrumImage()
	: pMaskArray (NULL)
	, pResultArray (NULL)
	, m_MaskColor(RGB(255,0,0))
	, m_CentralFreq (2.412)
	, m_YEnd (-90)
	, m_YStart(10)
{
	m_MaskCount=0;
	m_ResultCount=0;
	m_StartFreq=0;
}

CSpectrumImage::~CSpectrumImage()
{
	if (pMaskArray)
	{
		delete [] pMaskArray;
		pMaskArray = NULL;
	}

	if (pResultArray)
	{
		delete [] pResultArray;
		pResultArray = NULL;
	}
	
}



void CSpectrumImage::SetMask (int count, MYPOINT *pPointArray, COLORREF maskColor)
{
	if (pMaskArray)
	{
		delete [] pMaskArray;
		pMaskArray = NULL;
	}

	m_MaskColor = maskColor;
	m_MaskCount = count;
	pMaskArray = new MYPOINT[count];
	for (int i=0; i<count; ++i)
	{
		pMaskArray[i].x = pPointArray[i].x;
		pMaskArray[i].y = pPointArray[i].y;
	}
}

void CSpectrumImage::SetResult(int count, MYPOINT *pPointArray)
{
	if (pResultArray)
	{
		delete [] pResultArray;
		pResultArray = NULL;
	}

	m_ResultCount = count;
	pResultArray = new MYPOINT[count];
	
	for (int i=0; i<count; ++i)
	{
		pResultArray[i].x = pPointArray[i].x;
		pResultArray[i].y = pPointArray[i].y;
	}

}

void CSpectrumImage::SetFrequencyRange (const double start, const double center)
{
	m_CentralFreq = center;
	m_StartFreq = start;
}



void CSpectrumImage::DrawInnerSquareBackground (BYTE* pImage)
{
	// white canvas
	DWORD dwSize = PIXNUM * LINENUM * 3;
	memset (pImage, 255, dwSize);

	// make inner square black
	int xOffset = (PIXNUM - INNER_PIXNUM) / 2;
	int yOffset = (LINENUM - INNER_LINENUM) / 2;
	BYTE *pPtr;

	for (int y=yOffset; y< INNER_LINENUM + yOffset; ++y)
	{
		pPtr = pImage + (y*PIXNUM*3) + (xOffset*3);
		for (int x=0; x < INNER_PIXNUM; ++x)
		{
			memset (pPtr, 0, 3);
			pPtr += 3;
		}
	}

}

void CSpectrumImage::DrawYGrid (BYTE* pImage, CDC *pDC)
{
	// prepare the font
	char chText[256];
	int halfFontHeight = 7;
	CFont fontFreq;
	fontFreq.CreateFont (halfFontHeight*2, 0,0,0,FW_ULTRALIGHT ,0,0,0,DEFAULT_CHARSET,0,0,0,0,TEXT("Arial"));


	int yCount = 10;
	int start = m_YStart;
	int step = (m_YEnd - m_YStart) / yCount;
	int xOffset = (PIXNUM - INNER_PIXNUM) / 2;
	int yOffset = (LINENUM - INNER_LINENUM) / 2;
	int yHeight = INNER_LINENUM / yCount;
	int yPoint;


	for (int i=0, degree = start; i<= yCount; ++i, degree += step)
	{
		yPoint = yOffset + (i * yHeight);
		pDC->SelectStockObject(WHITE_PEN);
		pDC->MoveTo (xOffset, yPoint);
		pDC->LineTo (xOffset + INNER_PIXNUM, yPoint);

		// write y degree
		memset(chText, 0, 256);
		sprintf_s(chText, 256, "%d", degree);
		pDC->SelectStockObject(BLACK_PEN);
		pDC->SelectObject (fontFreq);
		pDC->TextOut (xOffset/2, yPoint-halfFontHeight, CString(chText), strlen(chText));
	}
}

void CSpectrumImage::DrawXGrid (BYTE* pImage, CDC *pDC)
{
	// prepare the font
	char chText[256];
	int fontHeight = 14;
	CFont fontFreq;
	fontFreq.CreateFont (fontHeight, 0,0,0,FW_ULTRALIGHT ,0,0,0,DEFAULT_CHARSET,0,0,0,0,TEXT("Arial"));


	int xCenter = PIXNUM / 2;
	int xCount = 16;
	int xWid = INNER_PIXNUM / xCount;
	int yPoint = INNER_LINENUM + (LINENUM - INNER_LINENUM) / 2;
	int xOffset = (PIXNUM - INNER_PIXNUM) / 2;
	int xPoint;

	double freqWid = (m_CentralFreq - m_StartFreq) * 2 / xCount;

	for (int i=0; i<= xCount; ++i)
	{
		xPoint = xOffset + (i * xWid);
		pDC->SelectStockObject(WHITE_PEN);
		pDC->MoveTo (xPoint, yPoint-5);
		pDC->LineTo (xPoint, yPoint);

		// write frequency
		double freq = m_StartFreq + (freqWid * i);
		memset(chText, 0, 256);
		sprintf_s(chText, 256, "%.1f", freq);
		if (i % (xCount / 4) == 0)
		{
			pDC->SelectStockObject(BLACK_PEN);
			pDC->SelectObject (fontFreq);
			pDC->TextOut (xPoint-10, yPoint, CString(chText), strlen(chText));
		}
	}
}

void CSpectrumImage::DrawMask (BYTE* pImage, CDC *pDC)
{
	CPen maskpen;
	maskpen.CreatePen (PS_SOLID, 0, m_MaskColor);
	pDC->SelectObject(maskpen);


	double pixelPerFreq = (INNER_PIXNUM / 2) / (m_CentralFreq - m_StartFreq);
	double pixelPerPSD  = INNER_LINENUM / (m_YStart - m_YEnd);
	int x, y;

	int xOffset = (PIXNUM - INNER_PIXNUM) / 2;
	int yOffset = (LINENUM - INNER_LINENUM) / 2;

	// left part
	x = (int)((m_CentralFreq + pMaskArray[0].x - m_StartFreq) * pixelPerFreq);
	y = (int)((m_YStart - pMaskArray[0].y) * pixelPerPSD);
	pDC->MoveTo (xOffset, y + yOffset);
	pDC->LineTo (x + xOffset, y + yOffset);


	// central
	for (int i=1; i<m_MaskCount; ++i)
	{
		x = (int)((m_CentralFreq + pMaskArray[i].x - m_StartFreq) * pixelPerFreq);
		y = (int)((m_YStart - pMaskArray[i].y) * pixelPerPSD);
		pDC->LineTo (x + xOffset, y + yOffset);
	}

	// right part
	pDC->LineTo (INNER_PIXNUM + xOffset, y + yOffset);

}


void CSpectrumImage::DrawResult(BYTE* pImage, CDC *pDC)
{
	CPen maskpen;
	maskpen.CreatePen (PS_SOLID, 0, RGB (255,255,0));
	pDC->SelectObject(maskpen);



	int xOffset = (PIXNUM - INNER_PIXNUM) / 2;
	int yOffset = (LINENUM - INNER_LINENUM) / 2;
	double pixelPerFreq = (INNER_PIXNUM / 2) / (m_CentralFreq - m_StartFreq);
	double pixelPerPSD  = INNER_LINENUM / (m_YStart - m_YEnd);
	int x;
	int y;

	// start point
	x = (int)((pResultArray[0].x - m_StartFreq) * pixelPerFreq);
	y = (int)((m_YStart - pResultArray[0].y) * pixelPerPSD);
	pDC->MoveTo (x + xOffset, y + yOffset);

	for (int i=0; i<m_ResultCount; ++i)
	{
		x = (int)((pResultArray[i].x - m_StartFreq) * pixelPerFreq);
		y = (int)((m_YStart - pResultArray[i].y) * pixelPerPSD);
		pDC->LineTo (x + xOffset, y + yOffset);
	}


}

bool CSpectrumImage::WriteToFile (LPCTSTR fileName, BITMAPINFO *pBitmapInfo, BYTE* pImage, const bool bAddDate)
{
	std::string strFileName;
	std::string strLogpath = CW2A(fileName);

	// check folder

	if (bAddDate)
	{
		std::string strTime = GetTimeStamp();
		strLogpath = MakeSTDstring("%s_%s.bmp", strLogpath.c_str(), strTime.c_str());
	}

	if (PathIsRelativeA (strLogpath.c_str()))
	{
		char szTemp[MAX_PATH] = {0};
		GetModuleFileNameA(NULL, szTemp, MAX_PATH);
		PathRemoveFileSpecA(szTemp);

		strFileName = szTemp;
		strFileName += "\\" + strLogpath;
	}
	else
	{
		strFileName = strLogpath;
	}

	m_CurrentFile = strFileName;

	// CreateDirectory if not exist
	char szDir[MAX_PATH] = {0};
	strcpy_s(szDir, MAX_PATH, strFileName.c_str());
	PathRemoveFileSpecA (szDir);
	if (! CreateDirectoryA (szDir, NULL))
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			strcpy_s(szDir, MAX_PATH, "C:\\");

			char szTemp[256] = {0};
			strcpy_s(szTemp, 256, strFileName.c_str());
			strFileName = std::string (szDir) + PathFindFileNameA (szTemp);
		}
	}


	DWORD dwTemp;
	DWORD dwImageSize = PIXNUM * LINENUM * 3;

	BITMAPFILEHEADER bmfh = {0};
	bmfh.bfType = 'B'+('M'<<8);
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + pBitmapInfo->bmiHeader.biSize;
	bmfh.bfSize = bmfh.bfOffBits + dwImageSize;

	HANDLE hFile = CreateFileA (strFileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL , 0);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	WriteFile (hFile, &bmfh, sizeof(BITMAPFILEHEADER), &dwTemp, 0);
	WriteFile (hFile, &pBitmapInfo->bmiHeader, sizeof (BITMAPINFOHEADER), &dwTemp, 0);
	WriteFile (hFile, pImage, dwImageSize, &dwTemp, 0);

	CloseHandle (hFile);


	// convert to jpeg
	CImage img;
	CA2T tstrFileName(strFileName.c_str());
	HRESULT hr = img.Load (tstrFileName);

	if(SUCCEEDED(hr))
	{
		std::string strJpegName(strFileName);
		std::string strFind(".bmp");
		strJpegName.replace(strJpegName.find(strFind), strFind.length(),".jpg");

		CA2T tstrJpegName(strJpegName.c_str());
		hr = img.Save (tstrJpegName, Gdiplus::ImageFormatJPEG);
		DeleteFileA (strFileName.c_str());

		m_CurrentFile = strJpegName;
	}

	return true;
}

void CSpectrumImage::SaveImage (LPCTSTR fileName, const bool bAddDate)
{
	
	HDC hDC=GetDC(NULL);
	
	BITMAPINFO bmInfo;
	memset (&bmInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmInfo.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth    = PIXNUM;
	bmInfo.bmiHeader.biHeight   = LINENUM;
	bmInfo.bmiHeader.biPlanes   = 1;
	bmInfo.bmiHeader.biBitCount = 24;
	bmInfo.bmiHeader.biCompression = BI_RGB;

	BYTE* pTempRawData;
	HBITMAP hTempBitmap;
	hTempBitmap = CreateDIBSection (hDC, &bmInfo, DIB_RGB_COLORS, (void**)&pTempRawData,0,0);

	// We can only draw to a DC
	CDC *pDC = new CDC();
	CBitmap *pTargetMap = CBitmap::FromHandle (hTempBitmap);
	pDC->CreateCompatibleDC (NULL);
	pDC->SelectObject (pTargetMap);


	DrawInnerSquareBackground (pTempRawData);
	DrawYGrid (pTempRawData, pDC);
	DrawXGrid (pTempRawData, pDC);
	DrawMask  (pTempRawData, pDC);
	DrawResult(pTempRawData, pDC);


	WriteToFile (fileName, &bmInfo, pTempRawData, bAddDate);

	//delete bitmap object
	DeleteObject(hTempBitmap);

}

std::string CSpectrumImage::GetTimeStamp()
{
	SYSTEMTIME* Systime = new SYSTEMTIME;
	GetLocalTime(Systime);

	char szTime[MAX_PATH];
	memset(szTime, 0, MAX_PATH);

	sprintf_s(szTime, MAX_PATH, "%04d%02d%02d%02d%02d%02d",
		Systime->wYear,
		Systime->wMonth,
		Systime->wDay,
		Systime->wHour,
		Systime->wMinute,
		Systime->wSecond
		);

	delete Systime;

	//return
	string strTime = szTime;
	return strTime;
}
