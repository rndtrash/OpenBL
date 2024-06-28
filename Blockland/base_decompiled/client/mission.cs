function clientCmdMissionStart (%seq)
{
}

function clientCmdMissionEnd (%seq)
{
	alxStopAll ();
	$lightingMission = 0;
	$sceneLighting::terminateLighting = 1;
}

