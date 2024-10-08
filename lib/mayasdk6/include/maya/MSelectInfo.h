
#ifndef _MSelectInfo
#define _MSelectInfo
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
// CLASS:    MSelectInfo
//
//   MSelectInfo is used with user defined shape selection and is passed
//   as an argument to the MPxSurfaceShapeUI::select method.
//   This class encapsulates all the selection state information for
//   selected objects.
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MSelectInfo)
//
// *****************************************************************************
//
#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MDrawInfo.h>

// *****************************************************************************

// DECLARATIONS

class MSelectionMask;
class MPoint;
class MPointArray;
class MVector;
class MSelectionList;
class MMatrix;

// *****************************************************************************

// CLASS DECLARATION (MSelectInfo)

/// Selection state information used in MPxSurfaceShapeUI::select (OpenMayaUI)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MSelectInfo : public MDrawInfo
{
public:
	///
	MSelectInfo();
	///
	MSelectInfo( const MSelectInfo& in );
	///
	~MSelectInfo();

public:

	// Public accessor methods

	///
    M3dView			view();

    ///
	bool			singleSelection() const;
    ///
	bool			selectClosest() const;
	///
    bool			selectable( MSelectionMask & mask ) const;
	///
    bool			selectableComponent( bool displayed,
										 MSelectionMask & mask ) const;

	///
    bool			isRay() const;

	MMatrix			getAlignmentMatrix() const;
	///
    void			getLocalRay( MPoint&, MVector & ) const;

	///
    bool			selectForHilite( const MSelectionMask & ) const;

	///
    bool			selectOnHilitedOnly() const;

    ///
	int				highestPriority() const;
    ///
	void			setHighestPriority( int );

	///
    void			addSelection( const MSelectionList &item,
								  const MPoint &point,
								  MSelectionList &list,
								  MPointArray &points,
								  const MSelectionMask & mask,
								  bool isComponent );

	///
    MDagPath		selectPath() const;


protected:
// No protected members

private:
	const char*	 className() const;
    MSelectInfo( void* in );
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MSelectInfo */
