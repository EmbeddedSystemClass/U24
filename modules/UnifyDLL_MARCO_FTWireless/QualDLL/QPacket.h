/**
* @file: QPacket.h
*
* reference:
*   80-39912-1_B_DMSS_Download_Protocol.pdf
*   80-V5348-1_F_STREAMING_DLoad_Protocol.pdf
*   80-VP758-1_A_Emergency_Download_Feature.pdf
*   80-V6135-1_F_Diag_Svc_ISOD.pdf
*/
#pragma once

static const int HEX_BUFFER_SIZE  = 0x03f9; // 0x03f9 = 1017
static const int PKT_BUFFER_SIZE  = 1024;
static const int QCMD_DELAY_LONG  = 1000; // delay 1 sec
static const int QCMD_DELAY_SHORT = 200;  // delay 200 ms
static const int QCMD_DELAY_OEPN  = 10*1000; // delay 10 sec
static const int QCMD_MAX_RETRY = 10;
static const int QCMD_DATA_SIZE = PKT_BUFFER_SIZE;

static const int ERROR_QCOM_DMSS_CMD_BASE =  39912 * 1000;
static const int ERROR_QCOM_STRMNG_CMD_BASE = 5348 * 1000;
static const int ERROR_QCOM_SPEEDUP_BASE =    9578 * 1000;
static const int ERROR_QCOM_EMDL_BASE =        758 * 1000;

/////////////////////////////////////////////////////////////////////////////
typedef enum QthAustinSpeedupImageType
{
	OPEN_MODE_NONE       = 0x00, // Not opened yet
	OPEN_MODE_PBL        = 0x01, // Primary Boot Loader
	OPEN_MODE_QCSBLHDCFG = 0x02, // QC 2ndary Boot Loader Header and
	OPEN_MODE_QCSBL      = 0x03, // QC 2ndary Boot Loader
	OPEN_MODE_OEMSBL     = 0x04, // OEM 2ndary Boot Loader
	OPEN_MODE_AMSS       = 0x05, // AMSS modem executable
	OPEN_MODE_APPS       = 0x06, // APPS executable
	OPEN_MODE_OBL        = 0x07, // OTP Boot Loader
	OPEN_MODE_FOTAUI     = 0x08, // FOTA UI binarh
	OPEN_MODE_CEFS       = 0x09, // Modem CEFS image
	OPEN_MODE_APPSBL     = 0x0A, // APPS Boot Loader
	OPEN_MODE_APPS_CEFS  = 0x0B, // APPS CEFS image
	OPEN_MODE_FLASH_BIN  = 0x0C, // Flash.bin image for Windows mobile
	OPEN_MODE_DSP1       = 0x0D, // DSP1 runtime image
	OPEN_MODE_CUSTOM     = 0x0E, // Image for user defined partition
	OPEN_MODE_DBL        = 0x0F, // DBL Image for SB Architecture 2.0
	OPEN_MODE_OSBL       = 0x10, // OSBL Image for SB Architecture 2.0
	OPEN_MODE_FSBL       = 0x11, // FSBL Image for SB Architecture 2.0
	OPEN_MODE_DSP2       = 0x12, // DSP3 executable
	OPEN_MODE_DT		 = 0x15, // QTH DT
	OPEN_MODE_RESERVED   = 0x16, // Austin RESERVED for backup NV
	OPEN_MODE_SIMSECURE  = 0x17, // Austin backup for SIMSECURE
	OPEN_MODE_BAK_DT     = 0x18, // backup DT image
	OPEN_MODE_BAK_FSBL   = 0x19, // backup FSBL image
	OPEN_MODE_BAK_OSBL   = 0x1A, // backup OSBL image
	OPEN_MODE_BAK_APPSBL = 0x1B, // backup APPSBL image
	Q8K_OPEN_MULTI_MODE_EMMC_USER = 0x21 
} qth_spdp_img_t;

