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

		// TODO: Convert `args` to `argc/v`.
		unsafe { Faroela.Initialize(&ctx, 0, null); }

		Log.Context = ctx;
		Log.Info("Hello, Pharaoh!");

		GC.KeepAlive(typeof(Factory).Assembly);
		BuildAvaloniaApp().StartWithClassicDesktopLifetime(args);

		unsafe { Faroela.Shutdown(&ctx); }
	}
}
