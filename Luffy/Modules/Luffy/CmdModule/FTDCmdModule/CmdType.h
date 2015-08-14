/*==== DECLARATION CONTROL ==================================================*/
#ifndef _CMD_TYPE_H_
#define _CMD_TYPE_H_

enum PacketCommandCode {
	PKT_REQUEST    = 0x10,
	PKT_ACK        = 0x11,
	PKT_PROGRESS   = 0x12,
	PKT_RESULT     = 0x13,
};

static const int PKT_BLOCK_SIZE =   32;

#define PKT_HEADER_SIZE  4

//erorr list
//we may need to verify
//1.which command is fail
//2.command fail reason

//Level 1:  transmission level (RS232/USB)
//The reason may be comport error,driver problem , or target no response
//Is it necessary to handle all kind of situation?
enum P21CommandTransErrorCode {
	HS_READ_DATA_FAIL         = -11,
	HS_WRITE_DATA_FAIL        = -12,
	HS_READ_DL_PROTOCOL_FAIL  = -13,
	HS_WRITE_DL_PROTOCOL_FAIL = -14,
	HS_READ_DL_DATA_FAIL      = -15,
	HS_WRITE_DL_DATA_FAIL     = -16,
};

//Level 2: A general command
enum P21CommandReturnCode {
	//FFYang def
	HS_PROGRESS = 88,
	HS_OK       =  1,
	HS_FAIL     =  0, //The command is complete but the target response fail.
	HS_TIMEOUT             = -1,
	HS_NOT_INITIALIZE      = -2,
	HS_OPEN_DEVICE_FAIL    = -3,
	HS_FILE_NOT_FOUND      = -4,
	HS_INVALID_CHECKSUM    = -5,
	HS_INVALID_SIZE        = -6,
	HS_PC_LOAD_FILE_FAIL   = -7,
	HS_ADB_OPEN_TCP_FAIL    = -8,  // 2010-06-23 Evonne: error code for open socket
	HS_STARTUP_SOCKET_FAIL  = -9,
	HS_CREATE_SOCKET_FAIL   = -10,
	HS_CONNECT_SOCKET_FAIL  = -11,

	//FFYang def
	HS_GET_DEVICE_FAIL = -98,
	HS_SET_DEVICE_FAIL = -99,
};

enum PortType {
	PORT_TCP = 2000, // TCP
};

#endif /* _CMD_TYPE_H_ */
