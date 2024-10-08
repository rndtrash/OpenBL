
#ifndef _MFnPointOnSurfaceManip
#define _MFnPointOnSurfaceManip
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
// CLASS:    MFnPointOnSurfaceManip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnPointOnSurfaceManip)
//
// Function set for a base manipulator - the PointOnSurfaceManip
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnBase.h>
#include <maya/MFnManip3D.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MPoint;

// *****************************************************************************

// CLASS DECLARATION (MFnPointOnSurfaceManip)

/// PointOnSurfaceManip function set (OpenMayaUI)
/**
MFnPointOnSurfaceManip is the function set for point on surface manipulators.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MFnPointOnSurfaceManip : public MFnManip3D
{
	declareDagMFn(MFnPointOnSurfaceManip, MFnManip3D);

public:
	///
	MObject		create(MStatus *ReturnStatus = NULL);
	///
	MObject		create(const MString &manipName,
					   const MString &paramName,
					   MStatus *ReturnStatus = NULL);
	///
	MStatus		connectToSurfacePlug(MPlug &surfacePlug);
	///
	MStatus		connectToParamPlug(MPlug &paramPlug);
	///
	MStatus		setDrawSurface(bool state);
	///
	MStatus		setDrawArrows(bool state);
	///
	MStatus		setParameters(double u, double v);
	///
	MStatus		getParameters(double &u, double &v);
	///
	bool		isDrawSurfaceOn(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	surfaceIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	paramIndex(MStatus *ReturnStatus = NULL) const;

protected:

private:

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnPointOnSurfaceManip */
