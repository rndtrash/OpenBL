function loadClientAddOns ()
{
	%dir = "Add-Ons/client/*.cs";
	%fileCount = getFileCount (%dir);
	%fileName = findFirstFile (%dir);
	%i = 0;
	while (%i < %fileCount)
	{
		exec (%fileName);
		%fileName = findNextFile (%dir);
		%i += 1;
	}
}

