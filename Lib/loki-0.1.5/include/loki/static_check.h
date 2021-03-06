////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Wesley Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

// Last update: June 20, 2001

#ifndef LOKI_STATIC_CHECK_INC_
#define LOKI_STATIC_CHECK_INC_

// $Header:   //BQT/VM67/Prjbase3/PlatformTool/Code/Lib/loki-0.1.5/include/loki/static_check.h_V   1.0   May 23 2011 15:29:56   JY Fang  $

namespace Loki
{
////////////////////////////////////////////////////////////////////////////////
// Helper structure for the STATIC_CHECK macro
////////////////////////////////////////////////////////////////////////////////

    template<int> struct CompileTimeError;
    template<> struct CompileTimeError<true> {};
}

////////////////////////////////////////////////////////////////////////////////
// macro STATIC_CHECK
// Invocation: STATIC_CHECK(expr, id)
// where:
// expr is a compile-time integral or pointer expression
// id is a C++ identifier that does not need to be defined
// If expr is zero, id will appear in a compile-time error message.
////////////////////////////////////////////////////////////////////////////////

#define LOKI_STATIC_CHECK(expr, msg) \
    { Loki::CompileTimeError<((expr) != 0)> ERROR_##msg; (void)ERROR_##msg; } 


////////////////////////////////////////////////////////////////////////////////
// Change log:
// March 20, 2001: add extra parens to STATIC_CHECK - it looked like a fun 
//     definition
// June 20, 2001: ported by Nick Thurn to gcc 2.95.3. Kudos, Nick!!!
////////////////////////////////////////////////////////////////////////////////

#endif // STATIC_CHECK_INC_

/* $Log:   //BQT/VM67/Prjbase3/PlatformTool/Code/Lib/loki-0.1.5/include/loki/static_check.h_V  $
 * 
 *    Rev 1.0   May 23 2011 15:29:56   JY Fang
 * Initial revision.
 * 
 *    Rev 1.1   Apr 01 2009 14:21:52   Kevin Tai
 * no change
 * 
 *    Rev 1.0   Apr 22 2008 20:56:20   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.2   Nov 10 2006 14:49:58   Alan Tu
 * Update*
 * 
 *    Rev 1.1   Nov 10 2006 14:31:14   Alan Tu
 * Update
// Revision 1.3  2006/01/16 19:05:09  rich_sposato
// Added cvs keywords.
*/
