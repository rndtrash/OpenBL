
#ifndef _MFnReflectShader
#define _MFnReflectShader
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
// CLASS:    MFnReflectShader
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MFnReflectShader)
//
// MFnReflectShader allows manipulation of dependency graph nodes representing
// reflective surface shaders.  This includes Phong and Blinn shaders. 
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES


#include <maya/MObject.h>
#include <maya/MStatus.h>
#include <maya/MFnLambertShader.h>
// *****************************************************************************

// DECLARATIONS


class MFnLambertShader;


class MColor;

// *****************************************************************************

// CLASS DECLARATION (MFnReflectShader)

/// Manage Reflective Surface Shaders
/**
  Facilitate the creation and manipulation of reflective surface shaders.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MFnReflectShader : public MFnLambertShader
{

	declareMFn( MFnReflectShader, MFn::kReflect );

public:
	///
	short       reflectedRayDepthLimit( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setReflectedRayDepthLimit( const short& new_limit );
	///
	MColor      specularColor( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setSpecularColor( const MColor& specular_color );
	///
	float       reflectivity( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setReflectivity( const float& reflectivity );
	///
	MColor      reflectedColor( MStatus * ReturnStatus = NULL ) const;
	///
	MStatus     setReflectedColor( const MColor& reflected_color );

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
#endif /* _MFnReflectShader */
