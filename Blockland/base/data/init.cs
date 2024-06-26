%file = findFirstFile ("base/data/terrains/*propertymap.cs");
while (%file !$= "")
{
	exec (%file);
	%file = findNextFile ("base/data/terrains/*propertymap.cs");
}
