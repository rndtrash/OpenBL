//-----------------------------------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

// This file contains all of the functions for converting an InteriorMap into an EditGeometry

#include "map2dif plus/convert.h"
#include "dgl/gBitmap.h"

extern const char* gWadPath;
extern GBitmap* loadBitmap(const char* file);

void loadTextures(InteriorMap* map)
{
   // Insert the dummy textures
   gWorkingGeometry->insertTexture("NULL");
   gWorkingGeometry->insertTexture("TRIGGER");

   // Basically we just need to load the textures to get the texture sizes
	GBitmap** textures = new GBitmap*[map->mInteriorRes->mMaterials.size()];

   Con::printf("   Loading textures:");
   dPrintf("\n");

	for (int i = 0; i < map->mInteriorRes->mMaterials.size(); i++)
	{
      // Get the texture file name
      char loadBuffer[4096];
      dSprintf(loadBuffer, sizeof(loadBuffer), "%s%s", gWadPath, map->mInteriorRes->mMaterials[i]);

      // loadBitmap() will walk up the path till it finds the texture
      textures[i] = loadBitmap(loadBuffer);

      // Notify the user we couldn't load the texture
      if (!textures[i])
      {
         Con::printf("      Unable to load texture %s", map->mInteriorRes->mMaterials[i]);
         dPrintf("      Unable to load texture %s\n", map->mInteriorRes->mMaterials[i]);
         dFflushStdout();
      }
      else
         Con::printf("      Loaded texture %s", map->mInteriorRes->mMaterials[i]);
	}

   // Divide our texgens through by the texture sizes
   if (map->mInteriorRes->mBrushFormat != InteriorMapResource::QuakeOld && map->mInteriorRes->mBrushFormat != InteriorMapResource::Valve220)
      return;

   for (U32 i = 0; i < map->mInteriorRes->mMaterials.size(); i++)
	{
      F32 width  = 16.0f;
      F32 height = 16.0f;

      if (textures[i])
      {
         width  = textures[i]->getWidth();
         height = textures[i]->getHeight();

         if (!isPow2((U32)width) || !isPow2((U32)height))
         {
            Con::printf("      Error: Non-power of two sized texture - %s (%d x %d)", map->mInteriorRes->mMaterials[i], (U32)width, (U32)height);
            dPrintf("      Error: Non-power of two sized texture - %s (%d x %d)\n", map->mInteriorRes->mMaterials[i], (U32)width, (U32)height);
         }
      }

      dFflushStdout();

      for (U32 j = 0; j < map->mInteriorRes->mBrushes.size(); j++)
      {
         for (U32 k = 0; k < map->mInteriorRes->mBrushes[j]->mFaces.mPolyList.size(); k++)
         {
            if (i == map->mInteriorRes->mBrushes[j]->mFaces.mPolyList[k].material)
            {
               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texDiv[0] = width;
               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texDiv[1] = height;

               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texGens[0].x /= width;
               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texGens[0].y /= width;
               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texGens[0].z /= width;
               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texGens[0].d /= width;

               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texGens[1].x /= height;
               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texGens[1].y /= height;
               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texGens[1].z /= height;
               map->mInteriorRes->mBrushes[j]->mTexInfos[k].texGens[1].d /= height;
            }
         }
      }
   }

   // Flag all of the faces that couldn't load a texture
   for (U32 i = 0; i < map->mInteriorRes->mMaterials.size(); i++)
	{
      if (textures[i])
         continue;

      for (U32 j = 0; j < map->mInteriorRes->mBrushes.size(); j++)
      {
         for (U32 k = 0; k < map->mInteriorRes->mBrushes[j]->mFaces.mPolyList.size(); k++)
         {
            if (i == map->mInteriorRes->mBrushes[j]->mFaces.mPolyList[k].material)
               map->mInteriorRes->mBrushes[j]->mFaces.mPolyList[k].material = -1;
         }
      }
   }

   // Clean up our textures
   for (int i = 0; i < map->mInteriorRes->mMaterials.size(); i++)
      delete textures[i];

   delete [] textures;
}

void convertInteriorMap(InteriorMap* map)
{
   // First copy over the worldspawn entity
   getWorldSpawn(map);

   gWorkingGeometry->mMinBound.set(1e10, 1e10, 1e10);
	gWorkingGeometry->mMaxBound.set(-1e10, -1e10, -1e10);

   // Next add our mInteriorRes->mBrushes
   getBrushes(map);

   // Now get our mInteriorRes->mEntities
   getEntities(map);
}

