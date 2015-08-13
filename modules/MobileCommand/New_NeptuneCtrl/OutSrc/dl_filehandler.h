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
#ifndef CSST_DL_FILEHANDLER_H
#define CSST_DL_FILEHANDLER_H

/*==== INCLUDES =============================================================*/
#include "types.h"
#include "filehandler.h"
#include <stdio.h>

/*==== CONSTS ===============================================================*/
#define SUCCESS             0
#define FAULT               1


#define MAX_NUM_OF_SCT      100

/*==== MACROS ===============================================================*/
/* Allocate primitive SDU with variable payload length */
//#define ALLOC_FILE_HDR(no_of_scn) (T_FILE_HDR *) malloc(sizeof(T_FILE_HDR) - \
//    (MAX_NUM_OF_SCT - no_of_scn) * sizeof(T_SECTION_HDR));
#define ALLOC_FILE_HDR(no_of_scn) (T_FILE_HDR *) new (std::nothrow) T_FILE_HDR[ sizeof(T_FILE_HDR) - (MAX_NUM_OF_SCT - no_of_scn) * sizeof(T_SECTION_HDR) ];
/*==== TYPES ================================================================*/

#define SERVER_IP  "10.82.201.27"
#define PORT      27015





#if 0
/*typedef enum
{
    COFF,
    SRECORD,
    RAW
} T_FILE_TYPE;
*/
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
} FILEHDR ;

/*  Definition of COOF Optional File Header */
typedef struct //size = 28
{
    U16  magic       ;   /* magic number                    */
    U16  vstamp      ;    /* version stamp                   */
    U32    tsize      ;    /* text size in bytes, padded      */
    U32    dsize      ;    /* initialized data (.data)        */
    U32    bsize      ;    /* unitialized data (.bss)         */
    U32    entry      ;    /* entry point                     */
    U32    text_start ;    /* base of text used for this file */
    U32    data_start ;    /* base of data used for this file */

} AOUTHDR ;

/*  Definition of COFF Section Header */
typedef struct 	//size = 48
{
    char            s_name[8] ;  /* section name             */
    U32   s_paddr   ;  /* physical address         */
    U32   s_vaddr   ;  /* virtual address          */
    U32   s_size    ;  /* section size             */
    U32   s_scnptr  ;  /* file ptr to raw data     */
    U32   s_relptr  ;  /* file ptr to relocation   */
    U32   s_lnnoptr ;  /* file ptr to line numbers */
    U32   s_nreloc  ;  /* # reloc entries          */
    U32   s_nlnno   ;  /* # line number entries    */
    U32   s_flags   ;  /* flags                    */
    U16  s_res     ;  /* reserved                 */
    U16  s_mempg   ;  /* Memory Page Number       */
} SCNHDR;

typedef struct
{
	FILEHDR			fileHdr;
	AOUTHDR		fileoptHdr;
	SCNHDR			sctHdr;
} COFFFILE;
#endif
/*======= FUNCTION PROTOTYPE DEFINITIONS ====================================*/
class Dl_Filehandler
{

public:
	HMODULE m_hDLL;
	int m_iReturnFault;
    Dl_Filehandler();
    ~Dl_Filehandler();

    U8 read_file(char* filename, T_FILE_HDR** file_hdr);
    U8 write_file(char* filename, T_FILE_HDR* file_hdr);
    void free_file_mem(T_FILE_HDR* file_hdr);
    U8 convert_buffer_endianess(U8* addr, U32 length, T_BUFFER_TYPE buffer_type);
private:
    void hextobytes(U8* dst, char* src, U16 bytes);
    U32 hextoint(char* src, U32 digits);

    void ConvertEndianU32(U32* vars);
    void ConvertEndianU16(U16* vars);

    U8 read_coff_file(FILE* file_ptr, T_FILE_HDR** file_data);
    void read_srecord_file(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr, T_FILE_TYPE type);
    void read_raw_file(FILE* file_ptr, T_FILE_HDR** file_data);
	U8 read_elf_image(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr, char* filename);
	U8 read_symbian_image(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr, char* filename);
	U8 read_wince_image(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr, char* filename);
	U8 read_palm_image(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr, char* filename);
	U8 read_linux_image(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr, char* filename);
	U8 read_signed_image(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr, char* filename);
	U8 read_unknown_binary_image(FILE *file_ptr, T_FILE_HDR** file_hdr_ptr, char* filename);
};

#endif /* CSST_DL_FILEHANDLER_H */

/*==== END OF FILE ==========================================================*/


