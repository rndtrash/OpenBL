
#ifndef _MFnClip
#define _MFnClip
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
// CLASS:    MFnClip
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnClip)
//
// MFnClip is the function set for clips.
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
class MTime;
class MDGModifier;

// *****************************************************************************

// CLASS DECLARATION (MFnClip)

/// clip function set (OpenMayaAnim)
/**
MFnClip is the function set for creating, querying and
editing animation clips.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MFnClip : public MFnDependencyNode
{
	declareMFn(MFnClip, MFnDependencyNode);

public:
	///
	MObject		createSourceClip ( const MTime& sourceStart,
								   const MTime& sourceDuration,
								   MDGModifier& dgMod,
								   MStatus * ReturnStatus = NULL );
	///
	MObject		createInstancedClip ( MObject& sourceClip,
									  const MTime& start,
									  MDGModifier& dgMod,
									  MStatus * ReturnStatus = NULL,
									  bool absolute = 1,
									  double postCycle = 0.0,
									  double weight = 1.0,
									  double scale = 1.0,
			  						  double preCycle = 0.0 );
	///
	bool 		isInstancedClip(MStatus *ReturnStatus = NULL);
	///
	bool		isPose(MStatus *ReturnStatus = NULL);
	///
	MObject 	sourceClip(MStatus *ReturnStatus = NULL);
	///
	double		getPreCycle(MStatus *ReturnStatus = NULL);
	///
	double		getPostCycle(MStatus *ReturnStatus = NULL);
	///
	double		getCycle(MStatus *ReturnStatus = NULL);
	///
	double		getWeight(MStatus *ReturnStatus = NULL);
	///
	double 		getScale(MStatus *ReturnStatus = NULL);
	///
	bool 		getAbsolute(MStatus *ReturnStatus = NULL);
	///
	bool 		getEnabled(MStatus *ReturnStatus = NULL);
	///
	MTime		getStartFrame(MStatus *ReturnStatus = NULL);
	///
	MTime		getSourceStart(MStatus *ReturnStatus = NULL);
	///
	MTime		getSourceDuration(MStatus *ReturnStatus = NULL);
	///
	MStatus		getMemberAnimCurves(MObjectArray& curves,
									MPlugArray& associatedAttrs);
	///
	MStatus		setPoseClip(bool state, MDGModifier* mod = NULL);
	///
	MStatus		setPreCycle(double cycle, MDGModifier* mod = NULL);
	///
	MStatus		setPostCycle(double cycle, MDGModifier* mod = NULL);
	///
	MStatus		setCycle(double cycle, MDGModifier* mod = NULL);
	///
	MStatus		setWeight(double wt, MDGModifier* mod = NULL);
	///
	MStatus		setScale(double scale, MDGModifier* mod = NULL);
	///
	MStatus		setAbsolute(bool abs, MDGModifier* mod = NULL);
	///
	MStatus		setEnabled(bool val, MDGModifier* mod = NULL);
	///
	MStatus		setStartFrame(const MTime& start, MDGModifier* mod = NULL);
	///
	MStatus		setSourceData(const MTime& start,const MTime& duration,
							  MDGModifier* mod = NULL);

protected:
// No protected members

private:

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MFnClip */
