using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace Pharaoh.UI;

public partial class DockView : UserControl
{
	public DockView()
	{
		AvaloniaXamlLoader.Load(this);

		/*var dock = this.FindNameScope()!.Find<ProportionalDock>("DockBase")!;

		ProportionalDock pane = new();
		AvaloniaXamlLoader.Load(pane);
		dock.Factory!.AddDockable(dock, pane);*/
	}
}
