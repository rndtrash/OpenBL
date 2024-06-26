
#ifndef _MDeviceChannel
#define _MDeviceChannel
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
// CLASS:    MDeviceChannel
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MDeviceChannel)
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>

// *****************************************************************************

// DECLARATIONS

class MDeviceState;
class MString;

// *****************************************************************************

// CLASS DECLARATION (MDeviceChannel)

/// Input device channel (OpenMayaUI)
/**


*/

#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MDeviceChannel
{
public:
	///
	MDeviceChannel( const MString&, MDeviceChannel* = NULL, int = -1 );

	///
	~MDeviceChannel();


	///
	MString 			name() const;
	///
	MString 			longName() const;

	///
	int				axisIndex() const;

	///
	bool     			hasChildren() const;
	///
	MDeviceChannel	 	parent() const;
	///
	MDeviceChannel 		childByIndex( int );
	///
	int					numChildren() const;

protected:
	// no protected members

private:
	friend class MPxMidiInputDevice;
	MDeviceChannel( void * );
	void * data;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MDeviceChannel */
