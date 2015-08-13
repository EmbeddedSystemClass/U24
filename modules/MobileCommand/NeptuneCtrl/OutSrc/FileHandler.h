/*-----------------------------------------------------------------------------
|  Project :  CSST
|  Module  :  Logical Link Layer - Host
+------------------------------------------------------------------------------
|             Copyright 2005 Texas Instruments.
|             All rights reserved.
|
|             This file is confidential and a trade secret of Texas
|             Instruments .
|             The receipt of or possession of this file does not convey
|             any rights to reproduce or disclose its contents or to
|             manufacture, use, or sell anything it may describe, in
|             whole, or in part, without the specific written consent of
|             Texas Instruments.
+------------------------------------------------------------------------------
| Filename:   ll_msg_handler.h
| Author:     Thomas Lund Soehus (tls@ti.com)
| Purpose:    Header file for the LL message handler
+----------------------------------------------------------------------------*/

/*==== DECLARATION CONTROL ==================================================*/
#ifndef CSST_FILEHANDLER_H
#define CSST_FILEHANDLER_H

/*==== INCLUDES =============================================================*/
#include "types.h"
#include <stdio.h>

/*==== CONSTS ===============================================================*/
#define SUCCESS                0
#define FAULT                  1

#define MAX_NUM_OF_SCT       100
#define MAX_SYMBOL_NAME_SIZE  50

/*==== MACROS ===============================================================*/
//#define ALLOC_FILE_HDR(no_of_scn) (T_FILE_HDR *) malloc(sizeof(T_FILE_HDR) - \
//    (MAX_NUM_OF_SCT - no_of_scn) * sizeof(T_SECTION_HDR));
#define ALLOC_FILE_HDR(no_of_scn) (T_FILE_HDR *) new (std::nothrow) T_FILE_HDR[ sizeof(T_FILE_HDR) - (MAX_NUM_OF_SCT - no_of_scn) * sizeof(T_SECTION_HDR) ];

#define ALLOC_COFF_FILE_HDR(no_of_scn) (T_COFF_FILE_HDR *) malloc(sizeof(T_COFF_FILE_HDR) - \
    (MAX_NUM_OF_SCT - no_of_scn) * sizeof(T_COFF_SECTION_HDR));

/*==== TYPES ================================================================*/
typedef enum
{
    M0_FILE,
    M32_FILE
} T_FILE_TYPE;

/*typedef enum
{
    COFF,
    SRECORD,
    RAW
} T_FILE_TYPE;
*/

typedef enum
{
    SYMBIAN,        //.img
    WINCE,          //.bin
    PALM,           //.widebin
    LINUX,          //.rr
    NUCLEUS,        //.???
    COFF,           //.out
    SIGNED_CODE,    //.sgn
    RAW,            //.raw
    ELF,            //elf
    SRECORD,
    UNKNOWN_BINARY  //.???  Anything but the above!
}IMAGETYPE; //Added by shyamala
typedef enum
{
    U_32,
    U_16
} T_BUFFER_TYPE;

typedef struct
{
    U32 dst_addr;
    U32 sct_size;
    U8 *data_ptr;
} T_SECTION_HDR;

typedef struct
{
    U32 num_of_sct;
    IMAGETYPE /*T_FILE_TYPE*/ file_type;
    U32 load_addr;
    T_SECTION_HDR section_hdr[MAX_NUM_OF_SCT];
} T_FILE_HDR;

/* Definition of COFF File Header */
typedef struct //size = 22
{
    U16  f_magic     ;  /* magic number         */
    U16  f_nscns     ;  /* number of sections   */
    U32  f_timdat    ;  /* time and date stamps */
    U32  f_symptr    ;  /* file ptr to symtab   */
    U32  f_nsyms     ;  /* # symtab entries     */
    U16  f_opthdr    ;  /* sizeof(opt hdr)      */
    U16  f_flags     ;  /* flags                */
    U16  f_target_id ;  /* Magic Number to indicate excecution  */
} FILEHDR;

/*  Definition of COFF Optional File Header */
typedef struct //size = 28
{
    U16  magic      ;    /* magic number                    */
    U16  vstamp     ;    /* version stamp                   */
    U32  tsize      ;    /* text size in bytes, padded      */
    U32  dsize      ;    /* initialized data (.data)        */
    U32  bsize      ;    /* unitialized data (.bss)         */
    U32  entry      ;    /* entry point                     */
    U32  text_start ;    /* base of text used for this file */
    U32  data_start ;    /* base of data used for this file */
} AOUTHDR;

typedef struct
{
    U32 high_ptr;
    U32 low_ptr;
} T_NAME_PTR;

typedef union
{
    char       n[8];
    T_NAME_PTR n_ptr;
} U_SYMBOL_NAME;

