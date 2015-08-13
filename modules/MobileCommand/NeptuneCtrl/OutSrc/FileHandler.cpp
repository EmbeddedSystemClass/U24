/*-----------------------------------------------------------------------------
|  Project :  CSST
|  Module  :  DNLD Module - Host
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
| Filename:   filehandler.cpp
| Author:     Witold Pietraszek (wpi@ti.com)
| Purpose:    Defines the filehandler functionality
+----------------------------------------------------------------------------*/

/*==== DECLARATION CONTROL ==================================================*/
#ifndef CSST_FILEHANDLER_C
#define CSST_FILEHANDLER_C
#endif

/*==== INCLUDES =============================================================*/
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "Filehandler.h"
#include "ElfHandler.h"
#include "ElfHeader.h"
#include "ImageDefs.h"
#include "error.h"
#include <iostream>
#include <string>

/*==== MACROS ===============================================================*/

/*==== GLOBALS ==============================================================*/

/*==== PRIVATE FUNCTIONS ====================================================*/
void FileHandler::hextobytes(U8 *dst, char *src, U16 bytes)
{
    while (bytes-- && *src) {
        *dst++ =
            (src[0] - (src[0] <= '9' ? '0' : 'A' - 10)) * 16 +
            (src[1] - (src[1] <= '9' ? '0' : 'A' - 10));
        src += 2;
    }
}

int compare_addrs(const void *v1, const void *v2)
{
    SCNHDR *hdr1 = (SCNHDR *)v1;
    SCNHDR *hdr2 = (SCNHDR *)v2;
    
    return(hdr1->s_vaddr - hdr2->s_vaddr);
}

U32 FileHandler::hextoint(char *src, U32 digits)
{
    U32 number = 0;
    char ch;

    while (digits-- && *src) {
        ch = *src++;
        if (ch >= '0' && ch <= '9')
            ch = ch - '0';
        else if (ch >= 'A' && ch <= 'F')
            ch = ch - 'A' + 10;
        else if (ch >= 'a' && ch <= 'f')
            ch = ch - 'a' + 10;
        else
            break;
        number = (number << 4) + ch;
    }
    return number;
}

void FileHandler::ConvertEndianU16(U16 *vars)
{
    U16 tmp[2];

    tmp[0] = (*vars<<8) & 0xFF00;
    tmp[1] = (*vars>>8) & 0xFF;

    *vars = tmp[0]+tmp[1];
}

void FileHandler::ConvertEndianU32(U32 *vars)
{
    U32 tmp[4];

    tmp[0] = (*vars<<24) & 0xFF000000;
    tmp[1] = (*vars<<8) & 0xFF0000;
    tmp[2] = (*vars>>8) & 0xFF00;
    tmp[3] = (*vars>>24) & 0xFF;

    *vars = tmp[0]+tmp[1]+tmp[2]+tmp[3];
}

U8 FileHandler::read_coff_file_hdr(FILE     * file_ptr,
                                   COFFFILE * const coff,
                                   U16      * const num_of_hdrs,
                                   U32      * const data_size,
                                   SCNHDR  ** scn_hdr_ptr)
{
    U8 reverse_coff = 0;
    U16 scn_index = 0;

    /* Read file header */
    if (fread(&coff->fileHdr, 22, 1, file_ptr) != 1)
        return FALSE;

    /* Check COFF version ID to determine if the file is reversed. Only COFF 
    file format version 2 (0x00C2) is supported */
    if(coff->fileHdr.f_magic==0x00C2)
        reverse_coff = 0;
    else if(coff->fileHdr.f_magic==0xC200)
        reverse_coff = 1;
    else
        return FALSE;

    /* Reverse file header if necessary */
    if(reverse_coff)
    {
        ConvertEndianU16(&coff->fileHdr.f_magic);
        ConvertEndianU16(&coff->fileHdr.f_nscns);
        ConvertEndianU32(&coff->fileHdr.f_timdat);
        ConvertEndianU32(&coff->fileHdr.f_symptr);
        ConvertEndianU32(&coff->fileHdr.f_nsyms);
        ConvertEndianU16(&coff->fileHdr.f_opthdr);
        ConvertEndianU16(&coff->fileHdr.f_flags);
        ConvertEndianU16(&coff->fileHdr.f_target_id);
    }

    /* Check optional file header */
    if((coff->fileHdr.f_opthdr != 0) && (coff->fileHdr.f_opthdr != 28))
    {
        return FALSE; /* Error in COFF file format */
    }

    /* Clear entry point */
    coff->fileoptHdr.entry = 0x00000000;

    /* Read optional file header if available */
    if(coff->fileHdr.f_opthdr == 28)
    {
        if(fread(&coff->fileoptHdr, 28, 1, file_ptr) != 1)
            return FALSE;

        /* Reverse optional file header if necessary */
        if(reverse_coff)
        {
            ConvertEndianU16(&coff->fileoptHdr.magic);
            ConvertEndianU16(&coff->fileoptHdr.vstamp);
            ConvertEndianU32(&coff->fileoptHdr.tsize);
            ConvertEndianU32(&coff->fileoptHdr.dsize);
            ConvertEndianU32(&coff->fileoptHdr.bsize);
            ConvertEndianU32(&coff->fileoptHdr.entry);
            ConvertEndianU32(&coff->fileoptHdr.text_start);
            ConvertEndianU32(&coff->fileoptHdr.data_start);
        }
    }

    /* Allocate memory for SCNHDRs */
    *scn_hdr_ptr = (SCNHDR *) malloc(coff->fileHdr.f_nscns * sizeof(SCNHDR));

    /* Get scn headers and file size */
    for(scn_index = 0; scn_index < coff->fileHdr.f_nscns; scn_index++)
    {
        /* Set file position to next section header */
        fseek(file_ptr, 22 + coff->fileHdr.f_opthdr + scn_index * 48, SEEK_SET);
    
        /* Read section header */
        fread(&coff->sctHdr, sizeof(SCNHDR), 1, file_ptr);

        /* Reverse section header if necessary */
        if(reverse_coff)
        {        
            ConvertEndianU32(&coff->sctHdr.s_paddr);
            ConvertEndianU32(&coff->sctHdr.s_vaddr);
            ConvertEndianU32(&coff->sctHdr.s_size);
            ConvertEndianU32(&coff->sctHdr.s_scnptr);
            ConvertEndianU32(&coff->sctHdr.s_relptr);
            ConvertEndianU32(&coff->sctHdr.s_lnnoptr);
            ConvertEndianU32(&coff->sctHdr.s_nreloc);
            ConvertEndianU32(&coff->sctHdr.s_nlnno);
            ConvertEndianU32(&coff->sctHdr.s_flags);
            ConvertEndianU16(&coff->sctHdr.s_res);
            ConvertEndianU16(&coff->sctHdr.s_mempg);
        }
 
        /* Read scn headers for section of importance. This is determined from 
        the section header flag and pointer to raw data. */
        if(!(coff->sctHdr.s_flags & 0x11) && (coff->sctHdr.s_flags & 0x60) && (coff->sctHdr.s_scnptr != 0))
        {
            memcpy(&(*scn_hdr_ptr)[*num_of_hdrs], &coff->sctHdr, sizeof(SCNHDR));

            *data_size += coff->sctHdr.s_size;
            (*num_of_hdrs)++;
        }
    }

    return TRUE;
}


U8 FileHandler::read_coff_file(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr)
{
    U16 scn_index = 0, current_scn = 0, num_of_scn = 0, num_of_hdrs = 0;
    U32 data_size = 0, scn_end_addr = 0xFFFFFFFF;
    COFFFILE coff;
    SCNHDR *scnhdr = NULL;

    if (read_coff_file_hdr(file_ptr, &coff, &num_of_hdrs, &data_size, &scnhdr) == FALSE)
        return FALSE;

    /* Sort sections headers */
    //qsort(scnhdr, current_scn, sizeof(SCNHDR), (int (*) (const void *, const void *))compare_addrs);
    qsort(scnhdr, num_of_hdrs, sizeof(SCNHDR), compare_addrs);

    /* Count number of scn */
    for(scn_index = 0; scn_index < num_of_hdrs; scn_index++)
    {
        if(scn_end_addr != scnhdr[scn_index].s_vaddr)
            num_of_scn++;
        
        scn_end_addr = scnhdr[scn_index].s_vaddr + scnhdr[scn_index].s_size;
    }

    /* Allocate DNLD file hdr and memory for data sections. */
    *file_hdr_ptr = ALLOC_FILE_HDR(num_of_scn);
    (*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(data_size);

    /* Fill DNLD file hdr. */
    (*file_hdr_ptr)->num_of_sct = num_of_scn;
    (*file_hdr_ptr)->file_type = COFF;
    (*file_hdr_ptr)->load_addr = coff.fileoptHdr.entry;

    /* Clear data_size and current sections*/
    data_size = current_scn = 0;
    scn_end_addr = 0xFFFFFFFF;

    /* Get number of sections and file size */
    for(scn_index = 0; scn_index < num_of_hdrs; scn_index++)
    {
            if(scn_end_addr != scnhdr[scn_index].s_vaddr) /* First part of scn */ 
            {
                (*file_hdr_ptr)->section_hdr[current_scn].dst_addr = scnhdr[scn_index].s_vaddr;
                (*file_hdr_ptr)->section_hdr[current_scn].sct_size = scnhdr[scn_index].s_size;
                
                (*file_hdr_ptr)->section_hdr[current_scn].data_ptr =
                    (*file_hdr_ptr)->section_hdr[0].data_ptr + data_size;

                current_scn++;
            }
            else
                (*file_hdr_ptr)->section_hdr[current_scn-1].sct_size += scnhdr[scn_index].s_size;
            
            
            fseek(file_ptr, scnhdr[scn_index].s_scnptr, SEEK_SET);
            fread((*file_hdr_ptr)->section_hdr[0].data_ptr+data_size, sizeof(U8), scnhdr[scn_index].s_size, file_ptr);

            data_size += scnhdr[scn_index].s_size;
            scn_end_addr = scnhdr[scn_index].s_vaddr + scnhdr[scn_index].s_size;
        
    }

    /* Free memory for SCNHDRs */
    free(scnhdr);
    
    return SUCCESS;
}

void FileHandler::read_srecord_file(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, T_FILE_TYPE type)
{
    char line[256];
    U32 num_of_scn = 0, data_size = 0, scn_end_addr = 0xFFFFFFFF;
    U32 scn_size = 0, rec_size = 0; 
    U8 addr_size = 0xFF;
    U32 i = 0;

    /* Determine address size within m0-file. It is assumed that all addresses
    within the file are of the same length. */
    fseek(file_ptr, 0, SEEK_SET);
    while((addr_size==0xFF) && (fgets(line, 256, file_ptr) != NULL))
    {
        switch (line[1])
        {
            case '1':
                addr_size = 2;
                break;
            case '2':
                addr_size = 3;
                break;
            case '3':
                addr_size = 4;
                break;
            default:
                break;
        }
    }

    /* Determine number of sections */
    fseek(file_ptr, 0, SEEK_SET);
    while (fgets(line, 256, file_ptr) != NULL) 
    {
        switch (line[1])
        {
            case '1':
            case '2':
            case '3':
                if(scn_end_addr != hextoint(&line[4], 2*addr_size))
                    num_of_scn++;    
    
                scn_end_addr = hextoint(&line[4], 2*addr_size)+hextoint(&line[2], 2)-addr_size-1;
                data_size += hextoint(&line[2], 2)-addr_size-1;
                break;
        }
    }
    
    /* Alloc file hdr and fill it */
    *file_hdr_ptr = ALLOC_FILE_HDR(num_of_scn);
    
    (*file_hdr_ptr)->num_of_sct = num_of_scn;
    (*file_hdr_ptr)->file_type = SRECORD;
    (*file_hdr_ptr)->load_addr = 0;
    
    /* Only one malloc for all sections */
    (*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(data_size);
    
    /* Clear counters */
    fseek(file_ptr, 0, SEEK_SET);
    scn_end_addr = 0xFFFFFFFF;
    
    while (fgets(line, 256, file_ptr) != NULL) 
    {
        switch (line[1])
        {
            case '1':
            case '2':
            case '3':
                /* Set variables for new section */
                if(scn_end_addr != hextoint(&line[4], 2*addr_size))
                {
                    (*file_hdr_ptr)->section_hdr[i].dst_addr = scn_end_addr =
                        hextoint(&line[4], 2*addr_size);

                    if(i!=0)
                    {
                        (*file_hdr_ptr)->section_hdr[i].data_ptr = 
                            (*file_hdr_ptr)->section_hdr[i-1].data_ptr + scn_size;    
                        (*file_hdr_ptr)->section_hdr[i-1].sct_size = scn_size;
                        scn_size = 0;
                    }
                    i++;
                }
                
                rec_size = hextoint(&line[2], 2) - addr_size - 1;
                hextobytes((U8 *)((*file_hdr_ptr)->section_hdr[i-1].data_ptr
                    +scn_size), &line[4+2*addr_size], rec_size);

                scn_size += rec_size;
                scn_end_addr += rec_size;
                break;
            case '7':
            case '8':
            case '9':
                (*file_hdr_ptr)->section_hdr[i-1].sct_size = scn_size;
                (*file_hdr_ptr)->load_addr = hextoint(&line[4], 2*addr_size);
                break;
        }
                
    }

    /* Reverse endianness (ASSUMPION: .m32 and .m0 have reverse endianness)*/
    if(type == M0_FILE)
    {
        for(i=0; i<data_size/2; i++)
            ConvertEndianU16((U16 *)((*file_hdr_ptr)->section_hdr[0].data_ptr + 2*i));
    }
}


void FileHandler::read_raw_file(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr)
{
    U32 scn_size = 0;

    /* Get file size */
    fseek(file_ptr, 0, SEEK_END);
    scn_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    
    /* Alloc file hdr and fill it */
    *file_hdr_ptr = ALLOC_FILE_HDR(1);
    
    (*file_hdr_ptr)->num_of_sct = 1;
    (*file_hdr_ptr)->file_type = RAW;
    (*file_hdr_ptr)->load_addr = 0;
    
    (*file_hdr_ptr)->section_hdr[0].dst_addr = 0;
    (*file_hdr_ptr)->section_hdr[0].sct_size = scn_size;
    (*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(scn_size);
    
    fread((*file_hdr_ptr)->section_hdr[0].data_ptr, 1, scn_size, file_ptr);
}


U8 FileHandler::read_elf_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char* filename)
{
    
    ElfHandler lelf;
    int offset;
    DWORD size = 0;
    DWORD oldsize = 0;
    DWORD min_address = 0xFFFFFFFF;    
    

    U32 scn_size = 0;

    /* Get file size */
    fseek(file_ptr, 0, SEEK_END);
    scn_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    
     if(lelf.Read(filename))
    {
        /* Alloc file hdr and fill it */
        *file_hdr_ptr = ALLOC_FILE_HDR(lelf.elf.ehdr->e_phnum);

        (*file_hdr_ptr)->num_of_sct = lelf.elf.ehdr->e_phnum;
         (*file_hdr_ptr)->file_type = ELF;

         (*file_hdr_ptr)->load_addr = 0; //SHY to check*/
   

        //find the minimum address
        for(offset = 0; offset < lelf.elf.ehdr->e_phnum;offset++)
        {
            Elf32_Phdr *tmp_phdr = lelf.elf.phdr + offset;
            min_address = min(min_address,tmp_phdr->p_vaddr);
        }
        //copy the image
        for(offset = 0; offset < lelf.elf.ehdr->e_phnum;offset++)
        {

            Elf32_Phdr *tmp_phdr = lelf.elf.phdr + offset;

            (*file_hdr_ptr)->section_hdr[offset].dst_addr = tmp_phdr->p_vaddr;
            (*file_hdr_ptr)->section_hdr[offset].sct_size = tmp_phdr->p_memsz;
            (*file_hdr_ptr)->section_hdr[offset].data_ptr = (U8 *)malloc(tmp_phdr->p_memsz);
            memcpy((*file_hdr_ptr)->section_hdr[offset].data_ptr, 
                                lelf.elf.psct->data, tmp_phdr->p_memsz);
        }

        return SUCCESS;
    }
    else
    {
        return FALSE;
}

}

U8 FileHandler::read_symbian_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename)
{

    __int64 FileSize;
    DWORD Imagesize ;
    FILE *fp = file_ptr;    
    //Get the File length
    if (fseek(fp, 0x0L, SEEK_END))
    {
    
        fclose(fp);
        return FAULT;
    }        
    if (fgetpos(fp, &FileSize))
    {
        mError =  CSST_FILE_HANDLER_FGETPOS_FAIL;
        fclose(fp);
        return FAULT;
    }    
    Imagesize = (DWORD) FileSize - 256L;    //Strip the Symbian header    
    
   
    if(Imagesize == -1)
        return  FAULT;
      
    //Point at the File's data contents
    if (fseek(fp, 0x100L, SEEK_SET))    
    {
        fclose(fp);
        return FAULT;
    }
    /* Alloc file hdr and fill it */
    *file_hdr_ptr = ALLOC_FILE_HDR(1);
    
    (*file_hdr_ptr)->num_of_sct = 1;
    (*file_hdr_ptr)->file_type = SYMBIAN;
    (*file_hdr_ptr)->load_addr = 0;
    
    (*file_hdr_ptr)->section_hdr[0].dst_addr = 0;
    (*file_hdr_ptr)->section_hdr[0].sct_size = Imagesize;
    (*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(Imagesize);
    
    fread((*file_hdr_ptr)->section_hdr[0].data_ptr, 1, Imagesize, fp);
    return SUCCESS;
    
}

U8 FileHandler::read_wince_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename)
{

    DWORD Imagesize;
    WINCE_IMAGE_HEADER imghdr;
    WINCE_RECORD_HEADER header;

    FILE *fp = file_ptr;
    int n_sct=0;//no of headers
    //Read the image first header
    if ((fread(&imghdr, 1, 15L, fp) != 15L))
    {
        mError =  CSST_FILE_HANDLER_FREAD_FAIL;
        fclose(fp);
        return FAULT;
    }
        Imagesize = imghdr.ImageLength;

    //Read the very first record header
    //We assume this will have the lowest address in the image
    if ((fread(&header, 1, 12L, fp) != 12L))
    {
        mError = CSST_FILE_HANDLER_FREAD_FAIL;                
        fclose(fp);
        return FAULT;
    }
    
    /* Alloc file hdr and fill it */
    
    if ((header.CheckSum != 0) || (header.RecordAddr != 0))
    {
        if(header.RecordLength != 0)        //need to add the data from this header
        {
            n_sct++; //added 
            *file_hdr_ptr = ALLOC_FILE_HDR(1);
            //Point into the image buffer
            //pImage = mpBuffer + header.RecordAddr - imghdr.ImageAddr;            
            (*file_hdr_ptr)->section_hdr[n_sct-1].dst_addr = header.RecordAddr ;
            (*file_hdr_ptr)->section_hdr[n_sct-1].sct_size = header.RecordLength;
            (*file_hdr_ptr)->section_hdr[n_sct-1].data_ptr = (U8 *)malloc(header.RecordLength);
            fread((*file_hdr_ptr)->section_hdr[n_sct-1].data_ptr, 1, header.RecordLength, fp);
            
        }
    }
    //go through the records
    do
    {                
        //Read a record header
        if ((fread(&header, 1, 12L, fp) != 12L))
        {
            //mError = FREAD_FAIL;                
            fclose(fp);
            return FALSE;
        }        
        if ((header.CheckSum != 0) || (header.RecordAddr != 0))
        {
            if(header.RecordLength != 0)        //need to add the data from this header
            {
                *file_hdr_ptr = ALLOC_FILE_HDR(1);
                n_sct++; //added 
                //Point into the image buffer
                //pImage = mpBuffer + header.RecordAddr - imghdr.ImageAddr;                

                (*file_hdr_ptr)->section_hdr[n_sct-1].dst_addr = header.RecordAddr ;
                (*file_hdr_ptr)->section_hdr[n_sct-1].sct_size = header.RecordLength;
                (*file_hdr_ptr)->section_hdr[n_sct-1].data_ptr = (U8 *)malloc(header.RecordLength);
                
                fread((*file_hdr_ptr)->section_hdr[n_sct-1].data_ptr, 1, header.RecordLength, fp);
                
            }
        }
    } while ((header.CheckSum != 0) || (header.RecordAddr != 0));
    
    
    (*file_hdr_ptr)->num_of_sct = n_sct;
    (*file_hdr_ptr)->file_type = WINCE;
    (*file_hdr_ptr)->load_addr = 0;

    return SUCCESS;
    
}

U8 FileHandler::read_palm_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename)
{
   
    __int64 FileSize;
    DWORD Imagesize ;
    FILE *fp = file_ptr;    
    //Get the File length
    if (fseek(fp, 0x0L, SEEK_END))
    {
        mError = CSST_FILE_HANDLER_FSEEK_FAIL;
        fclose(fp);
        return FAULT;
    }        
    if (fgetpos(fp, &FileSize))
    {
        mError = CSST_FILE_HANDLER_FGETPOS_FAIL;
        fclose(fp);
        return FAULT;
    }    
    Imagesize = (DWORD) FileSize - 256L;    //Strip the Symbian header    
    
   if(Imagesize == -1)
        return  FAULT;
      
    //Point at the File's data contents
    if (fseek(fp, 0x100L, SEEK_SET))    
    {
        fclose(fp);
        return FALSE;
    }
    /* Alloc file hdr and fill it */
    *file_hdr_ptr = ALLOC_FILE_HDR(1);
    
    (*file_hdr_ptr)->num_of_sct = 1;
    (*file_hdr_ptr)->file_type = PALM;
    (*file_hdr_ptr)->load_addr = 0;
    
    (*file_hdr_ptr)->section_hdr[0].dst_addr = 0;
    (*file_hdr_ptr)->section_hdr[0].sct_size = Imagesize;
    (*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(Imagesize);
    
    fread((*file_hdr_ptr)->section_hdr[0].data_ptr, 1, Imagesize, fp);
    return SUCCESS;
    
}

U8 FileHandler::read_linux_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename)
{
       DWORD Imagesize ;
    __int64 FileSize;

    FILE *fp = file_ptr;

    //Get the File length
    if (fseek(fp, 0x0L, SEEK_END))
    {
        fclose(fp);
        return FAULT;        
    }        
    if(fgetpos(fp, &FileSize))
    {    
        return FAULT;
    }
    Imagesize = (DWORD) FileSize - 0x42L;    //Convert to sane sizes (under 4 GB)...
    if(Imagesize == -1)
        return  FAULT;
    
    //Point at the File's data contents
    if (fseek(fp, 0x42L, SEEK_SET))    
    {
        fclose(fp);
        return FAULT;
    }
   /* Alloc file hdr and fill it */
    *file_hdr_ptr = ALLOC_FILE_HDR(1);
    
    (*file_hdr_ptr)->num_of_sct = 1;
    (*file_hdr_ptr)->file_type = LINUX;
    (*file_hdr_ptr)->load_addr = 0;
    
    (*file_hdr_ptr)->section_hdr[0].dst_addr = 0;
    (*file_hdr_ptr)->section_hdr[0].sct_size = Imagesize;
    (*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(Imagesize);
    
    fread((*file_hdr_ptr)->section_hdr[0].data_ptr, 1, Imagesize, fp);
    return SUCCESS;


}

U8 FileHandler::read_signed_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename)
{
    __int64 FileSize;
    DWORD Imagesize ;
    FILE *fp =file_ptr;    
    char *FileName = filename ;
    if (strlen(FileName) <= 0)
    {
        return 0;
    } else
    {
        //Get the File length
        if (fseek(fp, 0x0L, SEEK_END))
        {
            fclose(fp);
            return FAULT;
        }        
        if (fgetpos(fp, &FileSize))
        {
            fclose(fp);
            return FAULT;
        }    
        Imagesize = (DWORD) FileSize;        
        
    }

     if(Imagesize == -1)
        return  FAULT;
      
     //Point at the File's data contents
    if (fseek(fp, 0x0L, SEEK_SET))    
    {
        fclose(fp);
        return FAULT;
    }
    /* Alloc file hdr and fill it */
    *file_hdr_ptr = ALLOC_FILE_HDR(1);
    
    (*file_hdr_ptr)->num_of_sct = 1;
    (*file_hdr_ptr)->file_type = SIGNED_CODE;
    (*file_hdr_ptr)->load_addr = 0;
    
    (*file_hdr_ptr)->section_hdr[0].dst_addr = 0;
    (*file_hdr_ptr)->section_hdr[0].sct_size = Imagesize;
    (*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(Imagesize);
    
    fread((*file_hdr_ptr)->section_hdr[0].data_ptr, 1, Imagesize, fp);
    return SUCCESS;

}


U8 FileHandler::read_unknown_binary_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr,char *filename)
{
    __int64 FileSize;
    DWORD Imagesize ;
    FILE *fp =file_ptr;    
    char *FileName = filename ;
    if (strlen(FileName) <= 0)
    {
        return 0;
    } else
    {
        //Get the File length
        if (fseek(fp, 0x0L, SEEK_END))
        {
            fclose(fp);
            return FAULT;
        }        
        if (fgetpos(fp, &FileSize))
        {

            fclose(fp);
            return FAULT;
        }    
        Imagesize = (DWORD) FileSize;        
        
    }

   if(Imagesize == -1)
        return  FAULT;
      
    /* Alloc file hdr and fill it */
    *file_hdr_ptr = ALLOC_FILE_HDR(1);
    
    (*file_hdr_ptr)->num_of_sct = 1;
    (*file_hdr_ptr)->file_type = UNKNOWN_BINARY;
    (*file_hdr_ptr)->load_addr = 0;
    
    (*file_hdr_ptr)->section_hdr[0].dst_addr = 0;
    (*file_hdr_ptr)->section_hdr[0].sct_size = Imagesize;
    (*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(Imagesize);
    
    fread((*file_hdr_ptr)->section_hdr[0].data_ptr, 1, Imagesize, fp);
    return SUCCESS;

}


/*==== PUBLIC FUNCTIONS =====================================================*/

FileHandler::FileHandler()
{
    mpBuffer = NULL;
}

FileHandler::~FileHandler()
{
//        free(mpBuffer);
}

U8 FileHandler::convert_buffer_endianess(U8 *addr, U32 length, 
    T_BUFFER_TYPE buffer_type)
{
    U8 result = FAULT;
    U32 i;

    switch(buffer_type)
        {
        case(U_16):
            if(length % 2 == 0)
                {
                for(i=0; i<length; i+=2)
                     ConvertEndianU16((U16 *)(addr + i));

                result = SUCCESS;
                }
            break;
        case(U_32):
            if(length % 4 == 0)
                {
                for(i=0; i<length; i+=4)
                     ConvertEndianU32((U32 *)(addr + i));

                result = SUCCESS;
                }
            break;
        }

    return result;
}

void FileHandler::free_file_mem(T_FILE_HDR *file_hdr)
{
    /* RAWs only contain one scn, while COFFs and SRECORDs may contain several 
    scn which are allocated as one chunk of memory. Thus only one chunk has to
    be released (independently of the number of scns in the image */
    free(file_hdr->section_hdr[0].data_ptr);

    /* Free file header */
    free(file_hdr);
}

void FileHandler::free_coff_file_mem(T_COFF_FILE_HDR *file_hdr)
{
    /* The raw data allocation is placed in the T_COFF_FILE_HDR struct instead
       of the first section header. */
    free(file_hdr->raw_data_ptr);

    /* Free file header */
    free(file_hdr);
}

U8 FileHandler::write_file(char *filename, T_FILE_HDR *hdr_ptr)
{
    FILE *outFile;
    if(hdr_ptr->file_type == RAW)
    {
        /* Open file */
        outFile = fopen(filename, "wb");

        /* Write data */
        fwrite(hdr_ptr->section_hdr[0].data_ptr, 
            sizeof(U8), hdr_ptr->section_hdr[0].sct_size, outFile);

        /* Close file */
        fclose(outFile);

        return SUCCESS;
    }
    return FAULT;
}

U8 FileHandler::read_file(char *filename, T_FILE_HDR **hdr_ptr)
{
    char *ext; 
    FILE *inFile;

    /* Open the image file */
    if ((inFile = fopen(filename, "rb")) == NULL)
    {
        return FAULT;
    }


    /* Get file extension */
    ext = (filename + strlen(filename));
    while(*ext != '.')
    {
        if(ext == filename)
            return FAULT;
        else
            ext--;
    }
    ext++;
    /* File handlers */ 
    if(strncmp("out", ext, 3) == 0)
    {
        mImageType = COFF;
        read_coff_file(inFile, hdr_ptr);
    }
    else if(strncmp("m0", ext, 2) == 0)
    {
        mImageType = SRECORD;
        read_srecord_file(inFile, hdr_ptr, M0_FILE);
    }
    else if(strncmp("m32", ext, 3) == 0)
    {
        mImageType = SRECORD;
        read_srecord_file(inFile, hdr_ptr, M32_FILE);
    }

    else if(strncmp("raw", ext, 3) == 0)
    {
        mImageType = RAW;
        read_raw_file(inFile, hdr_ptr);
    } 
    else if(strncmp("elf", ext, 3) == 0)
    {
        mImageType = ELF;
        read_elf_image(inFile, hdr_ptr,filename);
    }
    else if(strncmp("img", ext, 3) == 0)
    {
        mImageType = SYMBIAN;
        read_symbian_image(inFile, hdr_ptr,filename);
    }
    else if(strncmp("bin", ext, 3) == 0)
    {
        mImageType = WINCE;
        read_wince_image(inFile, hdr_ptr,filename);
    }
    else if(strncmp("widebin", ext, 7) == 0)
    {
        mImageType = PALM;
        read_palm_image(inFile, hdr_ptr,filename);
    }
    else if(strncmp("rr", ext, 2) == 0)
    {
        mImageType = LINUX;
        read_linux_image(inFile, hdr_ptr,filename);
    }
    else if(strncmp("ift", ext, 3) == 0)
    {
        mImageType = SIGNED_CODE;
        read_raw_file(inFile, hdr_ptr);
    }
    else if(strncmp("2nd", ext, 3) == 0)
    {
        read_raw_file(inFile, hdr_ptr);
    }
    else 
    {   
        mImageType =UNKNOWN_BINARY;
        read_unknown_binary_image(inFile, hdr_ptr,filename);
    }
    
    fclose(inFile);
    return SUCCESS;
}

//Appended from here by shyamala
BOOL FileHandler::SetImageType(IMAGETYPE type)
{
    mImageType = type;
    return SUCCESS;
}


U32 FileHandler::GetImageSize(char *FileName, DWORD* filesize)
{
    DWORD retval = 0;

    strcpy(mFileName,FileName);
    SetImageType();
    switch(mImageType)
    {
    case SYMBIAN:
        retval = ReadSymbianImageSize(filesize);
        break;
    case WINCE:
        retval = ReadWINCEImageSize(filesize);
        break;
    case PALM:
        retval = ReadPalmImageSize(filesize);
        break;
    case LINUX:
        retval = ReadLinuxImageSize(filesize);
        break;
    case COFF:
        retval = ReadCoffImageSize(filesize);
        break;
    case ELF:
        retval = ReadElfImageSize(filesize);
        break;
    case SRECORD:
        retval = ReadSrecordImageSize(filesize);
        break;
    case RAW:
        retval = ReadRawImageSize(filesize);
        break;
    case SIGNED_CODE:
        retval = ReadSignedImageSize(filesize);
        break;
    case UNKNOWN_BINARY:
    default:
        retval = ReadImageSize(filesize);
        break;
    }

    return (retval);
}

DWORD FileHandler::ReadRawImageSize(DWORD *filesize)
{
    T_FILE_HDR *fhn_hdr;

    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
        //size  = fhn_hdr->section_hdr[0].sct_size ;
        *filesize = fhn_hdr->section_hdr[0].sct_size ;
        mImageSize = *filesize;
        //return size;
        return CSST_SUCCESS;
    }
    return CSST_FILE_HANDLER_FOPEN_FAIL;
}

