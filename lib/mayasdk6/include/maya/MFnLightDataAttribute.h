
#ifndef _MFnLightDataAttribute
#define _MFnLightDataAttribute
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
// CLASS:    MFnLightDataAttribute
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnLightDataAttribute)
//
// Function object for LightData attributes.  A LightData attribute describes
// a single light source, giving its direction and intensity, as well as
// specifying whether or not it contributes to the ambient, diffuse, and
// specular components of the shading model.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MFnAttribute.h>

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

class MString;

// CLASS DECLARATION (MFnLightDataAttribute)

/// Light data attribute function set
/**
  Function set for light data attributes.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnLightDataAttribute : public MFnAttribute 
{

	declareMFn(MFnLightDataAttribute, MFnAttribute);

public:
	///
	MObject    create( const MString& fullName,
					   const MString& briefName,
					   const MObject & direction,
					   const MObject & intensity,
					   const MObject & ambient,
					   const MObject & diffuse,
					   const MObject & specular,
					   const MObject & shadowFraciton,
					   const MObject & preShadowIntensity,
					   const MObject & blindData,
					   MStatus* ReturnStatus = NULL );

	///
	MStatus     getDefault( float & defDirectionX, 
							float & defDirectionY, 
							float & defDirectionZ,
							float & defIntensityR,
							float & defIntensityG, 
							float & defIntensityB,
							bool  & defAmbient, 
							bool  & defDiffuse, 
							bool  & defSpecular,
							float & defShadowFraction,
							float & defPreShadowIntensity,
							unsigned int & defBlindData);

	///
	MStatus     setDefault( float defDirectionX, 
							float defDirectionY, 
							float defDirectionZ,
							float defIntensityR,
							float defIntensityG, 
							float defIntensityB,
							bool  defAmbient, 
							bool  defDiffuse, 
							bool  defSpecular,
							float defShadowFraction,
							float defPreShadowIntensity,
							unsigned int defBlindData);

	///
	MObject     child( unsigned index, MStatus* returnStatus );

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
#endif /* _MFnLightDataAttribute */



