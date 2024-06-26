
#ifndef _MItCurveCV
#define _MItCurveCV
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
// CLASS:    MItCurveCV
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MItCurveCV)
//
//	This class is the iterator class for NURBS curve control vertices (CVs).
//	The iteration can be for a given curve or for a group of CVs.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDagNode.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MPointArray;
class MDoubleArray;
class MVector;
class MPoint;
class MDagPath;
class MPtrBase;

// *****************************************************************************

// CLASS DECLARATION (MItCurveCV)

/// Iterator for NURBS curve CVs
/**
  Iterate over CVs of a NURBS curve.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MItCurveCV
{
public:
	///
    MItCurveCV( MObject & curve, MStatus * ReturnStatus = NULL );
    ///
    MItCurveCV( const MDagPath & curve,
				MObject & component = MObject::kNullObj,
                MStatus * ReturnStatus = NULL );
    ///
    virtual ~MItCurveCV();
    ///
    bool        isDone( MStatus * ReturnStatus = NULL ) const;
    ///
    MStatus     next();
    ///
    MStatus     reset();
    ///
    MStatus     reset( MObject & curve );
    ///
    MStatus     reset( const MDagPath & curve,
						MObject & component = MObject::kNullObj );
    ///
    MPoint      position( MSpace::Space space = MSpace::kObject,
                          MStatus * ReturnStatus = NULL ) const;
    ///
    MStatus     setPosition( const MPoint & pt,
							 MSpace::Space space = MSpace::kObject );
    ///
    MStatus     translateBy( const MVector & vec,
							 MSpace::Space space = MSpace::kObject );
    ///
    int	    index( MStatus * ReturnStatus = NULL ) const;
	///
	MObject		cv( MStatus * ReturnStatus = NULL ) const;

    ///
	bool        hasHistoryOnCreate( MStatus * ReturnStatus = NULL ) const;
    ///
    MStatus     updateCurve();

protected:
// No protected members

private:
    static const char* className();
	inline void * updateGeomPtr() const;
    MPtrBase * f_shape;
    MPtrBase * f_geom;
    void *     f_path;
	void *     f_it;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MItCurveCV */
