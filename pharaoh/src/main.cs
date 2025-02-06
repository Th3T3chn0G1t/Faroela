// SPDX-License-Identifier: MIT

using Avalonia;

using Dock.Model.Avalonia;

using Pharaoh.UI;

namespace Pharaoh;

public class Startup
{
	public static AppBuilder BuildAvaloniaApp() => AppBuilder.Configure<PharaohApp>()
			.UsePlatformDetect()
			.LogToTrace();

	[STAThread]
	static void Main(string[] args)
	{
		IntPtr ctx;

		unsafe { Faroela.Initialize(&ctx); }

		Log.Context = ctx;
		Log.Info("Hello, Pharaoh!");

		GC.KeepAlive(typeof(Factory).Assembly);
		BuildAvaloniaApp().StartWithClassicDesktopLifetime(args);

		unsafe { Faroela.Shutdown(&ctx); }
	}
}
