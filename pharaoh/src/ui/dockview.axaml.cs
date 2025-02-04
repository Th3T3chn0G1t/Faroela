using Avalonia.Collections;
using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Markup.Xaml;

using Dock.Avalonia.Controls;
using Dock.Model;
using Dock.Model.Avalonia.Controls;
using Dock.Model.Avalonia.Json;
using Dock.Model.Core;

namespace Pharaoh.UI;

public partial class DockView : UserControl
{
    public DockView()
    {
        AvaloniaXamlLoader.Load(this);
    }
}
