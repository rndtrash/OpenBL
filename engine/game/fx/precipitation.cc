//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "game/fx/precipitation.h"

#include "dgl/dgl.h"
#include "math/mathIO.h"
#include "console/consoleTypes.h"
#include "sceneGraph/sceneGraph.h"
#include "sceneGraph/sceneState.h"
#include "terrain/sky.h"
#include "game/gameConnection.h"
#include "game/player.h"
#include "core/bitStream.h"
#include "platform/profiler.h"

static const U32 dropHitMask = TerrainObjectType |
                               InteriorObjectType |
                               WaterObjectType |
                               StaticShapeObjectType |
                               StaticTSObjectType;

IMPLEMENT_CO_NETOBJECT_V1(Precipitation);
IMPLEMENT_CO_DATABLOCK_V1(PrecipitationData);

//----------------------------------------------------------
// PrecipitationData
//----------------------------------------------------------
PrecipitationData::PrecipitationData()
{
   soundProfile      = NULL;
   soundProfileId    = 0;

   mDropName = StringTable->insert("");
   mSplashName = StringTable->insert("");

   mDropSize          = 0.5;
   mSplashSize        = 0.5;
   mUseTrueBillboards = true;
   mSplashMS          = 250;
}
IMPLEMENT_CONSOLETYPE(PrecipitationData)
IMPLEMENT_GETDATATYPE(PrecipitationData)
IMPLEMENT_SETDATATYPE(PrecipitationData)

void PrecipitationData::initPersistFields()
{
   Parent::initPersistFields();

   addField("soundProfile",      TypeAudioProfilePtr, Offset(soundProfile,       PrecipitationData));
   addField("dropTexture",       TypeFilename,        Offset(mDropName,          PrecipitationData));
   addField("splashTexture",     TypeFilename,        Offset(mSplashName,        PrecipitationData));
   addField("dropSize",          TypeF32,             Offset(mDropSize,          PrecipitationData));
   addField("splashSize",        TypeF32,             Offset(mSplashSize,        PrecipitationData));
   addField("splashMS",          TypeS32,             Offset(mSplashMS,          PrecipitationData));
   addField("useTrueBillboards", TypeBool,            Offset(mUseTrueBillboards, PrecipitationData));
}

bool PrecipitationData::onAdd()
{
   if (Parent::onAdd() == false)
      return false;

   if (!soundProfile && soundProfileId != 0)
      if (Sim::findObject(soundProfileId, soundProfile) == false)
         Con::errorf(ConsoleLogEntry::General, "Error, unable to load sound profile for precipitation datablock");

   return true;
}

void PrecipitationData::packData(BitStream* stream)
{
   Parent::packData(stream);

   if (stream->writeFlag(soundProfile != NULL))
      stream->writeRangedU32(soundProfile->getId(), DataBlockObjectIdFirst,
                             DataBlockObjectIdLast);

   stream->writeString(mDropName);
   stream->writeString(mSplashName);
   stream->write(mDropSize);
   stream->write(mSplashSize);
   stream->write(mSplashMS);
   stream->writeFlag(mUseTrueBillboards);
}

void PrecipitationData::unpackData(BitStream* stream)
{
   Parent::unpackData(stream);

   if (stream->readFlag())
      soundProfileId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);
   else
      soundProfileId = 0;

   mDropName = stream->readSTString();
   mSplashName = stream->readSTString();

   stream->read(&mDropSize);
   stream->read(&mSplashSize);
   stream->read(&mSplashMS);
   mUseTrueBillboards = stream->readFlag();
}

