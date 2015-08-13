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
#include <iostream>
#include <string>

#include "types.h"
#include "Filehandler.h"
#include "dl_filehandler.h"
#include "ElfHandler.h"
#include "ElfHeader.h"
#include "ImageDefs.h"

#if _MSC_VER >= 1400
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#endif // disable vs2005 stupid non-standard warning

/*==== MACROS ===============================================================*/

/*==== GLOBALS ==============================================================*/
typedef int (*fp_Stream_Client)(char* szServer, int port, char* SendMsg, int MsgLen);
typedef char* (*fp_errortostr)(int error);

fp_Stream_Client Stream_Client;
fp_errortostr errortostr;


/*==== PRIVATE FUNCTIONS ====================================================*/
#include "dl_filehandler.h"
void Dl_Filehandler::hextobytes(U8* dst, char* src, U16 bytes)
{
	while (bytes-- && *src) {
		*dst++ =
		    (src[0] - (src[0] <= '9' ? '0' : 'A' - 10)) * 16 +
		    (src[1] - (src[1] <= '9' ? '0' : 'A' - 10));
		src += 2;
	}
}

int compare_addrs(const void* v1, const void* v2)
{
	SCNHDR* hdr1 = (SCNHDR*)v1;
	SCNHDR* hdr2 = (SCNHDR*)v2;

	return(hdr1->s_vaddr - hdr2->s_vaddr);
}

U32 Dl_Filehandler::hextoint(char* src, U32 digits)
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

void Dl_Filehandler::ConvertEndianU16(U16* vars)
{
	U16 tmp[2];

	tmp[0] = (*vars << 8) & 0xFF00;
	tmp[1] = (*vars >> 8) & 0xFF;

	*vars = tmp[0] + tmp[1];
}

void Dl_Filehandler::ConvertEndianU32(U32* vars)
{
	U32 tmp[4];
	tmp[0] = (*vars << 24) & 0xFF000000;
	tmp[1] = (*vars << 8) & 0xFF0000;
	tmp[2] = (*vars >> 8) & 0xFF00;
	tmp[3] = (*vars >> 24) & 0xFF;

	*vars = tmp[0] + tmp[1] + tmp[2] + tmp[3];
}

