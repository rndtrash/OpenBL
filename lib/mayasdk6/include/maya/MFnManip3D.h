
#ifndef _MFnManip3D
#define _MFnManip3D
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
// CLASS:    MFnManip3D
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnManip3D)
//
// Function set for 3D manipulators.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnTransform.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MFnManip3D)

/// 3D manipulator function set (OpenMayaUI)
/**
MFnManip3D is the function set for 3D manipulators.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32


class OPENMAYAUI_EXPORT MFnManip3D : public MFnTransform
{
	declareDagMFn(MFnManip3D, MFnTransform);
public:
	///
	bool 			isVisible(MStatus *ReturnStatus) const;
	///
	MStatus			setVisible(bool isVisible);
	///
	float			manipScale(MStatus *ReturnStatus) const;
	///
	MStatus			setManipScale(float size);
	///
	bool			isOptimizePlaybackOn(MStatus *ReturnStatus) const;
	///
	MStatus			setOptimizePlayback(bool optimizePlayback);
	///
	static float	globalSize();
	///
	static void		setGlobalSize(float size);
	///
	static float	handleSize();
	///
	static void		setHandleSize(float size);
	///
	static float	lineSize();
	///
	static void		setLineSize(float size);
	///
	static MStatus	deleteManipulator(MObject& manip);

protected:

private:
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnManip3D */
