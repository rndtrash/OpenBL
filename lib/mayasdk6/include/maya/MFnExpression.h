
#ifndef _MFnExpression
#define _MFnExpression
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
// CLASS:    MFnExpression
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnExpression)
//
// Function set for expressions.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

class MDoubleArray;
class MPtrBase;

// *****************************************************************************

// CLASS DECLARATION (MFnExpression)

/// expression function set
/**

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnExpression : public MFnDependencyNode
{ 
	declareMFn( MFnExpression, MFnDependencyNode ); 

public:
	
	///
	enum UnitConversion {
		/// convert all units
		kAll,
		/// do not do any unit conversion
		kNone,
		/// only convert angular units
		kAngularOnly
	};

	///
	MObject  		create( const MString & expression,
							MObject & object = MObject::kNullObj,
							MStatus * ReturnStatus = NULL );

	///
	MStatus			getExpression( MString & expression );
	///
	MStatus			setExpression( MString & expression );

	///
	MStatus			getDefaultObject( MObject & object );
	///
	MStatus			setDefaultObject( MObject & object );
	///
	bool			isAnimated( MStatus * ReturnStatus = NULL );
	///
	MStatus			setAnimated( bool value = false );

	///
	MStatus			evaluate( MDoubleArray & result );

	///
	UnitConversion  unitConversion( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus         setUnitConversion( UnitConversion conversion );
							

protected:
// No protected members

private:
// No private members

}; 

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

#endif /* __cplusplus */
#endif /* _MFnExpression */
