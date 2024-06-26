
#ifndef _MFnRotateManip
#define _MFnRotateManip
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
// CLASS:    MFnRotateManip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnRotateManip)
//
// This class provides access to the built-in Maya rotate manipulator.  The 
// manipulator consists of three constrained-axis rotation rings, a view rotation
// ring, as well as an invisible trackball that allows the user to rotate in 
// arbitrary directions on the sphere.
// 
// The manipulator provides data to the plugin through the rotation manipVal.
// The rotation value is a vector consisting of x, y, and z rotations.  
// Rotations are measured from the initial rotation (usually <0,0,0>) of the 
// manipulator.
// 
// The manipulator can be configured either to display with an object (which 
// must be a DAG node) or to display at an arbitrary point using the
// rotationCenter manipVal.
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

// CLASS DECLARATION (MFnRotateManip)

/// RotateManip function set (OpenMayaUI)
/**
	MFnRotateManip is the function set for the Maya arcball rotation 
	manipulator.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MFnRotateManip : public MFnManip3D
{
	declareDagMFn(MFnRotateManip, MFnManip3D);

public:
	///
	enum RotateMode {
		/// Object Space (default)
		kObjectSpace = 0,
		/// World Space
		kWorldSpace,
		/// Gimbal
		kGimbal
	};

	///
	MObject		create(MStatus *ReturnStatus = NULL);
	///
	MObject		create(const MString &manipName,
					   const MString &rotationName,
					   MStatus *ReturnStatus = NULL);
	///
	MStatus		connectToRotationPlug(const MPlug &rotationPlug);
	///
	MStatus		connectToRotationCenterPlug(const MPlug &rotationCenterPlug);
	///
	MStatus		setInitialRotation(const MEulerRotation& rotation);
	///
	MStatus		displayWithNode(const MObject& node);
	///
	MStatus		setRotateMode(RotateMode mode);
	///
	RotateMode	rotateMode() const;
	///
	MStatus		setSnapMode(bool snapEnabled);
	///
	bool		isSnapModeOn() const;
	///
	MStatus		setSnapIncrement(double snapInc);
	///
	double		snapIncrement() const;
	///
	unsigned	rotationIndex(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	rotationCenterIndex(MStatus *ReturnStatus = NULL) const;

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
#endif /* _MFnRotateManip */
