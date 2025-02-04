// SPDX-License-Identifier: MIT

using System;
using System.Runtime.InteropServices;

namespace Pharaoh.Native;

public partial class Core
{
	[LibraryImport("faroela", EntryPoint = "faro_export_initialize", StringMarshalling = StringMarshalling.Utf8)]
    public static partial void Initialize();

	[LibraryImport("faroela", EntryPoint = "faro_export_shutdown", StringMarshalling = StringMarshalling.Utf8)]
    public static partial void Shutdown();
}
