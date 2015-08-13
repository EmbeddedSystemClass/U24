#ifndef _ELFHANDLERH_
#define _ELFHANDLERH_

#include "ElfHeader.h"
#include "windows.h"
#include "windef.h"
#include "winbase.h"

class ElfHandler
{
public:
	//Functions
	ElfHandler();
	~ElfHandler();
	unsigned short byte_swap2(unsigned short nValue);
	BOOL Read(const char *name);
	unsigned long byte_swap4(unsigned long nLongNumber);
public:	//Members
	ELFFILE elf;
private:
	BOOL m_allocated;
	BOOL isLittleEndian;
};
#endif