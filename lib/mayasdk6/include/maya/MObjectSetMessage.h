#ifndef _MObjectSetMessage
#define _MObjectSetMessage
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
// *****************************************************************************
//
// CLASS:    MObjectSetMessage
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MObjectSetMessage)
//
// This class is used to register callbacks with set specific event types.
// Using the addCallback() methods, a function can be registered that will
// be executed whenever the callback specific message/event occurs.
//
// To remove a callback, use MMessage::removeCallback(). All callbacks that
// are registered by a plug-in must be removed by that plug-in when it is
// unloaded. Failure to do so will result in a fatal error.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MMessage.h>

// *****************************************************************************

// DECLARATIONS

class MObject;

// *****************************************************************************

// CLASS DECLARATION (MObjectSetMessage)

/// Object Set Messages
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MObjectSetMessage : public MMessage
{ 
public:
	///
	static MCallbackId	addSetMembersModifiedCallback(
								MObject& node,
								void (*func)( MObject& node,
											  void* clientData ),
								void * clientData = NULL,
								MStatus * ReturnStatus = NULL );

private:
	static const char* className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************

#endif /* __cplusplus */
#endif /* _MObjectSetMessage */
