// ----------------------------------------------------------------
// fxDTSBrick
// 
// The back-end for bricks. Handles bricks, their connections and 
// attributes, then sends them off to the oct tree for rendering. 
//
// fxDTSBricks initially get sent to the SceneGraph simply to
// be registered in the scene. They are then removed from the 
// SceneGraph, sent to fxBrickBatcher, then to the oct tree. I think.
//
// This stuff is really messy for now. It's probably going to be
// messy for a while, until it's at a point where it's fully
// operational and it's worth cleaning it up.
// ----------------------------------------------------------------

#include "game/fx/fxDTSBrick.h"
#include "console/consoleTypes.h"
#include "game/gameBase.h"
#include "sceneGraph/sceneGraph.h"
#include "sceneGraph/sceneState.h"
#include "game/shapeBase.h"
#include "dgl/dgl.h"
#include "core/bitStream.h"
#include "sim/netConnection.h"

StringTableEntry fxDTSBrick::sprayCanDivisionName[16];
S32              fxDTSBrick::sprayCanDivisionSlot[64];
ColorF           fxDTSBrick::colorIDTable[64];
S32              fxDTSBrick::brickCount;

IMPLEMENT_CO_NETOBJECT_V1(fxDTSBrick);

fxDTSBrick::fxDTSBrick()
{
   mTypeMask |= fxDTSBrickObjectType;
   mNetFlags.set(Ghostable);

   mDataBlock   = NULL;
   colorID      = 5;
   printID      = 0;
   colorFxID    = 0;
   shapeFxID    = 0;
   isBasePlate  = false;
   isPlanted    = false;
   client       = -1;
   stackBL_ID   = -1;

   worldPos     = Point3F(0.0, 0.0, 0.0);

   ang          = AngAxisF();
}                                                                                                      

fxDTSBrick::~fxDTSBrick()
{
}

void fxDTSBrick::initPersistFields()
{
   Parent::initPersistFields();
   addGroup("fxDTSBrick Stuff");

   addField("dataBlock",   TypeGameBaseDataPtr, Offset(mDataBlock,  fxDTSBrick));
   addField("angleID",     TypeS8,              Offset(angleID,     fxDTSBrick));
   addField("colorID",     TypeS8,              Offset(colorID,     fxDTSBrick));
   addField("printID",     TypeS32,             Offset(printID,     fxDTSBrick));
   addField("colorFxID",   TypeS8,              Offset(colorFxID,   fxDTSBrick));
   addField("shapeFxID",   TypeS8,              Offset(shapeFxID,   fxDTSBrick));
   addField("isBasePlate", TypeBool,            Offset(isBasePlate, fxDTSBrick));
   addField("isPlanted",   TypeBool,            Offset(isPlanted,   fxDTSBrick));
   addField("client",      TypeS32,             Offset(client,      fxDTSBrick));
   addField("stackBL_ID",  TypeS32,             Offset(stackBL_ID,  fxDTSBrick));

   endGroup("fxDTSBrick Stuff");
}


// -------------------------------------------------------------------------------------
// SceneObject overrides

bool fxDTSBrick::onAdd()
{
   if (Parent::onAdd() == false)
      return false;


   if (mDataBlock != NULL)
   {
      mObjBox.min = -mDataBlock->brickDimensions;
      mObjBox.max = mDataBlock->brickDimensions;

      resetWorldBox();
      setTransform(mObjToWorld);
   }

   addToScene();

   fxDTSBrick::brickCount++;

   return true;
}

void fxDTSBrick::onRemove()
{
   removeFromScene();
   Parent::onRemove();
   fxDTSBrick::brickCount--;
}


void fxDTSBrick::addToScene()
{
   if (isClientObject())
   {
      if (!isPlanted /* && !octTree::smClientTree*/)
      {
         gClientContainer.addObject(this);
      }

      gClientSceneGraph->addObjectToScene(this);
      return;
   }

   gServerSceneGraph->addObjectToScene(this);
   return;
}


// Some boilerplate prepRenderImage code. This is used in the final game as well with stuff appended to it

bool fxDTSBrick::prepRenderImage(SceneState* state, const U32 stateKey, const U32 startZone, const bool modifyBaseZoneState)
{
   // Return if last state.
   if (isLastState(state, stateKey)) return false;
   // Set Last State.
   setLastState(state, stateKey);

   // Is Object Rendered?
   if (state->isObjectRendered(this))
   {
      // Yes, so get a SceneRenderImage.
      SceneRenderImage* image = new SceneRenderImage;
      // Populate it.
      image->obj = this;
      image->isTranslucent = false;
      image->sortType = SceneRenderImage::Normal;

      // Insert it into the scene images.
      state->insertRenderImage(image);
   }

   return false;
}


