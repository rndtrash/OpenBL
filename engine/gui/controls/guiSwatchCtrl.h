//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _GUISWATCHCTRL_H_
#define _GUISWATCHCTRL_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

class GuiSwatchCtrl : public GuiControl
{
private:
   typedef GuiControl Parent;

protected:
    ColorI mColor;

public:
   DECLARE_CONOBJECT(GuiSwatchCtrl);
   GuiSwatchCtrl();

   static void initPersistFields();

   void onRender(Point2I offset, const RectI &updateRect);
   void setColor(ColorI color);
   ColorI getColor();
};

#endif
