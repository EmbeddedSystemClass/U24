#ifndef _IMAGEDEFSH_
#define _IMAGEDEFSH_

typedef struct t_osthost_file
{
	char 			FileName[1000];
	DWORD			Size;
	DWORD			Address;
	BYTE			*Buffer;
} OSTHOST_FILE;

typedef struct t_wince_image_header
{
	BYTE	sync[7];		//Byte 0 should be a 'B' for binary
	DWORD	ImageAddr;		//Physical Start Address of Image on target
	DWORD	ImageLength;	//length in bytes of Image

} WINCE_IMAGE_HEADER;

//Header for WinCE
typedef struct t_wince_record_header
{
	DWORD	RecordAddr;		//Physical Start address of data record
							//If 0 then the record address is the end of the
							//file and record length contains the image 
							//start address
	DWORD	RecordLength;	//Length of Record in Bytes
	long	CheckSum;		//signed 32 bit sum of record bytes
	//not included so you can read the header by itself and put the image into a buffer
	//BYTE	*RecordData		//Record Data (of length RecordLength);

} WINCE_RECORD_HEADER;



typedef struct t_toc_entry
{
	DWORD		reference;		//Address of subimage
	DWORD		size;			//Size in Bytes of sub image
	DWORD		flags;			//Not used by Boot Code
	DWORD		align;			//Not used by Boot Code
	DWORD		spare;			//reserved for future use
	char		name[12];		//12 character name of subimage
} TOC_ENTRY;

//ASIC ID stuff
typedef struct t_asic_id_entry
{
	BYTE		block_ID;
	BYTE		length;
	BYTE		Fixed;
	BYTE		*Data;
}	ASIC_ID_ENTRY;


#endif;

