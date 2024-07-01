function HelpDlg::onWake (%this)
{
	HelpFileList.entryCount = 0;
	HelpFileList.clear ();
	%file = findFirstFile ("*.hfl");
	while (%file !$= "")
	{
		HelpFileList.fileName[HelpFileList.entryCount] = %file;
		HelpFileList.addRow (HelpFileList.entryCount, fileBase (%file));
		HelpFileList.entryCount += 1;
		%file = findNextFile ("*.hfl");
	}
	HelpFileList.sortNumerical (0);
	HelpFileList.setSelectedRow (0);
}

function HelpFileList::onSelect (%this, %row)
{
	%fo = new FileObject ("");
	%fo.openForRead (%this.fileName[%row]);
	%text = "";
	while (!%fo.isEOF ())
	{
		%text = %text @ %fo.readLine () @ "\n";
	}
	%fo.delete ();
	HelpText.setText (%text);
}

function getHelp (%helpName)
{
	Canvas.pushDialog (HelpDlg);
	if (%helpName !$= "")
	{
		%index = HelpFileList.findTextIndex (%helpName);
		HelpFileList.setSelectedRow (%index);
	}
}

function contextHelp ()
{
	%i = 0;
	while (%i < Canvas.getCount ())
	{
		if (Canvas.getObject (%i).getName () $= HelpDlg)
		{
			Canvas.popDialog (HelpDlg);
			return;
		}
		%i += 1;
	}
	%content = Canvas.getContent ();
	%helpPage = %content.getHelpPage ();
	getHelp (%helpPage);
}

function GuiControl::getHelpPage (%this)
{
	return %this.helpPage;
}

function GuiMLTextCtrl::onURL (%this, %url)
{
	gotoWebPage (%url);
}
