
#ifndef _MFnSphereData
#define _MFnSphereData
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
// CLASS:    MFnSphereData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnSphereData)
//
// MFnSphereData allows the creation and manipulation of sphere (represented
// as MObjects) data objects for use in the dependency graph.
// 
// If a user written dependency node either accepts or produces MObjects
// containing spheres, then this class is used to extract or create the
// data that comes from or goes to other dependency graph nodes. The
// MDataHandle::type method will return kSphere when data of this type is
// present. To access it, the MDataHandle::data method is used to get an
// MObject for the data and this should then be used to initialize an
// instance of MFnNurbsSurface.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnData.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnSphereData)

/// Sphere function set for dependency node data 
/**
  Create and manipulate Sphere dependency node data
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnSphereData : public MFnData 
{

	declareMFn(MFnSphereData, MFnData);

public:
	///
	MObject			create( double rad=1, MStatus* ReturnStatus = NULL );
	///
	double			radius( MStatus* ReturnStatus = NULL ) const;
	///
	MStatus			setRadius( double rad );

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
#endif /* _MFnSphereData */
