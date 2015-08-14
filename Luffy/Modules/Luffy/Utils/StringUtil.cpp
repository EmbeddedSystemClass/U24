#include "stdafx.h"
#include "StringUtil.h"

namespace StringUtility
{
	// ------------------- Common functions --------------------
	void STDStringTokenizer(
		const std::string& strSource,
		const std::string& strDelimiter,
		std::vector<std::string>& vList,
		bool bAppendDelimiter
		)
	{
		std::string::size_type iCurrPos = 0;
		std::string::size_type iLastDelimiter = std::string::npos;
		const std::string::size_type iTotalLen = strSource.size();
		const std::string::size_type iDelimiterLen = strDelimiter.size();

		vList.clear();

		while (iCurrPos < iTotalLen)
		{
			size_t x = 0;
			for (; x < iDelimiterLen; ++x)
			{
				std::string::size_type stFind = strSource.find(strDelimiter[x], iCurrPos);
				if (stFind == std::string::npos)
					;
				else
				{
					iLastDelimiter = stFind;
					size_t stStrLen = stFind - iCurrPos;

					if (bAppendDelimiter)
						++stStrLen;

					vList.push_back(strSource.substr(iCurrPos, stStrLen));
					iCurrPos = stFind + 1;
					break;
				}
			}

			if (x >= iDelimiterLen)
				break;
		}

		if (iLastDelimiter + 1 < iTotalLen)
			vList.push_back(strSource.substr(iLastDelimiter + 1, iTotalLen - iLastDelimiter - 1));
	}

	bool ReSplitVector(const std::vector<std::string>& vSource,
		const std::string& strDelimiter,
		std::vector<std::string>& vResult)
	{
		vResult.clear() ;

		for (unsigned int i = 0; i < vSource.size(); ++i)
		{
			std::string strTmp ;
			RemoveFrontSpace(vSource[i], strTmp) ;
			STDStringTokenizer(strTmp, strDelimiter, vResult) ;
		}

		return true ;
	}

	bool RemoveFrontSpace(const std::string& strOriginal, std::string& strDest)
	{
		// Remove white space
		unsigned int iPos = 0 ;
		for (int i=0; i<(int)strOriginal.length(); ++i) {
			if ( '\n'==strOriginal.at(i) || ' '==strOriginal.at(i) || '\t'==strOriginal.at(i) || '\0'==strOriginal.at(i) || NULL==strOriginal.at(i) ) {
				++iPos ;
			} else {
				break ;
			}
		}

		char szBuffer[MAX_PATH] ;
		::ZeroMemory(szBuffer, MAX_PATH) ;
		unsigned int iSize = strOriginal.length() - iPos ;
		strOriginal._Copy_s(szBuffer, MAX_PATH, iSize, iPos) ;
		strDest = szBuffer ;

		return true ;
	}

	std::string MakeSTDstring(const char* fmt, ...)
	{
		char chBuffer[4096];
		::ZeroMemory(chBuffer, 4096);

		va_list va;
		va_start(va,fmt);
		int len = vsprintf_s(chBuffer, 4096, fmt, va);
		va_end(va);

		std::string totalmsg;
		totalmsg = chBuffer;
		return totalmsg;
	}

	void STDStringCombine (std::string &strData, const std::string& strDelimiter, 
		const std::vector<std::string> vList, const int start, const int count)
	{
		strData.clear();

		int totalsize = vList.size();

		int end = (-1 == count) ? totalsize : start + count;
		if (end > totalsize)
			end = totalsize;

		for (int i=start; i<end; ++i)
		{
			strData += vList[i];
			strData += strDelimiter;
		}
		strData.erase (strData.end() - 1); // remove last comma
	}

	void STDStringCombine (std::string &strData, const std::string& strDelimiter, 
		const std::vector<int> vList, const int start, const int count)
	{
		strData.clear();

		int totalsize = vList.size();

		int end = (-1 == count) ? totalsize : start + count;
		if (end > totalsize)
			end = totalsize;

		for (int i=start; i<end; ++i)
		{
			strData += MakeSTDstring ("%d", vList[i]);
			strData += strDelimiter;
		}
		strData.erase (strData.end() - 1); // remove last comma
	}

	void TrimPerItem (std::vector<std::string> &vList)
	{
		std::string::iterator it;
		char ch;

		int total = vList.size();
		for (int i=0; i<total; ++i)
		{
			// 8,9,10,13 --> backspace, tab, linefeed, and carriage return

			// trim left
			for (it = vList[i].begin(); it != vList[i].end(); it++)
			{
				ch = *it;
				if ((ch != 8) && (ch != 9) && (ch != 10) && (ch != 13) && (ch!= ' '))
					break;
			}
			vList[i].erase (vList[i].begin(), it);

			// trim right
			for (it = vList[i].end()-1; it != vList[i].begin(); it--)
			{
				ch = *it;
				if ((ch != 8) && (ch != 9) && (ch != 10) && (ch != 13) && (ch!= ' '))
					break;
			}
			vList[i].erase (++it, vList[i].end());
		}
	}

	void TrimPerItem (std::string &strData, const std::string& strDelimiter)
	{
		std::vector<std::string> vTemp;
		STDStringTokenizer (strData, strDelimiter, vTemp);
		TrimPerItem (vTemp);
		STDStringCombine (strData, strDelimiter, vTemp);
	}

} // namespace String