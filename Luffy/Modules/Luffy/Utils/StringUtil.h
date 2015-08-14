#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <SetupAPI.h>

#include <string>
#include <vector>
#include <map>
#include <cctype>
	
#define ERROR_BOOL		-9999
#define ERROR_DOUBLE	-9999.0
#define ERROR_INT		-9999
#define ERROR_STRING	"-9999"
#define ERROR_UNIT		9999


namespace StringUtility
{
	// Common functions
	void STDStringTokenizer(const std::string& strSource,
		const std::string& strDelimiter,
		std::vector<std::string>& vList,
		bool bAppendDelimiter = false );
	bool ReSplitVector(const std::vector<std::string>& vSource,
		const std::string& strDelimiter,
		std::vector<std::string>& vResult );
	bool RemoveFrontSpace(const std::string& strOriginal, std::string& strDest);
	std::string MakeSTDstring(const char* fmt, ...);

	void STDStringCombine (
		std::string &strData, 
		const std::string& strDelimiter, 
		const std::vector<std::string> vList,
		const int start = 0,
		const int count = -1
		);
	void STDStringCombine (
		std::string &strData, 
		const std::string& strDelimiter, 
		const std::vector<int> vList,
		const int start = 0,
		const int count = -1
		);

	void TrimPerItem (std::vector<std::string> &vList);
	void TrimPerItem (std::string &strData, const std::string& strDelimiter);
}


#endif // End of #ifndef _STRING_UTIL_H_

