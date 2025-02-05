using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace Pharaoh.UI;

public partial class SceneHierarchy : UserControl
{
	public SceneHierarchy()
	{
		AvaloniaXamlLoader.Load(this);
	}
}