DWORD FileHandler::ReadSrecordImageSize(DWORD *filesize)
{
    T_FILE_HDR *fhn_hdr;

    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
        //size  = fhn_hdr->section_hdr[0].sct_size ;
        *filesize = fhn_hdr->section_hdr[0].sct_size ;
        mImageSize = *filesize;
        //return size;
        return CSST_SUCCESS;
    }
    return CSST_FILE_HANDLER_FOPEN_FAIL;
}

DWORD FileHandler::ReadSymbianImageSize(DWORD *filesize)
{

    
    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {

        *filesize  =  fhn_hdr->section_hdr[0].sct_size;
        mImageSize = *filesize;
        return CSST_SUCCESS;
    }
    return CSST_FILE_HANDLER_FOPEN_FAIL;
}

DWORD FileHandler::ReadWINCEImageSize(DWORD *filesize)
{
    U32 size=0;
    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {

        U16 n = fhn_hdr->num_of_sct;
        if(n>1)
            *filesize  = fhn_hdr->section_hdr[n-1].dst_addr + fhn_hdr->section_hdr[n-1].sct_size - fhn_hdr->section_hdr[0].dst_addr;
        else
            *filesize =  fhn_hdr->section_hdr[n-1].sct_size;
        mImageSize = *filesize;
        return CSST_SUCCESS;
    }
    return CSST_FILE_HANDLER_FOPEN_FAIL;

}

