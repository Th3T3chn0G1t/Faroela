// SPDX-License-Identifier: MIT

using Pharaoh.Native;

namespace Pharaoh;

public class Startup
{
	static void Main(string[] args)
	{
		Core.Initialize();

		Log.Info("Hello, Pharaoh!");

		Core.Shutdown();
	}
};
