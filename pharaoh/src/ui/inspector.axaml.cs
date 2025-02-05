using Avalonia.Controls;
using Avalonia.Markup.Xaml;

namespace Pharaoh.UI;

public partial class Inspector : UserControl
{
	public Inspector()
	{
		AvaloniaXamlLoader.Load(this);
	}
}
