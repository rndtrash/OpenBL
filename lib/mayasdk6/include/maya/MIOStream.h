#ifndef _MIOStream
#define _MIOStream
//
//-
// ==========================================================================
// Copyright  (C)  Alias Systems,  a division  of  Silicon Graphics  Limited.
// All rights  reserved.  These coded  instructions,  statements and computer
// programs  contain  unpublished information  proprietary to  Alias Systems,
// a  division  of  Silicon  Graphics  Limited,  which  is  protected by  the
// Canadian  and  US federal copyright law and  may not be disclosed to third
// parties or  copied  or  duplicated,  in  whole  or in part,  without prior
// written  consent  of Alias Systems, a division of Silicon Graphics Limited
// ==========================================================================
//+
//
// CLASS:    MIOStream
//
// *****************************************************************************

///
/**
	The MIOStream.h header file was created to centralize the including of the
	iostream header.  You can define REQUIRE_IOSTREAM to
	allow "iostream" to be included rather than "iostream.h".  
	This option is provided as a convenience for external developers.
*/

#if defined(REQUIRE_IOSTREAM) || defined (AW_NEW_IOSTREAMS) || (_MSC_VER >= 1300)
	#include <iostream>
	#define IOS_REF(iosref) std::iosref

	using std::cout;
	using std::cin;
	using std::cerr;
	using std::clog;

	using std::endl;
	using std::flush;
	using std::ws;

	using std::streampos;

	using std::iostream;
	using std::ostream;
	using std::istream;
	using std::ios;
	

#else
	#include <iostream.h>
	#define IOS_REF(iosref) iosref
#endif

#endif // _MIOStream
