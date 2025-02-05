using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace Pharaoh.UI;

public partial class ContentBrowser : UserControl
{
	public ContentBrowser()
	{
		AvaloniaXamlLoader.Load(this);
	}
}
