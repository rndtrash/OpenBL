
#ifndef _MFnScaleManip
#define _MFnScaleManip
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
// CLASS:    MFnScaleManip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnScaleManip)
//
// Function set for a base manipulator - the ScaleManip.
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

// *****************************************************************************

// CLASS DECLARATION (MFnScaleManip)

/// ScaleManip function set (OpenMayaUI)
/**
	MFnScaleManip is the function set for the standard Maya scale tool.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MFnScaleManip : public MFnManip3D
{
	declareDagMFn(MFnScaleManip, MFnManip3D);

public:
	///
	MObject		create(MStatus *ReturnStatus = NULL);
	///
	MObject		create(const MString &manipName,
					   const MString &scaleName,
					   MStatus *ReturnStatus = NULL);
	///
	MStatus		connectToScalePlug(const MPlug &scalePlug);
	///
	MStatus		connectToScaleCenterPlug(const MPlug &scaleCenterPlug);
	///
	MStatus		setInitialScale(const MVector& scale);
	///
	MStatus		displayWithNode(const MObject& node);
	///
	MStatus		setSnapMode(bool snapEnabled);
	///
	bool		isSnapModeOn() const;
	///
	MStatus		setSnapIncrement(double snapInc);
	///
	double		snapIncrement() const;
	///
	unsigned	scaleIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	scaleCenterIndex(MStatus *ReturnStatus = NULL) const;

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
#endif /* _MFnScaleManip */
