
#ifndef _MManipData
#define _MManipData
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
// CLASS:    MManipData
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MManipData)
//
//  This class encapulates manipulator data which is returned from the
//  manipulator conversion functions.
//
// *****************************************************************************
//
#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MManipData)

/// Manipulator Data (OpenMayaUI)
/**
This class encapulates manipulator data which is returned from the
manipulator conversion functions.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAUI_EXPORT MManipData
{
public:
	///
	MManipData();
	///
	~MManipData();
	///
	MManipData(const MManipData &);
	///
	MManipData(bool);
	///
	MManipData(short);
	///
	MManipData(int);
	///
	MManipData(unsigned);
	///
	MManipData(float);
	///
	MManipData(double);
	///
	MManipData(const MObject &);

	///
	MManipData &	operator=(const MManipData &);
	///
	MManipData &	operator=(bool);
	///
	MManipData &	operator=(short);
	///
	MManipData &	operator=(int);
	///
	MManipData &	operator=(unsigned);
	///
	MManipData &	operator=(float);
	///
	MManipData &	operator=(double);
	///
	MManipData &	operator=(const MObject &);

	///
	bool			isSimple()		const;

	///
	bool			asBool()		const;
	///
	short			asShort()		const;
	///
	int			asLong()		const;
	///
	unsigned		asUnsigned()	const;
	///
	float			asFloat()		const;
	///
	double			asDouble()		const;
	///
	MObject			asMObject()		const;


private:
	const char *className() const;
	bool		fIsSimple;
	double		fSimpleData;
	MObject		fComplexData;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MManipData */
