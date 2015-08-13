/*
 NFCerr.mc

   Error message codes from the Smart Card Resource Manager
   These messages must be reconciled with winerror.w
   They exist here to provide error messages on pre-Win2K systems.

*/
#ifndef NFC_S_SUCCESS
//
// =============================
// Facility NFC Error Messages
// =============================
//
#define NFC_S_SUCCESS NO_ERROR
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0x0
#define FACILITY_NFC                   	 0x10


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: NFC_F_INTERNAL_ERROR
//
// MessageText:
//
//  An internal consistency check failed.
//
#define NFC_F_INTERNAL_ERROR           ((DWORD)0x80100001L)

//
// MessageId: NFC_E_CANCELLED
//
// MessageText:
//
//  The action was cancelled by an NFCCancel request.
//
//#define NFC_E_CANCELLED                ((DWORD)0x80100002L)

//
// MessageId: NFC_E_INVALID_HANDLE
//
// MessageText:
//
//  The supplied handle was invalid.
//
#define NFC_E_INVALID_HANDLE           ((DWORD)0x80100003L)

//
// MessageId: NFC_E_INVALID_PARAMETER
//
// MessageText:
//
//  One or more of the supplied parameters could not be properly interpreted.
//
#define NFC_E_INVALID_PARAMETER        ((DWORD)0x80100004L)

//
// MessageId: NFC_E_INVALID_TARGET
//
// MessageText:
//
//  Registry startup information is missing or invalid.
//
//#define NFC_E_INVALID_TARGET           ((DWORD)0x80100005L)

//
// MessageId: NFC_E_NO_MEMORY
//
// MessageText:
//
//  Not enough memory available to complete this command.
//
#define NFC_E_NO_MEMORY                ((DWORD)0x80100006L)

//
// MessageId: NFC_F_WAITED_TOO_LONG
//
// MessageText:
//
//  An internal consistency timer has expired.
//
//#define NFC_F_WAITED_TOO_LONG          ((DWORD)0x80100007L)

//
// MessageId: NFC_E_INSUFFICIENT_BUFFER
//
// MessageText:
//
//  The data buffer to receive returned data is too small for the returned data.
//
//#define NFC_E_INSUFFICIENT_BUFFER      ((DWORD)0x80100008L)

//
// MessageId: NFC_E_UNKNOWN_READER
//
// MessageText:
//
//  The specified reader name is not recognized.
//
#define NFC_E_UNKNOWN_READER           ((DWORD)0x80100009L)

//
// MessageId: NFC_E_TIMEOUT
//
// MessageText:
//
//  The user-specified timeout value has expired.
//
#define NFC_E_TIMEOUT                  ((DWORD)0x8010000AL)

//
// MessageId: NFC_E_SHARING_VIOLATION
//
// MessageText:
//
//  The smart card cannot be accessed because of other connections outstanding.
//
#define NFC_E_SHARING_VIOLATION        ((DWORD)0x8010000BL)

//
// MessageId: NFC_E_NO_SMARTCARD
//
// MessageText:
//
//  The operation requires a Smart Card, but no Smart Card is currently in the device.
//
#define NFC_E_NO_SMARTCARD             ((DWORD)0x8010000CL)

//
// MessageId: NFC_E_UNKNOWN_CARD
//
// MessageText:
//
//  The specified smart card name is not recognized.
//
#define NFC_E_UNKNOWN_CARD             ((DWORD)0x8010000DL)

//
// MessageId: NFC_E_CANT_DISPOSE
//
// MessageText:
//
//  The system could not dispose of the media in the requested manner.
//
//#define NFC_E_CANT_DISPOSE             ((DWORD)0x8010000EL)

//
// MessageId: NFC_E_PROTO_MISMATCH
//
// MessageText:
//
//  The requested protocols are incompatible with the protocol currently in use with the smart card.
//
//#define NFC_E_PROTO_MISMATCH           ((DWORD)0x8010000FL)

//
// MessageId: NFC_E_NOT_READY
//
// MessageText:
//
//  The reader or smart card is not ready to accept commands.
//
//#define NFC_E_NOT_READY                ((DWORD)0x80100010L)

//
// MessageId: NFC_E_INVALID_VALUE
//
// MessageText:
//
//  One or more of the supplied parameters values could not be properly interpreted.
//
#define NFC_E_INVALID_VALUE            ((DWORD)0x80100011L)

//
// MessageId: NFC_E_SYSTEM_CANCELLED
//
// MessageText:
//
//  The action was cancelled by the system, presumably to log off or shut down.
//
//#define NFC_E_SYSTEM_CANCELLED         ((DWORD)0x80100012L)

//
// MessageId: NFC_F_COMM_ERROR
//
// MessageText:
//
//  An internal communications error has been detected.
//
//#define NFC_F_COMM_ERROR               ((DWORD)0x80100013L)

//
// MessageId: NFC_F_UNKNOWN_ERROR
//
// MessageText:
//
//  An internal error has been detected, but the source is unknown.
//
#define NFC_F_UNKNOWN_ERROR            ((DWORD)0x80100014L)