//----------------------------------------------------------
// Precipitation!
//----------------------------------------------------------
Precipitation::Precipitation()
{
   mTypeMask |= ProjectileObjectType;
   mNetFlags.set(Ghostable|ScopeAlways);

   mDropHead = NULL;
   mSplashHead = NULL;
   mNumDrops = 5000;
   mPercentage = 1.0;

   mMinSpeed = 1.5;
   mMaxSpeed = 2.0;

   mBoxWidth = 200;
   mBoxHeight = 100;

   mMinMass = 0.75;
   mMaxMass = 0.85;

   mMaxTurbulence = 0.1;
   mTurbulenceSpeed = 0.2;
   mUseTurbulence = false;

   mRotateWithCamVel = true;

   mDoCollision = true;

   mStormData.valid = false;
   mStormData.startPct = 0;
   mStormData.endPct = 0;
   mStormData.startTime = 0;
   mStormData.totalTime = 0;

   mAudioHandle = 0;
   mDropHandle = TextureHandle();
   mSplashHandle = TextureHandle();

   U32 count = 0;
   for (U32 v = 0; v < DROPS_PER_SIDE; v++)
   {
      const F32 y1 = (F32) v / DROPS_PER_SIDE;
      const F32 y2 = (F32)(v+1) / DROPS_PER_SIDE;

      for (U32 u = 0; u < DROPS_PER_SIDE; u++)
      {
         const F32 x1 = (F32) u / DROPS_PER_SIDE;
         const F32 x2 = (F32)(u+1) / DROPS_PER_SIDE;

         texCoords[4*count+0].x = x1;
         texCoords[4*count+0].y = y1;

         texCoords[4*count+1].x = x2;
         texCoords[4*count+1].y = y1;

         texCoords[4*count+2].x = x2;
         texCoords[4*count+2].y = y2;

         texCoords[4*count+3].x = x1;
         texCoords[4*count+3].y = y2;
         count++;
      }
   }

   count = 0;
   for (U32 v = 0; v < FRAMES_PER_SIDE; v++)
   {
      const F32 y1 = (F32) v / FRAMES_PER_SIDE;
      const F32 y2 = (F32)(v+1) / FRAMES_PER_SIDE;

      for (U32 u = 0; u < FRAMES_PER_SIDE; u++)
      {
         const F32 x1 = (F32) u / FRAMES_PER_SIDE;
         const F32 x2 = (F32)(u+1) / FRAMES_PER_SIDE;

         splashCoords[4*count+0].x = x1;
         splashCoords[4*count+0].y = y1;

         splashCoords[4*count+1].x = x2;
         splashCoords[4*count+1].y = y1;

         splashCoords[4*count+2].x = x2;
         splashCoords[4*count+2].y = y2;

         splashCoords[4*count+3].x = x1;
         splashCoords[4*count+3].y = y2;
         count++;
      }
   }
}

Precipitation::~Precipitation()
{
}

void Precipitation::inspectPostApply()
{
   if (isClientObject())
      fillDropList();

   setMaskBits(DataMask);
}

//--------------------------------------------------------------------------
// Console stuff...
//--------------------------------------------------------------------------
void Precipitation::initPersistFields()
{
   Parent::initPersistFields();

   addGroup("Movement");
   addField("minSpeed", TypeF32, Offset(mMinSpeed, Precipitation));
   addField("maxSpeed", TypeF32, Offset(mMaxSpeed, Precipitation));

   addField("minMass", TypeF32, Offset(mMinMass, Precipitation));
   addField("maxMass", TypeF32, Offset(mMaxMass, Precipitation));
   endGroup("Movement");

   addGroup("Turbulence");
   addField("maxTurbulence", TypeF32, Offset(mMaxTurbulence, Precipitation));
   addField("turbulenceSpeed", TypeF32, Offset(mTurbulenceSpeed, Precipitation));

   addField("rotateWithCamVel", TypeBool, Offset(mRotateWithCamVel, Precipitation));
   addField("useTurbulence", TypeBool, Offset(mUseTurbulence, Precipitation));
   endGroup("Turbulence");

   addField("numDrops", TypeS32, Offset(mNumDrops, Precipitation));
   addField("boxWidth", TypeF32, Offset(mBoxWidth, Precipitation));
   addField("boxHeight", TypeF32, Offset(mBoxHeight, Precipitation));

   addField("doCollision", TypeBool, Offset(mDoCollision, Precipitation));
}

//-----------------------------------
// Console methods...
ConsoleMethod(Precipitation, setPercentange, void, 3, 3, "precipitation.setPercentage(percentage <0.0 to 1.0>)")
{
   object->setPercentage(dAtof(argv[2]));
}

ConsoleMethod(Precipitation, modifyStorm, void, 4, 4, "precipitation.modifyStorm(Percentage <0 to 1>, Time<sec>)")
{
   object->modifyStorm(dAtof(argv[2]), dAtof(argv[3]) * 1000);
}

