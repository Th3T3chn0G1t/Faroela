// SPDX-License-Identifier: MIT

using System;
using System.Runtime.InteropServices;

namespace Pharaoh;

public unsafe partial class Faroela
{
	[LibraryImport("faroela", EntryPoint = "faroela_initialize", StringMarshalling = StringMarshalling.Utf8)]
	public static partial void Initialize(IntPtr* ctx);

	[LibraryImport("faroela", EntryPoint = "faroela_shutdown", StringMarshalling = StringMarshalling.Utf8)]
	public static partial void Shutdown(IntPtr* ctx);

	public static int TRACE = 0;
	public static int DEBUG = 1;
	public static int INFO = 2;
	public static int WARNING = 3;
	public static int ERROR = 4;
	public static int CRITICAL = 5;

	[LibraryImport("faroela", EntryPoint = "faroela_log", StringMarshalling = StringMarshalling.Utf8)]
	public static partial void Log(IntPtr ctx, int level, string message);
}
