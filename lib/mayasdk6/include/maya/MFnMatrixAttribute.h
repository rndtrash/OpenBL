
#ifndef _MFnMatrixAttribute
#define _MFnMatrixAttribute
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
// CLASS:    MFnMatrixAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnMatrixAttribute)
//
// Function object for matrix attributes.  These attributes accept either a
// matrix of float values or double values depending on the type specified to
// the create method.
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
class MMatrix;
class MFloatMatrix;

// CLASS DECLARATION (MFnMatrixAttribute)

/// Matrix attribute function set
/**
  Function set for matrix attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnMatrixAttribute : public MFnAttribute 
{

	declareMFn(MFnMatrixAttribute, MFnAttribute);

public:

	///
	enum Type {
		/// matrix of float values
		kFloat,
		/// matrix of double values
		kDouble
	};
 
    ///
	MObject     create( const MString& fullName,
					    const MString& briefName, 
						Type matrixType = kDouble,
					    MStatus* ReturnStatus = NULL ); 
    ///
	MStatus     getDefault( MMatrix & def );
    ///
	MStatus     getDefault( MFloatMatrix & def );
    ///
	MStatus     setDefault( const MMatrix & def );
    ///
	MStatus     setDefault( const MFloatMatrix & def ); 

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
#endif /* _MFnMatrixAttribute */