// If we're holding a tool that draws brick wireframes, this will draw the wireframes.
// Otherwise, we submit the brick to fxBrickBatcher.
void fxDTSBrick::renderObject(SceneState* state, SceneRenderImage*)
{
   // super duper placeholder for the time being just to demonstrate that 
   // fxDTSBricks are being created and their values can be used
   glPushMatrix();
   dglMultMatrix(&mObjToWorld);
   glScalef(mObjScale.x, mObjScale.y, mObjScale.z);
   ShapeBase::wireCube(mObjBox.max, Point3F(0, 0, 0));
   glPopMatrix();
}

void fxDTSBrick::setTransform(const MatrixF& mat)
{
   MatrixF workingMatrix;
   AngAxisF workingAng;

   workingMatrix = mat;
   workingAng.set(workingMatrix);

   angleID = 0;
   if (workingAng.axis.z != 0.0)
   {
      if (1.1 <= workingAng.axis.z || workingAng.axis.z <= 0.9)
      {
         angleID = 3;
      }
      else
      {
         angleID = (1.5 <= workingAng.angle / 1.570796) + 1;
      }
   }

   Parent::setTransform(workingMatrix);
   setMaskBits(PositionMask);
}


// -------------------------------------------------------------------------------------
// net stuff

U32 fxDTSBrick::packUpdate(NetConnection* con, U32 mask, BitStream* stream)
{
   U32 retMask = Parent::packUpdate(con, mask, stream);

   if (stream->writeFlag((mask & DataBlockMask) && mDataBlock != NULL)) {
      stream->writeRangedU32(mDataBlock->getId(), DataBlockObjectIdFirst,DataBlockObjectIdLast);
   }

   // I think blockland uses something similar to writeCompressedPoint when dealing with
   // fxDTSBrick positions.
   
   // There's usage of fxDTSBrick::bitProfileXY and fxDTSBrick::bitProfileZ when the 
   // position mask is set, but it looks pretty horrifying... won't touch it for now.

   if (stream->writeFlag(mask & PositionMask))
   {
      mObjToWorld.getColumn(3, &worldPos);
      stream->writeCompressedPoint(worldPos);

      if (mDataBlock->hasBrickFile == 0)
      {
         stream->writeInt(angleID, 2);
      }
      else if (mDataBlock->brickSize.x != mDataBlock->brickSize.y)
      {
         if (angleID == 0 || angleID == 2)
            stream->writeFlag(0);
         else
            stream->writeFlag(1);
      }
   }

   return(retMask);
}

void fxDTSBrick::unpackUpdate(NetConnection* con, BitStream* stream)
{
   Parent::unpackUpdate(con, stream);

   // DataBlockMask
   if (stream->readFlag()) {
      fxDTSBrickData* dptr = 0;
      SimObjectId id = stream->readRangedU32(DataBlockObjectIdFirst,
         DataBlockObjectIdLast);

      if (!Sim::findObject(id, dptr) || !setDataBlock(dptr))
         con->setLastError("Invalid packet fxDTSBrick::unpackUpdate()");
   }

   // PositionMask
   if (stream->readFlag())
   {
      AngAxisF workingAng;
      MatrixF workingMatrix;
      workingMatrix = mObjToWorld;

      workingAng.set(workingMatrix);

      // Brick position
      stream->readCompressedPoint(&worldPos);
      workingMatrix.setPosition(worldPos);

      // Brick rotation
      if (mDataBlock->hasBrickFile == 0)
      {
         angleID = stream->readInt(2);
      }
      else if (mDataBlock->brickSize.x == mDataBlock->brickSize.y)
      {
         angleID = 0;
      }
      else
      {
         angleID = stream->readFlag() != 0;
      }

      workingAng.axis = Point3F(0.0, 0.0, 0.0);
      workingAng.angle = 0.0;

      if (angleID == 1)
      {
         workingAng.axis.z = 1.0;
         workingAng.angle = 1.5708;
      }
      else if (angleID == 0)
      {
         workingAng.axis.x = 1.0;
         workingAng.angle = 0.0;
      }
      else if (angleID == 2)
      {
         workingAng.axis.z = 1.0;
         workingAng.angle = 3.14159;
      }
      else if (angleID == 3)
      {
         workingAng.axis.z = -1.0;
         workingAng.angle = 1.5708;
      }

      workingAng.setMatrix(&workingMatrix);
      workingMatrix.setPosition(worldPos);
      setTransform(workingMatrix);
   }
}


// -------------------------------------------------------------------------------------
// datablock data processing stuff