/*  Definition of COFF Section Header */
typedef struct //size = 48
{
    U_SYMBOL_NAME s_name    ;  /* section name             */
    U32           s_paddr   ;  /* physical address         */
    U32           s_vaddr   ;  /* virtual address          */
    U32           s_size    ;  /* section size             */
    U32           s_scnptr  ;  /* file ptr to raw data     */
    U32           s_relptr  ;  /* file ptr to relocation   */
    U32           s_lnnoptr ;  /* file ptr to line numbers */
    U32           s_nreloc  ;  /* # reloc entries          */
    U32           s_nlnno   ;  /* # line number entries    */
    U32           s_flags   ;  /* flags                    */
    U16           s_res     ;  /* reserved                 */
    U16           s_mempg   ;  /* Memory Page Number       */
} SCNHDR;

typedef struct
{
    FILEHDR fileHdr;
    AOUTHDR fileoptHdr;
    SCNHDR  sctHdr;
} COFFFILE;

typedef struct
{
    char sct_name[MAX_SYMBOL_NAME_SIZE];
    U32  dst_addr;
    U32  sct_size;
    U32  file_data_ptr;
    U8  *data_ptr;
} T_COFF_SECTION_HDR;

typedef struct
{
    U32 num_of_sct;
    U32 load_addr;
    U32 raw_data_size;
    U8 *raw_data_ptr;
    T_COFF_SECTION_HDR section_hdr[MAX_NUM_OF_SCT];
} T_COFF_FILE_HDR;

/*======= FUNCTION PROTOTYPE DEFINITIONS ====================================*/
class FileHandler
{

public:
    FileHandler();
    ~FileHandler();
    U8 *mpBuffer; //Added by sbyamala
    U8 read_file(char *filename, T_FILE_HDR **file_hdr);
    U8 write_file(char *filename, T_FILE_HDR *file_hdr);
    void free_file_mem(T_FILE_HDR *file_hdr);
    void free_coff_file_mem(T_COFF_FILE_HDR *file_hdr);
    U8 convert_buffer_endianess(U8 *addr, U32 length, T_BUFFER_TYPE buffer_type);
    BOOL SetImageType(IMAGETYPE type);//Added by shyamala
    //DWORD GetImageSize(char *FileName);//Added by shyamala
    U32 GetImageSize(char *FileName, DWORD* filesize);
    BOOL ReadFileToBuffer(BYTE *buffer, char *FileName);
    DWORD GetFileSize (char *FileName);
    DWORD GetmError ();
    BOOL SaveBinFile(char *FileName);
    void MoveDateToBuffer(T_FILE_HDR *fhn_hdr);
    U8 SetImageType(void);
    U8 getCoffFile(const char * const filename, T_COFF_FILE_HDR **file_hdr_ptr);
private:

    IMAGETYPE mImageType;////Added by sbyamala

    DWORD mError;//Added by sbyamala
    DWORD mImageSize; //Added by shyamala
    char mFileName[1000];//Added by shyamala
    void hextobytes(U8 *dst, char *src, U16 bytes);
    U32 hextoint(char *src, U32 digits);

    void ConvertEndianU32(U32 *vars);
    void ConvertEndianU16(U16 *vars);

    U8 read_coff_file_hdr(FILE *file_ptr, COFFFILE * const coff, U16 * const num_of_hdrs, U32 * const data_size, SCNHDR **scnhdr);
    U8 read_coff_file(FILE *file_ptr, T_FILE_HDR **file_data);
    void read_srecord_file(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, T_FILE_TYPE type);
    void read_raw_file(FILE *file_ptr, T_FILE_HDR **file_data);
    U8 read_elf_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char* filename);
    U8 read_symbian_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename);
    U8 read_wince_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename);
    U8 read_palm_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename);
    U8 read_linux_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename);
    U8 read_signed_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename);
    U8 read_unknown_binary_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename);
    DWORD ReadSymbianImageSize(DWORD *filesize);
    DWORD ReadWINCEImageSize(DWORD *filesize);
    DWORD ReadPalmImageSize(DWORD *filesize);
    DWORD ReadLinuxImageSize(DWORD *filesize);
    DWORD ReadCoffImageSize(DWORD *filesize);
    DWORD ReadElfImageSize(DWORD *filesize);
    DWORD ReadSignedImageSize(DWORD *filesize);
    DWORD ReadImageSize(DWORD *filesize);
    DWORD ReadRawImageSize(DWORD *filesize);
    //DWORD ReadSrecordImageSize(void);
    BOOL ReadWINCEImage(void);
    BOOL ReadSymbianImage(void);
    BOOL ReadPalmImage(void);
    BOOL ReadLinuxImage(void);
    BOOL ReadCoffImage(void);
    BOOL ReadElfImage(void);
    BOOL ReadImage(void);
    BOOL ReadSignedImage(void);
    BOOL ReadSrecordImage();
    BOOL FileHandler::ReadRawImage(void);
    DWORD ReadSrecordImageSize(DWORD *filesize);
};

#endif /* CSST_FILEHANDLER_H */

/*==== END OF FILE ==========================================================*/