void getBrushes(InteriorMap* map)
{
   CSGBrush* pBrush;

   for (U32 i = 0; i < map->mInteriorRes->mBrushes.size(); i++)
   {
      // Only load good mInteriorRes->mBrushes
      if (map->mInteriorRes->mBrushes[i]->mStatus != ConvexBrush::Good)
      {
         Con::printf("   Error: Brush %d - %s", i, map->mInteriorRes->mBrushes[i]->mDebugInfo);
         dPrintf("   Error: Brush %d - %s\n", i, map->mInteriorRes->mBrushes[i]->mDebugInfo);
         continue;
      }

      if (map->mInteriorRes->mBrushes[i]->mType == InteriorMapResource::Structural)
      {
         gWorkingGeometry->mStructuralBrushes.push_back(gBrushArena.allocateBrush());
         pBrush = gWorkingGeometry->mStructuralBrushes.last();
         pBrush->mBrushType = StructuralBrush;
      }
      else if (map->mInteriorRes->mBrushes[i]->mType == InteriorMapResource::Detail)
      {
         gWorkingGeometry->mDetailBrushes.push_back(gBrushArena.allocateBrush());
         pBrush = gWorkingGeometry->mDetailBrushes.last();
         pBrush->mBrushType = DetailBrush;
      }
      else if (map->mInteriorRes->mBrushes[i]->mType == InteriorMapResource::Portal)
      {
         gWorkingGeometry->mPortalBrushes.push_back(gBrushArena.allocateBrush());
         pBrush = gWorkingGeometry->mPortalBrushes.last();
         pBrush->mBrushType = PortalBrush;
      }
      else if (map->mInteriorRes->mBrushes[i]->mType == InteriorMapResource::Collision)
      {
         gWorkingGeometry->mSpecialCollisionBrushes.push_back(gBrushArena.allocateBrush());
         pBrush = gWorkingGeometry->mSpecialCollisionBrushes.last();
         pBrush->mBrushType = CollisionBrush;
      }
      else
         continue;

      pBrush->materialType = 0;
      pBrush->brushId      = gWorkingGeometry->mCurrBrushId++;

      for (U32 j = 0; j < map->mInteriorRes->mBrushes[i]->mFaces.mPolyList.size(); j++)
      {
         // Copy over the plane for the face
         CSGPlane plane;
         plane.flags = 0;
         plane.owningEntity = NULL;
         plane.winding.numNodes = 0;
         plane.winding.numZoneIds = 0;

         PlaneF normal = map->mInteriorRes->mBrushes[i]->mFaces.mPlaneList[map->mInteriorRes->mBrushes[i]->mFaces.mPolyList[j].plane];
         plane.planeEQIndex = gWorkingGeometry->insertPlaneEQ(Point3D(normal.x, normal.y, normal.z), normal.d);

         // Set up the texture
         S32 tx = map->mInteriorRes->mBrushes[i]->mFaces.mPolyList[j].material;

         if (tx > -1)
         {
            char* texture = (char*)map->mInteriorRes->mMaterials[tx];
            plane.pTextureName = gWorkingGeometry->insertTexture(texture);
         }
         else
         {
            plane.pTextureName = gWorkingGeometry->insertTexture("WHITE");
         }

         // Copy over the texgens
         PlaneF tGenX = map->mInteriorRes->mBrushes[i]->mTexInfos[j].texGens[0];
         PlaneF tGenY = map->mInteriorRes->mBrushes[i]->mTexInfos[j].texGens[1];

         plane.texGenIndex = addTexGen(tGenX, tGenY);

         pBrush->addPlane(plane);
      }
   }
}

// Entity conversions
void getWorldSpawn(InteriorMap* map)
{
   gWorkingGeometry->mWorldEntity = new WorldSpawnEntity;

   InteriorMapResource::Entity* world = map->getEntity("worldspawn");
   if (world)
   {
      char* value;

      value = world->getValue("detail_number");
      if (value)
         gWorkingGeometry->mWorldEntity->mDetailNumber = dAtoi(value);
      
      value = world->getValue("min_pixels");
      if (value)
         gWorkingGeometry->mWorldEntity->mMinPixels = dAtoi(value);

      value = world->getValue("geometry_scale");
      if (value)
         gWorkingGeometry->mWorldEntity->mGeometryScale = dAtof(value);

      value = world->getValue("light_geometry_scale");
      if (value)
         gWorkingGeometry->mWorldEntity->mLumelScale = dAtof(value);

      value = world->getValue("ambient_color");
      if (value)
      {
         dSscanf(value, "%f %f %f",
            &gWorkingGeometry->mWorldEntity->mAmbientColor.red,
            &gWorkingGeometry->mWorldEntity->mAmbientColor.green,
            &gWorkingGeometry->mWorldEntity->mAmbientColor.blue);
      }
   }
}

