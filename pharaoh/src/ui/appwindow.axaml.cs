using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Threading.Tasks;

using Avalonia.Collections;
using Avalonia.Controls;
using Avalonia.Interactivity;
using Avalonia.Markup.Xaml;
using Avalonia.Platform.Storage;

using Dock.Avalonia.Controls;
using Dock.Model;
using Dock.Model.Core;

namespace Pharaoh.UI;

public partial class AppWindow : Window
{
    public AppWindow()
    {
        AvaloniaXamlLoader.Load(this);
    }
}
