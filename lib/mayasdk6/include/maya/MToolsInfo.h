
#ifndef _MToolsInfo
#define _MToolsInfo
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
// CLASS:    MToolsInfo
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MToolsInfo)
//
// MToolsInfo is a caretaker class used to keep track of
// the dirty state of the current tool property sheet.
// The tool writer should make sure to call the
// setDirtyFlag method when any of the values are
// modified.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MTypes.h>

// *****************************************************************************

// DECLARATIONS

class MStatus;
class MPxContext;

// CLASS DECLARATION (MToolsInfo)

/// Tool information (OpenMayaUI)
/**
MToolsInfo provides methods for keeping track of the
state of the current tool property sheet.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32


class OPENMAYAUI_EXPORT MToolsInfo {
public:
	///
	static void		setDirtyFlag(const MPxContext &context);
	///
	static void		resetDirtyFlag();
	///
	static bool		isDirty();

private:
	static const char *className();
};


#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MToolsInfo */
