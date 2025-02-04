// SPDX-License-Identifier: MIT

using System;
using System.Runtime.InteropServices;

namespace Pharaoh.Native;

public partial class Log
{
	public static int TRACE = 0;
	public static int DEBUG = 1;
	public static int INFO = 2;
	public static int WARNING = 3;
	public static int ERROR = 4;
	public static int CRITICAL = 5;

	[LibraryImport("faroela", EntryPoint = "faro_export_log", StringMarshalling = StringMarshalling.Utf8)]
	private static partial void LogBase(int level, string message);

	public static void Trace(string message) => LogBase(TRACE, message);
	public static void Debug(string message) => LogBase(DEBUG, message);
	public static void Info(string message) => LogBase(INFO, message);
	public static void Warning(string message) => LogBase(WARNING, message);
	public static void Error(string message) => LogBase(ERROR, message);
	public static void Critical(string message) => LogBase(CRITICAL, message);
};
