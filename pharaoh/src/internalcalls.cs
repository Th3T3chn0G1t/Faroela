// SPDX-License-Identifier: MIT

using System;
using System.Runtime.InteropServices;

namespace Pharaoh;

public partial class InternalCalls
{
	[LibraryImport("faroela", EntryPoint = "faro_export_initialize", StringMarshalling = StringMarshalling.Utf8)]
    public static partial void Initialize();
};
