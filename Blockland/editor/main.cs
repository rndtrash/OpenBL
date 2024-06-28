//-----------------------------------------------------------------------------

// Package overrides to initialize the mod.
package Editor {
	function onStart()
	{
		Parent::onStart();
		echo("\n--------- Initializing MOD: Editor ---------");
		exec("./editor.cs");
		exec("./particleEditor.cs");
		exec("./GuiEditor.gui");
		exec("./ParticleEditor.gui");
	}

	function initClient()
	{
		Parent::initClient();
	}
}; // Editor package
activatePackage(Editor);