DWORD FileHandler::ReadPalmImageSize(DWORD *filesize)
{
    return (ReadSymbianImageSize(filesize));
}

DWORD FileHandler::ReadLinuxImageSize(DWORD *filesize)
{
    T_FILE_HDR *fhn_hdr;

    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
    
        *filesize =  fhn_hdr->section_hdr[0].sct_size;
        mImageSize = *filesize;
        return CSST_SUCCESS;
    }
    return CSST_FILE_HANDLER_FOPEN_FAIL;

}

DWORD FileHandler::ReadCoffImageSize(DWORD *filesize)
{
    
    U32 size=0;
    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {

        U16 n = fhn_hdr->num_of_sct;
        if(n>1)
            *filesize  = fhn_hdr->section_hdr[n-1].dst_addr + fhn_hdr->section_hdr[n-1].sct_size - fhn_hdr->section_hdr[0].dst_addr;
        else
            *filesize =  fhn_hdr->section_hdr[n-1].sct_size;
        mImageSize = *filesize;
        return CSST_SUCCESS;
    }
    return CSST_FILE_HANDLER_FOPEN_FAIL;

}


DWORD FileHandler::ReadElfImageSize(DWORD *filesize)
{
    ElfHandler lelf;
    int offset;
    T_FILE_HDR *fhn_hdr;
    
    DWORD size = 0;
    DWORD oldsize = 0;
    DWORD min_address = 0xFFFFFFFF;    
    DWORD max_address = 0x00000000;    
    
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
        //find the minimum address
        for(offset = 0; offset < fhn_hdr->num_of_sct;offset++)
        {
            //Elf32_Phdr *tmp_phdr = lelf.elf.phdr + offset;
            //min_address = min(min_address,tmp_phdr->p_vaddr);
            min_address = min(min_address,fhn_hdr->section_hdr[offset].dst_addr);
            max_address    = max(max_address,fhn_hdr->section_hdr[offset].dst_addr) + fhn_hdr->section_hdr[offset].sct_size;

        }
        mImageSize = max_address - min_address;
        * filesize = mImageSize;
        return CSST_SUCCESS;
        
    }
    
    return CSST_FILE_HANDLER_FOPEN_FAIL;    
}

