#ifndef _GUI_DIRECTORYFILELISTCTRL_H_
#define _GUI_DIRECTORYFILELISTCTRL_H_

#ifndef _PLATFORM_H_
#include "platform/platform.h"
#endif

#ifndef _GUITEXTLISTCTRL_H_
#include "gui/controls/guiTextListCtrl.h"
#endif

class GuiDirectoryFileListCtrl : public GuiTextListCtrl
{
private:
   typedef GuiTextListCtrl Parent;
protected:
   StringTableEntry mFilePath;
   StringTableEntry mFilter;

   void openDirectory();
public:
   GuiDirectoryFileListCtrl();
   DECLARE_CONOBJECT(GuiDirectoryFileListCtrl);

   /// Set the current path to grab files from
   bool setCurrentPath( StringTableEntry path, StringTableEntry filter );
   bool setCurrentFilter( StringTableEntry filter );

   /// Get the currently selected file's name
   StringTableEntry getSelectedFileName();


   virtual void onMouseDown(const GuiEvent &event);


   bool onWake();
};


#endif
