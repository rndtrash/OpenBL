
#ifndef _MFnEnumAttribute
#define _MFnEnumAttribute
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
// CLASS:    MFnEnumAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnEnumAttribute)
//
// Function object for enumerated attributes.  These attributes accept fields as
// input. Each field consists of a short and its associated string, which is a
// descriptive name for the short.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

class MString;

// CLASS DECLARATION (MFnEnumAttribute)

/// Enumerated attribute function set
/**
  Function set for enumerated attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnEnumAttribute : public MFnAttribute
{

	declareMFn(MFnEnumAttribute, MFnAttribute);

public:

    ///
	MObject     create( const MString& fullName,
					    const MString& briefName, 
						short defaultValue = 0,
					    MStatus* ReturnStatus = NULL ); 
	///
	MStatus		addField( const MString & fieldString, short index);
	///
	MString     fieldName( short index, MStatus *ReturnStatus = NULL ) const;
	///
	short       fieldIndex( const MString & fieldString,
							MStatus *ReturnStatus = NULL ) const;
	///
	MStatus		getMin ( short& minValue ) const;
	///
	MStatus		getMax ( short& maxValue ) const;

	///
	MStatus 	setDefault( short index ); 
	///
	MStatus 	setDefault( const MString &fieldString );

	///
	MStatus		getDefault( short &index ) const; 
	///
	MStatus		getDefault( MString &fieldString ) const; 


protected:
// No protected members

private:

	MStatus		commonGetDefault( short &index ) const; 
	MStatus		commonSetDefault( short index ) const; 
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnEnumAttribute */
