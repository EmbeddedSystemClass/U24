#ifndef _SERVICE_INTERFACE_H_
#define _SERVICE_INTERFACE_H_


#include <map>
#include "..\..\UI\UnifyUI_ServiceV2\PageInterface.h"

using namespace std;

//define callback function prototype
typedef size_t (*p_fn_Callback)(const wchar_t* , const wchar_t* , const wchar_t*);





// Define Service Interface
interface IServiceInterface
{
public:
	enum SrvParts
	{
		DLL			= 1  ,
		FROM_MAIN	= 10 ,
		FORM_EMPTY	= 20 ,
		FROM_DL		= 21 ,
		FORM_LOG	= 22 ,
		FORM_INFOWRITER	= 23 ,
	};



	virtual ~IServiceInterface() {}

	/* Set/Get Parameter Value */
	virtual size_t SetParameterValue(const wchar_t* wsz_keyword, wchar_t* wsz_value) = 0;
	virtual size_t GetParameterValue(const wchar_t* wsz_keyword, wchar_t* wsz_value, int i_size) = 0;

	/**
     *   UI will send message to DLL by call MsgHandler
	 *   @param  target		should be "DLL" , 
	 *	 @param  msg_key	information key
	 *	 @param	 msg_value	information value
     *   @return return 0 is OK. not 0 is other error code.
     */
	virtual size_t MsgHandler(const wchar_t* target, const wchar_t* msg_key , const wchar_t* msg_value ) = 0;		





	/**
     *   When user select the service Model, set selected Model to DLL
	 *   @param  szModel is selected model
     *   @return return 0 is OK. not 0 is other error code.
     */
	virtual size_t OnSelectModel  (const wchar_t* wszModel) = 0;	


	/**
     *   When user select the service station, set selected station to DLL
	 *   @param  szStation is selected station
     *   @return return 0 is OK. not 0 is other error code.
     */
	virtual size_t OnSelectStation  (const wchar_t* wszStation) = 0;	
	

	/**
     *   When UI release the station Dialog , notify DLL
	 *   @param  wszStation , the Station name of dlg that will be release
     *   @return return 0 is OK. not 0 is other error code.
     */
	virtual size_t OnReleaseStationDlg  (const wchar_t* wszStation) = 0;


	/**
     *   When user press the start button , notify DLL
	 *   @param  szStation is selected station
     *   @return return 0 is OK. not 0 is other error code.
     */
	virtual size_t OnStartPress  (const wchar_t* wszStation) = 0;


	/**
     *   To Reg the callback function.
	 *   @param  p_fn_Callback is a pinter to a callback function 
     *   @return return 0 is OK. not 0 is other error code.
     */
	virtual size_t RegCallback( p_fn_Callback )=0;


};

/**
 *   This is extern C function, to get c++ IServiceInterface instance.
 *   @param   Parameter is double pointer to IServiceInterface, we can get pointer to IServiceInterface
 *   @return  If success, return true. If fail, return false.
 */
typedef bool (__cdecl *p_fn_NewServiceInterface)(IServiceInterface**);

/**
 *   This is extern C function, to delete existing IServiceInterface instance.
 *   @param   pointer to IServiceInterface , this will be delete 
 *   @return  If success, return true. If fail, return false.
 */
typedef bool (__cdecl *p_fn_DelServiceInterface)(IServiceInterface*);




class CStnInstance
{
public:
	CStnInstance()
	{
		FormStyle=IServiceInterface::FORM_EMPTY;
		p_Dlg=NULL;
	}

public:
	IServiceInterface::SrvParts FormStyle;
	CPageInterface_Service*  p_Dlg;

};



class CChildFormMsg
{
public:
	CString target;
	CString msg_key;
	CString msg_value;
};



typedef struct ModelInfo {
	CString SelStation;

	//keep config file key and value mapping
	std::map<CString, CString> ModelConfig;

	//keep information of each station
	std::map<CString, CStnInstance*> StationList;

} ModelInfo_t;



typedef struct MainFormInfo {
	CString ToolDir;
	CString ModelsDir;
	CString SelModel;

	std::map<CString, CString> ModelList;

} MainformInfo_t;




#endif


