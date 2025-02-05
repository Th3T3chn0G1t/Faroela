using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace Pharaoh.UI;

public partial class Console : UserControl
{
	public Console()
	{
		AvaloniaXamlLoader.Load(this);
	}
}
