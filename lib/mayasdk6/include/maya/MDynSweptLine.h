
#ifndef _MDynSweptLine
#define _MDynSweptLine
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
// CLASS:    MDynSweptLine
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDynSweptLine)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MVector;

// *****************************************************************************

// CLASS DECLARATION (MDynSweptLine)

/// Class for evaluating curve segments as lines over time. (OpenMayaFX)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MDynSweptLine
{
public:
	///
					MDynSweptLine();
	///
					~MDynSweptLine();

	// Time-Dependent Access Methods
	//
	///
	MVector			vertex( int vertexId, double t = 1.0 );
	///
	MVector			normal( double x, double y, double z, double t = 1.0 );
	///
	MVector			tangent( double t = 1.0 );
	///
	double			length( double t = 1.0 );

protected:
// No protected members

private:
	static const char* className();
	MDynSweptLine( void* );
	void * fData;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDynSweptLine */
