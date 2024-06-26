#include "core/findMatch.h"
#include "gui/controls/guiDirectoryFileListCtrl.h"


IMPLEMENT_CONOBJECT( GuiDirectoryFileListCtrl );

GuiDirectoryFileListCtrl::GuiDirectoryFileListCtrl()
{
}

bool GuiDirectoryFileListCtrl::onWake()
{
   if( !Parent::onWake() )
      return false;

   setCurrentPath( "/", "*.*" );

   return true;
}

void GuiDirectoryFileListCtrl::onMouseDown(const GuiEvent &event)
{
   Parent::onMouseDown( event );

   if( event.mouseClickCount == 2 && getNamespace() )
      Con::executef(this, 1, "onDoubleClick");

}


void GuiDirectoryFileListCtrl::openDirectory()
{
   Vector<Platform::FileInfo> fileVector;
   Platform::dumpPath( mFilePath, fileVector, 0 );

   // Clear the current file listing
   clear();

   // Does this dir have any files?
   if( fileVector.empty() )
      return;

   // If so, iterate through and list them
   Vector<Platform::FileInfo>::iterator i = fileVector.begin();
   for( S32 j=0 ; i != fileVector.end(); i++, j++ )
   {
      if( FindMatch::isMatchMultipleExprs( mFilter, (*i).pFileName,false ) )
         addEntry( j++, (*i).pFileName );
   }

   //Done!
}

bool GuiDirectoryFileListCtrl::setCurrentPath( StringTableEntry path, StringTableEntry filter )
{
   // Oops, gotta give us a path to work with
   if( !path )
      return false;

   char ExpandedPath[512];
   char FullPath[512];
   dMemset( ExpandedPath, 0, 512 );
   dMemset( FullPath, 0, 512 );

   Con::expandScriptFilename( ExpandedPath, 512, path );

   if( ExpandedPath[0] != '/' )
      dSprintf( FullPath, 512, "%s/%s", Platform::getWorkingDirectory(), ExpandedPath );
   else
      dSprintf( FullPath, 512, "%s%s", Platform::getWorkingDirectory(), ExpandedPath );

   // Platform::isDirectory expects no trailing / so make sure we conform
   if( FullPath[ dStrlen( FullPath ) - 1 ] == '/' )
      FullPath[ dStrlen( FullPath ) - 1 ] = 0x00;

   if( ! filter )
      filter = StringTable->insert("*.*");

   // A bad path!?  For shame...
   if( !Platform::isDirectory( FullPath ) && !Platform::hasSubDirectory( FullPath ) )
      return false;

   // Store our new info
   mFilePath = StringTable->insert( FullPath );
   mFilter   = StringTable->insert( filter );

   // Update our view
   openDirectory();

   // Peace out!
   return true;
}

StringTableEntry GuiDirectoryFileListCtrl::getSelectedFileName()
{
   return StringTable->insert( getSelectedText() );
}

ConsoleMethod( GuiDirectoryFileListCtrl, setPath, bool, 3, 4, "setPath(path,filter) - directory to enumerate files from (without trailing slash)" )
{
   return object->setCurrentPath( argv[2], argv[3] );
}

ConsoleMethod( GuiDirectoryFileListCtrl, getSelectedFile, const char*, 2, 2, "getSelectedFile () - returns the currently selected file name" )
{
   return object->getSelectedFileName();
}