//--------------------------------------------------------------------------
// Backend
//--------------------------------------------------------------------------
bool Precipitation::onAdd()
{
   if(!Parent::onAdd())
      return false;

   if (isClientObject())
   {
      if (mDataBlock->soundProfile)
         mAudioHandle = alxPlay(mDataBlock->soundProfile, &getTransform() );

      fillDropList();
   }

   mObjBox.min.set(-1e6, -1e6, -1e6);
   mObjBox.max.set( 1e6,  1e6,  1e6);

   if (isClientObject())
   {
      mDropHandle = TextureHandle(((PrecipitationData*)mDataBlock)->mDropName, MeshTexture);
      mSplashHandle = TextureHandle(((PrecipitationData*)mDataBlock)->mSplashName, MeshTexture);
   }

   resetWorldBox();
   addToScene();

   return true;
}

void Precipitation::onRemove()
{
   removeFromScene();
   Parent::onRemove();

   if (mAudioHandle)
      alxStop(mAudioHandle);
   mAudioHandle = 0;

   if (isClientObject())
      killDropList();
}

bool Precipitation::onNewDataBlock(GameBaseData* dptr)
{
   mDataBlock = dynamic_cast<PrecipitationData*>(dptr);
   if (!mDataBlock || !Parent::onNewDataBlock(dptr))
      return false;

   if (isClientObject())
   {
      mDropHandle = TextureHandle(((PrecipitationData*)mDataBlock)->mDropName, MeshTexture);
      mSplashHandle = TextureHandle(((PrecipitationData*)mDataBlock)->mSplashName, MeshTexture);
   }

   scriptOnNewDataBlock();
   return true;
}

U32 Precipitation::packUpdate(NetConnection* con, U32 mask, BitStream* stream)
{
   Parent::packUpdate(con, mask, stream);

   if (stream->writeFlag(mask & DataMask))
   {
      stream->write(mNumDrops);
      stream->write(mMinSpeed);
      stream->write(mMaxSpeed);
      stream->write(mBoxWidth);
      stream->write(mBoxHeight);
      stream->write(mMinMass);
      stream->write(mMaxMass);
      stream->write(mMaxTurbulence);
      stream->write(mTurbulenceSpeed);
      stream->writeFlag(mUseTurbulence);
      stream->writeFlag(mRotateWithCamVel);
      stream->writeFlag(mDoCollision);
   }

   if (stream->writeFlag(mask & PercentageMask))
   {
      stream->write(mPercentage);
   }

   if (stream->writeFlag(!(mask & ~(DataMask | PercentageMask | StormMask)) && (mask & StormMask)))
   {
      stream->write(mStormData.endPct);
      stream->write(mStormData.totalTime);
   }

   return 0;
}

void Precipitation::unpackUpdate(NetConnection* con, BitStream* stream)
{
   Parent::unpackUpdate(con, stream);

   U32 oldDrops = mNumDrops * mPercentage;
   if (stream->readFlag())
   {
      stream->read(&mNumDrops);
      stream->read(&mMinSpeed);
      stream->read(&mMaxSpeed);
      stream->read(&mBoxWidth);
      stream->read(&mBoxHeight);
      stream->read(&mMinMass);
      stream->read(&mMaxMass);
      stream->read(&mMaxTurbulence);
      stream->read(&mTurbulenceSpeed);
      mUseTurbulence = stream->readFlag();
      mRotateWithCamVel = stream->readFlag();
      mDoCollision = stream->readFlag();
   }

   if (stream->readFlag())
   {
      stream->read(&mPercentage);
   }

   if (stream->readFlag())
   {
      F32 pct;
      U32 time;
      stream->read(&pct);
      stream->read(&time);
      modifyStorm(pct, time);
   }

   U32 newDrops = mNumDrops * mPercentage;
   if (isClientObject() && oldDrops != newDrops)
      fillDropList();
}

//--------------------------------------------------------------------------
// Support functions
//--------------------------------------------------------------------------
VectorF Precipitation::getWindVelocity()
{
   Sky* sky = gClientSceneGraph->getCurrentSky();
   return (sky && sky->mEffectPrecip) ? -sky->getWindVelocity() : VectorF(0,0,0);
}

