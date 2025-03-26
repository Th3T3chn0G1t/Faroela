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

	public static void Trace(string message) => Faroela.Log(Context, Faroela.Verbosity.Trace, message);
	public static void Debug(string message) => Faroela.Log(Context, Faroela.Verbosity.Debug, message);
	public static void Info(string message) => Faroela.Log(Context, Faroela.Verbosity.Info, message);
	public static void Warning(string message) => Faroela.Log(Context, Faroela.Verbosity.Warning, message);
	public static void Error(string message) => Faroela.Log(Context, Faroela.Verbosity.Error, message);
	public static void Critical(string message) => Faroela.Log(Context, Faroela.Verbosity.Critical, message);
}
