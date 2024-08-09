// ----------------------------------------------------------------
// fxDTSBrickData
// Handles brick metadata
// ----------------------------------------------------------------

#include "game/fx/fxDTSBrickData.h"
#include "console/consoleTypes.h"
#include "game/gameBase.h"
#include "core/bitStream.h"
#include "ts/tsShapeInstance.h"

IMPLEMENT_CO_DATABLOCK_V1(fxDTSBrickData);

fxDTSBrickData::fxDTSBrickData()
{
   collisionShapeName  = "";
   brickFile           = "";

   // temp name: could potentially be a bool for having a .blb file, but all bricks should have blb files...
   hasBrickFile        = false;
   
   isWaterBrick        = false;
                       
   brickSize           = Point3I(0, 0, 0);

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

   addField("brickSizeX",          TypeS32,        Offset(brickSize.x,          fxDTSBrickData));
   addField("brickSizeY",          TypeS32,        Offset(brickSize.y,          fxDTSBrickData));
   addField("brickSizeZ",          TypeS32,        Offset(brickSize.z,          fxDTSBrickData));

   addField("indestructable",      TypeBool,       Offset(indestructable,      fxDTSBrickData));
   addField("alwaysShowWireFrame", TypeBool,       Offset(alwaysShowWireFrame, fxDTSBrickData));
   addField("isWaterBrick",        TypeBool,       Offset(isWaterBrick,        fxDTSBrickData));

   endGroup("fxDTSBrickData Stuff");
}

// this networking stuff is real messy at this point in time, don't mind it...
void fxDTSBrickData::packData(BitStream* stream)
{
   Parent::packData(stream);

   char buf[90];

   // brick size
   stream->writeInt(brickSize.x + -1, 6);
   stream->writeInt(brickSize.y + -1, 6);
   stream->writeInt(brickSize.z + -1, 8);

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

      // path minus /, .dts
      U32 len = dStrlen(path) - 5;
      dStrncpy(buf, path + 1, len);
      buf[len] = 0;

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
   brickSize.x = stream->readInt(6) + 1;
   brickSize.y = stream->readInt(6) + 1;
   brickSize.z = stream->readInt(8) + 1;

   // unknown purpose, brickDimensions is currently just a guess name...
   brickDimensions.x = (this->brickSize.x + 1) / 5.0;
   brickDimensions.y = this->brickSize.y * 0.5;
   brickDimensions.z = this->brickSize.z * 0.5;

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


// loads in data from the .blb file and sets brick attributes

bool fxDTSBrickData::preload(bool server, char errorBuffer[256])
{
   if (!Parent::preload(server, errorBuffer))
      return false;

   // stream->readLine wants unsigned chars?
   U8 buf[256];

   // load the collision shape as a resource
   if (collisionShapeName && collisionShapeName[0] != '\0')
   {
      Resource<TSShape> result = ResourceManager->load(collisionShapeName);

      collisionShape = result;

      if (!bool(collisionShape))
      {
         dSprintf(errorBuffer, sizeof(errorBuffer), "fxDTSBrickData: Couldn\'t load shape \"%s\"", collisionShapeName);
         return false;
      }

      // pretty sure this is the same as in shapeBase (TGE 1.2)
      // gathers collision details
      for (S32 i = 0; i < 8; i++) {
         char buff[128];
         dSprintf(buff, sizeof(buff), "Collision-%d", i + 1);
         collisionDetails[i] = collisionShape->findDetail(buff);

         if (collisionDetails[i] != -1) {
            collisionShape->computeBounds(collisionDetails[i], collisionBounds[i]);
            collisionShape->getAccelerator(collisionDetails[i]);

            if (!collisionShape->bounds.isContained(collisionBounds[i]))
            {
               if (Con::getBoolVariable("$Pref::Debug::ShowShapeMessages", false))
                  Con::warnf("Warning: shape %s collision detail %d (Collision-%d) bounds exceed that of shape.", collisionShapeName, i, collisionDetails[i]);
               
               collisionBounds[i] = collisionShape->bounds;
            }
            else if (collisionBounds[i].isValidBox() == false)
            {
               Con::errorf("Error: shape %s-collision detail %d (Collision-%d) bounds box invalid!", collisionShapeName, i, collisionDetails[i]);
               collisionBounds[i] = collisionShape->bounds;
            }
         }

         dSprintf(buff, sizeof(buff), "LOS-%d", i + 1 + 8);
         if ((LOSDetails[i] = collisionShape->findDetail(buff)) == -1)
            LOSDetails[i] = collisionDetails[i];
      }
   }


   if (brickFile == NULL || brickFile[0] == '\0')
   {
      dSprintf(errorBuffer, sizeof(errorBuffer), "fxDTSBrickData: No brick file file defined!");
      return false;
   }

   Stream* stream = ResourceManager->openStream(brickFile);

   if (stream != NULL)
   {
      F32 dimensions = this->brickSize.x * this->brickSize.y * this->brickSize.z;
      unk_point_1.x = this->brickSize.x * 0.5;
      unk_point_1.y = this->brickSize.y * 0.5;
      unk_point_1.z = this->brickSize.z / 5.0;
      brickDimensions = unk_point_1 * 0.5;

      // brick dimensions
      stream->readLine(buf, sizeof(buf));
      dSscanf((const char*)buf, "%d %d %d", &this->brickSize.x, &this->brickSize.y, &this->brickSize.z);

      // read brick type
      stream->readLine(buf, sizeof(buf));

      if (dStrncmp((const char*)buf, "BRICK", 5) != 0)
      {
         // special bricks (things that use .dts models, prints, etc) need to handle
         // parsing data such as brick placement coverage, UV coordinates etc.
         // let's just do nothing here for now... not worth doing anything until we
         // have an actual brick rendering system
      }
      else
      {
         // standard bricks have nothing but their coordinates in their file
         hasBrickFile = true;
      }
   }

   return true;
}