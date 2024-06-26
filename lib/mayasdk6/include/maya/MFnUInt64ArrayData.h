
#ifndef _MFnUInt64ArrayData
#define _MFnUInt64ArrayData
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
// CLASS:    MFnUInt64ArrayData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnUInt64ArrayData)
//
// MFnUInt64ArrayData allows the creation and manipulation of MUint64Array data
// objects for use in the dependency graph.  
//
// If a user written dependency node either accepts or produces MUint64Arrays,
// then this class is used to extract or create the data that comes from or
// goes to other dependency graph nodes.  The MDataHandle::type method will
// return kUInt64ArrayData when data of this type is present.  To access it,
// the MDataHandle::data method is used to get an MObject for the data and
// this should then be used to initialize an instance of MFnUInt64ArrayData.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnData.h>

// *****************************************************************************

// DECLARATIONS

class MUint64Array;

// *****************************************************************************

// CLASS DECLARATION (MFnUInt64ArrayData)

/// MUint64 array function set for dependency node data
/**
  Create and manipulate MUint64Array dependency node data.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnUInt64ArrayData : public MFnData 
{

	declareMFn(MFnUInt64ArrayData, MFnData);

public:
	///
	unsigned		length( MStatus* ReturnStatus = NULL ) const;
	///
	MUint64          operator[]( unsigned index ) const;
	///
	MUint64&	        operator[]( unsigned index );
	///
	MStatus			copyTo( MUint64Array& ) const;
	///
	MStatus			set( const MUint64Array& newArray );
	///
	MUint64Array	array( MStatus*ReturnStatus=NULL );
	///
	MObject			create( MStatus*ReturnStatus=NULL );
	///
	MObject			create( const MUint64Array& in, MStatus*ReturnStatus=NULL );

protected:
// No protected members

private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnUInt64ArrayData */
