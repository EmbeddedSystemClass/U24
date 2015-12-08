// Advanced Command Word
#ifndef _RY4S_H_
#define _RY4S_H_
// Function ID
/* Basic Format:
(1) Find ROCKEY4 SMART
    Input Parameter:
    function = 0
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    Return Value:
    *lp1 is the Hardware ID (HID) of ROCKEY4 SMART
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(2) Find Next ROCKEY4 SMART
    Input Parameter:
    function = 1
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    Return Value:
    *lp1 is the Hardware ID (HID) of ROCKEY4 SMART
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.
    
(3) Open ROCKEY4 SMART
    Input Parameter:
    function = 2
    *p1 = pass1
    *p2 = pass2
    *p3 = pass3
    *p4 = pass4
    *lp1 = Hardware ID
    Return Value:
    *handle is the pointer for ROCKEY4 SMART operation's handle
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(4) Close ROCKEY4 SMART
    Input Parameter:
    function = 3
    *handle = handle of ROCKEY4 SMART
    Return Value:
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(5) Read ROCKEY4 SMART
    Input Parameter:
    function = 4
    *handle = handle of ROCKEY4 SMART
    *p1 = pos
    *p2 = length
    buffer = pointer to the character buffer
    Return Value:
    buffer = read data into the buffer
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(6) Write ROCKEY4 SMART
    Input Parameter:
    function = 5
    *handle = handle of ROCKEY4 SMART
    *p1 = pos
    *p2 = length
    buffer = pointer to the character buffer
    Return Value:
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(7) Random Number
    Input Parameter:
    function = 6
    *handle = handle of ROCKEY4 SMART
    Return Value:
    *p1,*p2,*p3,*p4 = Random Number
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.
    
(8) Seed Code
    Input Parameter:
    function = 7
    *handle = handle of ROCKEY4 SMART
    *lp2 = seed code
    Return Value:
    *p1 = Return Code 1
    *p2 = Return Code 2
    *p3 = Return Code 3
    *p4 = Return Code 4
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(9) Write User ID [*]
    Input Parameter:
    function = 8
    *handle = handle of ROCKEY4 SMART
    *lp1 = User ID
    Return Value:
    Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(10) Read User ID
     function = 9
     *handle = handle of ROCKEY4 SMART
     Return Value:
     *lp1 = User ID
     Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(11) Set a ROCKEY4 SMART Module [*]
     Input Parameter:
     function = 10
     *handle = handle of ROCKEY4 SMART
     *p1 = Module Number
     *p2 = Module Unit Value
     *p3 = If decreasing is allowed (1 = Allowed, 0 = Not allowed)
     Return Value:
     Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(12) Check the validity of a ROCKEY4 SMART Module
     Input Parameter:
     function = 11
     *handle = handle of ROCKEY4 SMART
     *p1 = Module Number
     Return Value:
     *p2 = 1 means this module is valid
     *p3 = 1 means this module is allowed to be decreased
     Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(13) Write Algorithm [*]
     Input Parameter:
     function = 12
     *handle = handle of ROCKEY4 SMART
     *p1 = pos
     buffer = algorithm command string
     Return Value:
     Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.
     
(14) Calculate 1 (Module Zone, ID High-bit, ID Low-bit, Random Number)
     Input Parameter:
     function = 13
     *handle = handle of ROCKEY4 SMART
     *lp1 = Start point of calculation
     *lp2 = Module Number
     *p1 = Input Value 1
     *p2 = Input Value 2
     *p3 = Input Value 3
     *p4 = Input Value 4
     Return Value:
     *p1 = Return Code 1
     *p2 = Return Code 2
     *p3 = Return Code 3
     *p4 = Return Code 4
     Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(15) Calculate 2
     Input Parameter:
     function = 14
     *handle = handle of ROCKEY4 SMART
     *lp1 = Start point of calculation
     *lp2 = Seed code
     *p1 = Input Value 1
     *p2 = Input Value 2
     *p3 = Input Value 3
     *p4 = Input Value 4
     Return Value:
     *p1 = Return Code 1
     *p2 = Return Code 2
     *p3 = Return Code 3
     *p4 = Return Code 4
     Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(16) Calculate 3
     Input Parameter:
     function = 15
     *handle = handle of ROCKEY4 SMART
     *lp1 = Start point of calculation
     *lp2 = Start address of password
     *p1 = Input Value 1
     *p2 = Input Value 2
     *p3 = Input Value 3
     *p4 = Input Value 4
     Return Value:
     *p1 = Return Code 1
     *p2 = Return Code 2
     *p3 = Return Code 3
     *p4 = Return Code 4
     Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.

(17) Decrease Module Unit
     Input Parameter:
     function = 16
     *handle = handle of ROCKEY4 SMART
     *p1 = Module Number
     Return 0 means the function was executed successfully, otherwise, corresponding error codes will be returned.
*/

#define  RY_FIND                        1       // Find Dongle
#define  RY_FIND_NEXT                   2       // Find Next Dongle
#define  RY_OPEN                        3       // Open Dongle
#define  RY_CLOSE                       4       // Close Dongle
#define  RY_READ                        5       // Read Dongle
#define  RY_WRITE                       6       // Write Dongle
#define  RY_RANDOM                      7       // Generate Random Number
#define  RY_SEED                        8       // Generate Seed Code
#define  RY_WRITE_USERID                9       // Write User ID
#define  RY_READ_USERID                 10      // Read User ID
#define  RY_SET_MODULE                  11      // Set Module
#define  RY_CHECK_MODULE                12      // Check Module
#define  RY_WRITE_ARITHMETIC            13      // Write Algorithm
#define  RY_CALCULATE1                  14      // Calculate 1
#define  RY_CALCULATE2                  15      // Calculate 2
#define  RY_CALCULATE3                  16      // Calculate 3
#define  RY_DECREASE                    17      // Decrease Module Unit


#define  RY_SET_COUNTER                 20      // Set Counter
#define  RY_GET_COUNTER                 21      // Get Counter
#define  RY_DEC_COUNTER                 22
#define  RY_SET_TIMER                   23      // Set Timer
#define  RY_GET_TIMER                   24      // Get Timer
#define  RY_ADJUST_TIMER                25      // Adjust System Clock
#define  RY_SET_TIMER_ITV               26
#define  RY_GET_TIMER_ITV               27
#define  RY_DEC_TIMER                   28
#define  RY_SET_RSAKEY_N                29      // Write N into RSA keypair
#define  RY_SET_RSAKEY_D                30      // Write D into RSA keypair
#define  RY_UPDATE_GEN_HEADER           31      // Generate header of cipher-text file
#define  RY_UPDATE_GEN                  32      // Generate cipher-text file
#define  RY_UPDATE_CHECK                33      // Check and Upgrade cipher-text file
#define  RY_UPDATE                      34      // Upgrade cipher-text file
#define  RY_UNPACK                      35

#define  RY_SET_DES_KEY                 41      // Set DES Key
#define  RY_DES_ENC                     42      // DES Encryption
#define  RY_DES_DEC                     43      // DES Decryption
#define  RY_RSA_ENC                     44      // RSA Encryption
#define  RY_RSA_DEC                     45      // RSA Decryption

//New function in cos 1.03
#define RY_READ_EX                      46      // Read User Memory
#define RY_WRITE_EX                     47      // Write User Memory

#define RY_SET_COUNTER_EX               0xA0    // Set value to Count Unit (DWORD)
#define RY_GET_COUNTER_EX               0xA1    // Get value from Count Unit (DWORD)
#define RY_SET_TIMER_EX                 0xA2    // Set value to Time Unit (DWORD)
#define RY_GET_TIMER_EX                 0xA3    // Get value from Time Unit (DWORD)
#define RY_ADJUST_TIMER_EX              0xA4    // Adjust Timer in Dongle 
#define RY_UPDATE_GEN_HEADER_EX         0xA5    // Generate Update File Head
#define RY_UPDATE_GEN_EX                0xA6    // Generate Update File
#define RY_UPDATE_CHECK_EX              0xA7    // Check the validity of Update File
#define RY_UPDATE_EX                    0xA8    // Update Dongle
#define RY_SET_UPDATE_KEY               0xA9    // Set RSA Keys for Generate Update File
#define RY_ADD_UPDATE_HEADER            0xAA    // Fill in the content of Update File Head
#define RY_ADD_UPDATE_CONTENT           0xAB    // Fill in the content of Update File Body
#define RY_GET_TIME_DWORD               0xAC    // Translate Time to DWORD value (the minutes from 2006.1.1)

