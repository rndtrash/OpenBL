
#ifndef _MFnLatticeData
#define _MFnLatticeData
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
// CLASS:    MFnLatticeData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnLatticeData)
//
//	This simple function set is used to create lattice data objects that can
//	be transmitted through the dependency graph.  It can also be used to
//	extract the lattice geometry from the data object.
//
//	See MDataHandle and MDataBlock for more information about getting and
//	setting data in the dependency graph.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnGeometryData.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnLatticeData)

/// Lattice Data dependency graph type
/**
  Manage lattice data that is passed between dependency graph nodes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnLatticeData : public MFnGeometryData 
{

	declareMFn(MFnLatticeData, MFnGeometryData);

public:
	///
	MObject create( MStatus* ReturnStatus = NULL );

	///
	MObject lattice( MStatus* ReturnStatus = NULL );

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
#endif /* _MFnLatticeData */
