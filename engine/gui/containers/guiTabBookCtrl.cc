//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------
#include "console/console.h"
#include "console/consoleTypes.h"
#include "dgl/dgl.h"
#include "console/simBase.h"
#include "gui/core/guiCanvas.h"
#include "gui/containers/guiTabBookCtrl.h"
#include "platform/event.h"
#include "core/fileStream.h"
#include "gui/containers/guiScrollCtrl.h"
#include "gui/editor/guiEditCtrl.h"
#include "gui/controls/guiPopUpCtrl.h"


// So we can set tab alignment via gui editor
static EnumTable::Enums tabAlignEnums[] =
{
   { GuiTabBookCtrl::AlignTop,   "Top"    },
//   { GuiTabBookCtrl::AlignLeft,  "Left"   },
   { GuiTabBookCtrl::AlignBottom,"Bottom" },
//   { GuiTabBookCtrl::AlignRight,	"Right"  }
};
static EnumTable gTabAlignEnums(/*4*/2,&tabAlignEnums[0]);


IMPLEMENT_CONOBJECT(GuiTabBookCtrl);

GuiTabBookCtrl::GuiTabBookCtrl()
{
   VECTOR_SET_ASSOCIATION(mPages);
   mTabHeight = 24;
   mLastTabHeight = mTabHeight;
   mTabWidth = 64;
   mLastTabWidth = mTabWidth;
   mTabPosition = GuiTabBookCtrl::AlignTop;
   mLastTabPosition = mTabPosition;
   mActivePage = NULL;
   mHoverTab = NULL;
   mHasTexture = false;
   mBitmapBounds = NULL;
   mBounds.extent.set( 400, 300 );
   mPageRect = RectI(0,0,0,0);
   mTabRect = RectI(0,0,0,0);

}

void GuiTabBookCtrl::initPersistFields()
{
   Parent::initPersistFields();
   addField("TabPosition",		TypeEnum,		Offset(mTabPosition,GuiTabBookCtrl), 1, &gTabAlignEnums );
   addField("TabHeight",		TypeS32,		Offset(mTabHeight,GuiTabBookCtrl) );
   addField("TabWidth",		TypeS32,		Offset(mTabWidth,GuiTabBookCtrl));

}


// Empty for now, will implement for handling design time context menu for manipulating pages
ConsoleMethod( GuiTabBookCtrl, addPage, void, 2, 2, "(no arguments expected)")
{
   object->addNewPage();
}

//ConsoleMethod( GuiTabBookCtrl, removePage, void, 2, 2, "()")
//{
//}


bool GuiTabBookCtrl::onAdd()
{
   Parent::onAdd();

   mTabRect = getTabBaseRect( Point2I(0,0) );
   mPageRect = getPageRect( mTabRect, Point2I(0,0) );

   return true;
}


void GuiTabBookCtrl::onRemove()
{
   Parent::onRemove();
}

void GuiTabBookCtrl::onChildRemoved( GuiControl* child )
{
   Vector<GuiTabPageCtrl*>::iterator i = mPages.begin();
   for( ; i != mPages.end(); i++)
   {
      GuiTabPageCtrl* tab = (*i);
      if( tab == child )
      {
         if( tab == mActivePage )
            mActivePage = NULL;
         mPages.erase( i );
         break;
      }
   }

   if( mPages.empty() )
      mActivePage = NULL;
   else if (mActivePage == NULL )
      mActivePage = (*mPages.begin());

}

void GuiTabBookCtrl::onChildAdded( GuiControl *child )
{
   GuiTabPageCtrl *page = dynamic_cast<GuiTabPageCtrl*>(child);
   if( !page )
   {
      Con::warnf("GuiTabBookCtrl::onChildAdded - attempting to add NON GuiTabPageCtrl as child page");
      SimObject *simObj = reinterpret_cast<SimObject*>(child);
      removeObject( simObj );
      if( mActivePage )
      {
         mActivePage->addObject( simObj );
      }
      else
      {
         Con::warnf("GuiTabBookCtrl::onChildAdded - unable to find active page to reassign ownership of new child control to, placing on parent");
         GuiControl *rent = getParent();
         if( rent )
            rent->addObject( simObj );
      }
      return;
   }
   child->resize( mPageRect.point, mPageRect.extent );
   mPages.push_back( page );
   selectPage( page );
}


bool GuiTabBookCtrl::onWake()
{
   if (! Parent::onWake())
      return false;

   mHasTexture = mProfile->constructBitmapArray();
   if( mHasTexture )
      mBitmapBounds = mProfile->mBitmapArrayRects.address();

   return true;
}

void GuiTabBookCtrl::onSleep()
{
   Parent::onSleep();
}