DWORD FileHandler::ReadSignedImageSize(DWORD *filesize)
{
    return(ReadImageSize(filesize));
}

DWORD FileHandler::ReadImageSize(DWORD *filesize)
{
    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
    
        *filesize =  fhn_hdr->section_hdr[0].sct_size;
        mImageSize = *filesize;
        return CSST_SUCCESS;
    }
    return CSST_FILE_HANDLER_FOPEN_FAIL;

}


BOOL FileHandler::ReadFileToBuffer(BYTE *buffer, char *FileName)
{
    BOOL retval = FALSE;
    //Set the memory buffer pointer member
    mpBuffer = buffer;
    strcpy(mFileName,FileName);

    switch(mImageType)
    {
    case SYMBIAN:
        retval = ReadSymbianImage();
        break;
    case WINCE:
        retval = ReadWINCEImage();
        break;
    case PALM:
        retval = ReadPalmImage();
        break;
    case LINUX:
        retval = ReadLinuxImage();
        break;
    case COFF:
        retval = ReadCoffImage();
        break;
    case ELF:
        retval = ReadElfImage();
        break;
    case SIGNED_CODE:
        retval = ReadSignedImage();
        break;    
    case SRECORD:
        retval = ReadSrecordImage();
        break;
    case RAW:
        retval = ReadRawImage();
        break;            
    case UNKNOWN_BINARY:
        retval = ReadImage();
        break;        

    }
    return retval;
}

