/* TripleDes.h

  Copyright (C) 2004 ActualResearch, Inc.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Porochnya Dmitry	flame@rsdn.ru; admin@actualresearch.com

*/
//---------------------------------------------------------------------------
#ifndef TripleDesH
#define TripleDesH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
// FUNCTION PROTOTYPES
//---------------------------------------------------------------------------
typedef BOOL (WINAPI *PFILEENCRYPT)(const char* FileName, const char* Password);
typedef BOOL (WINAPI *PFILEDECRYPT)(const char* FileName, const char* Password);
typedef BYTE* (WINAPI *PENCRYPTBUFFER)(BYTE* In, DWORD InSize,const char* Password, DWORD* ResultSize);
typedef BYTE* (WINAPI *PDECRYPTBUFFER)(BYTE* In, DWORD InSize,const char* Password);
typedef void (WINAPI *PFREEBUFFER)(BYTE* In);
//---------------------------------------------------------------------------
/*
EXAMPLE OF USAGE:

    PFILEENCRYPT FileEncrypt;
    PFILEDECRYPT FileDecrypt;
    PENCRYPTBUFFER EncryptBuffer;
    PDECRYPTBUFFER DecryptBuffer;
    PFREEBUFFER FreeBuffer;

    HANDLE hLib = ::LoadLibrary("tripledes.dll");
    if(!hLib)
    	return;

     FileEncrypt = (PFILEENCRYPT) ::GetProcAddress(hLib,"FileEncrypt");
     FileDecrypt = (PFILEDECRYPT) ::GetProcAddress(hLib,"FileDecrypt");
     EncryptBuffer = (PENCRYPTBUFFER) ::GetProcAddress(hLib,"EncryptBuffer");
     DecryptBuffer = (PDECRYPTBUFFER) ::GetProcAddress(hLib,"DecryptBuffer");
     FreeBuffer = (PFREEBUFFER) ::GetProcAddress(hLib,"FreeBuffer");

     if(!FileEncrypt || !FileDecrypt ||
        !EncryptBuffer || !DecryptBuffer ||
        !FreeBuffer)
        {
         ::FreeLibrary(hLib);
         return;
        }

      // deal with files
      FileEncrypt("C:\\test.txt","123456");
      FileDecrypt("C:\\test.txt","123456");

      //deal with buffers
		DWORD dwEncSize;
		BYTE* encripted = EncryptBuffer("Yep!",4,"123456",&dwEncSize);

		if(encripted)
     		{
     		BYTE* decripted = DecryptBuffer(encripted,dwEncSize,"123456");
         		if(decripted)
		          {
	    	        ::MessageBox(NULL,(char*)decripted,NULL,MB_OK);
	    	      	FreeBuffer(decripted);
    		      } // if(decripted)

	        FreeBuffer(encripted);
    	  } // if(encripted)


      

    ::FreeLibrary(hLib);

*/
//---------------------------------------------------------------------------
/*
  FileEncrypt - encrypt file with password.

  File name - full path to file
  Password - not empty password string

return TRUE if successful, FALSE otherwise. To get extended error information, call GetLastError();
Possible error codes:

    ERROR_SUCCESS - success
	ERROR_INVALID_PARAMETER - invalid "FileName" parameter
    ERROR_INVALID_PASSWORD  - invalid "Password" parameter
    ERROR_FILE_NOT_FOUND    - file not found
    ERROR_FILE_INVALID      - unable to get file size
    ERROR_INVALID_ACCESS    - unable to create file mapping

NOTE:  The file should not be open in other program because function write encrypted data "in place".

Example of usage:

      if(FileEncrypt("C:\\test.txt","password"))
       {
        ...
       }
*/
//---------------------------------------------------------------------------
// BOOL WINAPI FileEncrypt(const char* FileName, const char* Password);
//---------------------------------------------------------------------------
/*
  FileDecrypt - decrypt file with password.

  File name - full path to file
  Password - not empty password string

return TRUE if successful, FALSE otherwise. To get extended error information, call GetLastError();
Possible error codes:

    ERROR_SUCCESS - success
	ERROR_INVALID_PARAMETER - invalid "FileName" parameter
    ERROR_INVALID_PASSWORD  - invalid "Password" parameter
    ERROR_FILE_NOT_FOUND    - file not found
    ERROR_FILE_INVALID      - unable to get file size
    ERROR_INVALID_ACCESS    - unable to create file mapping

NOTE:  The file should not be open in other program because function write decrypted data "in place".

Example of usage:

      if(FileDecrypt("C:\\test.txt","password"))
       {
        ...
       }

*/
//---------------------------------------------------------------------------
// BOOL WINAPI FileDecrypt(const char* FileName, const char* Password);
//---------------------------------------------------------------------------
/*
   EncryptBuffer - encrypt byte buffer and return encrypted buffer

   In - buffer to encrypt
   InSize - input buffer length
   ResultSize - result buffer length
   Password - password

To get extended error information, call GetLastError();
Possible error codes:

    ERROR_SUCCESS - success
	ERROR_INVALID_PARAMETER - In == NULL or !InSize or ResultSize==NULL
    ERROR_INVALID_PASSWORD  - invalid "Password" parameter
    ERROR_NOT_ENOUGH_MEMORY    - not enough memory


NOTE: The buffer, returned as function result, should be released by calling
	  FreeBuffer


Example of usage:

	DWORD dwResult;
	BYTE* buff = EncryptBuffer("test string",lstrlen("test string"),"password",&dwResult);
    if(buff)
     {
      ...

      FreeBuffer(buff);
     }
*/
//---------------------------------------------------------------------------
// BYTE* WINAPI EncryptBuffer(BYTE* In, DWORD InSize,const char* Password, DWORD* ResultSize);
//---------------------------------------------------------------------------
/*
   DecryptBuffer - encrypt byte buffer and return encrypted buffer

   In - buffer to decrypt
   InSize - input buffer length (always n*8, where n - num of octets)
   Password - password

To get extended error information, call GetLastError();
Possible error codes:

    ERROR_SUCCESS - success
	ERROR_INVALID_PARAMETER - In == NULL or !InSize
    ERROR_INVALID_PASSWORD  - invalid "Password" parameter
    ERROR_NOT_ENOUGH_MEMORY    - not enough memory


NOTE: The buffer, returned as function result, should be released by calling
	  FreeBuffer


Example of usage:

	BYTE* buff = DecryptBuffer("test string",lstrlen("test string"),"password");
    if(buff)
     {
      ...

      FreeBuffer(buff);
     }
*/
//---------------------------------------------------------------------------
// BYTE* WINAPI DecryptBuffer(BYTE* In, DWORD InSize,const char* Password);
//---------------------------------------------------------------------------
/*
  Free buffer - free buffer, returned as result of calling  EncryptBuffer or
  				DecryptBuffer

  In  - byte buffer

Example of usage:

	BYTE* buff = ... ;
    ...
    FreeBuffer(buff);
*/
//---------------------------------------------------------------------------
// void WINAPI FreeBuffer(BYTE* In);
//---------------------------------------------------------------------------
#endif