/**
* DMSS Download Protocol
* used by OSBL
* 80-39912-1_B_DMSS_Download_Protocol.pdf
*/
typedef enum QcomDmssCmd
{
	DMSS_CMD_WRITE  = 0x01, /* Write a block of data to memory (Host)  */
	DMSS_CMD_ACK    = 0x02, /* Acknowledge receiving a packet  (Phone) */
	DMSS_CMD_NAK    = 0x03, /* Acknowledge a bad packet        (Phone) */
	DMSS_CMD_ERASE  = 0x04, /* Erase a block of memory         (Host)  */
	DMSS_CMD_GO     = 0x05, /* Begin execution at an address   (Host)  */
	DMSS_CMD_NOP    = 0x06, /* No operation, for debug         (Host)  */
	DMSS_CMD_PREQ   = 0x07, /* Request implementation info     (Host)  */
	DMSS_CMD_PARAMS = 0x08, /* Provide implementation info     (Phone) */
	DMSS_CMD_DUMP   = 0x09, /* Debug: dump a block of memory   (Host)  */
	DMSS_CMD_RESET  = 0x0A, /* Reset the phone                 (Host)  */
	DMSS_CMD_UNLOCK = 0x0B, /* Unlock access to secured ops    (Host)  */
	DMSS_CMD_VERREQ = 0x0C, /* Request software version info   (Host)  */
	DMSS_CMD_VERRSP = 0x0D, /* Provide software version info   (Phone) */
	DMSS_CMD_PWROFF = 0x0E, /* Turn phone power off            (Host)  */
	DMSS_CMD_WRITE_32BIT = 0x0F, /* Write a block of data to 32-bit memory address (Host) */
	DMSS_CMD_MEM_DEBUG_QUERY = 0x10, /* Memory debug query     (Host)  */
	DMSS_CMD_MEM_DEBUG_INFO  = 0x11, /* Memory debug info      (Phone) */
	DMSS_CMD_MEM_READ_REQ    = 0x12, /* Memory read request    (Host)  */
	DMSS_CMD_MEM_READ_RESP   = 0x13, /* Memory read response   (Phone) */
	// Note: 14-7F Reserved
	// Note: 80-FE Reserved for factory test commands
	// Note: FF is Illegal
	QTH_DMSS_CMD_FLASHWRITE = 0x1d,
	QTH_DMSS_CMD_CHECKSUM   = 0x1e,
	DMSS_CMD_DLOAD_SWITCH   = 0x3A,
	QTH_DMSS_CMD_HS_WRITE     = 0x2F, /* Qisda YuTeng 080715 used for high speed transfer */
	QTH_DMSS_CMD_HS_WRITE_RSP = 0xAA, /* Qisda YuTeng 080715 used for high speed transfer */
} qcom_dmss_cmd_t;

/**
* Streaming DLoad Protocol
* used by HEX
* 80-V5348-1_F_STREAMING_DLoad_Protocol.pdf
*/
typedef enum QcomStreamingCmd
{
	STREAMING_CMD_HELLO	   = 0x01, /* Initialize connection    */
	STREAMING_CMD_HELLORESP= 0x02, /* Acknowledge the connection, ready to unlock */
	STREAMING_CMD_READ     = 0x03, /* Read some number of bytes from phone memory */
	STREAMING_CMD_READDATA = 0x04, /* Response to a simple read */
	STREAMING_CMD_SIMWRITE = 0x05, /* Write data without erase */
	STREAMING_CMD_WRODEOK  = 0x06, /* Response to simple write */
	STREAMING_CMD_STRWRITE = 0x07, /* Streaming write command */
	STREAMING_CMD_BOLCKWRI = 0x08, /* Response to stream write */ 
	STREAMING_CMD_NOP      = 0x09, /* No operation */
	STREAMING_CMD_NOPRESP  = 0x0A, /* Response to NOP */
	STREAMING_CMD_RESET    = 0x0B, /* Reset target */
	STREAMING_CMD_RESETACK = 0x0C, /* Response to reset */
	STREAMING_CMD_ERROR    = 0x0D, /* Target error */
	STREAMING_CMD_LOG      = 0x0E, /* Target informational message */
	STREAMING_CMD_UNLOCK   = 0x0F, /* Unlock target */
	STREAMING_CMD_UNLOCKED = 0x10, /* Response to unlock */
	STREAMING_CMD_POWEROFF = 0x11, /* Power off target */
	STREAMING_CMD_POWDOWN  = 0x12, /* Response to power off */
	STREAMING_CMD_OPEN     = 0x13, /* Open for writing image */
	STREAMING_CMD_OPENED   = 0x14, /* Response to open for writing image */
	STREAMING_CMD_CLOSE    = 0x15, /* Close and flush last partial write to flash */
	STREAMING_CMD_CLOSED   = 0x16, /* Response to close and flush last partial write to flash */
	STREAMING_CMD_SECMODE  = 0x17, /* Send security mode to use for programming images */
	STREAMING_CMD_SECMODER = 0x18, /* Response to send security mode*/
	STREAMING_CMD_PATTTIONT    = 0x19, /* Send partition table to use for programming images */
	STREAMING_CMD_PATTTIONTR   = 0x1A, /* Response to send partition table */
	STREAMING_CMD_OPENMIMAGE   = 0x1B, /* Open for writing image ( Multi-image mode only ) */
	STREAMING_CMD_OPENEDMIMAGE = 0x1C, /* Response to open for writing image */
	STREAMING_CMD_ERASEFLASH  = 0x1D, /* Erase entire Flash device */
	STREAMING_CMD_EFLASERASED = 0x1E, /* Response to erase flash */
	STREAMING_CMD_GETECCSTATE = 0x1F, /* Read current ECC generation status */
	STREAMING_CMD_ECCSTATE    = 0x20, /* Response to Get ECC state with current stats */
	STREAMING_CMD_SETECC      = 0x21, /* Enable/disable hardware ECC generation */
	STREAMING_CMD_SETECCR     = 0x22,  /* Response to set ECC */
	STREAMING_CMD_IRAMREAD    = 0x26, /* Read some number of bytes from phone memory (IARM) */
	STREAMING_CMD_FASTBOOTFLAG    = 0x2A, /* Send fastboot flag for rebooting to enter fastboot mode */
	STREAMING_CMD_SBLREAD		= 0xF1, /* Error code for dump SBL fail */
	STREAMING_CMD_PBLREAD		= 0xF2 /* Error code for demp PBL fail */
} qcom_strmng_cmd_t;

