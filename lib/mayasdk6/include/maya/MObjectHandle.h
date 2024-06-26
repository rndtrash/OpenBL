#ifndef _MObjectHandle
#define _MObjectHandle
//
// *****************************************************************************
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
// CLASS:    MObjectHandle
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MObjectHandle)
//
//  MObjectHandle is a wrapper class for the MObject class. An MObjectHandle
//	will provide a user with added information on the validity of an MObject.
//	Each MObjectHandle that is created registers an entry into a table to
//	maintain state tracking of the MObject that the handle was created for.
//	This will help users track when an MObject is invalid and should be
//	re-retrieved.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES

#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

////////////////////////////////////////////////////////////////////////////////

// CLASS DECLARATION (MObjectHandle)

/// Generic Class for validating MObjects

class OPENMAYA_EXPORT MObjectHandle
{
public:
	///
	MObjectHandle(); 
	///
	MObjectHandle( const MObject& object );
	/// 
	MObjectHandle( const MObjectHandle &handle ); 
	///
	~MObjectHandle();
	///
	MObject			object() const;
	///
	const MObject & objectRef() const;
	///
	bool			isValid() const;
	///
	bool			isAlive() const;
	
	///
	bool			operator==(const MObject &object) const;
	///
	bool			operator==(const MObjectHandle &object) const;
	///
	bool			operator!=(const MObject &object) const; 
	///
	bool			operator!=(const MObjectHandle &object) const;
	
	/// 
	MObjectHandle & operator=(const MObject &object );
	/// 
	MObjectHandle & operator=(const MObjectHandle &other );

private:

	// Internal object registration methods
	void			registerObject();
	void			deregisterObject();
	
	// Internal destructor callback
	static void		registerDestroyedCallback();
	static void		deregisterDestroyedCallback();
	static bool		fIsCallbackInitialized;
	static void		destroyedCallback(void*);

	MObject			fObject;
	bool			fIsAlive;
	bool			fIsDependNode;
	static int		fHandleCount;
};

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MObjectHandle */

