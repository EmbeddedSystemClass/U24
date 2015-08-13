// FastbootRWFA.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FastbootRWFA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	} else {
	}

	return nRetCode;
}

FASTBOOTRWFA_API bool  ReadFA (char* sz_port, unsigned char* sz_FAData, char* sz_errorCode, char* sz_errorMsg)
{
	/* Check input */
	if (sz_port == NULL) {
		strcpy(sz_errorMsg, "[ReadFA] COMPort is empty.");
		return false;
	}

	/* Get COM port */
	int i_port = 0;
	i_port = ::atoi(sz_port);
	char sz_comPort[10];
	memset(sz_comPort, 0, sizeof(sz_comPort));
	sprintf(sz_comPort, "#%02x#{", i_port);

	/* Get ADB Device */
	ADBDevice* p_adbDevice = NULL;
	p_adbDevice = new ADBDevice(adb_class_idFA, 0, 0, DEVICE_INTERFACE_ID, sz_comPort, 100);
	if (p_adbDevice) {
		if (p_adbDevice->IsActive() == false) {
			delete p_adbDevice;
			p_adbDevice = NULL;
			strcpy(sz_errorMsg, "[ReadFA] Get ADB device Fail.");
			return false;
		}
	} else {
		strcpy(sz_errorMsg, "[ReadFA] Get ADB device Fail.");
		return false;
	}

	/* Read FA */
	int i_length = FALENGTH;
	bool b_result = false;
	char sz_buffer[512] = {0};

	// dump protocol --- "dump:%s"  example:"dump:otpfa"
	char sz_command[64] = {0};
	sprintf(sz_command, "dump:%s", "otpfa");

	// ADB Write download protocol
	if (p_adbDevice->Write(sz_command, strlen(sz_command))) {

		// ADB Read
		RESPONSE enum_readRosponse = UNKNOW;
		enum_readRosponse = p_adbDevice->ReadResponse();
		if (enum_readRosponse == RESPONSE::OKAY) {
				
			// upload command --- "upload:%08x"  example:"upload:0007d000"
			char sz_command[64] = {0};
			sprintf(sz_command, "upload:%08x", i_length);

			// ADB Write
			if (p_adbDevice->Write(sz_command, strlen(sz_command))) {
				b_result = (p_adbDevice->ReadResponse() == RESPONSE::DATA);
				if (!b_result) {
					delete p_adbDevice;
					p_adbDevice = NULL;
					strcpy(sz_errorMsg, "[ReadFA] Read upload command response Fail.");
					return false;
				} else {
					unsigned long l_readLen = 0;
					if (p_adbDevice->Read(sz_buffer, sizeof(sz_buffer), l_readLen) && (l_readLen == FALENGTH)) {
						memcpy(sz_FAData, sz_buffer, FALENGTH);
					} else {
						delete p_adbDevice;
						p_adbDevice = NULL;
						strcpy(sz_errorMsg, "[ReadFA] Read upload data Fail.");
						return false;
					}

					b_result = (p_adbDevice->ReadResponse() == RESPONSE::OKAY);
					if (!b_result) {
						delete p_adbDevice;
						p_adbDevice = NULL;
						strcpy(sz_errorMsg, "[ReadFA] Read upload data response Fail.");
						return false;
					}
				}
			} else {
				delete p_adbDevice;
				p_adbDevice = NULL;
				strcpy(sz_errorMsg, "[ReadFA] Write upload command Fail.");
				return false;
			}
		}
		else if (enum_readRosponse == RESPONSE::FAILFAEMPTY) {
			char sz_buffer[512] = {0};
			memcpy(sz_FAData, sz_buffer, FALENGTH);
		} else {
			delete p_adbDevice;
			p_adbDevice = NULL;
			strcpy(sz_errorMsg, "[ReadFA] Read dump command response Fail.");
			return false;
		}
	} else {
		delete p_adbDevice;
		p_adbDevice = NULL;
		strcpy(sz_errorMsg, "[ReadFA] Write dump command Fail.");
		return false;
	}

	delete p_adbDevice;
	p_adbDevice = NULL;

	return true;
}

FASTBOOTRWFA_API bool  WriteFA(char* sz_port, unsigned char* sz_FAData, char* sz_errorCode, char* sz_errorMsg)
{	
	/* Check input */
	if (sz_port == NULL) {
		strcpy(sz_errorMsg, "[WriteFA] COMPort is empty.");
		return false;
	}

	/* Get COM port */
	int i_port = 0;
	i_port = ::atoi(sz_port);
	char sz_comPort[10];
	memset(sz_comPort, 0, sizeof(sz_comPort));
	sprintf(sz_comPort, "#%02x#{", i_port);

	/* Get ADB Device */
	ADBDevice* p_adbDevice = NULL;
	p_adbDevice = new ADBDevice(adb_class_idFA, 0, 0, DEVICE_INTERFACE_ID, sz_comPort, 100);
	if (p_adbDevice) {
		if (p_adbDevice->IsActive() == false) {
			delete p_adbDevice;
			p_adbDevice = NULL;
			return false;
		}
	} else {
		return false;
	}

	/* Write FA */
	int i_length = FALENGTH;
	bool b_result = false;

	// download protocol --- "download:%08x"  example:"download:00162c00"
	char sz_command[64] = {0};
	sprintf(sz_command, "download:%08x", i_length);

	// ADB Write download protocol
	if (p_adbDevice->Write(sz_command, strlen(sz_command))) {

		// ADB Read
		if (p_adbDevice->ReadResponse() == RESPONSE::DATA) {
			char* c_p_data = (char*) sz_FAData;
			int i_count = 0;
			bool b_writeSuccess = true;
	
			// Max write byte:4096
			while (i_length > 0 && b_writeSuccess) 
			{
				int xfer = (i_length > 4096) ? 4096 : i_length;

				// ADB Write
				b_writeSuccess = p_adbDevice->Write(c_p_data, xfer);
					i_count   += xfer;
					i_length -= xfer;
					c_p_data  += xfer;
			};

			if (!b_writeSuccess) {
				delete p_adbDevice;
				p_adbDevice = NULL;
				strcpy(sz_errorMsg, "[WriteFA] Write download data Fail.");
				return false;
			}

			if (p_adbDevice->ReadResponse() == RESPONSE::OKAY) {

				// flash command --- ""flash:%s""  example:"flash boot"
				char sz_command[64] = {0};
				sprintf(sz_command, "flash:%s", "otpfa");

				// ADB Write
				if (p_adbDevice->Write(sz_command, strlen(sz_command))) {
					b_result = (p_adbDevice->ReadResponse() == RESPONSE::OKAY);
					if (!b_result) {
						delete p_adbDevice;
						p_adbDevice = NULL;
						strcpy(sz_errorMsg, "[WriteFA] Read flash response Fail.");
						return false;
					}
				} else {
					delete p_adbDevice;
					p_adbDevice = NULL;
					strcpy(sz_errorMsg, "[WriteFA] Write flash command Fail.");
					return false;
				}
			} else {
				delete p_adbDevice;
				p_adbDevice = NULL;
				strcpy(sz_errorMsg, "[WriteFA] Read download data response Fail.");
				return false;
			}
		} else {
			delete p_adbDevice;
			p_adbDevice = NULL;
			strcpy(sz_errorMsg, "[WriteFA] Read download response Fail.");
			return false;	
		}

	} else {
		delete p_adbDevice;
		p_adbDevice = NULL;
		strcpy(sz_errorMsg, "[WriteFA] Write download command Fail.");
		return false;
	}

	delete p_adbDevice;
	p_adbDevice = NULL;

	return true;
}