void GuiTabBookCtrl::addNewPage()
{
   char textbuf[1024];

   GuiTabPageCtrl * page = new GuiTabPageCtrl();

   page->setField("profile", "GuiTabPageProfile");

   dSprintf(textbuf, sizeof(textbuf), "TabBookPage%d_%d", getId(), page->getId());
   page->registerObject(textbuf);

   this->addObject( page );
}

void GuiTabBookCtrl::resize(const Point2I &newPosition, const Point2I &newExtent)
{
   Parent::resize( newPosition, newExtent );

   mTabRect = getTabBaseRect( Point2I(0,0) );
   mPageRect = getPageRect( mTabRect, Point2I(0,0) );

   RectI pageRect = getPageRectChild();

   // Resize Children
   SimSet::iterator i;
   for(i = begin(); i != end(); i++)
   {
      GuiControl *ctrl = static_cast<GuiControl *>(*i);
      ctrl->mBounds = pageRect;
   }
}

void GuiTabBookCtrl::childResized(GuiControl *child)
{
   Parent::childResized( child );
   child->mBounds = getPageRectChild();
}

GuiTabPageCtrl *GuiTabBookCtrl::findHitTab( const GuiEvent &event )
{
   return findHitTab( event.mousePoint );
}

GuiTabPageCtrl *GuiTabBookCtrl::findHitTab( Point2I hitPoint )
{
   S32 hitTab = 0; // Which tab did we hit?

   hitPoint = globalToLocalCoord( hitPoint );

   switch( mTabPosition )
   {
   case AlignTop:
   case AlignBottom:
      hitTab = (S32)mFloor( (F32)hitPoint.x / (F32)mTabWidth );
      break;
   case AlignLeft:
   case AlignRight:
      hitTab = (S32)mFloor( (F32)hitPoint.y / (F32)mTabWidth );
      break;
   };

   if( mPages.size() > hitTab && hitTab >= 0 )
      return mPages[hitTab];

   return NULL;
}

RectI GuiTabBookCtrl::getHitRect( Point2I offset )
{
   // We can get our rect without a specified offset, but if we don't specify one
   // when rendering with GUI editor open, our offset is wrong because the editor
   // offsets the canvas to accomodate creation of it's own controls

   RectI Result = RectI( localToGlobalCoord( Point2I(0,0) ), mBounds.extent );

   if( ! offset.isZero() )
      Result.point -= offset;
   switch( mTabPosition )
   {
   case AlignTop:
      Result.extent.y = mTabHeight;
      break;
   case AlignBottom:
      Result.point.y = (Result.point.y + Result.extent.y) - mTabHeight;
      Result.extent.y = mTabHeight;
      break;
   case AlignLeft:
      Result.extent.x = mTabHeight;
      break;
   case AlignRight:
      Result.point.x = (Result.point.x + Result.extent.x) - mTabHeight;
      Result.extent.x = mTabHeight;
      break;
   }
   return Result;

}

RectI GuiTabBookCtrl::getTabBaseRect( Point2I offset = Point2I(0,0) )
{
   // We can get our rect without a specified offset, but if we don't specify one
   // when rendering with GUI editor open, our offset is wrong because the editor
   // offsets the canvas to accomodate creation of it's own controls

   RectI Result = RectI( offset.isZero() ? mBounds.point : offset, mBounds.extent );
   switch( mTabPosition )
   {
   case AlignTop:
      Result.extent.y = mTabHeight;
      break;
   case AlignBottom:
      Result.point.y = (Result.point.y + Result.extent.y) - mTabHeight;
      Result.extent.y = mTabHeight;
      break;
   case AlignLeft:
      Result.extent.x = mTabHeight;
      break;
   case AlignRight:
      Result.point.x = (Result.point.x + Result.extent.x) - mTabHeight;
      Result.extent.x = mTabHeight;
      break;
   }
   return Result;

}

RectI GuiTabBookCtrl::getPageRect( const RectI &tabBaseRect, Point2I offset )
{
   RectI Result;
   switch( mTabPosition )
   {
   case AlignTop:
      Result.point.x = tabBaseRect.point.x;
      Result.extent.x = tabBaseRect.extent.x;
      Result.point.y = tabBaseRect.point.y + tabBaseRect.extent.y;
      Result.extent.y = mBounds.extent.y - mTabHeight;
      break;
   case AlignBottom:
      Result.point.x = tabBaseRect.point.x;
      Result.extent.x = tabBaseRect.extent.x;
      Result.point.y = offset.y;
      Result.extent.y = mBounds.extent.y - mTabHeight;
      break;
   case AlignLeft:
      Result.point.x = offset.x + mTabHeight;
      Result.extent.x = mBounds.extent.x - mTabHeight;
      Result.point.y = offset.y;
      Result.extent.y = mBounds.extent.y;
      break;
   case AlignRight:
      Result.point.x = offset.x;
      Result.extent.x = mBounds.extent.x - mTabHeight;
      Result.point.y = offset.y;
      Result.extent.y = mBounds.extent.y;
      break;
   }

   return Result;
}

