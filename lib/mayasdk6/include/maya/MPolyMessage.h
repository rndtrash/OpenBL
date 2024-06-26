
#ifndef _MPolyMessage
#define _MPolyMessage
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
// CLASS:    MPolyMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPolyMessage)
//
// This class is used to register callbacks for poly messages.  Currently,
// you can use this class to listen to component id changes for vertices,
// edges and faces.
//
// To remove a callback use MMessage::removeCallback.
// All callbacks that are registered by a plug-in must be removed by that
// plug-in when it is unloaded. Failure to do so will result in a fatal error.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MMessage.h>
#include <maya/MString.h>

// *****************************************************************************

// DECLARATIONS

class MUintArray;
class MObject;

// *****************************************************************************

// CLASS DECLARATION (MPolyMessage)

/// Polygon component id change messages (OpenMaya)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MPolyMessage : public MMessage
{
public:
	enum
	{
		///
		kVertexIndex = 0,
		///
		kEdgeIndex = 1,
		///
		kFaceIndex = 2,
		///
		kLastErrorIndex = 3
	};
	
	///
	static MCallbackId	addPolyComponentIdChangedCallback (
								MObject& node,
								bool wantIdModifications[],
								unsigned int count,
								void (*func)(MUintArray componentIds[],
												unsigned int count,
												void *clientData),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL);
	///								
	static unsigned int deletedId( void );

private:
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MPolyMessage */
