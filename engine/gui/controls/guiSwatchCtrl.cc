//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "console/console.h"
#include "gui/controls/guiSwatchCtrl.h"
#include "dgl/dgl.h"
#include "console/consoleTypes.h"

IMPLEMENT_CONOBJECT(GuiSwatchCtrl);

//--------------------------------------------------------------------------
GuiSwatchCtrl::GuiSwatchCtrl() {
	mColor = ColorI(255, 255, 255, 255);
}

//--------------------------------------------------------------------------
void GuiSwatchCtrl::initPersistFields()
{
    Parent::initPersistFields();
    addField("color", TypeColorI, Offset(mColor, GuiSwatchCtrl));
}

//--------------------------------------------------------------------------
void GuiSwatchCtrl::onRender(Point2I offset, const RectI &updateRect)
{
   dglDrawRectFill(updateRect, mColor);
   renderChildControls(offset, updateRect);
}

//--------------------------------------------------------------------------
void GuiSwatchCtrl::setColor(ColorI color)
{
    mColor = color;
}

//--------------------------------------------------------------------------
ColorI GuiSwatchCtrl::getColor()
{
    return mColor;
}

//--------------------------------------------------------------------------
ConsoleMethod(GuiSwatchCtrl, setColor, void, 3, 3, "color") {
    F32 r, g, b, a;
    dSscanf(argv[2], "%f %f %f %f", &r, &g, &b, &a);

    U8 red   = static_cast<U8>(mClampF(r, 0.0f, 1.0f) * 255.0f);
    U8 green = static_cast<U8>(mClampF(g, 0.0f, 1.0f) * 255.0f);
    U8 blue  = static_cast<U8>(mClampF(b, 0.0f, 1.0f) * 255.0f);
    U8 alpha = static_cast<U8>(mClampF(a, 0.0f, 1.0f) * 255.0f);

    ColorI color(red, green, blue, alpha);
    object->setColor(color);
}

//--------------------------------------------------------------------------
ConsoleMethod(GuiSwatchCtrl, getColor, const char*, 2, 2, "") {
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