#ifndef _GUISTACKCTRL_H_
#define _GUISTACKCTRL_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

#include "dgl/dgl.h"
#include "console/console.h"
#include "console/consoleTypes.h"

/// A vertical stack of GUI controls.
///
/// This maintains a vertical stack of GUI controls. If one is deleted, or
/// resized, then the stack is resized to fit. The order of the stack is
/// determined by the internal order of the children (ie, order of addition).
///
/// Children set their own height but are constrained to the width of the
/// stack control.
///
/// @todo Make this support horizontal stacks.
class GuiStackControl : public GuiControl
{
private:
   typedef GuiControl Parent;
   bool             mResizing;
   bool             mStackFromBottom;
   F32              mPadding;

public:
   GuiStackControl();

   void resize(const Point2I &newPosition, const Point2I &newExtent);
   void childResized(GuiControl *child);

   bool onWake();
   void onSleep();

   void updatePanes();

   S32 getCount() { return size(); }; /// Returns the number of children in the stack

   void addObject(SimObject *obj);

   static void initPersistFields();
   DECLARE_CONOBJECT(GuiStackControl);
};

#endif