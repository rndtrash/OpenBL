
#ifndef _MAttributeSpecArray
#define _MAttributeSpecArray
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
// CLASS:    MAttributeSpecArray
//
//    Array class for MAttributeSpec.
//
//    See MAttributeSpec for more information.
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MAttributeSpecArray)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MAttributeSpec;

// *****************************************************************************

// CLASS DECLARATION (MAttributeSpecArray)

/// An attribute specification array
/**
  Implement an array of MAttributeSpecs data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MAttributeSpecArray  
{

public:
	///
	MAttributeSpecArray();
	///
	MAttributeSpecArray( const MAttributeSpecArray& other );
	///
	~MAttributeSpecArray();

	///
	MAttributeSpec	operator[]( unsigned index ) const;
 	///
 	MAttributeSpecArray & operator=( const MAttributeSpecArray & other );

	///
	MStatus			set( const MAttributeSpec& element, unsigned index );
	///
	MStatus			setLength( unsigned length );
	///
	unsigned		length() const;
	///
	MStatus			remove( unsigned index );
	///
	MStatus			insert( const MAttributeSpec & element, unsigned index );
	///
	MStatus			append( const MAttributeSpec & element );
	///
	MStatus			clear();
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;

protected:
// No protected members

private:
	MAttributeSpecArray( void* );
	void * arr;
	bool   own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MAttributeSpecArray */
