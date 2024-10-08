
#ifndef _MFnWeightGeometryFilter
#define _MFnWeightGeometryFilter
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
// CLASS:    MFnWeightGeometryFilter
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnWeightGeometryFilter)
//
// MFnWeightGeometryFilter is the function set for weight geometry filters.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnGeometryFilter.h>
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

// CLASS DECLARATION (MFnWeightGeometryFilter)

/// Weight geometry filter function set (OpenMayaAnim)
/**
MFnWeightGeometryFilter is the function set for editing the weights of
clusters, cluster flexors, and user-defined deformers derived from
MPxDeformerNode.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnWeightGeometryFilter : public MFnGeometryFilter
{
	declareMFn(MFnWeightGeometryFilter, MFnGeometryFilter);

public:
	///
	MStatus		getWeights(unsigned index,
						   const MObject &components,
						   MFloatArray &weights) const;
	///
	MStatus		getWeights(const MDagPath &path,
						   const MObject &components,
						   MFloatArray &weights) const;
	///
	MStatus		setWeight(const MDagPath &path,
						  unsigned index,
						  const MObject &components,
						  float weight,
						  MFloatArray *oldValues = NULL);
	///
	MStatus		setWeight(const MDagPath &path,
						  const MObject &components,
						  float weight,
						  MFloatArray *oldValues = NULL);
	///
	MStatus		setWeight(const MDagPath &path,
						  unsigned index,
						  const MObject &components,
						  MFloatArray &values);
	///
	MStatus		setWeight(const MDagPath &path,
						  const MObject &components,
						  MFloatArray &values);
	///
	MStatus		getWeightPlugStrings(const MSelectionList &list,
									 MString &plugStrings) const;
	///
	MStatus		getWeightPlugStrings(const MSelectionList &list,
									 MStringArray &plugStringArray) const;

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
#endif /* _MFnWeightGeometryFilter */
