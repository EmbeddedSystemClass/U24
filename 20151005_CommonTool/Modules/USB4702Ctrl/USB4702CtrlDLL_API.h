#ifndef _USB4702DLL_API_H
#define	_USB4702DLL_API_H



#ifndef _USB4702_DLL_EXPORT	// For importing variables
	#define USB4702_DLL __declspec(dllimport)
#else						// For exporting variables 
	#define USB4702_DLL __declspec(dllexport)
#endif // _INSTRUMENT_DLL_EXPORT

#ifndef _USB4702_DLL_EXPORT	// For importing functions
	#ifndef __cplusplus
		#define USB4702DLLAPI __declspec(dllimport)
	#else
		#define USB4702DLLAPI extern "C" __declspec(dllimport)
	#endif // __cplusplus
#else						// For exporting functions
	#ifndef __cplusplus
		#define USB4702DLLAPI __declspec(dllexport)
	#else
		#define USB4702DLLAPI extern "C" __declspec(dllexport)
	#endif // __cplusplus
#endif // _USB4702_DLL_EXPORT

/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* TYPE/STRUCTURE DEFINITION                                                */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* EXPORTED CLASS	                                                        */
/*--------------------------------------------------------------------------*/
// example: class INSTRUMENTDLL CMyClass
//			{
//				// Declaration 
//			};


/*--------------------------------------------------------------------------*/
/* EXPORTED VARIABLES                                                       */
/*--------------------------------------------------------------------------*/
// Mark's Notice
// Please use keyword "extern" if a 'constant' variable is exported.
// example of a constant: INSTRUMENTDLL extern const COLORREF MyColor = RGB(0, 0, 0);
// example of a variable: INSTRUMENTDLL int MyInt;

/*--------------------------------------------------------------------------*/
/* EXPORTED FUNCTIONS                                                       */
/*--------------------------------------------------------------------------*/
// example: INSTRUMENTDLLAPI int __stdcall func(void); 


USB4702DLLAPI int __stdcall USB4702_Init(int nAddress);
USB4702DLLAPI int __stdcall USB4702_GetDOVal(int nAddress, char * buf);
USB4702DLLAPI int __stdcall USB4702_SetDOVal(int nAddress, char * val);
USB4702DLLAPI int __stdcall USB4702_GetADCVol(int nAddress, char * Channel, char * buf);
USB4702DLLAPI int __stdcall USB4702_GetADCVolEx(int nAddress, char * Channel, char * buf);
USB4702DLLAPI int __stdcall USB4702_SetAOVol(int nAddress, char * Channel, char * vol);
USB4702DLLAPI int __stdcall USB4702_SetDOBit(int nAddress, char * bit);
USB4702DLLAPI int __stdcall USB4702_ClearDOBit(int nAddress, char * bit);

#endif // _USB4702DLL_API_H

