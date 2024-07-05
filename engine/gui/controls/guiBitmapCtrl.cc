//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "console/console.h"
#include "console/consoleTypes.h"
#include "dgl/dgl.h"

#include "gui/controls/guiBitmapCtrl.h"

IMPLEMENT_CONOBJECT(GuiBitmapCtrl);

GuiBitmapCtrl::GuiBitmapCtrl(void)
{
   mBitmapName = StringTable->insert("");
	startPoint.set(0, 0);
	mWrap = false;
    mColor = ColorI(255, 255, 255, 255);
    mLockAspectRatio = 0;
    mOverflowImage = 0;
    mAlignLeft = 0;
}


void GuiBitmapCtrl::initPersistFields()
{
   Parent::initPersistFields();
   addGroup("Misc");		
   addField("bitmap", TypeFilename, Offset(mBitmapName, GuiBitmapCtrl));
   addField("wrap",   TypeBool,     Offset(mWrap,       GuiBitmapCtrl));
   addField("lockAspectRatio", TypeBool, Offset(mLockAspectRatio, GuiBitmapCtrl)); // not implemented yet
   addField("alignLeft", TypeBool, Offset(mAlignLeft, GuiBitmapCtrl));             // not implemented yet
   addField("overflowImage", TypeBool, Offset(mOverflowImage, GuiBitmapCtrl));     // not implemented yet
   addField("mColor", TypeColorI, Offset(mColor, GuiBitmapCtrl));
   endGroup("Misc");		
}

ConsoleMethod( GuiBitmapCtrl, setValue, void, 4, 4, "(int xAxis, int yAxis)"
              "Set the offset of the bitmap.")
{
   object->setValue(dAtoi(argv[2]), dAtoi(argv[3]));
}

ConsoleMethod( GuiBitmapCtrl, setBitmap, void, 3, 3, "(string filename)"
              "Set the bitmap displayed in the control. Note that it is limited in size, to 256x256.")
{
   object->setBitmap(argv[2]);
}

bool GuiBitmapCtrl::onWake()
{
   if (! Parent::onWake())
      return false;
   setActive(true);
   setBitmap(mBitmapName);
   return true;
}

void GuiBitmapCtrl::onSleep()
{
   mTextureHandle = NULL;
   Parent::onSleep();
}

//-------------------------------------
void GuiBitmapCtrl::inspectPostApply()
{
   // if the extent is set to (0,0) in the gui editor and appy hit, this control will
   // set it's extent to be exactly the size of the bitmap (if present)
   Parent::inspectPostApply();

   if (!mWrap && (mBounds.extent.x == 0) && (mBounds.extent.y == 0) && mTextureHandle)
   {
      TextureObject *texture = (TextureObject *) mTextureHandle;
      mBounds.extent.x = texture->bitmapWidth;
      mBounds.extent.y = texture->bitmapHeight;
   }
}

void GuiBitmapCtrl::setBitmap(const char *name, bool resize)
{
   mBitmapName = StringTable->insert(name);
   if (*mBitmapName) {
      mTextureHandle = TextureHandle(mBitmapName, BitmapTexture, true);

      // Resize the control to fit the bitmap
      if (resize) {
         TextureObject* texture = (TextureObject *) mTextureHandle;
         mBounds.extent.x = texture->bitmapWidth;
         mBounds.extent.y = texture->bitmapHeight;
         Point2I extent = getParent()->getExtent();
         parentResized(extent,extent);
      }
   }
   else
      mTextureHandle = NULL;
   setUpdate();
}


void GuiBitmapCtrl::setBitmap(const TextureHandle &handle, bool resize)
{
   mTextureHandle = handle;

   // Resize the control to fit the bitmap
   if (resize) {
      TextureObject* texture = (TextureObject *) mTextureHandle;
      mBounds.extent.x = texture->bitmapWidth;
      mBounds.extent.y = texture->bitmapHeight;
      Point2I extent = getParent()->getExtent();
      parentResized(extent,extent);
   }
}