#define  RY_VERSION                     100     // Get COS Version

#define DES_SINGLE_MODE                 0
#define DES_TRIPLE_MODE                 1
#define RSA_PRIVATE_KEY                 0
#define RSA_PUBLIC_KEY                  1
#define RSA_ROCKEY_PADDING              0
#define RSA_USER_PADDING                1


// Error Code
#define ERR_SUCCESS                     0  // Success
#define ERR_NO_ROCKEY                   3  // No driver installed
#define ERR_INVALID_PASSWORD            4  // Found Rockey, but basic passwords (pass1, pass2) are wrong
#define ERR_INVALID_PASSWORD_OR_ID      5  // Wrong password or Hardware ID
#define ERR_SETID                       6  // Error in Hardware ID setting
#define ERR_INVALID_ADDR_OR_SIZE        7  // rror in read/write address or length
#define ERR_UNKNOWN_COMMAND             8  // No such command
#define ERR_NOTBELEVEL3                 9  // Internal Error
#define ERR_READ                        10 // Read Error
#define ERR_WRITE                       11 // Write Error
#define ERR_RANDOM                      12 // Random Number Error
#define ERR_SEED                        13 // Seed Code Error
#define ERR_CALCULATE                   14 // Error in Calculation
#define ERR_NO_OPEN                     15 // Unopen dongle before operation
#define ERR_OPEN_OVERFLOW               16 // Too many open dongles (>16)
#define ERR_NOMORE                      17 // No more dongles
#define ERR_NEED_FIND                   18 // No "Find" function called before using "FindNext"
#define ERR_DECREASE                    19 // Decrease Error
#define ERR_AR_BADCOMMAND               20 // Error in Arithmetic Instruction
#define ERR_AR_UNKNOWN_OPCODE           21 // Error in Arithmetic Operator
#define ERR_AR_WRONGBEGIN               22 // Constant cannot be used in the first arithmetic instruction
#define ERR_AR_WRONG_END                23 // Constant cannot be used in the last arithmetic instruction
#define ERR_AR_VALUEOVERFLOW            24 // The value of Constant cannot be greater than 63
#define ERR_TOOMUCHTHREAD               25 // The threads opened in the same process cannot be greater than 100 
#define ERR_INVALID_RY4S                30 // Try to handle non-ROCKEY4 SMART dongle 
#define ERR_INVALID_PARAMETER           31 // Invalid parameter
#define ERR_INVALID_TIMEVALUE           32 // Invalid Date-Time value

// Set DES Key Error
#define ERR_SET_DES_KEY                 40
// DES Encryption Error 
#define ERR_DES_ENCRYPT                 41
// DES Decryption Error 
#define ERR_DES_DECRYPT                 42
// Error in writing N into RSA keypair
#define ERR_SET_RSAKEY_N                43
// Error in writing D into RSA keypair
#define ERR_SET_RSAKEY_D                44
// RSA Encryption Error 
#define ERR_RSA_ENCRYPT                 45
// RSA Decryption Error
#define ERR_RSA_DECRYPT                 46
//非法的数据长度
#define ERR_INVALID_LENGTH              47

#define ERR_RECEIVE_NULL                0x100   // Receive nothing
#define ERR_INVALID_BUFFER              0x101   // NULL Pointer
#define ERR_UNKNOWN_SYSTEM              0x102   // Unknown Operating System
#define ERROR_UNINIT_TIME_UNIT          0x103   // Time uint uninitialized
#define ERR_UNKNOWN                     0xFFFF  // Unknown Error


EXTERN_C __declspec(dllexport) WORD WINAPI Rockey(WORD function, WORD* handle, DWORD* lp1,  DWORD* lp2, WORD* p1, WORD* p2, WORD* p3, WORD* p4, BYTE* buffer);

#endif


