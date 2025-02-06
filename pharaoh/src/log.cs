// SPDX-License-Identifier: MIT

using System;

namespace Pharaoh;

public class Log
{
	public static IntPtr Context
	{
		private get;
		set;
	}

	public static void Trace(string message) => Faroela.Log(Context, Faroela.TRACE, message);
	public static void Debug(string message) => Faroela.Log(Context, Faroela.DEBUG, message);
	public static void Info(string message) => Faroela.Log(Context, Faroela.INFO, message);
	public static void Warning(string message) => Faroela.Log(Context, Faroela.WARNING, message);
	public static void Error(string message) => Faroela.Log(Context, Faroela.ERROR, message);
	public static void Critical(string message) => Faroela.Log(Context, Faroela.CRITICAL, message);
}
