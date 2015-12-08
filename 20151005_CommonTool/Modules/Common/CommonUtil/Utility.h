
#ifndef _UTILITY_H_
#define _UTILITY_H_

#define QWORD __int64
#define LODWORD(a) ((DWORD)(a))
#define HIDWORD(a) ((DWORD)((a)>>32))

#include <vector>
#include <string>
#include <atlenc.h>

enum ERRORCODE_OBEX
{
	ERR_ACCESSDENIED = 0,
	ERR_PAYMENTREQUIRED,
	ERR_NOTFOUND,
	ERR_MEMORY_FULL,
	ERR_UNSPECIFIED
};

inline QWORD Bytes2QWORD( BYTE c0, BYTE c1, BYTE c2, BYTE c3, BYTE c4, BYTE c5, BYTE c6, BYTE c7 )
{
    QWORD qw = QWORD(c0) | (QWORD(c1)<<8) | (QWORD(c2)<<16) | (QWORD(c3)<<24) | (QWORD(c4)<<32) | (QWORD(c5)<<40) | (QWORD(c6)<<48) | (QWORD(c7)<<56); 
    return qw;
}

inline HRESULT MapObexResponseCode( BYTE byCode )
{
    switch( byCode )
    {
    case 0xc1:          return ERR_ACCESSDENIED;
    case 0xc2:          return ERR_PAYMENTREQUIRED;
    case 0xc3:          return ERR_ACCESSDENIED;
    case 0xc4:          return ERR_NOTFOUND;
    case 0xe0:          return ERR_MEMORY_FULL;
    default:            return ERR_UNSPECIFIED;
    }
}

CString dirname( LPCTSTR szPathname );
CString basename( LPCTSTR szPathname );

CString adjustslash( LPCTSTR szPathname );

bool IsValidFilename( LPCTSTR szFilename );

namespace QSYNC_LIB 
{

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

	bool SearchDirectory(std::vector<TSTRING>& vDirName, const TSTRING &strRootFolder); //only in 1 level
	bool SearchFiles(std::vector<TSTRING>& vFileName, 
					 const TSTRING& strRootFolder, 
					 const TSTRING& refstrExtension/*if=empty, means no restrictions*/,
					 bool bSkipExtension = true/*true=remove .extension*/);//only in 1 level

	//MD5 
	#define md5byte unsigned char
	typedef unsigned int UWORD32;

	struct MD5Context {
		UWORD32 buf[4];
		UWORD32 bytes[2];
		UWORD32 in[16];
	};

	void MD5(md5byte *buffer, unsigned int length, md5byte *digest);
	void MD5Init(struct MD5Context *context);
	void MD5Update(struct MD5Context *context, md5byte const *buf, unsigned len);
	void MD5Final(unsigned char digest[16], struct MD5Context *context);
	void MD5Transform(UWORD32 buf[4], UWORD32 const in[16]);
	bool GetMD5Content(const std::string& strContent, std::string& strDigest);

	bool GetFileContent(const std::string& strFilePath, std::string& strContent);
	bool WriteFile(const std::string& strFilePath,const std::string& strContent);
	

} // namespace QSYNC_LIB

#endif // _UTILITY_H_
