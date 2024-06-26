//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "gui/containers/guiStackCtrl.h"

IMPLEMENT_CONOBJECT(GuiStackControl);

GuiStackControl::GuiStackControl()
{
   mMinExtent.set(60, 60);
   mResizing = false;
   mStackFromBottom = false;
   mPadding = 0;
}

void GuiStackControl::initPersistFields()
{
  Parent::initPersistFields();

  addField("stackFromBottom", TypeBool, Offset(mStackFromBottom, GuiStackControl));
  addField("padding", TypeF32, Offset(mPadding, GuiStackControl));
}

bool GuiStackControl::onWake()
{
   if ( !Parent::onWake() )
      return false;

   updatePanes();

   return true;
}

void GuiStackControl::onSleep()
{
   Parent::onSleep();
}

void GuiStackControl::updatePanes()
{
   // Prevent recursion
   if(mResizing) return;

   mResizing = true;

   // Store the sum of the heights of our controls.
   S32 totalHeight=0;

   Point2I curPos;

   if(mStackFromBottom)
   {
      // If we go from the bottom, things are basically the same...
      // except we have to assign positions in an arse backwards way
      // (literally :P)

      // Figure out how high everything is going to be...
      // Place each child...
      for(S32 i=0; i<size(); i++)
      {
         // Place control
         GuiControl * gc = dynamic_cast<GuiControl*>(operator [](i));

         if(gc)
         {
            Point2I childExtent = gc->getExtent();
            totalHeight += childExtent.y;
         }
      }

      // Figure out where we're starting...
      curPos = getPosition();
      curPos.y += totalHeight;

      // Offset so the bottom control goes in the right place...
      GuiControl * gc = dynamic_cast<GuiControl*>(operator [](size()-1));
      if(gc)
         curPos.y -= gc->getExtent().y;


      // Now work up from there!
      for(S32 i=size()-1; i>=0; i--)
      {
         // Place control
         GuiControl * gc = dynamic_cast<GuiControl*>(operator [](i));

         if(gc)
         {
            // We must place the child...

            // Make it have our width but keep its height
            Point2I childExtent = gc->getExtent();

            // Update our state...
            curPos.y -= childExtent.y - mPadding;

            // And resize...
            gc->resize(curPos - getPosition(), Point2I(getExtent().x, childExtent.y));
         }
      }
   }
   else
   {
      // Position and resize everything...
      curPos = getPosition();

      // Place each child...
      for(S32 i=0; i<size(); i++)
      {
         // Place control
         GuiControl * gc = dynamic_cast<GuiControl*>(operator [](i));

         if(gc)
         {
            // We must place the child...

            // Make it have our width but keep its height
            Point2I childExtent = gc->getExtent();

            gc->resize(curPos - getPosition(), Point2I(getExtent().x, childExtent.y));

            // Update our state...
            curPos.y    += childExtent.y + mPadding;
            totalHeight += childExtent.y + mPadding;
         }
      }

      // Conform our size to the sum of the child sizes.
      curPos.x = getExtent().x;
      curPos.y = totalHeight;
      resize(getPosition(), curPos);
   }

   mResizing = false;
}

void GuiStackControl::resize(const Point2I &newPosition, const Point2I &newExtent)
{
   //call set update both before and after
   setUpdate();
   Point2I actualNewExtent = Point2I(  getMax(mMinExtent.x, newExtent.x),
                                       getMax(mMinExtent.y, newExtent.y));
   mBounds.set(newPosition, actualNewExtent);

   GuiControl *parent = getParent();
   if (parent)
      parent->childResized(this);
   setUpdate();

   updatePanes();
}

void GuiStackControl::addObject(SimObject *obj)
{
   Parent::addObject(obj);

   updatePanes();
}

void GuiStackControl::childResized(GuiControl *child)
{
   updatePanes();
}