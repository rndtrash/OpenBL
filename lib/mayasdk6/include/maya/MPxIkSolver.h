
#ifndef _MPxIkSolver
#define _MPxIkSolver
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
// CLASS:    MPxIkSolver
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxIkSolver)
//
//  The base class for user defined ik solvers.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MStatus.h>
#include <maya/MTypes.h>
#include <maya/MObject.h>

// *****************************************************************************

// DECLARATIONS

class MString;
class MArgList;
class MIkHandleGroup;
class MMatrix;
class MDoubleArray;

// *****************************************************************************

// CLASS DECLARATION (MPxIkSolver)

/// OBSOLETE CLASS: Base class for user defined IK solvers  (OpenMayaAnim)
/**

MPxIkSolver is OBSOLETE, and will be removed in a future version of Maya.
The MPxIkSolver class has been replaced with the MPxIkSolverNode class.
This is an obsolete base class for writing user-defined IK solvers.

*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAANIM_EXPORT MPxIkSolver
{
public:
	///
	virtual ~MPxIkSolver();

	///
	static void			registerSolver( const MString & solverName,
								MCreatorFunction creatorFunction );


	///
	virtual MStatus		preSolve();
	///
	virtual MStatus		doSolve();
	///
	virtual MStatus		postSolve( MStatus );


	// These methods MUST be overridden by the user.
	//
	///
	virtual MString		solverTypeName() const;
	///
	virtual bool		isSingleChainOnly() const;
	///
	virtual bool		isPositionOnly() const;
	///
	virtual bool		hasJointLimitSupport() const;
	///
	virtual bool		hasUniqueSolution() const;
	///
	virtual bool		groupHandlesByTopology() const;


	///
	virtual MStatus		setFuncValueTolerance( double tolerance );
	///
	virtual MStatus		setMaxIterations( int value );


	///
	MIkHandleGroup * 	handleGroup() const;
	///
	virtual void 		setHandleGroup( MIkHandleGroup* );
	///
	const MMatrix *		toWorldSpace() const;
	///
	const MMatrix *		toSolverSpace() const;
	///
	double 				funcValueTolerance() const;
	///
	int 				maxIterations() const;
	///
	virtual void 		snapHandle( MObject& handle );

	void				create();

protected:

	///
	MStatus				getJointAngles( MDoubleArray& ) const;
	///
	MStatus				setJointAngles( const MDoubleArray& );
	///
	void				setToRestAngles();

	///
	MPxIkSolver();

	virtual const char*	className() const;

private:
	void*	instance;

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxIkSolver */
