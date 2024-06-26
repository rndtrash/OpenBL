//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (c) 2002 GarageGames.Com
//-----------------------------------------------------------------------------

#include "dgl/dgl.h"
#include "gui/core/guiDefaultControlRender.h"
#include "gui/core/guiTypes.h"
#include "core/color.h"
#include "math/mRect.h"

static ColorI colorLightGray(192, 192, 192);
static ColorI colorGray(128, 128, 128);
static ColorI colorDarkGray(64, 64, 64);
static ColorI colorWhite(255,255,255);
static ColorI colorBlack(0,0,0);

void renderRaisedBox(RectI &bounds, GuiControlProfile *profile)
{
   S32 l = bounds.point.x, r = bounds.point.x + bounds.extent.x - 1;
	S32 t = bounds.point.y, b = bounds.point.y + bounds.extent.y - 1;

	dglDrawRectFill( bounds, profile->mFillColor);
   dglDrawLine(l, t, l, b - 1, profile->mBevelColorHL);
   dglDrawLine(l, t, r - 1, t, profile->mBevelColorHL);

   dglDrawLine(l, b, r, b, profile->mBevelColorLL);
   dglDrawLine(r, b - 1, r, t, profile->mBevelColorLL);

	dglDrawLine(l + 1, b - 1, r - 1, b - 1, profile->mBorderColor);
   dglDrawLine(r - 1, b - 2, r - 1, t + 1, profile->mBorderColor);
}

void renderSlightlyRaisedBox(RectI &bounds, GuiControlProfile *profile)
{
   S32 l = bounds.point.x, r = bounds.point.x + bounds.extent.x - 1;
	S32 t = bounds.point.y, b = bounds.point.y + bounds.extent.y - 1;

	dglDrawRectFill( bounds, profile->mFillColor);
   dglDrawLine(l, t, l, b, profile->mBevelColorHL);
   dglDrawLine(l, t, r, t, profile->mBevelColorHL);
	dglDrawLine(l + 1, b, r, b, profile->mBorderColor);
   dglDrawLine(r, t + 1, r, b - 1, profile->mBorderColor);
}

void renderLoweredBox(RectI &bounds, GuiControlProfile *profile)
{
   S32 l = bounds.point.x, r = bounds.point.x + bounds.extent.x - 1;
	S32 t = bounds.point.y, b = bounds.point.y + bounds.extent.y - 1;

	dglDrawRectFill( bounds, profile->mFillColor);

	dglDrawLine(l, b, r, b, profile->mBevelColorHL);
   dglDrawLine(r, b - 1, r, t, profile->mBevelColorHL);

   dglDrawLine(l, t, r - 1, t, profile->mBevelColorLL);
   dglDrawLine(l, t + 1, l, b - 1, profile->mBevelColorLL);

	dglDrawLine(l + 1, t + 1, r - 2, t + 1, profile->mBorderColor);
   dglDrawLine(l + 1, t + 2, l + 1, b - 2, profile->mBorderColor);
}

void renderSlightlyLoweredBox(RectI &bounds, GuiControlProfile *profile)
{
   S32 l = bounds.point.x, r = bounds.point.x + bounds.extent.x - 1;
	S32 t = bounds.point.y, b = bounds.point.y + bounds.extent.y - 1;

	dglDrawRectFill( bounds, profile->mFillColor);
   dglDrawLine(l, b, r, b, profile->mBevelColorHL);
   dglDrawLine(r, t, r, b - 1, profile->mBevelColorHL);
	dglDrawLine(l, t, l, b - 1, profile->mBorderColor);
   dglDrawLine(l + 1, t, r - 1, t, profile->mBorderColor);
}

void renderBorder(RectI &bounds, GuiControlProfile *profile)
{
   S32 l = bounds.point.x, r = bounds.point.x + bounds.extent.x - 1;
	S32 t = bounds.point.y, b = bounds.point.y + bounds.extent.y - 1;

   switch(profile->mBorder)
   {
      case 1:
         dglDrawRect(bounds, profile->mBorderColor);
         break;
      case 2:
         dglDrawLine(l + 1, t + 1, l + 1, b - 2, profile->mBevelColorHL);
         dglDrawLine(l + 2, t + 1, r - 2, t + 1, profile->mBevelColorHL);
         dglDrawLine(r, t, r, b, profile->mBevelColorHL);
         dglDrawLine(l, b, r - 1, b, profile->mBevelColorHL);
         dglDrawLine(l, t, r - 1, t, profile->mBorderColorNA);
         dglDrawLine(l, t + 1, l, b - 1, profile->mBorderColorNA);
         dglDrawLine(l + 1, b - 1, r - 1, b - 1, profile->mBorderColorNA);
         dglDrawLine(r - 1, t + 1, r - 1, b - 2, profile->mBorderColorNA);
         break;
      case 3:
         dglDrawLine(l, b, r, b, profile->mBevelColorHL);
         dglDrawLine(r, t, r, b - 1, profile->mBevelColorHL);
         dglDrawLine(l + 1, b - 1, r - 1, b - 1, profile->mFillColor);
         dglDrawLine(r - 1, t + 1, r - 1, b - 2, profile->mFillColor);
         dglDrawLine(l, t, l, b - 1, profile->mBorderColorNA);
         dglDrawLine(l + 1, t, r - 1, t, profile->mBorderColorNA);
         dglDrawLine(l + 1, t + 1, l + 1, b - 2, profile->mBevelColorLL);
         dglDrawLine(l + 2, t + 1, r - 2, t + 1, profile->mBevelColorLL);
         break;
      case 4:
         dglDrawLine(l, t, l, b - 1, profile->mBevelColorHL);
         dglDrawLine(l + 1, t, r, t, profile->mBevelColorHL);
         dglDrawLine(l, b, r, b, profile->mBevelColorLL);
         dglDrawLine(r, t + 1, r, b - 1, profile->mBevelColorLL);
         dglDrawLine(l + 1, b - 1, r - 1, b - 1, profile->mBorderColor);
         dglDrawLine(r - 1, t + 1, r - 1, b - 2, profile->mBorderColor);
         break;
      case 5:
         renderFilledBorder( bounds, profile );
         break;
   }
}

void renderFilledBorder( RectI &bounds, GuiControlProfile *profile )
{
   renderFilledBorder( bounds, profile->mBorderColorHL, profile->mFillColor );
}

void renderFilledBorder( RectI &bounds, ColorI &borderColor, ColorI &fillColor )
{
   S32 l = bounds.point.x, r = bounds.point.x + bounds.extent.x - 1;
   S32 t = bounds.point.y, b = bounds.point.y + bounds.extent.y - 1;

   RectI fillBounds = bounds;
   fillBounds.inset( 1, 1 );
   dglDrawRect( bounds, borderColor ); 
   dglDrawRectFill( fillBounds, fillColor );
}

