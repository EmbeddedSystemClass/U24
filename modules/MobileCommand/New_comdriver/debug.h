#ifndef __debug_H__ 
#define __debug_H__ 

#ifdef _TRACE_ENABLE_
#define TRACE printf 
#define DLTRACE(x) printf("DL: "); printf(x);
#else
#define TRACE
#endif

#endif /* __debug_H__  */