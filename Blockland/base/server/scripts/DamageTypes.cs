function AddDamageType (%name, %deathMessageSuicide, %deathMessageMurder, %vehicleDamageScale, %direct)
{
	eval ("%exists = $DamageType::" @ %name @ ";");
	if (%exists > 0)
	{
		%idx = %exists;
	}
	else 
	{
		$MaxDamageType += 1;
		%idx = $MaxDamageType;
		eval ("$DamageType::" @ %name @ " = $maxDamageType;");
	}
	$DamageType_Array[%idx] = %name;
	$DeathMessage_Suicide[%idx] = %deathMessageSuicide;
	$DeathMessage_Murder[%idx] = %deathMessageMurder;
	if (%vehicleDamageScale $= "")
	{
		%vehicleDamageScale = 1;
	}
	$Damage::VehicleDamageScale[%idx] = %vehicleDamageScale;
	if (%direct $= "")
	{
		%direct = 0;
	}
	$Damage::Direct[%idx] = %direct;
}

function initDefaultDamageTypes ()
{
	$MaxDamageType = 0;
	AddDamageType ("Default", '<bitmap:add-ons/ci/skull> %1', '%2 <bitmap:add-ons/ci/skull> %1!', 1, 0);
	AddDamageType ("Suicide", '<bitmap:add-ons/ci/skull> %1', '%2 <bitmap:add-ons/ci/skull> %1', 1, 0);
	AddDamageType ("Direct", '<bitmap:add-ons/ci/generic> %1', '%2 <bitmap:add-ons/ci/generic> %1', 1, 1);
	AddDamageType ("Radius", '<bitmap:add-ons/ci/bomb> %1', '%2 <bitmap:add-ons/ci/splat> %1', 1, 0);
	AddDamageType ("Impact", '<bitmap:add-ons/ci/splat> %1', '%2 <bitmap:add-ons/ci/splat> %1', 1, 0);
	AddDamageType ("Fall", '<bitmap:add-ons/ci/crater> %1', '%2 <bitmap:add-ons/ci/crater> %1', 1, 0);
	AddDamageType ("Vehicle", '<bitmap:add-ons/ci/car> %1', '%2 <bitmap:add-ons/ci/car> %1', 1, 0);
}

function dumpDamageTypes ()
{
	%i = 1;
	while (%i <= $MaxDamageType)
	{
		echo ($DamageType_Array[%i]);
		echo ("  SuicideMsg: ", getTaggedString ($DeathMessage_Suicide[%i]));
		echo ("   MurderMsg: ", getTaggedString ($DeathMessage_Murder[%i]));
		%i += 1;
	}
}

