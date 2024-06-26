#ifndef _MUintArray
#define _MUintArray
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
// CLASS:    MUintArray
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MUintArray)
//
//	This class implements an array of unsigned integers.  Common convenience functions
//	are available, and the implementation is compatible with the internal
//	Maya implementation so that it can be passed efficiently between plugins
//	and internal maya data structures.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MUintArray)

/// Array of integers data type
/**
  Implement an array of integers data type.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MUintArray  
{

public:
 	///
					MUintArray();
 	///
					MUintArray( const MUintArray& other );
 	///
					MUintArray( const unsigned int src[], unsigned count );
	///
					MUintArray( unsigned initialSize, 
							   unsigned int initialValue = 0 );
	///
					~MUintArray();
	///
 	unsigned int	operator[]( unsigned index ) const;
 	///
 	unsigned int&	 operator[]( unsigned index );
 	///
 	MUintArray &     operator=( const MUintArray & other );
	///
	MStatus			set( unsigned int element, unsigned index );
 	///
	MStatus			setLength( unsigned length );
	///
 	unsigned        length() const;
 	///
 	MStatus			remove( unsigned index );
 	///
 	MStatus			insert( unsigned int element, unsigned index );
 	///
 	MStatus			append( unsigned int element );
 	///
 	MStatus         copy( const MUintArray& source );
 	///
 	MStatus		 	clear();
	///
	MStatus			get( unsigned int[] ) const;
	///
	void			setSizeIncrement ( unsigned newIncrement );
	///
	unsigned		sizeIncrement () const;
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MUintArray &array);
protected:
// No protected members

private:
	MUintArray( void* );
	friend class MPolyMessage;
 	void* fArray;
	bool   own;
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MUintArray */
