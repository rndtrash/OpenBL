#ifndef _MLightLinks
#define _MLightLinks
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
// CLASS:    MLightLinks
//
// ***************************************************************************
//
// CLASS DESCRIPTION (MLightLinks)
//
// MLightLinks is a class which provides access to Maya's light linking 
// information.  It parses the information expressed by the light linker nodes
// in the scene, and provides a simple queryable interface for it.
//
// ***************************************************************************

#if defined __cplusplus

// ***************************************************************************
// INCLUDED HEADER FILES

#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MIOStream.h>

// ***************************************************************************
// CLASS DECLARATION (MLightLinks)

/// Class providing read-only Light Linking API functions (OpenMayaRender)
/**
*/
class OPENMAYARENDER_EXPORT MLightLinks
{
public:

	///
	MLightLinks();

	///
	~MLightLinks();

	///
	MStatus parseLinks( const MObject& linkNode = MObject::kNullObj, 
						bool verbose = false, 
						IOS_REF(ostream) *stream = NULL );

	///
	MStatus getLinkedLights( const MDagPath& path, 
							 const MObject& component, 
							 MDagPathArray& lights );

	///
	MStatus getLinkedObjects( const MDagPath& light, 
							  MSelectionList& objects );

	///
	MStatus getIgnoredLights( const MDagPath& path, 
							  const MObject& component, 
							  MDagPathArray& lights );

	///
	MStatus getIgnoredObjects( const MDagPath& light, 
							   MSelectionList& objects );

private:
	void *f_linkUtil;
};


// ***************************************************************************
#endif /* __cplusplus */
#endif /* _MLightLinks */
