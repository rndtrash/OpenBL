#ifndef _MQuaternion
#define _MQuaternion
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
// CLASS:    MQuaternion
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MQuaternion)
//
//	Quaternions can be used to specify orientations and rotations of 3-D
//	objects relative to a starting reference, similar to the way that cartesian
//	vectors can be used to specify positions and translations of 3-D objects
//	relative to an origin.  Quaternions represent orientations as a single
//	rotation, just as rectangular co-ordinates represent position as a single
//	vector.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include <maya/MTypes.h>
#include <maya/MTransformationMatrix.h>

// *****************************************************************************

// DECLARATIONS

class MMatrix;
class MEulerRotation;

#define kQuaternionEpsilon 1.0e-10

// *****************************************************************************

// CLASS DECLARATION (MQuaternion)

/// Quaternion math
/**
This class provides methods for working with Quaternions.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MQuaternion  
{
public:
	///
					MQuaternion();
	///
					MQuaternion(const MQuaternion &src);
	///
					MQuaternion(double xx, double yy, double zz, double ww);
	///
					MQuaternion(const double[4]);
	///
					MQuaternion(const MVector &a, const MVector &b);
	///
					MQuaternion(const MVector &a, const MVector &b, 
								double angleFactor);
	///
					MQuaternion(double angle, const MVector &axisb);
	///
					~MQuaternion();
	///
 	MQuaternion &	operator=(const MQuaternion& src);
	///
	MQuaternion &	operator=(const MMatrix &matrix);
	///
	MQuaternion &	operator=(const MEulerRotation &matrix);
	///
	operator		MMatrix() const;
	///
	MMatrix			asMatrix() const;
	///
	MEulerRotation	asEulerRotation() const;
	///
	MQuaternion &	setAxisAngle(const MVector &axis, double theta);
	///
	bool			getAxisAngle(MVector &axis, double &theta) const;
	///
	MQuaternion &	setToXAxis(double theta);
	///
	MQuaternion &	setToYAxis(double theta);
	///
	MQuaternion &	setToZAxis(double theta);
	///
	void			get(double[4]) const;
	///
	double &		operator[](unsigned i);
	///
	double			operator[](unsigned i) const;
	///
 	MQuaternion   	operator+(const MQuaternion &other) const;
	///
 	MQuaternion   	operator*(const MQuaternion &other) const;
	///
 	MQuaternion & 	operator*=(const MQuaternion &rhs);
	///
 	friend OPENMAYA_EXPORT MQuaternion operator*(double scale, 
												 const MQuaternion &other);
	///
 	MQuaternion   	operator-(const MQuaternion &other) const;
	///
 	MQuaternion   	operator-() const;
	///
	MQuaternion &	negateIt();
	///
 	bool           	operator==(const MQuaternion &other) const;
	///
 	bool          	operator!=(const MQuaternion &other) const;
	///
 	bool          	isEquivalent(const MQuaternion &other, 
								 double tolerance = kQuaternionEpsilon) const;
	///
	MQuaternion &	scaleIt(double scale);
	///
	MQuaternion 	normal() const;
	///
	MQuaternion &	normalizeIt();
	///
	MQuaternion 	conjugate() const;
	///
	MQuaternion &	conjugateIt();
	///
	MQuaternion 	inverse() const;
	///
	MQuaternion &	invertIt();
	///
	MQuaternion		log() const;
	///
	MQuaternion		exp() const;
	
	/// The multiplicative identity.
	static const MQuaternion identity;

	///
	friend OPENMAYA_EXPORT MQuaternion slerp(const MQuaternion &p,
											 const MQuaternion &q,
											 double t);
	///
	friend OPENMAYA_EXPORT MQuaternion slerp(const MQuaternion &p,
											 const MQuaternion &q,
											 double t,
											 short spin);
	///
	friend OPENMAYA_EXPORT MQuaternion squad(const MQuaternion &p,
											 const MQuaternion &a,
											 const MQuaternion &b,
											 const MQuaternion &q,
											 double t);
	///
	friend OPENMAYA_EXPORT MQuaternion squad(const MQuaternion &p,
											 const MQuaternion &a,
											 const MQuaternion &b,
											 const MQuaternion &q,
											 double t, short spin);
	///
	friend OPENMAYA_EXPORT MQuaternion squadPt(const MQuaternion &q0,
											   const MQuaternion &q1,
											   const MQuaternion &q2);
	///
	friend OPENMAYA_EXPORT IOS_REF(ostream) &operator<<(IOS_REF(ostream) &os, 
											   const MQuaternion &q);

	/// The quaternion's imaginary x component.
	double x;

	/// The quaternion's imaginary y component.
	double y;

	/// The quaternion's imaginary z component.
	double z;

	/// The quaternion's real component.
	double w;

protected:
	// No protected members

private:
	// No private members
};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MQuaternion */
