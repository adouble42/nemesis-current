/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Volume_Version
#define TC_HEADER_Volume_Version

#include "Platform/PlatformBase.h"
#include "Common/Tcdefs.h"

namespace nemesis
{
	class Version
	{
	public:
		static const string String ()					{ return VERSION_STRING; }
		static const uint16 Number ()					{ return VERSION_NUM; }
	};
}

#endif // TC_HEADER_Volume_Version
