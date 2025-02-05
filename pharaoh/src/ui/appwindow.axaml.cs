using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace Pharaoh.UI;

public partial class AppWindow : Window
{
	public AppWindow()
	{
		AvaloniaXamlLoader.Load(this);
	}

	// TODO: Some of these (ones that open further menus especially) can probably be handled entirely within Xaml.

	public void FileMenuOpen() => throw new NotImplementedException();
	public void FileMenuClearRecent() => throw new NotImplementedException();
	public void FileMenuSave() => throw new NotImplementedException();
	public void FileMenuSaveAs() => throw new NotImplementedException();

	public void FileMenuNewProject() => throw new NotImplementedException();

	public void FileMenuExit() => throw new NotImplementedException();

	public void EditMenuUndo() => throw new NotImplementedException();
	public void EditMenuRedo() => throw new NotImplementedException();

	public void EditMenuPreferences() => throw new NotImplementedException();
	
	public void ViewMenuToolSceneHierarchy() => throw new NotImplementedException();
	public void ViewMenuToolContentBrowser() => throw new NotImplementedException();
	public void ViewMenuToolConsole() => throw new NotImplementedException();
	public void ViewMenuToolInspector() => throw new NotImplementedException();
	public void ViewMenuToolViewport() => throw new NotImplementedException();
	
	public void ViewMenuSaveLayout() => throw new NotImplementedException();
	public void ViewMenuLoadLayout() => throw new NotImplementedException();
	
	public void ProjectMenuExport() => throw new NotImplementedException();
	public void ProjectMenuSettings() => throw new NotImplementedException();
	
	public void HelpMenuManual() => throw new NotImplementedException();
}