//// Defines for Open Multi command
typedef enum QcomStreamingOpenMultiStatus
{
	OPEN_MULTI_SUCCESS             = 0x00,
	OPEN_MULTI_LENGTH_EXCEEDED     = 0x01,
	OPEN_MULTI_PAYLOAD_NOT_ALLOWED = 0x02,
	OPEN_MULTI_PAYLOAD_REQUIRED    = 0x03,
	OPEN_MULTI_UNKNOWN_ERROR       = 0x04,
	OPEN_MULTI_UNKNOWN_PARTITION   = 0x05,
};

typedef enum QcomNvStat
{
	NV_DONE_S,          //!<' Request completed okay 
	NV_BUSY_S,          //!<' Request is queued
	NV_BADCMD_S,        //!<' Unrecognizable command field
	NV_FULL_S,          //!<' The NVM is full
	NV_FAIL_S,          //!<' Command failed, reason other than NVM was full 
	NV_NOTACTIVE_S,     //!<' Variable was not active
	NV_BADPARM_S,       //!<' Bad parameter in command block
	NV_READONLY_S,      //!<' Parameter is write-protected and thus read only
	NV_BADTG_S,         //!<' Item not valid for Target 
	NV_NOMEM_S,         //!<' free memory exhausted
	NV_NOTALLOC_S,      //!<' address is not a valid allocation
	NV_UNREADABLE_S  = 98, ////  created by Nek Wang
	NV_SPLOCK_S		 = 99, ////  created by Nek Wang
	NV_STAT_ENUM_PAD = 0x7FFF //!<' Pad to 16 bits on ARM
} qcom_nv_stat_t;

typedef enum QcomDiagCmd
{
	DIAG_BAD_PARM_F				= 0x14, /* Invalid parmaeter Response */
	DIAG_CMD_ReadNVitem			= 0x26, /* Nonvolatile item read request Message */
	DIAG_CMD_WriteNVitem		= 0x27, /* Nonvolatile item read request Message */
	DIAG_CMD_ChaneMode			= 0x29, /* Mode change Request Message */	
	DIAG_SPC_F					= 0x41, /* Send the Service Prog. Code to allow SP */
	DIAG_BAD_SPC_MODE_F			= 0x42, /* Invalid nv_read/write because SP is locked */
	DIAG_SUBSYS_CMD_F			= 0x4B, /* SubSystem dispatcher (extended diag cmd) */
	DIAG_SUBSYS_DISPATCH_CMD	= 0x80,
	DIAG_PROTOCOL_LOOPBACK_F	= 0x7B  /* Is Phone Connected request MEssage */
} qcom_diag_cmd_t;

