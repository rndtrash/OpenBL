
#ifndef _MIkHandleGroup
#define _MIkHandleGroup
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
// CLASS:    MIkHandleGroup
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MIkHandleGroup)
//
//
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MIkHandleGroup)

/// IK handle groups (OpenMayaAnim)
/**
  Ik handle group class.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MIkHandleGroup
{
public:
 	///
					MIkHandleGroup();
 	///
					~MIkHandleGroup();

	///
	int			priority(MStatus *ReturnStatus = NULL) const;
	///
	int			solverID(MStatus *ReturnStatus = NULL) const;
	///
	int			solverPriority(MStatus *ReturnStatus = NULL) const;
	///
	MStatus			setPriority( int );
	///
	MStatus			setSolverID( int );
	///
	bool			checkEffectorAtGoal(MStatus *ReturnStatus = NULL);
	///
	MStatus			solve();


	///
	int 			dofCount(MStatus *ReturnStatus = NULL) const;
	///
	int				handleCount(MStatus *ReturnStatus = NULL) const;
	///
	MObject			handle( int ith, MStatus *ReturnStatus = NULL );



protected:
// No protected members

private:
	friend class MPxIkSolver;
	friend class MPxIkSolverNode;
	static const char* className();
	MIkHandleGroup( void * );
 	void * f_data;
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MIkHandleGroup */
