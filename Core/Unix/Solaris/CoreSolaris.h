/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#ifndef TC_HEADER_Core_CoreSolaris
#define TC_HEADER_Core_CoreSolaris

#include "System.h"
#include "Core/Unix/CoreUnix.h"

namespace nemesis
{
	class CoreSolaris : public CoreUnix
	{
	public:
		CoreSolaris ();
		virtual ~CoreSolaris ();

		virtual HostDeviceList GetHostDevices (bool pathListOnly = false) const; 

	protected:
		virtual DevicePath AttachFileToLoopDevice (const FilePath &filePath, bool readOnly) const;
		virtual void DetachLoopDevice (const DevicePath &devicePath) const;
		virtual MountedFilesystemList GetMountedFilesystems (const DevicePath &devicePath = DevicePath(), const DirectoryPath &mountPoint = DirectoryPath()) const;
		virtual void MountFilesystem (const DevicePath &devicePath, const DirectoryPath &mountPoint, const string &filesystemType, bool readOnly, const string &systemMountOptions) const;

	private:
		CoreSolaris (const CoreSolaris &);
		CoreSolaris &operator= (const CoreSolaris &);
	};
}

#endif // TC_HEADER_Core_CoreSolaris