void Precipitation::fillDropList()
{
   AssertFatal(isClientObject(), "Precipitation is doing stuff on the server - BAD!");

   F32 density = Con::getFloatVariable("$pref::precipitationDensity", 1.0f);
   U32 newDropCount = (U32)(mNumDrops * mPercentage * density);
   U32 dropCount = 0;
   if (mDropHead)
   {
      Raindrop* curr = mDropHead;
      while (curr)
      {
         dropCount++;
         curr = curr->next;
         if (dropCount == newDropCount && curr)
         {
            //delete the remaining drops
            Raindrop* next = curr->next;
            curr->next = NULL;
            while (next)
            {
               Raindrop* last = next;
               next = next->next;
               last->next = NULL;
               destroySplash(last);
               delete last;
            }
            break;
         }
      }
   }

   if (dropCount < newDropCount)
   {
      //move to the end
      Raindrop* curr = mDropHead;
      if (curr)
      {
         while (curr->next)
            curr = curr->next;
      }
      else
      {
         mDropHead = curr = new Raindrop;
         spawnNewDrop(curr);
         dropCount++;
      }

      //and add onto it
      while (dropCount < newDropCount)
      {
         curr->next = new Raindrop;
         curr = curr->next;
         spawnNewDrop(curr);
         dropCount++;
      }
   }
}

void Precipitation::killDropList()
{
   AssertFatal(isClientObject(), "Precipitation is doing stuff on the server - BAD!");

   Raindrop* curr = mDropHead;
   while (curr)
   {
      Raindrop* next = curr->next;
      delete curr;
      curr = next;
   }
   mDropHead = NULL;
}

void Precipitation::spawnDrop(Raindrop *drop)
{
   PROFILE_START(PrecipSpawnDrop);
   AssertFatal(isClientObject(), "Precipitation is doing stuff on the server - BAD!");

   drop->velocity = Platform::getRandom() * (mMaxSpeed - mMinSpeed) + mMinSpeed;

   drop->position.x = Platform::getRandom() * mBoxWidth - (mBoxWidth / 2);
   drop->position.y = Platform::getRandom() * mBoxWidth - (mBoxWidth / 2);

   drop->texCoordIndex = (U32)(Platform::getRandom() * ((F32)DROPS_PER_SIDE*DROPS_PER_SIDE - 0.5));
   drop->valid = true;
   drop->time = Platform::getRandom() * M_2PI;
   drop->mass = Platform::getRandom() * (mMaxMass - mMinMass) + mMinMass;
   PROFILE_END();
}

void Precipitation::spawnNewDrop(Raindrop *drop)
{
   AssertFatal(isClientObject(), "Precipitation is doing stuff on the server - BAD!");

   spawnDrop(drop);
   drop->position.z = Platform::getRandom() * mBoxHeight - (mBoxHeight / 2);
}

void Precipitation::findDropCutoff(Raindrop *drop)
{
   PROFILE_START(PrecipFindDropCutoff);
   AssertFatal(isClientObject(), "Precipitation is doing stuff on the server - BAD!");

   if (mDoCollision)
   {
      VectorF windVel = getWindVelocity();
      VectorF velocity = windVel / drop->mass - VectorF(0, 0, drop->velocity);
      velocity.normalize();

      Point3F end   = drop->position + 100 * velocity;
      Point3F start = drop->position - 500 * velocity;

      RayInfo rInfo;
      if (getContainer()->castRay(start, end, dropHitMask, &rInfo))
      {
         drop->hitPos = rInfo.point;
         drop->hitType = rInfo.object->getTypeMask();
      }
      else
         drop->hitPos = Point3F(0,0,-1000);

      drop->valid = drop->position.z > drop->hitPos.z;
   }
   else
   {
      drop->hitPos = Point3F(0,0,-1000);
      drop->valid = true;
   }
   PROFILE_END();
}

