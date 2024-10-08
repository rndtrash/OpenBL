
#ifndef _MAnimCurveClipboardItem
#define _MAnimCurveClipboardItem

//
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
// CLASS:    MAnimCurveClipboardItem
//
//
//******************************************************************************
//
// CLASS DESCRIPTION (MAnimCurveClipboardItem)
//
//  This class provides a wrapper to the clipboard item used to hold
//  on to cut/copy/paste information
//
//******************************************************************************

#if defined __cplusplus

//******************************************************************************
// INCLUDED HEADER HILES
//******************************************************************************


#include <maya/MFnAnimCurve.h>
#include <maya/MObject.h>


//******************************************************************************
// DECLARATIONS
//******************************************************************************

//	CLASS DECLARATION (MAnimCurveClipboardItem)

///	Wrapper for a clipboard item. (OpenMayaAnim)
/**
	This class provides a wrapper to the clipboard item used to hold
	on to cut/copy/paste information
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MAnimCurveClipboardItem
{
public:
	///
					MAnimCurveClipboardItem();
	///
					MAnimCurveClipboardItem( const MAnimCurveClipboardItem & );
	///
					~MAnimCurveClipboardItem();

	///
	const MObject		animCurve( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus				getAddressingInfo( unsigned &rowCount,
										   unsigned &childCount,
										   unsigned &attrCount) const;
	///
	const MString &		fullAttributeName( MStatus * ReturnStatus=NULL ) const;
	///
	const MString &		leafAttributeName( MStatus * ReturnStatus=NULL ) const;
	///
	const MString &		nodeName( MStatus * ReturnStatus=NULL ) const;
	///
	MFnAnimCurve::AnimCurveType	animCurveType( MStatus
												* ReturnStatus=NULL ) const;
	///
	MStatus				setAnimCurve( const MObject & curve );
	///
	MStatus				setAddressingInfo( unsigned rowCount,
										   unsigned childCount,
										   unsigned attributeCount );
	///
	MStatus				setNameInfo( const MString & nodeName,
									 const MString & fullName,
									 const MString & leafName );
	///
	MAnimCurveClipboardItem &operator =  (const MAnimCurveClipboardItem &);
	///
	bool	 				operator == (const MAnimCurveClipboardItem &) const;

protected:
// No protected members

private:
	void *							fCurve;
	MFnAnimCurve::AnimCurveType		fAnimCurveType;

	unsigned						fRowCount;
	unsigned						fChildCount;
	unsigned						fAttrCount;
	MString							fFullAttrName;
	MString							fLeafAttrName;
	MString							fNodeName;
	static const char*				className();
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

//******************************************************************************
#endif /* __cplusplus */
#endif /* _MAnimCurveClipboardItem */
