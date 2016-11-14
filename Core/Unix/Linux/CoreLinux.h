/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#ifndef TC_HEADER_Core_CoreLinux
#define TC_HEADER_Core_CoreLinux

#include "System.h"
#include "Core/Unix/CoreUnix.h"

namespace nemesis
{
	class CoreLinux : public CoreUnix
	{
	public:
		CoreLinux ();
		virtual ~CoreLinux ();

		virtual HostDeviceList GetHostDevices (bool pathListOnly = false) const; 

	protected:
		virtual DevicePath AttachFileToLoopDevice (const FilePath &filePath, bool readOnly) const;
		virtual void DetachLoopDevice (const DevicePath &devicePath) const;
		virtual void DismountNativeVolume (shared_ptr <VolumeInfo> mountedVolume) const;
		virtual MountedFilesystemList GetMountedFilesystems (const DevicePath &devicePath = DevicePath(), const DirectoryPath &mountPoint = DirectoryPath()) const;
		virtual void MountFilesystem (const DevicePath &devicePath, const DirectoryPath &mountPoint, const string &filesystemType, bool readOnly, const string &systemMountOptions) const;
		virtual void MountVolumeNative (shared_ptr <Volume> volume, MountOptions &options, const DirectoryPath &auxMountPoint) const;

	private:
		CoreLinux (const CoreLinux &);
		CoreLinux &operator= (const CoreLinux &);
	};
}

#endif // TC_HEADER_Core_CoreLinux