void getEntities(InteriorMap* map)
{
   for (U32 i = 0; i < map->mInteriorRes->mEntities.size(); i++)
   {
      // Going to use an odd version of the old map2dif entity code
      EditGeometry::Entity* pEntity = NULL;

      InteriorMapResource::Entity* ent = map->mInteriorRes->mEntities[i];

      if (dStricmp(ent->classname, DetailEntity::getClassName()) == 0)
         pEntity = new DetailEntity;
      else if (dStricmp(ent->classname, CollisionEntity::getClassName()) == 0)
         pEntity = new CollisionEntity;
      else if (dStricmp(ent->classname, PortalEntity::getClassName()) == 0)
         pEntity = new PortalEntity;
      else if (dStricmp(ent->classname, TargetEntity::getClassName()) == 0)
         pEntity = new TargetEntity;

      // lighting: emitters
      if (dStricmp(ent->classname, PointEmitterEntity::getClassName()) == 0)
         pEntity = new PointEmitterEntity;
      else if (dStricmp(ent->classname, SpotEmitterEntity::getClassName()) == 0)
         pEntity = new SpotEmitterEntity;

      // lighting: lights
      else if (dStricmp(ent->classname, LightEntity::getClassName()) == 0)
      {
         // For now just shove quake lights into omni_lights
         if (map->mInteriorRes->mBrushFormat == InteriorMapResource::QuakeOld)
         {
            pEntity = new LightOmniEntity;
            LightOmniEntity* pLight = static_cast<LightOmniEntity*>(pEntity);
            pLight->mColor.set(0.6f, 0.6f, 0.6f);
         }
         else
            pEntity = new LightEntity;
      }

      else if (dStricmp(ent->classname, "light_globe") == 0)
      {
         // For now just shove quake lights into omni_lights
         pEntity = new LightOmniEntity;
         LightOmniEntity* pLight = static_cast<LightOmniEntity*>(pEntity);
         pLight->mColor.set(0.6f, 0.6f, 0.6f);
      }

      // lighting: scripted lights
      else if (dStricmp(ent->classname, LightOmniEntity::getClassName()) == 0)
         pEntity = new LightOmniEntity;
      else if (dStricmp(ent->classname, LightSpotEntity::getClassName()) == 0)
         pEntity = new LightSpotEntity;

      // lighting: animated lights
      else if (dStricmp(ent->classname, LightStrobeEntity::getClassName()) == 0)
         pEntity = new LightStrobeEntity;
      else if (dStricmp(ent->classname, LightPulseEntity::getClassName()) == 0)
         pEntity = new LightPulseEntity;
      else if (dStricmp(ent->classname, LightPulse2Entity::getClassName()) == 0)
         pEntity = new LightPulse2Entity;
      else if (dStricmp(ent->classname, LightFlickerEntity::getClassName()) == 0)
         pEntity = new LightFlickerEntity;
      else if (dStricmp(ent->classname, LightRunwayEntity::getClassName()) == 0)
         pEntity = new LightRunwayEntity;

      // Special classes
      else if (dStricmp(ent->classname, MirrorSurfaceEntity::getClassName()) == 0)
         pEntity = new MirrorSurfaceEntity;

      // Trigger Classes
      else if (dStricmp(ent->classname, TriggerEntity::getClassName()) == 0)
         pEntity = new TriggerEntity;

      // Game Classes
      else if (dStricmp(ent->classname, GameEntity::getClassName()) == 0)
         pEntity = new GameEntity;

      // If we have an entity then parse it
      if (pEntity)
      {
         if (pEntity->parseEntityDescription(ent) == false)
         {
            dPrintf("   Error processing entity %s", ent->classname);
            delete pEntity;
         }
         else
         {
            gWorkingGeometry->mEntities.push_back(pEntity);

            if (dStricmp(ent->classname, PortalEntity::getClassName()) == 0)
               gWorkingGeometry->mPortalEntities.push_back(pEntity);
         }
      }
   }
}