#ifndef _FXDTSBRICKDATA_H_
#define _FXDTSBRICKDATA_H_

#ifndef _GAMEBASE_H_
#include "game/gameBase.h"
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

   S32 brickSizeX;
   S32 brickSizeY;
   S32 brickSizeZ;

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

   static void initPersistFields();

   void packData(BitStream* stream);
   void unpackData(BitStream* stream);
};

DECLARE_CONSOLETYPE(fxDTSBrickData)

#endif // _FXDTSBRICKDATA_H_
