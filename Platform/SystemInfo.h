/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Platform_SystemInfo
#define TC_HEADER_Platform_SystemInfo

#include "PlatformBase.h"

namespace nemesis
{
	class SystemInfo
	{
	public:
		static wstring GetPlatformName ();
		static vector <int> GetVersion ();
		static bool IsVersionAtLeast (int versionNumber1, int versionNumber2, int versionNumber3 = 0);

	protected:
		SystemInfo ();
	};
}

#endif // TC_HEADER_Platform_SystemInfo
