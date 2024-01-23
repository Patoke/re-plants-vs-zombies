#ifndef __SEXYMEMMGR_H__
#define __SEXYMEMMGR_H__

//////////////////////////////////////////////////////////////////////////
//						HOW TO USE THIS FILE
//
//			In the desired .CPP file (NOT header file), AFTER ALL of your
//	#include declarations, do a #include "memmgr.h" or whatever you renamed
//	this file to. It's very important that you do it only in the .cpp and
//	after every other include file, otherwise it won't compile.  The memory leaks
//  will appear in a file called mem_leaks.txt and they will also be printed out
//  in the output window when the program exits.
//
//////////////////////////////////////////////////////////////////////////


#include <list>
#include <stdio.h>
#include <stdlib.h>

extern void SexyDumpUnfreed();

#if defined(SEXY_MEMTRACE) && !defined(RELEASEFINAL)

/************************************************************************/
/* DO NOT CALL THESE TWO METHODS DIRECTLY								*/
/************************************************************************/
void SexyMemAddTrack(void* addr,  int asize,  const char *fname, int lnum);
void SexyMemRemoveTrack(void *addr);


//Replacement for the standard "new" operator, records size of allocation and 
//the file/line number it was on
inline void* __cdecl operator new(unsigned int size, const char* file, int line)
{
	void* ptr = (void*)malloc(size);
	SexyMemAddTrack(ptr, size, file, line);
	return(ptr);
}

//Same as above, but for arrays
inline void* __cdecl operator new[](unsigned int size, const char* file, int line)
{
	void* ptr = (void*)malloc(size);
	SexyMemAddTrack(ptr, size, file, line);
	return(ptr);
}


// These single argument new operators allow vc6 apps to compile without errors
inline void* __cdecl operator new(unsigned int size)
{
	void* ptr = (void*)malloc(size);
	return(ptr);
}

inline void* __cdecl operator new[](unsigned int size)
{
	void* ptr = (void*)malloc(size);
	return(ptr);
}


//custom delete operators
inline void __cdecl operator delete(void* p)
{
	SexyMemRemoveTrack(p);
	free(p);
}

inline void __cdecl operator delete[](void* p)
{
	SexyMemRemoveTrack(p);
	free(p);
}

//needed in case in the constructor of the class we're newing, it throws an exception
inline void __cdecl operator delete(void* pMem, const char *file, int line)
{
	free(pMem);
}

inline void __cdecl operator delete[](void* pMem, const char *file, int line)
{
	free(pMem);
}


#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW


#endif // SEXY_MEMTRACE


#endif