void Precipitation::createSplash(Raindrop *drop)
{
   PROFILE_START(PrecipCreateSplash);
   if (drop != mSplashHead && !(drop->nextSplashDrop || drop->prevSplashDrop))
   {
      if (!mSplashHead)
      {
         mSplashHead = drop;
         drop->prevSplashDrop = NULL;
         drop->nextSplashDrop = NULL;
      }
      else
      {
         mSplashHead->prevSplashDrop = drop;
         drop->nextSplashDrop = mSplashHead;
         drop->prevSplashDrop = NULL;
         mSplashHead = drop;
      }
   }

   drop->animStartTime = Platform::getVirtualMilliseconds();
   PROFILE_END();
}

void Precipitation::destroySplash(Raindrop *drop)
{
   PROFILE_START(PrecipDestroySplash);
   if (drop == mSplashHead)
   {
      mSplashHead = NULL;
   }
   else
   {
      // Unlink.
      if (drop->nextSplashDrop)
         drop->nextSplashDrop->prevSplashDrop = drop->prevSplashDrop;

      if (drop->prevSplashDrop)
         drop->prevSplashDrop->nextSplashDrop = drop->nextSplashDrop;

      drop->nextSplashDrop = NULL;
      drop->prevSplashDrop = NULL;
   }

   PROFILE_END();
}

//--------------------------------------------------------------------------
// Processing
//--------------------------------------------------------------------------
void Precipitation::setPercentage(F32 pct)
{
   mPercentage = pct;
   if (isServerObject())
      setMaskBits(PercentageMask);
}

void Precipitation::modifyStorm(F32 pct, U32 ms)
{
   pct = mClampF(pct, 0, 1);
   mStormData.endPct = pct;
   mStormData.totalTime = ms;

   if (isServerObject())
   {
      setMaskBits(StormMask);
      return;
   }

   mStormData.startTime = Platform::getVirtualMilliseconds();
   mStormData.startPct = mPercentage;
   mStormData.valid = true;
}

void Precipitation::interpolateTick(F32 delta)
{
   PROFILE_START(PrecipInterpolate);
   AssertFatal(isClientObject(), "Precipitation is doing stuff on the server - BAD!");

   Raindrop* curr = mDropHead;

   VectorF windVel = getWindVelocity();
   F32 dt = 1-delta;

   while (curr)
   {
      if (!curr->toRender)
      {
         curr = curr->next;
         continue;
      }

      VectorF turbulence = dt * windVel;
      if (mUseTurbulence)
      {
         F32 renderTime = curr->time + dt * mTurbulenceSpeed;
         turbulence += VectorF(mSin(renderTime), mCos(renderTime), 0) * mMaxTurbulence;
      }

      curr->renderPosition = curr->position + turbulence / curr->mass;
      curr->renderPosition.z -= dt * curr->velocity;

      curr = curr->next;
   }
   PROFILE_END();
}