RectI GuiTabBookCtrl::getPageRectChild()
{
   RectI Result;
   switch( mTabPosition )
   {
   case AlignTop:
      Result.point.x = 0;
      Result.extent.x = mBounds.extent.x;
      Result.point.y = mTabHeight;
      Result.extent.y = mBounds.extent.y - mTabHeight;
      break;
   case AlignBottom:
      Result.point.x = 0;
      Result.extent.x = mBounds.extent.x;
      Result.point.y = 0;
      Result.extent.y = mBounds.extent.y - mTabHeight;
      break;
   case AlignLeft:
      Result.point.x = mTabHeight;
      Result.extent.x = mBounds.extent.x - mTabHeight;
      Result.point.y = 0;
      Result.extent.y = mBounds.extent.y;
      break;
   case AlignRight:
      Result.point.x = 0;
      Result.extent.x = mBounds.extent.x - mTabHeight;
      Result.point.y = 0;
      Result.extent.y = mBounds.extent.y;
      break;
   }

   return Result;
}

void GuiTabBookCtrl::onPreRender()
{
   // sometimes we need to resize because of a changed persistent field
   // that's what this does
   solveDirty();
}

void GuiTabBookCtrl::solveDirty()
{
   bool dirty = false;
   if( mTabPosition != mLastTabPosition )
   {
      mLastTabPosition = mTabPosition;
      dirty = true;
   }

   if( mTabHeight != mLastTabHeight )
   {
      mLastTabHeight = mTabHeight;
      dirty = true;
   }

   if( mTabWidth != mLastTabWidth )
   {
      mLastTabWidth = mTabWidth;
      dirty = true;
   }

   if( dirty )
      resize( mBounds.point, mBounds.extent );

}

void GuiTabBookCtrl::onRender(Point2I offset, const RectI &updateRect)
{
   RectI tabRect = getTabBaseRect( offset );
   RectI pageRect = getPageRect( tabRect, offset );


   // We're so nice we'll store the old modulation before we clear it for our rendering! :)
   ColorI oldModulation;
   dglGetBitmapModulation( &oldModulation );

   // Wipe it out
   dglClearBitmapModulation();

   // Render our tabs
   renderTabs( tabRect );

   // This is ideal, we know what our active tab is, so we only render it
   if( mActivePage != NULL )
   {
      mActivePage->onRender( pageRect.point, pageRect );
   }
   else
   {
      // Render only the active page
      if( ! mPages.empty() )
      {
         GuiTabPageCtrl* tab = reinterpret_cast<GuiTabPageCtrl*>(*mPages.begin());
         tab->onRender( pageRect.point, pageRect );
         mActivePage = tab;
      }
      else
      {
         // if we have no active page, render a filled rect only
         dglDrawRectFill( pageRect, mProfile->mFillColor );
      }
   }

   // Restore old modulation
   dglSetBitmapModulation( oldModulation );


}

void GuiTabBookCtrl::renderTabs( const RectI &bounds )
{
   RectI oldClip = dglGetClipRect();

   dglSetClipRect( bounds );

   S32 fitTabs = 0; // How many tabs can we fit in the book?
   S32 extentY,extentX;


   switch( mTabPosition )
   {
   case AlignTop:
   case AlignBottom:
      fitTabs = (S32)mCeil( (F32)bounds.extent.x / (F32)mTabWidth );
      extentX = mTabWidth;
      extentY = mTabHeight;
      break;
   case AlignLeft:
   case AlignRight:
      fitTabs = (S32)mCeil( (F32)bounds.extent.y / (F32)mTabWidth );
      extentY = mTabWidth;
      extentX = mTabHeight;
      break;
   };

   Vector<GuiTabPageCtrl*>::iterator i = mPages.begin();
   for( S32 j=0 ; ( i != mPages.end() && j < fitTabs ) ; i++, j++)
   {
      RectI tabBounds = RectI( bounds.point.x, bounds.point.y, extentX, extentY );
      switch( mTabPosition )
      {
      case AlignTop:
      case AlignBottom:
         tabBounds.point.x = bounds.point.x + ( j * mTabWidth );
         break;
      case AlignLeft:
      case AlignRight:
         tabBounds.point.y = bounds.point.y + ( j * mTabWidth );
         break;
      };

      GuiTabPageCtrl *tab = reinterpret_cast<GuiTabPageCtrl*>(*i);

      renderTab( tabBounds, tab );
   }

   dglSetClipRect( oldClip );

}