//
// MessageId: NFC_E_INVALID_ATR
//
// MessageText:
//
//  An ATR obtained from the registry is not a valid ATR string.
//
//#define NFC_E_INVALID_ATR              ((DWORD)0x80100015L)

//
// MessageId: NFC_E_NOT_TRANSACTED
//
// MessageText:
//
//  An attempt was made to end a non-existent transaction.
//
//#define NFC_E_NOT_TRANSACTED           ((DWORD)0x80100016L)

//
// MessageId: NFC_E_READER_UNAVAILABLE
//
// MessageText:
//
//  The specified reader is not currently available for use.
//
#define NFC_E_READER_UNAVAILABLE       ((DWORD)0x80100017L)

//
// MessageId: NFC_P_SHUTDOWN
//
// MessageText:
//
//  The operation has been aborted to allow the server application to exit.
//
//#define NFC_P_SHUTDOWN                 ((DWORD)0x80100018L)

//
// MessageId: NFC_E_PCI_TOO_SMALL
//
// MessageText:
//
//  The PCI Receive buffer was too small.
//
//#define NFC_E_PCI_TOO_SMALL            ((DWORD)0x80100019L)

//
// MessageId: NFC_E_READER_UNSUPPORTED
//
// MessageText:
//
//  The reader driver does not meet minimal requirements for support.
//
#define NFC_E_READER_UNSUPPORTED       ((DWORD)0x8010001AL)

//
// MessageId: NFC_E_DUPLICATE_READER
//
// MessageText:
//
//  The reader driver did not produce a unique reader name.
//
//#define NFC_E_DUPLICATE_READER         ((DWORD)0x8010001BL)

//
// MessageId: NFC_E_CARD_UNSUPPORTED
//
// MessageText:
//
//  The smart card does not meet minimal requirements for support.
//
#define NFC_E_CARD_UNSUPPORTED         ((DWORD)0x8010001CL)

//
// MessageId: NFC_E_NO_SERVICE
//
// MessageText:
//
//  The Smart card resource manager is not running.
//
//#define NFC_E_NO_SERVICE               ((DWORD)0x8010001DL)

//
// MessageId: NFC_E_SERVICE_STOPPED
//
// MessageText:
//
//  The Smart card resource manager has shut down.
//
//#define NFC_E_SERVICE_STOPPED          ((DWORD)0x8010001EL)

//
// MessageId: NFC_E_UNEXPECTED
//
// MessageText:
//
//  An unexpected card error has occurred.
//
//#define NFC_E_UNEXPECTED               ((DWORD)0x8010001FL)

//
// MessageId: NFC_E_ICC_INSTALLATION
//
// MessageText:
//
//  No Primary Provider can be found for the smart card.
//
//#define NFC_E_ICC_INSTALLATION         ((DWORD)0x80100020L)

//
// MessageId: NFC_E_ICC_CREATEORDER
//
// MessageText:
//
//  The requested order of object creation is not supported.
//
//#define NFC_E_ICC_CREATEORDER          ((DWORD)0x80100021L)

//
// MessageId: NFC_E_UNSUPPORTED_FEATURE
//
// MessageText:
//
//  This smart card does not support the requested feature.
//
//#define NFC_E_UNSUPPORTED_FEATURE      ((DWORD)0x80100022L)

//
// MessageId: NFC_E_DIR_NOT_FOUND
//
// MessageText:
//
//  The identified directory does not exist in the smart card.
//
//#define NFC_E_DIR_NOT_FOUND            ((DWORD)0x80100023L)

//
// MessageId: NFC_E_FILE_NOT_FOUND
//
// MessageText:
//
//  The identified file does not exist in the smart card.
//
//#define NFC_E_FILE_NOT_FOUND           ((DWORD)0x80100024L)

//
// MessageId: NFC_E_NO_DIR
//
// MessageText:
//
//  The supplied path does not represent a smart card directory.
//
//#define NFC_E_NO_DIR                   ((DWORD)0x80100025L)

//
// MessageId: NFC_E_NO_FILE
//
// MessageText:
//
//  The supplied path does not represent a smart card file.
//
//#define NFC_E_NO_FILE                  ((DWORD)0x80100026L)

//
// MessageId: NFC_E_NO_ACCESS
//
// MessageText:
//
//  Access is denied to this file.
//
//#define NFC_E_NO_ACCESS                ((DWORD)0x80100027L)

//
// MessageId: NFC_E_WRITE_TOO_MANY
//
// MessageText:
//
//  The smartcard does not have enough memory to store the information.
//
//#define NFC_E_WRITE_TOO_MANY           ((DWORD)0x80100028L)

//
// MessageId: NFC_E_BAD_SEEK
//
// MessageText:
//
//  There was an error trying to set the smart card file object pointer.
//
//#define NFC_E_BAD_SEEK                 ((DWORD)0x80100029L)

