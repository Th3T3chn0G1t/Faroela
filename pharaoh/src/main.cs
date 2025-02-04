using System;
using System.Runtime.InteropServices;

namespace Pharaoh;

partial class InternalCalls
{
	[LibraryImport("faroela", EntryPoint = "faro_export_test", StringMarshalling = StringMarshalling.Utf8)]
    public static partial void Test(string message);
};

public class Startup
{
	static void Main(string[] args)
	{
		Console.WriteLine("Hello, Pharaoh!");
		InternalCalls.Test("Hello, Faroela!");
	}
};
