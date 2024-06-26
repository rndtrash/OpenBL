README for OSX build

Contents:

1) General Notes
    1.1) Supported Versions of Mac OS
    1.2) Recommended Development Tools
2) Initial Setup, Tips & Goodies
    2.1) Getting Debugging to Work
        2.1.1) For Xcode2.1+
        2.1.2) For ProjectBuilder, Xcode1.5, Xcode2.0
    2.2) Script Editing in Xcode
        2.2.1) Installing .cs Language Definitions in Xcode
    2.3) Putting Game Content Inside the App Bundle
3) Known issues


1) General Notes
---------------------------------------------------------------------------

1.1) Supported Versions of Mac OS
Recent versions of Torque on Macintosh require MacOS 10.2 or later.
Attempting to support older versions of the MacOS is not recommended.

1.2) Recommended Development Tools
Only the Xcode 2.1 project file is currently being kept up to date.
Previous projects aren't guaranteed to work properly, and may have issues
not listed herein. Thus, Xcode is the recommended development environment.
It is of course possible to use any development environment that can
produce a Macintosh application, and can use the standard BSD & MacOSX
headers and frameworks.


2) Initial Setup, Tips & Goodies
---------------------------------------------------------------------------

2.1) Getting Debugging to Work

  2.1.1) For Xcode2.1+
  ----------------------------------------
  - locate the file "fix xcode2.1 build folders.command".
  - double-click it to launch it.
  - the Terminal will open, and the script will run.
  - close the Terminal window.
  
  The script will move aside any existing 'build' directory, and replace it
  with a clean one containing symbolic links ( aliases ) to the 
  "../example" directory. If you are using a different directory name than
  "example", you can edit the script in any text editor and replace
  "example" with your directory of choice.

  2.1.2) For ProjectBuilder, Xcode1.5, Xcode2.0
  ----------------------------------------
  - launch the PB project.
  - select/click the topmost group (should have same name as the project)
  - from menu choose File->Show Info
  - perform the following three steps in order:
  - first: select "Place intermediate build products for this project in a
    separate location"
  - second: select "Place build products for this project in a separate
    location"
  - third: enter "../example" (without the quotes), in the upper text box.
  - replace "example" with your folder name if you're using another name.
  - close the window.
  - close the project file & reopen it to force a save.

  ----------------------------------------
  Your project should now directly build the target executable directly
  into the runtime folder, which also means it is properly located for
  running or debugging from within Xcode / Project Builder.



2.2) Script Editing in Xcode
The Xcode project file now contains references to the script files.
This make searching more complete, and provides a decent script editor.

  2.2.1) Installing .cs Language Definitions in Xcode  
  ----------------------------------------
  - open the "torque/pb/script editing" folder.
  - locate the Torque.pbfilespec and Torque.pblangspec files therein.
  - locate the directory /Library/Application Support/Xcode/Specifications/
  - create the Specifications directory if it does not exist.
  - install the .pbfilespec and .pblangspec files in Specifications/ .
  - quit and restart Xcode.


2.3) Putting Game Content Inside the App Bundle
The engine will assume that game content is beside the main.cs file.
It will look first in YourGame.app/Contents/Resources, then it will 
look in the folder where YourGame.app resides for a file named 'main.cs'.
So, when you are ready to distribute your game, you should place the game
content inside the application bundle like this:
- in the Finder, control-click(or right-click) on your built application.
- choose 'Show Package Contents'.
- a new window will open, with one folder: 'Contents'.
- Navigate to the Contents/Resources folder.
- copy all your game files (main.cs, common/, etc) into Contents/Resources.
- run the game & test all functionality.


3) Known issues
---------------------------------------------------------------------------

- No external console window. in debug in Xcode, printfs go to the debug
console pane. At some point, we'll likely create a simple console library
to be used when a tertiary console window is needed. In the mean time, you
can open the console.logfile using the Console application. Console will
automatically 'tail' the file, always displaying the most current contents.
  
- ADDENDUM: printfs may also output to the system log in the Console, and
will output to the Terminal if you manually launch the Torque app directly
from the commandline.

- OSX handles cmdline args like the PC, if you launch it from the cmdline.
Note that this means that you can add args in the project file for debug vs release, etc.
Take a peek at "example/Launch using show mod.command" in a text editor to see how you can use cmdline args from an external script.
The maccmdline.txt workaround still works, if you prefer.

- mac performance vs the PC has improved greatly. There is still ground to
gain performance here, but it may not be a good investment of time. As a
general rule, only consider optimization when you're done with everything
else.

- The mac platform code is a mess. A total rewrite is on my todo list.