void Precipitation::processTick(const Move *)
{
   //nothing to do on the server
   if (isServerObject())
      return;

   //we need to update positions and do some collision here
   GameConnection* conn = GameConnection::getConnectionToServer();
   if (!conn)
      return; //need connection to server

   ShapeBase* camObj = conn->getCameraObject();
   if (!camObj)
      return;

   PROFILE_START(PrecipProcess);

   //update the storm if necessary
   if (mStormData.valid)
   {
      F32 t = (Platform::getVirtualMilliseconds() - mStormData.startTime) / (F32)mStormData.totalTime;
      if (t >= 1)
      {
         mPercentage = mStormData.endPct;
         mStormData.valid = false;
      }
      else
         mPercentage = mStormData.startPct * (1-t) + mStormData.endPct * t;
      fillDropList();
   }

   MatrixF camMat;
   camObj->getEyeTransform(&camMat);
   Point3F camPos, camDir;
   camMat.getColumn(3, &camPos);
   camMat.getColumn(1, &camDir);
   camDir.normalize();
   F32 fovDot = camObj->getCameraFov() / 180;

   Raindrop* curr = mDropHead;

   //make a box
   Box3F box(camPos.x - mBoxWidth / 2, camPos.y - mBoxWidth / 2, camPos.z - mBoxHeight / 2,
             camPos.x + mBoxWidth / 2, camPos.y + mBoxWidth / 2, camPos.z + mBoxHeight / 2);

   //offset the renderbox in the direction of the camera direction
   //in order to have more of the drops actually rendered
   box.min.x += camDir.x * mBoxWidth / 4;
   box.max.x += camDir.x * mBoxWidth / 4;
   box.min.y += camDir.y * mBoxWidth / 4;
   box.max.y += camDir.y * mBoxWidth / 4;
   box.min.z += camDir.z * mBoxHeight / 4;
   box.max.z += camDir.z * mBoxHeight / 4;

   VectorF windVel = getWindVelocity();

   while (curr)
   {
      //update position
      if (mUseTurbulence)
         curr->time += mTurbulenceSpeed;
      curr->position += windVel / curr->mass;
      curr->position.z -= curr->velocity;

      //wrap position
      wrapDrop(curr, box);

      if (curr->valid)
      {
         if (curr->position.z < curr->hitPos.z)
         {
            curr->valid = false;

            // Bump back so we don't spawn splashes where they ought not
            // be. It might be better to revisit this and use the hitPos.
            curr->position -= windVel / curr->mass;
            curr->position.z += curr->velocity;

            //do some funky effect thingy for hitting something
            if (mSplashHandle.getGLName() != 0)
               createSplash(curr);
         }
      }

      //render test
      VectorF lookVec = curr->position - camPos;
      curr->toRender = curr->valid ? mDot(lookVec, camDir) > fovDot : false;

      curr = curr->next;
   }

   //update splashes
   curr = mSplashHead;
   U32 currTime = Platform::getVirtualMilliseconds();
   while (curr)
   {
      F32 pct = (F32)(currTime - curr->animStartTime) / mDataBlock->mSplashMS;
      if (pct >= 1.0f)
      {
         Raindrop *next = curr->nextSplashDrop;
         destroySplash(curr);
         curr = next;
         continue;
      }

      curr->splashIndex = (U32)((FRAMES_PER_SIDE*FRAMES_PER_SIDE) * pct);
      curr = curr->nextSplashDrop;
   }
   PROFILE_END();
}

//--------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------
bool Precipitation::prepRenderImage(SceneState* state, const U32 stateKey,
                                    const U32 /*startZone*/, const bool /*modifyBaseState*/)
{
   if (isLastState(state, stateKey))
      return false;
   setLastState(state, stateKey);

   // This should be sufficient for most objects that don't manage zones, and
   //  don't need to return a specialized RenderImage...
   if (state->isObjectRendered(this)) {
      SceneRenderImage* image = new SceneRenderImage;
      image->obj = this;
      image->isTranslucent = true;
      image->sortType = SceneRenderImage::EndSort;
      state->insertRenderImage(image);
   }

   if (!mAudioHandle && mDataBlock->soundProfile)
      mAudioHandle = alxPlay(mDataBlock->soundProfile, &getTransform());

   return false;
}

void Precipitation::renderObject(SceneState* state, SceneRenderImage*)
{
   PROFILE_START(PrecipRender);
   AssertFatal(dglIsInCanonicalState(), "Error, GL not in canonical state on entry");

   RectI viewport;
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   dglGetViewport(&viewport);

   // Uncomment this if this is a "simple" (non-zone managing) object
   state->setupObjectProjection(this);

   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   renderPrecip(state);
   renderSplashes(state);

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   glMatrixMode(GL_MODELVIEW);
   glPopMatrix();

   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   dglSetViewport(viewport);

   AssertFatal(dglIsInCanonicalState(), "Error, GL not in canonical state on exit");
   PROFILE_END();
}


