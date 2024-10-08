
#ifndef _MMessage
#define _MMessage
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
// CLASS:    MMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MMessage)
//
// This is the base class for message callbacks.
// 
// There are three types of messages in maya that are exposed through the
// API and they are the following:
// scene messages, dependency graph messages, and dependency node messages.
// can be registered for these messages using the addCallback
// methods of the MSceneMessage, MDGMessage, and MNodeMessage classes.
// 
// When a callback is added a number or id is returned. This id is used to
// keep track of the callback and is necessary for removing it.
// A callback id with the value 'NULL' represents an invalid callback.
// Use the removeCallback member function of this class for removing a
// callback.
//
// It is the user's responsibility to keep track of the callback id's
// and remove all callbacks for a plug-in when it is unloaded.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

typedef unsigned int MCallbackId;

// Declaration for the callback table
typedef struct MMessageNode {
    void*           fClientPtr;
	void*			fServerPtr;
	void*           fSubClientPtr;
	MCallbackId		fId;
	bool			isValid;	// Unused
} * MMessageNodePtr;

// *****************************************************************************

// CLASS DECLARATION (MMessage)

class MIntArray;
class MObject;

/// Message base class
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MMessage
{ 
public:	
	///
    static MStatus  removeCallback( MCallbackId id );
	///
    static MStatus  removeCallbacks( MIntArray &ids );
	///
	static MCallbackId	currentCallbackId( MStatus* ReturnStatus = NULL );
	///
	static MStatus	nodeCallbacks( MObject& node, MIntArray& ids );

protected:
    static void addNode( MMessageNodePtr node ); 
    static void removeNode( MMessageNodePtr node );

private:
	static const char* 		className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MMessage */