bool fxDTSBrick::onNewDataBlock(fxDTSBrickData* dptr)
{
   mDataBlock = dynamic_cast<fxDTSBrickData*>(dptr);
   if (mDataBlock != NULL)
   {
      topArea    = mDataBlock->topArea;
      bottomArea = mDataBlock->bottomArea;
      northArea  = mDataBlock->northArea;
      eastArea   = mDataBlock->eastArea;
      southArea  = mDataBlock->southArea;
      westArea   = mDataBlock->westArea;

      if (topArea == 0)
         topArea = 9999;

      if (bottomArea == 0)
         bottomArea = 9999;

      mObjBox.min = -mDataBlock->brickDimensions;
      mObjBox.max = mDataBlock->brickDimensions;
      
      SceneObject::resetWorldBox();
      setTransform(mObjToWorld);
      return true;
   }
   Con::errorf("ERROR: fxDTSBrick::onNewDataBlock() - invalid datablock");
   return false;
}


bool fxDTSBrick::setDataBlock(fxDTSBrickData* dptr)
{
   if (isGhost() || isProperlyAdded()) {
      if (mDataBlock != dptr) {
         // this lets clients pick up on mDataBlock's data
         setMaskBits(DataBlockMask);
         return onNewDataBlock(dptr);
      }
   }
   else
      mDataBlock = dptr;
   return true;
}



// --------------------------------------------
// fxDTSBrick console methods
// --------------------------------------------
ConsoleMethod(fxDTSBrick, getDataBlock, S32, 2, 2, "()Return the datablock this fxBrickDB is using.")
{
   if (object->mDataBlock != NULL)
      return object->mDataBlock->getId();
}

ConsoleMethod(fxDTSBrick, setDataBlock, bool, 3, 3, "(datablock)")
{
   fxDTSBrickData* data;
   if (Sim::findObject(argv[2], data)) {
      return object->setDataBlock(data);
   }
   Con::errorf("Could not find data block \"%s\"", argv[2]);
   return false;
}

ConsoleMethod(fxDTSBrick, getAngleID, S32, 2, 2, "()")
{
   return object->angleID;
}

// --------------------------------------------
// Functions to help with the spray can GUI
// --------------------------------------------

ConsoleFunction(setSprayCanDivision, void, 4, 4, "(int index, int slot, string name)")
{
   S32 index = dAtoi(argv[1]);
   S32 slot  = dAtoi(argv[2]);

   if (index < 16)
   {
      if (slot < 64)
      {
         StringTableEntry name = StringTable->insert(argv[3], false);
         fxDTSBrick::sprayCanDivisionName[index] = name;
         fxDTSBrick::sprayCanDivisionSlot[index] = slot;
         return;
      }
      Con::errorf("Error: setSprayCanDivision() - index out of range.");
   }
   else
   {
      Con::errorf("Error: setSprayCanDivision() - slot out of range.");
   }
   return;
}

ConsoleFunction(setColorTable, void, 3, 3, "(int colorID, string color)")
{
   S32 colorID = dAtoi(argv[1]);

   F32 r, g, b, a;

   if (colorID > 63)
   {
      Con::errorf("ERROR: setColorTable() - id out of range (%d)", colorID);
      return;
   }

   dSscanf(argv[2], "%f %f %f %f", &r, &g, &b, &a);

   fxDTSBrick::colorIDTable[colorID].red   = (r * 255.0) / 255.0;
   fxDTSBrick::colorIDTable[colorID].green = (g * 255.0) / 255.0;
   fxDTSBrick::colorIDTable[colorID].blue  = (b * 255.0) / 255.0;
   fxDTSBrick::colorIDTable[colorID].alpha = (a * 255.0) / 255.0;
   return;
}

ConsoleFunction(getSprayCanDivisionName, const char*, 2, 2, "(int index)")
{
   S32 index = dAtoi(argv[1]);

   if (index < 16)
      return fxDTSBrick::sprayCanDivisionName[index];

   Con::errorf("Error: getSprayCanDivisionName() - index out of range.");
   return "";
}

ConsoleFunction(getSprayCanDivisionSlot, S32, 2, 2, "(int index)")
{
   S32 index = dAtoi(argv[1]);

   if (index < 16)
      return fxDTSBrick::sprayCanDivisionSlot[index];

   Con::errorf("Error: getSprayCanDivisionSlot() - index out of range.");
   return 0;
}

ConsoleFunction(getColorIDTable, const char*, 2, 2, "(int index)")
{
   char* buf = Con::getReturnBuffer(256);
   S32 index = dAtoi(argv[1]);
   F32 r, g, b, a;

   if (index <= 64)
   {
      ColorF color = fxDTSBrick::colorIDTable[index];
      r = color.red;
      g = color.green;
      b = color.blue;
      a = color.alpha;
   }
   else
   {
      Con::errorf("");
      Con::errorf("Error: getColorIDTable() - index \'%d\' out of range");
      Con::evaluatef("backtrace();");
      Con::errorf("");
      r = 0.0;
      g = 0.0;
      b = 0.0;
      a = 0.0;
   }
   dSprintf(buf, 256, "%f %f %f %f", r, g, b, a);
   return buf;
}