// ----------------------------------------------------------------
// fxDTSBrickData
// Handles brick metadata
// ----------------------------------------------------------------

#include "game/fx/fxDTSBrickData.h"
#include "console/consoleTypes.h"
#include "game/gameBase.h"
#include "core/bitStream.h"

IMPLEMENT_CO_DATABLOCK_V1(fxDTSBrickData);

fxDTSBrickData::fxDTSBrickData()
{
   collisionShapeName  = "";
   brickFile           = "";

   // temp name: could potentially be a bool for having a .blb file, but all bricks should have blb files...
   hasBrickFile        = false;
   
   isWaterBrick        = false;
                       
   brickSizeX          = 0;
   brickSizeY          = 0;
   brickSizeZ          = 0;

   brickDimensions     = Point3F(0.0, 0.0, 0.0);
                       
   topArea             = 0;
   bottomArea          = 0;
   northArea           = 0;
   eastArea            = 0;
   southArea           = 0;
   westArea            = 0;
                       
   canCoverTop         = true;
   canCoverBottom      = true;
   canCoverNorth       = true;
   canCoverEast        = true;
   canCoverSouth       = true;
   canCoverWest        = true;
                       
   hasPrint            = 0;
   category            = "";
   subCategory         = "";
   uiName              = "";
   iconName            = "";

   indestructable      = false;
   alwaysShowWireFrame = false;
}

void fxDTSBrickData::initPersistFields()
{
   addGroup("fxDTSBrickData Stuff");

   addField("collisionShapeName",  TypeFilename,   Offset(collisionShapeName,  fxDTSBrickData));
   addField("brickFile",           TypeFilename,   Offset(brickFile,           fxDTSBrickData));

   addField("topArea",             TypeS32,        Offset(topArea,             fxDTSBrickData));
   addField("bottomArea",          TypeS32,        Offset(bottomArea,          fxDTSBrickData));
   addField("northArea",           TypeS32,        Offset(northArea,           fxDTSBrickData));
   addField("eastArea",            TypeS32,        Offset(eastArea,            fxDTSBrickData));
   addField("southArea",           TypeS32,        Offset(southArea,           fxDTSBrickData));
   addField("westArea",            TypeS32,        Offset(westArea,            fxDTSBrickData));

   addField("canCoverTop",         TypeBool,       Offset(canCoverTop,         fxDTSBrickData));
   addField("canCoverBottom",      TypeBool,       Offset(canCoverBottom,      fxDTSBrickData));
   addField("canCoverNorth",       TypeBool,       Offset(canCoverNorth,       fxDTSBrickData));
   addField("canCoverEast",        TypeBool,       Offset(canCoverEast,        fxDTSBrickData));
   addField("canCoverSouth",       TypeBool,       Offset(canCoverSouth,       fxDTSBrickData));
   addField("canCoverWest",        TypeBool,       Offset(canCoverWest,        fxDTSBrickData));

   addField("hasPrint",            TypeBool,       Offset(hasPrint,            fxDTSBrickData));
   addField("category",            TypeFilename,   Offset(category,            fxDTSBrickData));
   addField("subCategory",         TypeFilename,   Offset(subCategory,         fxDTSBrickData));
   addField("uiName",              TypeCaseString, Offset(uiName,              fxDTSBrickData));
   addField("iconName",            TypeFilename,   Offset(iconName,            fxDTSBrickData));

   addField("brickSizeX",          TypeS32,        Offset(brickSizeX,          fxDTSBrickData));
   addField("brickSizeY",          TypeS32,        Offset(brickSizeY,          fxDTSBrickData));
   addField("brickSizeZ",          TypeS32,        Offset(brickSizeZ,          fxDTSBrickData));

   addField("indestructable",      TypeBool,       Offset(indestructable,      fxDTSBrickData));
   addField("alwaysShowWireFrame", TypeBool,       Offset(alwaysShowWireFrame, fxDTSBrickData));
   addField("isWaterBrick",        TypeBool,       Offset(isWaterBrick,        fxDTSBrickData));

   endGroup("fxDTSBrickData Stuff");
}

void fxDTSBrickData::packData(BitStream* stream)
{
   Parent::packData(stream);

   char buf[90];

   // brick size
   stream->writeInt(brickSizeX + -1, 6);
   stream->writeInt(brickSizeY + -1, 6);
   stream->writeInt(brickSizeZ + -1, 8);

   // .blb file path
   if (!stream->writeFlag(hasBrickFile))
      stream->writeString(brickFile);

   // .dts file path
   if (collisionShapeName == "" || (dStrlen(collisionShapeName) == 0) ||  (dStrncmp("base/data/shapes/bricks/", collisionShapeName, dStrlen("base/data/shapes/bricks")) != 0))
   {
      stream->writeFlag(0);
      stream->writeString(collisionShapeName);
   }
   else
   {
      stream->writeFlag(true);

      const char* path = dStrrchr(collisionShapeName, '/');
      dStrncpy(buf, path + 1, dStrlen(path) - 5);
      buf[dStrlen(path)] = 0;

      stream->writeString(buf);
   }

   // categorization metadata
   stream->writeString(category);
   stream->writeString(subCategory);
   stream->writeString(uiName);
   
   // brick icons
   if (iconName != "" && dStrlen(iconName) != 0 && dStrncmp("base/client/ui/brickIcons/", iconName, dStrlen("base/client/ui/brickIcons")) == 0)
   {
      stream->writeFlag(true);

      const char* path = dStrrchr(iconName, '/');
      dStrncpy(buf, path + 1, dStrlen(path));
      buf[dStrlen(path)] = 0;

      stream->writeString(buf);
      stream->writeFlag(alwaysShowWireFrame);
      stream->writeFlag(hasPrint);
      return;
   }

   stream->writeFlag(false);
   stream->writeString(iconName);
   stream->writeFlag(alwaysShowWireFrame);
   stream->writeFlag(hasPrint);
}


void fxDTSBrickData::unpackData(BitStream* stream)
{
   Parent::packData(stream);

   char buf[256];
   char buf2[256];

   // brick sizes
   brickSizeX = stream->readInt(6) + 1;
   brickSizeY = stream->readInt(6) + 1;
   brickSizeZ = stream->readInt(8) + 1;

   // unknown purpose, brickDimensions is currently just a guess name...
   brickDimensions.x = (this->brickSizeX + 1) / 5.0;
   brickDimensions.y = this->brickSizeY * 0.5;
   brickDimensions.z = this->brickSizeZ * 0.5;

   // unknown Point3F
   unk_point_1 = brickDimensions * 0.5;

   // .blb file path
   hasBrickFile = stream->readFlag();
   if (hasBrickFile == 0)
      brickFile = stream->readSTString(false);

   // .dts file path
   if (!stream->readFlag())
   {
      collisionShapeName = stream->readSTString(false);
   }
   else
   {
      stream->readString(buf);
      dSprintf(buf2, 256, "base/data/shapes/bricks/%s.dts", buf);
      collisionShapeName = StringTable->insert(buf2, false);
   }

   // categorization metadata
   category = stream->readSTString(false);
   subCategory = stream->readSTString(false);
   uiName = stream->readSTString(false);

   // icon file path
   if (!stream->readFlag())
   {
       iconName = stream->readSTString(false);
   }
   else
   {
      stream->readString(buf2);
      dSprintf(buf, 256, "base/client/ui/brickIcons/%s", buf2);
      iconName = StringTable->insert(buf, false);
   }

   alwaysShowWireFrame = stream->readFlag();
   hasPrint            = stream->readFlag();
}
