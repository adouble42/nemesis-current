/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#ifndef TC_HEADER_Core_CoreMacOSX
#define TC_HEADER_Core_CoreMacOSX

#include "System.h"
#include "Core/Unix/FreeBSD/CoreFreeBSD.h"

namespace nemesis
{
	class CoreMacOSX : public CoreFreeBSD
	{
	public:
		CoreMacOSX ();
		virtual ~CoreMacOSX ();

		virtual void CheckFilesystem (shared_ptr <VolumeInfo> mountedVolume, bool repair = false) const; 
		virtual shared_ptr <VolumeInfo> DismountVolume (shared_ptr <VolumeInfo> mountedVolume, bool ignoreOpenFiles = false, bool syncVolumeInfo = false);
		virtual string GetDefaultMountPointPrefix () const { return "/Volumes/truecrypt"; }

	protected:
		virtual void MountAuxVolumeImage (const DirectoryPath &auxMountPoint, const MountOptions &options) const;

	private:
		CoreMacOSX (const CoreMacOSX &);
		CoreMacOSX &operator= (const CoreMacOSX &);
	};
}

#endif // TC_HEADER_Core_CoreMacOSX
