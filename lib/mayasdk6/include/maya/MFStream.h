#ifndef _MFStream
#define _MFStream
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
// CLASS:    MFStream
//
// *****************************************************************************

///
/**
	The MFStream.h header file was created to centralize the including of the
	fstream header.  You can define REQUIRE_IOSTREAM to
	allow "iostream" to be included rather than "iostream.h".  
	This option is provided as a convenience for external developers.
*/

#if defined(REQUIRE_IOSTREAM) || defined (AW_NEW_IOSTREAMS) || (_MSC_VER >= 1300)
	#include <fstream>
	using std::endl;
	using std::streambuf;
	using std::filebuf;
	using std::ifstream;
	using std::ofstream;
	using std::fstream;

#else
	#include <fstream.h>

#endif

#endif // _MFStream
