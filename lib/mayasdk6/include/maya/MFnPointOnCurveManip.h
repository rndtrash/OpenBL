
#ifndef _MFnPointOnCurveManip
#define _MFnPointOnCurveManip
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
// CLASS:    MFnPointOnCurveManip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnPointOnCurveManip)
//
// Function set for a base manipulator - the PointOnCurveManip.
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

// CLASS DECLARATION (MFnPointOnCurveManip)

/// PointOnCurveManip function set (OpenMayaUI)
/**
MFnPointOnCurveManip is the function set for point on curve manipulators.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MFnPointOnCurveManip : public MFnManip3D
{
	declareDagMFn(MFnPointOnCurveManip, MFnManip3D);

public:
	///
	MObject		create(MStatus *ReturnStatus = NULL);
	///
	MObject		create(const MString &manipName,
					   const MString &paramName,
					   MStatus *ReturnStatus = NULL);
	///
	MStatus		connectToCurvePlug(MPlug &curvePlug);
	///
	MStatus		connectToParamPlug(MPlug &paramPlug);
	///
	MStatus		setDrawCurve(bool state);
	///
	MStatus		setParameter(double parameter);
	///
	bool		isDrawCurveOn(MStatus *ReturnStatus = NULL) const;
	///
	double		parameter(MStatus *ReturnStatus = NULL) const;
	///
	MPoint		curvePoint(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	curveIndex(MStatus *ReturnStatus = NULL) const;
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
#endif /* _MFnPointOnCurveManip */