BOOL FileHandler::ReadRawImage()
{

    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
            
        U8 * tempbuffer = mpBuffer;
        memcpy(tempbuffer, fhn_hdr->section_hdr[0].data_ptr , fhn_hdr->section_hdr[0].sct_size);
        return SUCCESS;
    }
    return FAULT;

}

BOOL FileHandler::ReadSrecordImage()
{

    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
            
        U8 * tempbuffer = mpBuffer;
        memcpy(tempbuffer, fhn_hdr->section_hdr[0].data_ptr , fhn_hdr->section_hdr[0].sct_size);
        return SUCCESS;
    }
    return FAULT;

}

BOOL FileHandler::ReadSymbianImage()
{

    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
        U8 * tempbuffer = mpBuffer;
        memcpy(tempbuffer, fhn_hdr->section_hdr[0].data_ptr , fhn_hdr->section_hdr[0].sct_size);
        return SUCCESS;
    }
    return FAULT;
}

//WinCE images have 27 byte headers
BOOL FileHandler::ReadWINCEImage()
{
    U32 size=0, scn_size=0;
    U16 i;
    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {

        U8 * tempbuffer = mpBuffer;

        for(i=0; i<fhn_hdr->num_of_sct; i++)
        {

            tempbuffer = mpBuffer + (fhn_hdr->section_hdr[i].dst_addr - fhn_hdr->section_hdr[0].dst_addr); 
            memcpy(tempbuffer, fhn_hdr->section_hdr[i].data_ptr , fhn_hdr->section_hdr[i].sct_size);
        }
        return SUCCESS;
    }
    return FAULT;
}

//Palm Widebin is same format as Symbian image
BOOL FileHandler::ReadPalmImage()
{
    return (ReadSymbianImage());
}

//Read the Ridge Run file which has a 66 byte header 
//(actually not sure about this one)
BOOL FileHandler::ReadLinuxImage()
{
    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {
        U8 * tempbuffer = mpBuffer;
        memcpy(tempbuffer, fhn_hdr->section_hdr[0].data_ptr , fhn_hdr->section_hdr[0].sct_size);
        return SUCCESS;
    }
    return FAULT;
}

BOOL FileHandler::ReadCoffImage()
{

    U32 size=0, scn_size=0;
    U16 i;
    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {

        U8 * tempbuffer = mpBuffer;

        for(i=0; i<fhn_hdr->num_of_sct; i++)
        {

            tempbuffer = mpBuffer + (fhn_hdr->section_hdr[i].dst_addr - fhn_hdr->section_hdr[0].dst_addr); 
            memcpy(tempbuffer, fhn_hdr->section_hdr[i].data_ptr , fhn_hdr->section_hdr[i].sct_size);
        }
        return SUCCESS;
    }
    return FAULT;
}

BOOL FileHandler::ReadElfImage()
{
    ElfHandler lelf;
    int offset;
    DWORD size = 0;
    DWORD oldsize = 0;
    DWORD min_address = 0xFFFFFFFF;    
    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {

        U8 * tempbuffer = mpBuffer;
        //find the minimum address
        for(offset = 0; offset < fhn_hdr->num_of_sct;offset++)
        {
            //Elf32_Phdr *tmp_phdr = lelf.elf.phdr + offset;
            //min_address = min(min_address,tmp_phdr->p_vaddr);
            min_address = min(min_address,fhn_hdr->section_hdr[offset].dst_addr);
        }
        for(offset=0; offset<fhn_hdr->num_of_sct; offset++)
        {

            tempbuffer = mpBuffer + (fhn_hdr->section_hdr[offset].dst_addr - min_address); 
            memcpy(tempbuffer, fhn_hdr->section_hdr[offset].data_ptr , fhn_hdr->section_hdr[offset].sct_size);
        }
        return SUCCESS;
    }
    return FAULT;

}

BOOL FileHandler::ReadSignedImage()
{

    T_FILE_HDR *fhn_hdr;
    if(read_file(mFileName, &fhn_hdr)== SUCCESS)
    {

        U8 * tempbuffer = mpBuffer;
        memcpy(tempbuffer, fhn_hdr->section_hdr[0].data_ptr , fhn_hdr->section_hdr[0].sct_size);
        return SUCCESS;
    }
    return FAULT;
}

BOOL FileHandler::ReadImage()
{
    FILE *fp;
    DWORD nread;

    //open the image file
    if ((fp = fopen(mFileName, "rb")) == NULL)
    {
        mError =  CSST_FILE_HANDLER_FOPEN_FAIL;
        return FAULT;
    }    
    //Read the Image into RAM
    if (nread = fread(mpBuffer, sizeof(BYTE), mImageSize, fp) 
        != (mImageSize * sizeof (BYTE)))
    {
        mError =  CSST_FILE_HANDLER_FREAD_FAIL;
        fclose(fp);
        return FAULT;
    }
    fclose(fp);
    return SUCCESS;
}


DWORD FileHandler::GetFileSize (char *FileName)
{
    FILE *fp;
    __int64 FileSize;   

    strcpy(mFileName,FileName);
    //open the file
    if ((fp = fopen(mFileName, "rb")) == NULL)
    {
        mError =  CSST_FILE_HANDLER_FOPEN_FAIL;
        return FAULT;
    }     
    //point to the end of the file
    if (fseek(fp, 0L, SEEK_END))
    {
        mError =  CSST_FILE_HANDLER_FSEEK_FAIL;
        return FAULT;
    }
    //Get the file pointer offset (size of the file)
    if(fgetpos(fp, &FileSize))
    {
        mError =  CSST_FILE_HANDLER_FGETPOS_FAIL;
        return FAULT;
    }
    return((DWORD)FileSize);

}


BOOL FileHandler::SaveBinFile(char *FileName)
{    
    FILE    *fp;    
    
    if ((fp = fopen(FileName, "wb")) != NULL)
    {
        if (fwrite(mpBuffer, sizeof(BYTE), mImageSize, fp) != mImageSize)
        {
            fclose(fp);
            return FALSE;            
        }
        fclose(fp);
    } else
    {
        return FALSE;
    }
    fclose(fp);
    return SUCCESS;
}

DWORD FileHandler::GetmError ()
{
  return mError;
}


void FileHandler::MoveDateToBuffer(T_FILE_HDR *fhn_hdr)
{

    U32 size=0, scn_size=0;
    U16 i;
    U16 n = fhn_hdr->num_of_sct;
    if(n>1)
        size  = fhn_hdr->section_hdr[n-1].dst_addr + fhn_hdr->section_hdr[n-1].sct_size - fhn_hdr->section_hdr[0].dst_addr;
    else
        size =  fhn_hdr->section_hdr[n-1].sct_size;

    mpBuffer = (U8 *)malloc(size);

    U8 * tempbuffer = mpBuffer;

    for(i=0; i<fhn_hdr->num_of_sct; i++)
    {

        tempbuffer = mpBuffer + (fhn_hdr->section_hdr[i].dst_addr - fhn_hdr->section_hdr[0].dst_addr); 
        memcpy(tempbuffer, fhn_hdr->section_hdr[i].data_ptr , fhn_hdr->section_hdr[i].sct_size);
                
    }
}


