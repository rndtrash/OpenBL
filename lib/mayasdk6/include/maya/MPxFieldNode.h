
#ifndef _MPxFieldNode
#define _MPxFieldNode
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
// CLASS:    MPxFieldNode
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MPxFieldNode)
//
// MPxFieldNode allows the creation and manipulation of dependency graph nodes
// representing fields.  This is the top level of a hierarchy of field
// node function sets.  It permits manipulation of the attributes common to
// all types of fields.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#if defined(OSMac_MachO_)
#	include <OpenGL/gl.h>
#else
#	include <GL/gl.h>
#endif

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MPxFieldNode)

/// Base class for user defined fields (OpenMayaFX)
/**
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYAFX_EXPORT MPxFieldNode : public MPxNode
{
public:
	///
	MPxFieldNode();
	///
	virtual ~MPxFieldNode();
	///
	virtual MPxNode::Type type() const;
	///
	virtual MStatus		compute( const MPlug& plug, MDataBlock& dataBlock );
	///
    virtual MStatus getForceAtPoint(const MVectorArray&  point,
                            const MVectorArray&  velocity,
                            const MDoubleArray&  mass,
                            MVectorArray&        force,
                            double deltaTime);
	///
	virtual MStatus iconSizeAndOrigin(	GLuint& width,
						GLuint& height,
						GLuint& xbo,
						GLuint& ybo   );
	///
	virtual MStatus iconBitmap(GLubyte* bitmap);

	/////////////////////////
	// Inherited attributes
	/////////////////////////

	// General attributes for every field.
	//
	/// magnitude attribute
	static MObject	mMagnitude;
	/// attenuation attribute
	static MObject	mAttenuation;
	/// maximum distance attribute
	static MObject	mMaxDistance;
	/// flag for maximum distance attribute
	static MObject	mUseMaxDistance;
	/// flag for per vertex attribute
	static MObject	mApplyPerVertex;

	// Attributes for input force data.
	//
	/// input attribute compound, multi
	static MObject	mInputData;
	/// child attribute, multi
	static MObject	mInputPositions;
	/// child attribute, multi
	static MObject	mInputVelocities;
	/// child attribute, multi
	static MObject	mInputMass;
	/// child attribute
	static MObject	mDeltaTime;

	// Attribute for input forces.
	//
	/// forces input attribute, multi
	static MObject	mInputForce;

	// Attribute for output forces.
	//
	/// forces output attribute, multi
	static MObject	mOutputForce;

	// Attributes for ownership.
	//
	/// X component of mOwnerCentroid
	static MObject	mOwnerCentroidX;
	/// Y component of mOwnerCentroid
	static MObject	mOwnerCentroidY;
	/// Z component of mOwnerCentroid
	static MObject	mOwnerCentroidZ;
	/// owner centroid attribute
	static MObject	mOwnerCentroid;
	/// owner position attribute, multi
	static MObject	mOwnerPosData;
	/// owner velocity attribute, multi
	static MObject	mOwnerVelData;

	// Attribute for world position.
	//
	/// world matrix attribute
	static MObject	mWorldMatrix;

	/// Attribute for input pp data from particleShape
	static MObject  mInputPPData;

	/// Attribute for input pp data from owner particleShape
	static MObject  mOwnerPPData;

protected:
// No protected members

private:
// No private members

	static void			initialSetup();
	static const char*	className();


};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32


// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MPxFieldNode */
