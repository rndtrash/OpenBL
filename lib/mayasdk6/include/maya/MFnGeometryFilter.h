
#ifndef _MFnGeometryFilter
#define _MFnGeometryFilter
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
// CLASS:    MFnGeometryFilter
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnGeometryFilter)
//
// MFnGeometryFilter is the function set for geometry filters (deformers)
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
// *****************************************************************************

// DECLARATIONS
class MObject;
class MDagPath;
class MIntArray;
class MFloatArray;
class MObjectArray;
class MSelectionList;
class MString;

// *****************************************************************************

// CLASS DECLARATION (MFnGeometryFilter)

///  geometry filter function set (OpenMayaAnim)
/**
MFnGeometryFilter is the function set for deformers.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnGeometryFilter : public MFnDependencyNode
{
	declareMFn(MFnGeometryFilter, MFnDependencyNode);

public:
	///
	MStatus		getInputGeometry(MObjectArray &objects) const;
	///
	MStatus		getOutputGeometry(MObjectArray &objects) const;
	///
	MObject		inputShapeAtIndex(unsigned index,
								  MStatus *ReturnStatus = NULL) const;
	///
	MObject		outputShapeAtIndex(unsigned index,
								   MStatus *ReturnStatus = NULL) const;
	///
	unsigned	indexForOutputShape(const MObject &shape,
									MStatus *ReturnStatus = NULL) const;
	///
	MStatus		getPathAtIndex(unsigned index,
							   MDagPath &path) const;
	///
	unsigned	indexForGroupId(unsigned groupId,
								MStatus *ReturnStatus = NULL) const;
	///
	unsigned	groupIdAtIndex(unsigned index,
							   MStatus *ReturnStatus = NULL) const;
	///
	unsigned	numOutputConnections(MStatus *ReturnStatus = NULL) const;
	///
	unsigned	indexForOutputConnection(unsigned connectionIndex,
										 MStatus *ReturnStatus = NULL) const;
	///
	MObject		deformerSet(MStatus *ReturnStatus = NULL) const;
	///
	float		envelope(MStatus *ReturnStatus = NULL) const;
	///
	MStatus		setEnvelope(float envelope);

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
#endif /* _MFnGeometryFilter */