//
// MessageId: NFC_E_INVALID_CHV
//
// MessageText:
//
//  The supplied PIN is incorrect.
//
//#define NFC_E_INVALID_CHV              ((DWORD)0x8010002AL)

//
// MessageId: NFC_E_UNKNOWN_RES_MNG
//
// MessageText:
//
//  An unrecognized error code was returned from a layered component.
//
//#define NFC_E_UNKNOWN_RES_MNG          ((DWORD)0x8010002BL)

//
// MessageId: NFC_E_NO_SUCH_CERTIFICATE
//
// MessageText:
//
//  The requested certificate does not exist.
//
//#define NFC_E_NO_SUCH_CERTIFICATE      ((DWORD)0x8010002CL)

//
// MessageId: NFC_E_CERTIFICATE_UNAVAILABLE
//
// MessageText:
//
//  The requested certificate could not be obtained.
//
//#define NFC_E_CERTIFICATE_UNAVAILABLE  ((DWORD)0x8010002DL)

//
// MessageId: NFC_E_NO_READERS_AVAILABLE
//
// MessageText:
//
//  Cannot find a smart card reader.
//
#define NFC_E_NO_READERS_AVAILABLE     ((DWORD)0x8010002EL)

//
// MessageId: NFC_E_COMM_DATA_LOST
//
// MessageText:
//
//  A communications error with the smart card has been detected.  Retry the operation.
//
#define NFC_E_COMM_DATA_LOST           ((DWORD)0x8010002FL)

//
// MessageId: NFC_E_NO_KEY_CONTAINER
//
// MessageText:
//
//  The requested key container does not exist on the smart card.
//
//#define NFC_E_NO_KEY_CONTAINER         ((DWORD)0x80100030L)

//
// MessageId: NFC_E_SERVER_TOO_BUSY
//
// MessageText:
//
//  The Smart card resource manager is too busy to complete this operation.
//
//#define NFC_E_SERVER_TOO_BUSY          ((DWORD)0x80100031L)

//
// These are warning codes.
//
//
// MessageId: NFC_W_UNSUPPORTED_CARD
//
// MessageText:
//
//  The reader cannot communicate with the smart card, due to ATR configuration conflicts.
//
#define NFC_W_UNSUPPORTED_CARD         ((DWORD)0x80100065L)

//
// MessageId: NFC_W_UNRESPONSIVE_CARD
//
// MessageText:
//
//  The smart card is not responding to a reset.
//
#define NFC_W_UNRESPONSIVE_CARD        ((DWORD)0x80100066L)

//
// MessageId: NFC_W_UNPOWERED_CARD
//
// MessageText:
//
//  Power has been removed from the smart card, so that further communication is not possible.
//
#define NFC_W_UNPOWERED_CARD           ((DWORD)0x80100067L)

//
// MessageId: NFC_W_RESET_CARD
//
// MessageText:
//
//  The smart card has been reset, so any shared state information is invalid.
//
#define NFC_W_RESET_CARD               ((DWORD)0x80100068L)

//
// MessageId: NFC_W_REMOVED_CARD
//
// MessageText:
//
//  The smart card has been removed, so that further communication is not possible.
//
#define NFC_W_REMOVED_CARD             ((DWORD)0x80100069L)

//
// MessageId: NFC_W_SECURITY_VIOLATION
//
// MessageText:
//
//  Access was denied because of a security violation.
//
#define NFC_W_SECURITY_VIOLATION       ((DWORD)0x8010006AL)

//
// MessageId: NFC_W_WRONG_CHV
//
// MessageText:
//
//  The card cannot be accessed because the wrong PIN was presented.
//
#define NFC_W_WRONG_CHV                ((DWORD)0x8010006BL)

//
// MessageId: NFC_W_CHV_BLOCKED
//
// MessageText:
//
//  The card cannot be accessed because the maximum number of PIN entry attempts has been reached.
//
#define NFC_W_CHV_BLOCKED              ((DWORD)0x8010006CL)

//
// MessageId: NFC_W_EOF
//
// MessageText:
//
//  The end of the smart card file has been reached.
//
#define NFC_W_EOF                      ((DWORD)0x8010006DL)

//
// MessageId: NFC_W_CANCELLED_BY_USER
//
// MessageText:
//
//  The action was cancelled by the user.
//
#define NFC_W_CANCELLED_BY_USER        ((DWORD)0x8010006EL)

//
// MessageId: NFC_W_CARD_NOT_AUTHENTICATED
//
// MessageText:
//
//  No PIN was presented to the smart card.
//
#define NFC_W_CARD_NOT_AUTHENTICATED   ((DWORD)0x8010006FL)


//
// MessageId: NFC_W_MIFARE_AUTHENTICATION_FAIL
//
// MessageText:
//
//  No PIN was presented to the smart card.
//
#define NFC_W_MIFARE_AUTHENTICATION_FAIL   ((DWORD)0x80100070L)


#endif // NFC_S_SUCCESS
