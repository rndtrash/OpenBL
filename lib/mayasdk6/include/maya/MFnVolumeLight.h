
#ifndef _MFnVolumeLight
#define _MFnVolumeLight
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
// CLASS:    MFnVolumeLight
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnVolumeLight)
//
// MFnVolumeLight facilitates creation and manipulation of dependency graph nodes
// representing volume lights.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnPointLight.h>


// *****************************************************************************
 
// DECLARATIONS
 
class MRampAttribute;

// *****************************************************************************

// CLASS DECLARATION (MFnVolumeLight)

/// Manage volume Light dependency Nodes
/**
  Facilitate the creation and manipulation of volume light nodes
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnVolumeLight : public MFnPointLight
{

	declareDagMFn(MFnVolumeLight, MFnPointLight);
public:
	///
	enum MLightShape 
	{
	///
		kBoxVolume,
	///
		kSphereVolume,
	///
		kCylinderVolume,
	///
		kConeVolume
	};
	///
	enum MLightDirection
	{
	///
		kOutward,
	///
		kInward,
	///
		kDownAxis
	};
			
	///
	MObject     create( bool UIvisible = true, MStatus * ReturnStatus = NULL );
	///
	MObject     create( const MObject& parent, bool UIvisible = true,
						MStatus * ReturnStatus = NULL );
	///
	MFnVolumeLight::MLightShape  lightShape ( MStatus * ReturnStatus ) const;
	///
	MStatus     setLightShape ( const MFnVolumeLight::MLightShape& light_shape);
	///
	MFnVolumeLight::MLightDirection volumeLightDirection (MStatus * ReturnStatus) const;
	///
	MStatus		setVolumeLightDirection (const MFnVolumeLight::MLightDirection& volume_light_direction);
	///
	float		arc ( MStatus * ReturnStatus ) const;
	///
	MStatus		setArc (const float& arc);
	///
	float		coneEndRadius ( MStatus * ReturnStatus ) const;
	///
	MStatus		setConeEndRadius (const float& cone_end_radius);
	///
	bool		emitAmbient ( MStatus * ReturnStatus ) const;
	///
	MStatus		setEmitAmbient (const bool& emit_ambient);
	///
	MRampAttribute colorRamp (MStatus * ReturnStatus) const ;
	///
	MRampAttribute penumbraRamp (MStatus * ReturnStatus) const;

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
#endif /* _MFnVolumeLight */



