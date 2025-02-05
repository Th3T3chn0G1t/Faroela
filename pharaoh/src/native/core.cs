// SPDX-License-Identifier: MIT

using System.Runtime.InteropServices;

namespace Pharaoh.Native;

public partial class Core
{
	[LibraryImport("faroela", EntryPoint = "faroela_initialize", StringMarshalling = StringMarshalling.Utf8)]
	public static partial void Initialize();

	[LibraryImport("faroela", EntryPoint = "faroela_shutdown", StringMarshalling = StringMarshalling.Utf8)]
	public static partial void Shutdown();
}