U8 FileHandler::SetImageType()
{
    char *ext; 
    FILE *inFile;

    /* Open the image file */
    if ((inFile = fopen(mFileName, "rb")) == NULL)
    {
        return FAULT;
    }

    /* Get file extension */
    ext = mFileName;
    while(1)
    {
        while(*ext != '.')
        {
            if(*ext == NULL)
                return FAULT;
            else
                ext++;
        }
        ext++;
        if(* ext != '.' && *ext != '\\')
            break;
        else
            continue;
    }

    /* File handlers */ 
    if(strncmp("out", ext, 3) == 0)
    {
        mImageType = COFF;
    }   
    else if((strncmp("m0", ext, 2) == 0) || (strncmp("m32", ext, 3) == 0))
    {
        mImageType = SRECORD;
    }
    else if(strncmp("raw", ext, 3) == 0)
    {
        mImageType = RAW;
    } 
    else if(strncmp("elf", ext, 3) == 0)
    {
        mImageType = ELF;
    }
    else if(strncmp("img", ext, 3) == 0) //unknown extension
    {
        mImageType = SYMBIAN;
    
    }
   else if(strncmp("bin", ext, 3) == 0)
    {
        mImageType = WINCE;
    }
    else if(strncmp("widebin", ext, 7) == 0)
    {
        mImageType = PALM;
    
    }
    else if(strncmp("rr", ext, 2) == 0)
    {
        mImageType = LINUX;
    }
    else if(strncmp("ift", ext, 3) == 0)
    {
        mImageType = SIGNED_CODE;
    }
    else 
    {    
        mImageType =UNKNOWN_BINARY;
    } 

    fclose(inFile);
    return SUCCESS;
}

U8 FileHandler::getCoffFile(const char * const filename, T_COFF_FILE_HDR **file_hdr_ptr)
{
    FILE *imageFile;

    /* Open the image file */
    if (filename == NULL || *filename == NULL)
    {
        return FALSE;
    }

    if ((imageFile = fopen(filename, "rb")) == NULL)
    {
        printf("Failure: Cannot open image file %s\n", filename);
        return FALSE;
    }

    mImageType = COFF;

    COFFFILE coff;
    U16 num_of_hdrs = 0;
    U32 data_size = 0;
    SCNHDR *scnhdr = NULL;

    if (read_coff_file_hdr(imageFile, &coff, &num_of_hdrs, &data_size, &scnhdr) == FALSE)
    {
        if (scnhdr != NULL) free(scnhdr);
        fclose(imageFile);
        return FALSE;
    }

    qsort(scnhdr, num_of_hdrs, sizeof(SCNHDR), compare_addrs);

    *file_hdr_ptr = ALLOC_COFF_FILE_HDR(num_of_hdrs);
    if (*file_hdr_ptr == NULL)
    {
        return FALSE;
    }

    (*file_hdr_ptr)->raw_data_ptr = (U8 *)malloc(data_size);
    (*file_hdr_ptr)->num_of_sct = num_of_hdrs;
    (*file_hdr_ptr)->load_addr = coff.fileoptHdr.entry;

    data_size = 0;

    /* Get number of sections and file size */
    for(U16 scn_index = 0; scn_index < num_of_hdrs; scn_index++)
    {
        (*file_hdr_ptr)->section_hdr[scn_index].dst_addr = scnhdr[scn_index].s_vaddr;
        (*file_hdr_ptr)->section_hdr[scn_index].sct_size = scnhdr[scn_index].s_size;
        (*file_hdr_ptr)->section_hdr[scn_index].file_data_ptr = scnhdr[scn_index].s_scnptr;
        (*file_hdr_ptr)->section_hdr[scn_index].data_ptr = (*file_hdr_ptr)->raw_data_ptr + data_size;

        // Initialize full name string to empty
        (*file_hdr_ptr)->section_hdr[scn_index].sct_name[0] = '\0';

        // Get section name if necessary
        if (scnhdr[scn_index].s_name.n[0] == 0x00)
        {
            // Save current file location
            U32 current_file_location = ftell(imageFile);

            // Go to symbol table entry in COFF file corresponding to this offset
            U32 string_table_ptr = coff.fileHdr.f_symptr +
                                   (coff.fileHdr.f_nsyms * 18) +
                                   scnhdr[scn_index].s_name.n_ptr.low_ptr;
            fseek(imageFile, string_table_ptr, SEEK_SET);

            // Copy file name from symbol table
            U16 i = 0;
            int ch = 0;

            do
            {
                ch = fgetc(imageFile);
                (*file_hdr_ptr)->section_hdr[scn_index].sct_name[i] = ch;
                i++;
            } while ((i < MAX_SYMBOL_NAME_SIZE) && ch != '\0');

            // String name is too long
            if (i == MAX_SYMBOL_NAME_SIZE)
            {
                (*file_hdr_ptr)->section_hdr[scn_index].sct_name[MAX_SYMBOL_NAME_SIZE - 1] = '\0';
            }

            // Restore the position in file
            fseek(imageFile, current_file_location, SEEK_SET);
        }
        else
        {
            if (strlen(scnhdr[scn_index].s_name.n) >= 8)
            {
                for (U8 i = 0; i < 8; i++)
                {
                    (*file_hdr_ptr)->section_hdr[scn_index].sct_name[i] = scnhdr[scn_index].s_name.n[i];
                }
                (*file_hdr_ptr)->section_hdr[scn_index].sct_name[i] = '\0';
            }
            else
            {
                strcpy((*file_hdr_ptr)->section_hdr[scn_index].sct_name, scnhdr[scn_index].s_name.n);
            }
        }

        // Retrieve raw data
        fseek(imageFile, scnhdr[scn_index].s_scnptr, SEEK_SET);
        fread((*file_hdr_ptr)->raw_data_ptr + data_size, sizeof(U8), scnhdr[scn_index].s_size, imageFile);

        data_size += scnhdr[scn_index].s_size;
    }

    (*file_hdr_ptr)->raw_data_size = data_size;

    free(scnhdr);
    fclose(imageFile);
    return TRUE;
}