//--------------------------------------------------------------------------
void Precipitation::renderPrecip(SceneState *state)
{
   GameConnection* conn = GameConnection::getConnectionToServer();
   if (!conn)
      return; //need connection to server

   ShapeBase* camObj = conn->getCameraObject();
   if (!camObj)
      return;

   PROFILE_START(PrecipRenderPrecip);

   Point3F camPos = state->getCameraPosition();
   VectorF camVel = camObj->getVelocity();

   Raindrop *curr = mDropHead;

   glEnable(GL_BLEND);
   glDepthMask(GL_FALSE);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, mDropHandle.getGLName());

   static Point3F verts[4];
   glVertexPointer(3, GL_FLOAT, 0, verts);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);

   glColor3f(1.0, 1.0, 1.0);

   VectorF windVel = getWindVelocity();

   while (curr)
   {
      if (!curr->toRender)
      {
         curr = curr->next;
         continue;
      }

      Point3F pos = curr->renderPosition;
      VectorF orthoDir = (camPos - pos);

      VectorF velocity = windVel / curr->mass;
      if (mRotateWithCamVel && camVel != VectorF(0,0,0))
      {
         F32 distance = orthoDir.len();
         velocity -= camVel / (distance > 2 ? distance : 2) * 0.3;
      }

      velocity.z -= curr->velocity;
      velocity.normalize();
      orthoDir.normalize();

      VectorF right;
      VectorF up;

      // two forms of billboards - true billboards (1st codeblock)
      // or axis-aligned with velocity (2nd codeblock)
      // the axis-aligned billboards are aligned with the velocity
      // of the raindrop, and tilted slightly towards the camera
      if (mDataBlock->mUseTrueBillboards)
      {
         state->mModelview.getRow(0,&right);
         state->mModelview.getRow(2,&up);
         right.normalize();
         up.normalize();
      }
      else
      {
         right = mCross(-velocity, orthoDir);
         right.normalize();
         up = mCross(orthoDir, right) * 0.5 - velocity * 0.5;
         up.normalize();
      }
      right *= mDataBlock->mDropSize;
      up *= mDataBlock->mDropSize;

      verts[0] = pos + right + up;
      verts[1] = pos - right + up;
      verts[2] = pos - right - up;
      verts[3] = pos + right - up;

      glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[4*curr->texCoordIndex]);

      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

      //debug collision render
      //if (curr->cutoffHeight != -1000)
      //{
      //   VectorF windVel = getWindVelocity();
      //   VectorF velocity = windVel / curr->mass - VectorF(0, 0, curr->velocity);
      //   velocity.normalize();

      //   Point3F start = curr->position;// - 10000 * velocity;
      //   F32 height = start.z - curr->cutoffHeight;
      //   F32 t = height / velocity.z;
      //   Point3F end = start - t * velocity;

      //   glDisable(GL_TEXTURE_2D);
      //   glDisable(GL_BLEND);

      //   glBegin(GL_LINES);
      //   glColor3f(1.0, 0.0, 0.0);
      //   glVertex3fv(&(start.x));
      //   glColor3f(0.0, 1.0, 0.0);
      //   glVertex3fv(&(end.x));
      //   glEnd();

      //   glBegin(GL_TRIANGLE_FAN);
      //   glColor3f(0.0,0.0,1.0);
      //   glVertex3fv(&((end - right + up).x));
      //   glVertex3fv(&((end + right + up).x));
      //   glVertex3fv(&((end + right - up).x));
      //   glVertex3fv(&((end - right - up).x));
      //   glEnd();

      //   glEnable(GL_TEXTURE_2D);
      //   glEnable(GL_BLEND);
      //}
      //end debug collision render

      curr = curr->next;
   }

   glDisable(GL_TEXTURE_2D);

   glDepthMask(GL_TRUE);
   glDisable(GL_BLEND);

   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);

   PROFILE_END();
}

void Precipitation::renderSplashes(SceneState *state)
{
   PROFILE_START(PrecipRenderSplash);

   //setup the billboard
   VectorF right, up;
   state->mModelview.getRow(0, &right);
   state->mModelview.getRow(2, &up);
   right.normalize();
   up.normalize();
   right *= mDataBlock->mSplashSize;
   up *= mDataBlock->mSplashSize;

   glColor3f(1.0, 1.0, 1.0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, mSplashHandle.getGLName());
   glEnable(GL_BLEND);
   glDepthMask(GL_FALSE);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   static Point3F verts[4];
   glVertexPointer(3, GL_FLOAT, 0, verts);

   Raindrop *curr = mSplashHead;
   while (curr)
   {
      verts[0] = curr->hitPos + right + up;
      verts[1] = curr->hitPos - right + up;
      verts[2] = curr->hitPos - right - up;
      verts[3] = curr->hitPos + right - up;

      glTexCoordPointer(2, GL_FLOAT, 0, &splashCoords[4*curr->splashIndex]);

      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

      curr = curr->nextSplashDrop;
   }
   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisable(GL_BLEND);
   glDepthMask(GL_TRUE);
   glDisable(GL_TEXTURE_2D);
   PROFILE_END();
}