U8 Dl_Filehandler::read_coff_file(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr)
{
	U8 reverse_coff = 0;
	U16 scn_index = 0, current_scn = 0, num_of_scn = 0, num_of_hdrs = 0;
	U32 data_size = 0, scn_end_addr = 0xFFFFFFFF;
	COFFFILE coff;
	SCNHDR *scnhdr;

	/* Read file header */
	if (fread(&coff.fileHdr, 22, 1, file_ptr) != 1)
		return FALSE;

	/* Check COFF version ID to determine if the file is reversed. Only COFF
	file format version 2 (0x00C2) is supported */
	if (coff.fileHdr.f_magic == 0x00C2)
		reverse_coff = 0;
	else if (coff.fileHdr.f_magic == 0xC200)
		reverse_coff = 1;
	else
		return FALSE;

	/* Reverse file header if necessary */
	if (reverse_coff) {
		ConvertEndianU16(&coff.fileHdr.f_magic);
		ConvertEndianU16(&coff.fileHdr.f_nscns);
		ConvertEndianU32(&coff.fileHdr.f_timdat);
		ConvertEndianU32(&coff.fileHdr.f_symptr);
		ConvertEndianU32(&coff.fileHdr.f_nsyms);
		ConvertEndianU16(&coff.fileHdr.f_opthdr);
		ConvertEndianU16(&coff.fileHdr.f_flags);
		ConvertEndianU16(&coff.fileHdr.f_target_id);
	}

	/* Check optional file header */
	if ((coff.fileHdr.f_opthdr != 0) && (coff.fileHdr.f_opthdr != 28)) {
		return FALSE; /* Error in COFF file format */
	}

	/* Clear entry point */
	coff.fileoptHdr.entry = 0x00000000;

	/* Read optional file header if available */
	if (coff.fileHdr.f_opthdr == 28) {
		if (fread(&coff.fileoptHdr, 28, 1, file_ptr) != 1)
			return FALSE;

		/* Reverse optional file header if necessary */
		if (reverse_coff) {
			ConvertEndianU16(&coff.fileoptHdr.magic);
			ConvertEndianU16(&coff.fileoptHdr.vstamp);
			ConvertEndianU32(&coff.fileoptHdr.tsize);
			ConvertEndianU32(&coff.fileoptHdr.dsize);
			ConvertEndianU32(&coff.fileoptHdr.bsize);
			ConvertEndianU32(&coff.fileoptHdr.entry);
			ConvertEndianU32(&coff.fileoptHdr.text_start);
			ConvertEndianU32(&coff.fileoptHdr.data_start);
		}
	}

	/* Allocate memory for SCNHDRs */
	scnhdr = (SCNHDR *) malloc(coff.fileHdr.f_nscns * sizeof(SCNHDR));

	/* Get scn headers and file size */
	for (scn_index = 0; scn_index < coff.fileHdr.f_nscns; scn_index++) {
		/* Set file position to next section header */
		fseek(file_ptr, 22 + coff.fileHdr.f_opthdr + scn_index * 48, SEEK_SET);

		/* Read section header */
		fread(&coff.sctHdr, sizeof(SCNHDR), 1, file_ptr);

		/* Reverse section header if necessary */
		if (reverse_coff) {
			ConvertEndianU32(&coff.sctHdr.s_paddr);
			ConvertEndianU32(&coff.sctHdr.s_vaddr);
			ConvertEndianU32(&coff.sctHdr.s_size);
			ConvertEndianU32(&coff.sctHdr.s_scnptr);
			ConvertEndianU32(&coff.sctHdr.s_relptr);
			ConvertEndianU32(&coff.sctHdr.s_lnnoptr);
			ConvertEndianU32(&coff.sctHdr.s_nreloc);
			ConvertEndianU32(&coff.sctHdr.s_nlnno);
			ConvertEndianU32(&coff.sctHdr.s_flags);
			ConvertEndianU16(&coff.sctHdr.s_res);
			ConvertEndianU16(&coff.sctHdr.s_mempg);
		}

		/* Read scn headers for section of importance. This is determined from
		the section header flag and pointer to raw data. */
		if (!(coff.sctHdr.s_flags & 0x11) && (coff.sctHdr.s_flags & 0x60) && (coff.sctHdr.s_scnptr != 0)) {
			memcpy(&scnhdr[num_of_hdrs], &coff.sctHdr, sizeof(SCNHDR));

			data_size += coff.sctHdr.s_size;
			num_of_hdrs++;
		}
	}

	/* Sort sections headers */
	qsort(scnhdr, num_of_hdrs, sizeof(SCNHDR), compare_addrs);

	/* Count number of scn */
	for (scn_index = 0; scn_index < num_of_hdrs; scn_index++) {
		if (scn_end_addr != scnhdr[scn_index].s_vaddr)
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
	for (scn_index = 0; scn_index < num_of_hdrs; scn_index++) {
		if (scn_end_addr != scnhdr[scn_index].s_vaddr) { /* First part of scn */
			(*file_hdr_ptr)->section_hdr[current_scn].dst_addr = scnhdr[scn_index].s_vaddr;
			(*file_hdr_ptr)->section_hdr[current_scn].sct_size = scnhdr[scn_index].s_size;
			(*file_hdr_ptr)->section_hdr[current_scn].data_ptr =
			    (*file_hdr_ptr)->section_hdr[0].data_ptr + data_size;

			current_scn++;
		}
		else {
			(*file_hdr_ptr)->section_hdr[current_scn-1].sct_size += scnhdr[scn_index].s_size;
		}

		fseek(file_ptr, scnhdr[scn_index].s_scnptr, SEEK_SET);
		fread((*file_hdr_ptr)->section_hdr[0].data_ptr + data_size, sizeof(U8), scnhdr[scn_index].s_size, file_ptr);

		data_size += scnhdr[scn_index].s_size;
		scn_end_addr = scnhdr[scn_index].s_vaddr + scnhdr[scn_index].s_size;
	}

	/* Free memory for SCNHDRs */
	free(scnhdr);

	return SUCCESS;
}

void Dl_Filehandler::read_srecord_file(FILE* file_ptr, T_FILE_HDR** file_hdr_ptr, T_FILE_TYPE type)
{
	char line[256];
	U32 num_of_scn = 0, data_size = 0, scn_end_addr = 0xFFFFFFFF;
	U32 scn_size = 0, rec_size = 0;
	U8 addr_size = 0xFF;
	U32 i = 0;

	/* Determine address size within m0-file. It is assumed that all addresses
	within the file are of the same length. */
	fseek(file_ptr, 0, SEEK_SET);
	while ((addr_size == 0xFF) && (fgets(line, 256, file_ptr) != NULL)) {
		switch (line[1]) {
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
	while (fgets(line, 256, file_ptr) != NULL) {
		switch (line[1]) {
		case '1':
		case '2':
		case '3':
			if (scn_end_addr != hextoint(&line[4], 2*addr_size))
				num_of_scn++;

			scn_end_addr = hextoint(&line[4], 2 * addr_size) + hextoint(&line[2], 2) - addr_size - 1;
			data_size += hextoint(&line[2], 2) - addr_size - 1;
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

	while (fgets(line, 256, file_ptr) != NULL) {
		switch (line[1]) {
		case '1':
		case '2':
		case '3':
			/* Set variables for new section */
			if (scn_end_addr != hextoint(&line[4], 2*addr_size)) {
				(*file_hdr_ptr)->section_hdr[i].dst_addr = scn_end_addr =
				            hextoint(&line[4], 2 * addr_size);

				if (i != 0) {
					(*file_hdr_ptr)->section_hdr[i].data_ptr =
					    (*file_hdr_ptr)->section_hdr[i-1].data_ptr + scn_size;
					(*file_hdr_ptr)->section_hdr[i-1].sct_size = scn_size;
					scn_size = 0;
				}
				i++;
			}

			rec_size = hextoint(&line[2], 2) - addr_size - 1;
			hextobytes((U8*)((*file_hdr_ptr)->section_hdr[i-1].data_ptr + scn_size),
				&line[4+2*addr_size], (U16)rec_size);

			scn_size += rec_size;
			scn_end_addr += rec_size;
			break;
		case '7':
		case '8':
		case '9':
			(*file_hdr_ptr)->section_hdr[i-1].sct_size = scn_size;
			(*file_hdr_ptr)->load_addr = hextoint(&line[4], 2 * addr_size);
			break;
		}
	}

	/* Reverse endianness (ASSUMPION: .m32 and .m0 have reverse endianness)*/
	if (type == M0_FILE) {
		for (i = 0; i < data_size / 2; i++)
			ConvertEndianU16((U16 *)((*file_hdr_ptr)->section_hdr[0].data_ptr + 2*i));
	}
}

void Dl_Filehandler::read_raw_file(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr)
{
	U32 scn_size = 0;

	/* Get file size */
	fseek(file_ptr, 0, SEEK_END);
	scn_size = ftell(file_ptr);
	fseek(file_ptr, 0, SEEK_SET);

	/* Alloc file hdr and fill it */
	*file_hdr_ptr = ALLOC_FILE_HDR(1);
	if ( *file_hdr_ptr == NULL) {
		m_iReturnFault = -9;
		return;
	}

	(*file_hdr_ptr)->num_of_sct = 1;
	(*file_hdr_ptr)->file_type = RAW;
	(*file_hdr_ptr)->load_addr = 0;

	(*file_hdr_ptr)->section_hdr[0].dst_addr = 0;
	(*file_hdr_ptr)->section_hdr[0].sct_size = scn_size;

//    char *tmp = "Before read_raw_file memory malloc";
//	Stream_Client(SERVER_IP, PORT, tmp, strlen(tmp));

	//(*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(scn_size);
	(*file_hdr_ptr)->section_hdr[0].data_ptr = new (std::nothrow) U8[scn_size];

//	tmp = "After read_raw_file memory malloc";
//	Stream_Client(SERVER_IP, PORT, tmp, strlen(tmp));

	if ((*file_hdr_ptr)->section_hdr[0].data_ptr == NULL) {
		//if (malloc(scn_size)==NULL)
		m_iReturnFault = -9;
//		tmp = "Set m_iReturnFault -9 and return";
//		Stream_Client(SERVER_IP, PORT, tmp, strlen(tmp));
		return;
	}

	fread((*file_hdr_ptr)->section_hdr[0].data_ptr, 1, scn_size, file_ptr);
}

U8 Dl_Filehandler::read_elf_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char* filename)
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

	if (lelf.Read(filename)) {
		/* Alloc file hdr and fill it */
		*file_hdr_ptr = ALLOC_FILE_HDR(lelf.elf.ehdr->e_phnum);

		(*file_hdr_ptr)->num_of_sct = lelf.elf.ehdr->e_phnum;
		(*file_hdr_ptr)->file_type = ELF;
		(*file_hdr_ptr)->load_addr = 0; //SHY to check*/

		//find the minimum address
		for (offset = 0; offset < lelf.elf.ehdr->e_phnum;offset++) {
			Elf32_Phdr *tmp_phdr = lelf.elf.phdr + offset;
			min_address = min(min_address, tmp_phdr->p_vaddr);
		}
		//copy the image
		for (offset = 0; offset < lelf.elf.ehdr->e_phnum;offset++) {

			Elf32_Phdr *tmp_phdr = lelf.elf.phdr + offset;

			(*file_hdr_ptr)->section_hdr[offset].dst_addr = tmp_phdr->p_vaddr;
			(*file_hdr_ptr)->section_hdr[offset].sct_size = tmp_phdr->p_memsz;
			(*file_hdr_ptr)->section_hdr[offset].data_ptr = (U8 *)malloc(tmp_phdr->p_memsz);
			memcpy((*file_hdr_ptr)->section_hdr[offset].data_ptr,
			       lelf.elf.psct->data, tmp_phdr->p_memsz);
		}

		return TRUE;
	}
	else {
		return FALSE;
	}
}

U8 Dl_Filehandler::read_symbian_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char *filename)
{
	__int64 FileSize;
	DWORD Imagesize ;
	FILE *fp = file_ptr;
	//Get the File length
	if (fseek(fp, 0x0L, SEEK_END)) {
		fclose(fp);
		return -1;
	}
	if (fgetpos(fp, &FileSize)) {
		//mError = FGETPOS_FAIL;
		fclose(fp);
		return -1;
	}
	Imagesize = (DWORD) FileSize - 256L;	//Strip the Symbian header
	if (Imagesize == -1)
		return  FALSE;

	//Point at the File's data contents
	if (fseek(fp, 0x100L, SEEK_SET)) {
		fclose(fp);
		return FALSE;
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

	return TRUE;
}

U8 Dl_Filehandler::read_wince_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char *filename)
{
	DWORD Imagesize;
	WINCE_IMAGE_HEADER imghdr;
	WINCE_RECORD_HEADER header;

	FILE *fp = file_ptr;
	int n_sct = 0;//no of headers
	//Read the image first header
	if ((fread(&imghdr, 1, 15L, fp) != 15L)) {
		fclose(fp);
		return FAULT;
	}
	Imagesize = imghdr.ImageLength;

	//Read the very first record header
	//We assume this will have the lowest address in the image
	if ((fread(&header, 1, 12L, fp) != 12L)) {
		fclose(fp);
		return FAULT;
	}

	/* Alloc file hdr and fill it */
	if ((header.CheckSum != 0) || (header.RecordAddr != 0)) {
		if (header.RecordLength != 0) {	//need to add the data from this header
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
	do {
		if ((fread(&header, 1, 12L, fp) != 12L)) { //Read a record header
			//mError = FREAD_FAIL;
			fclose(fp);
			return FALSE;
		}
		if ((header.CheckSum != 0) || (header.RecordAddr != 0)) {
			if (header.RecordLength != 0) {	//need to add the data from this header
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
	}
	while ((header.CheckSum != 0) || (header.RecordAddr != 0));

	(*file_hdr_ptr)->num_of_sct = n_sct;
	(*file_hdr_ptr)->file_type = WINCE;
	(*file_hdr_ptr)->load_addr = 0;

	return SUCCESS;
}

U8 Dl_Filehandler::read_palm_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char *filename)
{
	__int64 FileSize;
	DWORD Imagesize ;
	FILE *fp = file_ptr;
	//Get the File length
	if (fseek(fp, 0x0L, SEEK_END)) {
		//mError = FSEEK_FAIL;
		fclose(fp);
		return -1;
	}
	if (fgetpos(fp, &FileSize)) {
		//mError = FGETPOS_FAIL;
		fclose(fp);
		return -1;
	}
	Imagesize = (DWORD) FileSize - 256L;	//Strip the Symbian header

	if (Imagesize == -1)
		return  FALSE;

	//Point at the File's data contents
	if (fseek(fp, 0x100L, SEEK_SET)) {
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

	return TRUE;
}

U8 Dl_Filehandler::read_linux_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char *filename)
{
	DWORD Imagesize ;
	__int64 FileSize;

	FILE *fp = file_ptr;

	//Get the File length
	if (fseek(fp, 0x0L, SEEK_END)) {
		fclose(fp);
		return -1;
	}
	if (fgetpos(fp, &FileSize)) {
		return -1;
	}
	Imagesize = (DWORD) FileSize - 0x42L;	//Convert to sane sizes (under 4 GB)...
	if (Imagesize == -1)
		return  FALSE;

	//Point at the File's data contents
	if (fseek(fp, 0x42L, SEEK_SET)) {
		fclose(fp);
		return FALSE;
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

	return TRUE;
}

U8 Dl_Filehandler::read_signed_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char *filename)
{
	__int64 FileSize;
	DWORD Imagesize ;
	FILE *fp = file_ptr;
	char *FileName = filename ;
	if (strlen(FileName) <= 0) {
		return 0;
	}
	else {
		//Get the File length
		if (fseek(fp, 0x0L, SEEK_END)) {
			fclose(fp);
			return -1;
		}
		if (fgetpos(fp, &FileSize)) {
			fclose(fp);
			return -1;
		}
		Imagesize = (DWORD) FileSize;

	}

	if (Imagesize == -1)
		return  FALSE;

	//Point at the File's data contents
	if (fseek(fp, 0x0L, SEEK_SET)) {
		fclose(fp);
		return FALSE;
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
	return TRUE;

}

U8 Dl_Filehandler::read_unknown_binary_image(FILE *file_ptr, T_FILE_HDR **file_hdr_ptr, char *filename)
{
	__int64 FileSize;
	DWORD Imagesize ;
	FILE *fp = file_ptr;
	char *FileName = filename ;
	if (strlen(FileName) <= 0) {
		return 0;
	}
	else {
		//Get the File length
		if (fseek(fp, 0x0L, SEEK_END)) {
			fclose(fp);
			return -1;
		}
		if (fgetpos(fp, &FileSize)) {
			fclose(fp);
			return -1;
		}
		Imagesize = (DWORD) FileSize;

	}

	if (Imagesize == -1)
		return  FALSE;

	/* Alloc file hdr and fill it */
	*file_hdr_ptr = ALLOC_FILE_HDR(1);

	(*file_hdr_ptr)->num_of_sct = 1;
	(*file_hdr_ptr)->file_type = UNKNOWN_BINARY;
	(*file_hdr_ptr)->load_addr = 0;

	(*file_hdr_ptr)->section_hdr[0].dst_addr = 0;
	(*file_hdr_ptr)->section_hdr[0].sct_size = Imagesize;
	(*file_hdr_ptr)->section_hdr[0].data_ptr = (U8 *)malloc(Imagesize);

	fread((*file_hdr_ptr)->section_hdr[0].data_ptr, 1, Imagesize, fp);
	return TRUE;

}


/*==== PUBLIC FUNCTIONS =====================================================*/

Dl_Filehandler::Dl_Filehandler()
{
	m_hDLL = LoadLibrary("ClientDLL.dll");
	if (m_hDLL != NULL) {
		Stream_Client = (fp_Stream_Client)GetProcAddress(m_hDLL, "Stream_Client");
		errortostr = (fp_errortostr)GetProcAddress(m_hDLL, "errortostr");
		if (Stream_Client == NULL || errortostr == NULL) { // handle the error
			FreeLibrary(m_hDLL);
			m_hDLL = NULL;
		}
	}
	else {
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}

	//char tmp = "";
	//Stream_Client(SERVER_IP, PORT, tmp, strlen(tmp));

	m_iReturnFault = -5;
}

Dl_Filehandler::~Dl_Filehandler()
{
}

U8 Dl_Filehandler::convert_buffer_endianess(U8 *addr, U32 length,
        T_BUFFER_TYPE buffer_type)
{
	U8 result = FAULT;
	U32 i;

	switch (buffer_type) {
	case(U_16):
		if (length % 2 == 0) {
			for (i = 0; i < length; i += 2)
				ConvertEndianU16((U16 *)(addr + i));

			result = SUCCESS;
		}
		break;
	case(U_32):
		if (length % 4 == 0) {
			for (i = 0; i < length; i += 4)
				ConvertEndianU32((U32 *)(addr + i));

			result = SUCCESS;
		}
		break;
	}

	return result;
}

void Dl_Filehandler::free_file_mem(T_FILE_HDR *file_hdr)
{
	/* RAWs only contain one scn, while COFFs and SRECORDs may contain several
	scn which are allocated as one chunk of memory. Thus only one chunk has to
	be released (independently of the number of scns in the image */
	//free(file_hdr->section_hdr[0].data_ptr);
	if (file_hdr->section_hdr[0].data_ptr != NULL)
		delete []file_hdr->section_hdr[0].data_ptr;

	//FF:set point to NULL to solve reboot bug
	file_hdr->section_hdr[0].data_ptr = NULL;
	/* Free file header */

	//FF:set point to NULL to solve reboot bug
	//free(file_hdr);
	if ( file_hdr != NULL)
		delete []file_hdr;
	file_hdr = NULL;
//	char *tmp = "Dl_Filehandler::free_file_mem return";
//	Stream_Client(SERVER_IP, PORT, tmp, strlen(tmp));

}

U8 Dl_Filehandler::write_file(char *filename, T_FILE_HDR *hdr_ptr)
{
	FILE *outFile;
	if (hdr_ptr->file_type == RAW) {
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

U8 Dl_Filehandler::read_file(char *filename, T_FILE_HDR **hdr_ptr)
{
	char *ext;
	FILE *inFile;

	/* Open the image file */
	if ((inFile = fopen(filename, "rb")) == NULL) {
		//printf("Failure: could not open file\n");
		fclose(inFile);
		return FAULT;
	}

	/* Get file extension */
	ext = (filename + strlen(filename));
	while (*ext != '.') {
		if (ext == filename) {
			fclose(inFile);
			return FAULT;
		}
		else
			ext--;
	}
	ext++;

	/* File handlers */
	if (strncmp("out", ext, 3) == 0) {
		//printf("COFF file\n");
		read_coff_file(inFile, hdr_ptr);
	}
	else if (strncmp("m0", ext, 2) == 0) {
		//printf("S-Record file\n");
		read_srecord_file(inFile, hdr_ptr, M0_FILE);
	}
	else if (strncmp("m32", ext, 3) == 0) {
		//printf("S-Record file\n");
		read_srecord_file(inFile, hdr_ptr, M32_FILE);
	}
	else if ((strncmp("raw", ext, 3) == 0) || (0 == strncmp("xsvf", ext, 4))) {
		//printf("Binary RAW file\n");
		read_raw_file(inFile, hdr_ptr);
		if (m_iReturnFault == -9) {
			m_iReturnFault = -5;
			fclose(inFile);
			return FAULT;
		}

	}
	else if (strncmp("elf", ext, 3) == 0) {
		//printf("ELF file\n");
		read_elf_image(inFile, hdr_ptr, filename);
	}
	else if (strncmp("img", ext, 3) == 0) { //unknown extension
		//printf("SYMBIAN file\n");
		//   read_symbian_image(inFile, hdr_ptr,filename);
		read_raw_file(inFile, hdr_ptr);
		if (m_iReturnFault == -9) {
			m_iReturnFault = -5;
			fclose(inFile);
			return FAULT;
		}

	}
	else if (strncmp("bin", ext, 3) == 0) {
		//printf("WINCE IMage\n");
		// read_wince_image(inFile, hdr_ptr,filename);
		read_raw_file(inFile, hdr_ptr);
		if (m_iReturnFault == -9) {
			m_iReturnFault = -5;
			fclose(inFile);
			return FAULT;
		}
	}
	else if (strncmp("widebin", ext, 7) == 0) {
		//printf("PALM Image\n");
		read_palm_image(inFile, hdr_ptr, filename);
	}
	else if (strncmp("rr", ext, 2) == 0) {
		//printf("LINUX IMAGE\n");
		read_linux_image(inFile, hdr_ptr, filename);
	}
	else if (strncmp("ift", ext, 3) == 0) {
		//printf("SIGNED CODE \n");
		//read_signed_image(inFile, hdr_ptr,filename);
		read_raw_file(inFile, hdr_ptr);
		if (m_iReturnFault == -9) {
			m_iReturnFault = -5;
			fclose(inFile);
			return FAULT;
		}
	}
	else if (strncmp("2nd", ext, 3) == 0) {
		//printf("Binary RAW file\n");
		read_raw_file(inFile, hdr_ptr);
		if (m_iReturnFault == -9) {
			m_iReturnFault = -5;
			fclose(inFile);
			return FAULT;
		}
	}
	else {
		//printf("UNKNOWN BINARY \n");
		//   read_unknown_binary_image(inFile, hdr_ptr,filename);
		read_raw_file(inFile, hdr_ptr);
		if (m_iReturnFault == -9) {
			m_iReturnFault = -5;
			fclose(inFile);
			return FAULT;
		}
	}

	fclose(inFile);
	return SUCCESS;
}
