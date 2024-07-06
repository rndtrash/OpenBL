//-----------------------------------------------------------------------------
// Torque Game Engine
//
// Copyright (c) 2001 GarageGames.Com
//-----------------------------------------------------------------------------

#include "console/console.h"
#include "console/consoleTypes.h"
#include "dgl/dgl.h"

#include "gui/controls/guiBitmapCtrl.h"

class GuiFadeinBitmapCtrl : public GuiBitmapCtrl
{
   typedef GuiBitmapCtrl Parent;
public:
   U32 wakeTime;
   bool done;
   bool wait;
   U32 fadeinTime;
   U32 waitTime;
   U32 fadeoutTime;

   GuiFadeinBitmapCtrl()
   {
      wakeTime    = 0;
      fadeinTime  = 1000;
      waitTime    = 2000;
      fadeoutTime = 1000;
      done        = false;
      wait        = false;
   }
   void onPreRender()
   {
      Parent::onPreRender();
      setUpdate();
   }
   void onMouseDown(const GuiEvent &)
   {
      Con::executef(this, 1, "click");
   }
   bool onKeyDown(const GuiEvent &)
   {
      Con::executef(this, 1, "click");
      return true;
   }
   DECLARE_CONOBJECT(GuiFadeinBitmapCtrl);
   bool onWake()
   {
      if(!Parent::onWake())
         return false;
      wakeTime = Platform::getRealMilliseconds();
      done = false;
      wait = false;
      return true;
   }
   void onRender(Point2I offset, const RectI &updateRect)
   {
      Parent::onRender(offset, updateRect);
      U32 elapsed = Platform::getRealMilliseconds() - wakeTime;

      U32 alpha;
      if (elapsed < fadeinTime)
      {
          // fade out
          alpha = 255 * F32(elapsed) / F32(fadeinTime);
      }
      else if (elapsed < (fadeinTime+waitTime))
      {
         // wait
         if (!wait)
         {
            Con::executef(this, 1, "onWait");
            wait = true;
         }
         alpha = 0;
      }
      else if (elapsed < (fadeinTime+waitTime+fadeoutTime))
      {
         // fade-in
         elapsed -= (fadeoutTime + waitTime);
         alpha = (F32(elapsed) / F32(fadeoutTime)) * -255.0 + 255.0;
      }
      else
      {
         // done state
         if (!done)
         {
             done = true;
             Con::executef(this, 1, "onDone");
         }
         alpha = fadeoutTime ? 255 : 0;
      }
      mColor = ColorI(255, 255, 255, alpha);
   }
   static void initPersistFields()
   {
      Parent::initPersistFields();
      addField("fadeinTime", TypeS32, Offset(fadeinTime, GuiFadeinBitmapCtrl));
      addField("waitTime", TypeS32, Offset(waitTime, GuiFadeinBitmapCtrl));
      addField("fadeoutTime", TypeS32, Offset(fadeoutTime, GuiFadeinBitmapCtrl));
      addField("done", TypeBool, Offset(done, GuiFadeinBitmapCtrl));
   }
};

ConsoleMethod(GuiFadeinBitmapCtrl, reset, bool, 2, 2, "") {
    object->wakeTime = Platform::getRealMilliseconds();
    object->done = false;
    object->wait = false;
    return true;
}

IMPLEMENT_CONOBJECT(GuiFadeinBitmapCtrl);

























