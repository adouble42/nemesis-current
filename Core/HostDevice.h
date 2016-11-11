/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Core_HostDevice
#define TC_HEADER_Core_HostDevice

#include "Platform/Platform.h"
#include "Platform/Serializable.h"

namespace nemesis
{
	struct HostDevice;
	typedef list < shared_ptr <HostDevice> > HostDeviceList;

	struct HostDevice : public Serializable
	{
		HostDevice ()
			: Removable (false),
			Size (0)
		{
		}

		virtual ~HostDevice ()
		{
		}

		TC_SERIALIZABLE (HostDevice);

		DirectoryPath MountPoint;
		wstring Name;
		DevicePath Path;
		bool Removable;
		uint64 Size;
		uint32 SystemNumber;

		HostDeviceList Partitions;
	};
}

#endif // TC_HEADER_Core_HostDevice
