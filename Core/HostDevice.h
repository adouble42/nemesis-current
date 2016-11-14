/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
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
