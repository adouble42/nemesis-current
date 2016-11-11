/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Core_Unix_MountedFilesystem
#define TC_HEADER_Core_Unix_MountedFilesystem

#include "Platform/Platform.h"

namespace nemesis
{
	struct MountedFilesystem
	{
	public:
		DevicePath Device;
		DirectoryPath MountPoint;
		string Type;
	};

	typedef list < shared_ptr <MountedFilesystem> > MountedFilesystemList;
}

#endif // TC_HEADER_Core_Unix_MountedFilesystem