void GuiBitmapCtrl::onRender(Point2I offset, const RectI &updateRect)
{
   if (mTextureHandle)
   {
      dglClearBitmapModulation();
      dglSetBitmapModulation(mColor);
		if(mWrap)
		{
 			TextureObject* texture = (TextureObject *) mTextureHandle;
			RectI srcRegion;
			RectI dstRegion;
			float xdone = ((float)mBounds.extent.x/(float)texture->bitmapWidth)+1;
			float ydone = ((float)mBounds.extent.y/(float)texture->bitmapHeight)+1;

			int xshift = startPoint.x%texture->bitmapWidth;
			int yshift = startPoint.y%texture->bitmapHeight;
			for(int y = 0; y < ydone; ++y)
				for(int x = 0; x < xdone; ++x)
				{
		 			srcRegion.set(0,0,texture->bitmapWidth,texture->bitmapHeight);
  					dstRegion.set( ((texture->bitmapWidth*x)+offset.x)-xshift,
								      ((texture->bitmapHeight*y)+offset.y)-yshift,
								      texture->bitmapWidth,	
								      texture->bitmapHeight);
   				dglDrawBitmapStretchSR(texture,dstRegion, srcRegion, false);
				}
		}
        else if (mLockAspectRatio)
        {
            RectI rect;
            TextureObject* texture = (TextureObject*)mTextureHandle;

            float extentAspectRatio = (float)mBounds.extent.x / (float)mBounds.extent.y;
            float bitmapAspectRatio = (float)texture->bitmapWidth / (float)texture->bitmapHeight;

            if (extentAspectRatio <= bitmapAspectRatio)
            {
                rect.extent.y = mBounds.extent.x / bitmapAspectRatio;
                rect.extent.x = mBounds.extent.x;
                rect.point.y = (mBounds.extent.y - rect.extent.y) * 0.5;
                rect.point.x = 0;
            }
            else
            {
                rect.extent.x = mBounds.extent.y * bitmapAspectRatio;
                rect.extent.y = mBounds.extent.y;
                rect.point.y = 0;
                rect.point.x = 0;
                if (!mAlignLeft)
                {
                    rect.point.x = (mBounds.extent.x - rect.extent.x) * 0.5;
                }
            }
            rect.point.y += offset.y;
            rect.point.x += offset.x;
            dglDrawBitmapStretch(texture, rect, 0);
        }
        else
        {
            RectI rect(offset, mBounds.extent);
            dglDrawBitmapStretch(mTextureHandle, rect);
        }
   }

   if (mProfile->mBorder || !mTextureHandle)
   {
      RectI rect(offset.x, offset.y, mBounds.extent.x, mBounds.extent.y);
      dglDrawRect(rect, mProfile->mBorderColor);
   }

   renderChildControls(offset, updateRect);
}

void GuiBitmapCtrl::setValue(S32 x, S32 y)
{
   if (mTextureHandle)
   {
		TextureObject* texture = (TextureObject *) mTextureHandle;
		x+=texture->bitmapWidth/2;
		y+=texture->bitmapHeight/2;
  	}
  	while (x < 0)
  		x += 256;
  	startPoint.x = x % 256;
  				
  	while (y < 0)
  		y += 256;
  	startPoint.y = y % 256;
}

void GuiBitmapCtrl::setColor(ColorI color)
{
    mColor = color;
}

ColorI GuiBitmapCtrl::getColor()
{
    return mColor;
}

ConsoleMethod(GuiBitmapCtrl, setColor, void, 3, 3, "color") {
    F32 r, g, b, a;
    dSscanf(argv[2], "%f %f %f %f", &r, &g, &b, &a);

    U8 red = static_cast<U8>(mClampF(r, 0.0f, 1.0f) * 255.0f);
    U8 green = static_cast<U8>(mClampF(g, 0.0f, 1.0f) * 255.0f);
    U8 blue = static_cast<U8>(mClampF(b, 0.0f, 1.0f) * 255.0f);
    U8 alpha = static_cast<U8>(mClampF(a, 0.0f, 1.0f) * 255.0f);

    ColorI color(red, green, blue, alpha);
    object->setColor(color);
}

ConsoleMethod(GuiBitmapCtrl, getColor, const char*, 2, 2, "") {
    char* temp = Con::getReturnBuffer(64);

    ColorF color = (ColorF)object->getColor();

    dSprintf(temp, 64, "%f %f %f %f",
        color.red,
        color.green,
        color.blue,
        color.alpha
    );

    return temp;
}