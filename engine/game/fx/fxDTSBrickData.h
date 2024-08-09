#ifndef _FXDTSBRICKDATA_H_
#define _FXDTSBRICKDATA_H_

#ifndef _GAMEBASE_H_
#include "game/gameBase.h"
#endif

#ifndef _TSSHAPEINSTANCE_H
#include "ts/tsShapeInstance.h"
#endif

class fxDTSBrickData : public GameBaseData
{
public:
   typedef GameBaseData Parent;

   DECLARE_CONOBJECT(fxDTSBrickData);

   fxDTSBrickData();

   StringTableEntry collisionShapeName;
   StringTableEntry brickFile;

   bool isWaterBrick;

   Point3I brickSize;

   S32 topArea;
   S32 bottomArea;
   S32 northArea;
   S32 eastArea;
   S32 southArea;
   S32 westArea;

   S32 canCoverTop;
   S32 canCoverBottom;
   S32 canCoverNorth;
   S32 canCoverEast;
   S32 canCoverSouth;
   S32 canCoverWest;
   S32 hasPrint;

   bool indestructable;
   bool alwaysShowWireFrame;

   StringTableEntry category;
   StringTableEntry subCategory;
   StringTableEntry uiName;
   StringTableEntry iconName;

   bool hasBrickFile;

   Point3F brickDimensions;
   Point3F unk_point_1;

   // Collision
   Resource<TSShape> collisionShape;
   TSShapeInstance*  collisionShapeInstance;

   S32   collisionDetails[8];
   Box3F collisionBounds[8]; 
   S32   LOSDetails[8];    

   static void initPersistFields();

   void packData(BitStream* stream);
   void unpackData(BitStream* stream);

   bool preload(bool server, char errorBuffer[256]);
};

DECLARE_CONSOLETYPE(fxDTSBrickData)

#endif // _FXDTSBRICKDATA_H_
