$cursorControlled = 1;
function cursorOff ()
{
	if ($cursorControlled)
	{
		lockMouse (1);
	}
	Canvas.cursorOff ();
}

function cursorOn ()
{
	if ($cursorControlled)
	{
		lockMouse (0);
	}
	Canvas.cursorOn ();
	Canvas.setCursor (DefaultCursor);
}

package CanvasCursor
{
	function GuiCanvas::checkCursor (%this)
	{
		%cursorShouldBeOn = 0;
		%i = 0;
		while (%i < %this.getCount ())
		{
			%control = %this.getObject (%i);
			if (%control.noCursor $= "")
			{
				%cursorShouldBeOn = 1;
				break;
			}
			%i += 1;
		}
		if (%cursorShouldBeOn != %this.isCursorOn ())
		{
			if (%cursorShouldBeOn)
			{
				cursorOn ();
			}
			else 
			{
				cursorOff ();
			}
		}
		%this.checkTabFocus ();
	}
	
function GuiCanvas::checkTabFocus (%this)
	{
		%i = 0;
		while (%i < %this.getCount ())
		{
			%control = %this.getObject (%i);
			if (%control.noTabFocus == 1)
			{
				%this.canTabFocus (0);
				return;
			}
			%i += 1;
		}
		%this.canTabFocus (1);
	}
	
function GuiCanvas::setContent (%this, %ctrl)
	{
		Parent::setContent (%this, %ctrl);
		%this.checkCursor ();
	}
	
function GuiCanvas::pushDialog (%this, %ctrl)
	{
		Parent::pushDialog (%this, %ctrl);
		%this.checkCursor ();
	}
	
function GuiCanvas::popDialog (%this, %ctrl)
	{
		Parent::popDialog (%this, %ctrl);
		%this.checkCursor ();
	}
	
function GuiCanvas::popLayer (%this, %layer)
	{
		Parent::popLayer (%this, %layer);
		%this.checkCursor ();
	}
	

};

activatePackage (CanvasCursor);