typedef enum H21DiagSubSysCmd
{
	SUB_SYSCMD_DOWNLOAD = 0x5,      
	SUB_SYSCMD_CDROM = 0x6,     
	SUB_SYSCMD_RESET = 0x7,
	SUB_SYSCMD_ERASE_AMSS_ONE_BLOCK	= 0x8
} h21_diag_sub_syscmd_t;

typedef struct RequestQthDmssCommand
{
	byte cmd_code;
	byte image_type;
	byte image_size[4];
} req_qth_dmss_cmd_t;

typedef struct ResponesQthChecksum
{
	byte code;
	byte checksum[4];
} rsp_qth_checksum_t;

typedef struct RequestQthDmssDdrWrite
{
	byte cmd_code;
	byte image_type;
	byte image_size[4];
	byte length[2];
} req_qth_dmss_ddr_write_t;

typedef struct RequestGeneralCommand
{
	byte code;
} req_cmd_t;

typedef struct ResponseGeneralCommand
{
	byte code;
	byte data[QCMD_DATA_SIZE];
} rsp_cmd_t;

typedef struct ResponseStatus
{
	byte code;
	byte status;
	byte data[QCMD_DATA_SIZE];
} rsp_status_t;

typedef struct ResponseDmssVersion
{
	byte code;
	byte length;
	byte version[32];
	byte data[QCMD_DATA_SIZE];
} rsp_dmss_version_t;

typedef struct ResponseDmssParameter
{
	byte code;
	byte version;
	byte min_version;
	byte max_write_size[2];
	byte model;
	byte device_size;
	byte device_type;
} rsp_dmss_param_t;

typedef struct RequestDmssWrite32b
{
	byte code;
	byte address[4];
	byte length[2];
	byte data[QCMD_DATA_SIZE];
} req_dmss_write_32b_t;

typedef struct RequestDmssGo
{
	byte code;
	byte segment[2];
	byte offset[2];
} req_dmss_go_t;

typedef struct RequestStreamingHello
{
	byte code;
	byte magic_number[32];
	byte version;
	byte compatible_version;
	byte feature_bits;
} req_strmng_hello_t;

typedef struct ResponseStreamingHello
{
	byte code;
	byte magic_number[32];
	byte version;
	byte compatible_version;
	byte max_block_size[4];
	byte base_address[4];
	byte data[QCMD_DATA_SIZE];
} rsp_strmng_hello_t;

typedef struct RequestStreamingStrWrite
{
	byte code;
	byte address[4];
	byte data[QCMD_DATA_SIZE];
} req_strmng_write_t;

typedef struct RequestStreamingStrRead
{
	byte code;
	byte address[4];
	byte length[2];
} req_strmng_Read_t;

typedef struct ResponseStreamingStrRead
{
	BYTE code ;
	BYTE address[4];
	BYTE dummy[2048];
}rsp_strmng_read_t;

typedef struct RequestStreamingOpenImage
{
	byte code;
	byte type;
} req_strmng_img_t;

typedef struct RequestStreamingOpenImage7k
{
	byte code;
	byte type;
	byte header[512];
} req_strmng_img_7k_t;

typedef struct RequestStreamingSecurityMode
{
	byte cmd_code;
	byte mode;
} req_strmng_sec_mode_t;

typedef struct RequestPartitionTable
{
	byte code;
	byte repartition;
	byte table[QCMD_DATA_SIZE];
} req_strmng_ptl_t;

enum ResponsePartitionTableStatus
{
	QPTL_ACCEPT = 0x0,
	QPTL_DIFFER = 0x1,
	QPTL_NACCPT = 0x2,
	QPTL_ERASE_FAIL = 0x3,
};

typedef struct RequestSpcCommand
{
	byte code;
	byte spc[6];
} req_spc_t;

typedef struct NvCommand
{
	byte code;
	byte id[2];
	byte data[128];
	byte status[2];
} nv_cmd_t;

typedef struct RequestStreamingIRAMRead
{
	byte code;
	byte address[4];
	byte length[2];
} req_strmng_iram_read_t;

typedef struct ResponseStreamingIRAMRead
{
	byte code;
	byte address[4];
	byte dummy[2048];
} rsp_strmng_iram_read_t;