void GuiTabBookCtrl::renderTab( RectI tabRect, GuiTabPageCtrl *tab )
{
   StringTableEntry text = tab->getText();
   ColorI oldColor;

   dglGetBitmapModulation( &oldColor );

   // Is this a skinned control?
   if( mHasTexture && mProfile->mBitmapArrayRects.size() == NumBitmaps)
   {
      S32 tabTextureIndex = 0;
      RectI stretchRect;

      if ( mActivePage == tab )
         stretchRect = mBitmapBounds[ TabSelected ];
      else if( mHoverTab == tab )
         stretchRect = mBitmapBounds[ TabHover ];
      else
         stretchRect = mBitmapBounds[ TabNormal ];

      dglDrawBitmapStretchSR(mProfile->mTextureHandle,tabRect,stretchRect, (mTabPosition == AlignBottom) ? /* Render texture upside down */ GFlip_Y : 0 );
   }
   else
   {
      // If this isn't a skinned control or the bitmap is simply missing, handle it WELL
      if ( mActivePage == tab )
         dglDrawRectFill(tabRect, mProfile->mFillColor);
      else if( mHoverTab == tab )
         dglDrawRectFill(tabRect, mProfile->mFillColorHL);
      else
         dglDrawRectFill(tabRect, mProfile->mFillColorNA);

   }


   dglSetBitmapModulation(mProfile->mFontColor);

   renderJustifiedText( tabRect.point, tabRect.extent, text );

   dglSetBitmapModulation( oldColor);

}



void GuiTabBookCtrl::selectPage( S32 index )
{
   if( mPages.size() < index )
      return;

   // Select the page
   selectPage( mPages[ index ] );
}


void GuiTabBookCtrl::selectPage( GuiTabPageCtrl *page )
{
   Vector<GuiTabPageCtrl*>::iterator i = mPages.begin();
   for( ; i != mPages.end() ; i++ )
   {
      GuiTabPageCtrl *tab = reinterpret_cast<GuiTabPageCtrl*>(*i);
      if( page == tab )
      {
         mActivePage = tab;
         tab->setVisible( true );

         // Notify User
         char *retBuffer = Con::getReturnBuffer( 512 );
         dStrcpy( retBuffer, tab->getText() );
         Con::executef( this, 2, "onTabSelected",  retBuffer );

      }
      else
         tab->setVisible( false );
   }
}

void GuiTabBookCtrl::onMouseDown(const GuiEvent &event)
{
   RectI TabBase = getHitRect();
   if( TabBase.pointInRect( event.mousePoint ) )
   {
      GuiTabPageCtrl *tab = findHitTab( event.mousePoint );
      if( tab != NULL )
         selectPage( tab );
   }
}

void GuiTabBookCtrl::onRightMouseDown(const GuiEvent &event)
{
   if (! mActive)
   {
      Parent::onRightMouseDown(event);
      return;
   }

   setFirstResponder();

   //search for the control hit in any layer below the edit layer
   GuiControl *hitCtrl = findHitControl(globalToLocalCoord(event.mousePoint), mLayer - 1);
   if (hitCtrl != this)
   {
      Con::executef(this, 1, "onClearSelected");
   }
}

void GuiTabBookCtrl::onMouseMove(const GuiEvent &event)
{
   RectI TabBase = getHitRect();


   if( TabBase.pointInRect( event.mousePoint ) )
   {

      GuiTabPageCtrl *tab = findHitTab( event.mousePoint );
      if( tab != NULL && mHoverTab != tab )
         mHoverTab = tab;
      else if ( !tab )
         mHoverTab = NULL;
   }
   Parent::onMouseMove( event );
}

void GuiTabBookCtrl::onMouseLeave( const GuiEvent &event )
{
   mHoverTab = NULL;
}

void GuiTabBookCtrl::onMouseDownEditor(const GuiEvent &event, Point2I offset)
{
   RectI TabBase = getHitRect( offset );
   if( TabBase.pointInRect( event.mousePoint ) )
   {
      GuiTabPageCtrl *tab = findHitTab( event.mousePoint + offset );
      if( tab != NULL )
         selectPage( tab );
   }

   // This shouldn't be called if it's not design time, but check just incase
   if ( GuiControl::smDesignTime )
   {
      // If we clicked in the editor and our addset is the tab book
      // ctrl, select the child ctrl so we can edit it's properties
      GuiEditCtrl* edit = GuiControl::smEditorHandle;
      if( edit  && ( edit->getAddSet() == this ) && mActivePage != NULL )
          edit->select( mActivePage );
   }

}
void GuiTabBookCtrl::onRightMouseDownEditor(const GuiEvent &event, Point2I offset)
{
   //////GuiPopUpTextListCtrl
   //GuiPopUpMenuCtrl * menu = new GuiPopUpMenuCtrl();
   //menu->setField("profile", "GuiPopUpMenuProfile");
   //menu->setField("text", "Duh");

   ////now add the entries
   //menu->addEntry("Test",1);
   //menu->onAction();

   //menu->registerObject();

}

