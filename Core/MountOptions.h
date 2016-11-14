/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#ifndef TC_HEADER_Core_MountOptions
#define TC_HEADER_Core_MountOptions

#include "Platform/Serializable.h"
#include "Volume/Keyfile.h"
#include "Volume/Volume.h"
#include "Volume/VolumeSlot.h"
#include "Volume/VolumePassword.h"

namespace nemesis
{
	struct MountOptions : public Serializable
	{
		MountOptions ()
			:
			CachePassword (false),
			NoFilesystem (false),
			NoHardwareCrypto (false),
			NoKernelCrypto (false),
			PartitionInSystemEncryptionScope (false),
			PreserveTimestamps (true),
			Protection (VolumeProtection::None),
			Removable (false),
			SharedAccessAllowed (false),
			SlotNumber (0),
			UseBackupHeaders (false)
		{
		}

		MountOptions (const MountOptions &other) { CopyFrom (other); }
		virtual ~MountOptions () { }

		MountOptions &operator= (const MountOptions &other) { CopyFrom (other); return *this; }

		TC_SERIALIZABLE (MountOptions);

		bool CachePassword;
		wstring FilesystemOptions;
		wstring FilesystemType;
		shared_ptr <KeyfileList> Keyfiles;
		shared_ptr <DirectoryPath> MountPoint;
		bool NoFilesystem;
		bool NoHardwareCrypto;
		bool NoKernelCrypto;
		shared_ptr <VolumePassword> Password;
		bool PartitionInSystemEncryptionScope;
		shared_ptr <VolumePath> Path;
		bool PreserveTimestamps;
		VolumeProtection::Enum Protection;
		shared_ptr <VolumePassword> ProtectionPassword;
		shared_ptr <KeyfileList> ProtectionKeyfiles;
		bool Removable;
		bool SharedAccessAllowed;
		VolumeSlotNumber SlotNumber;
		bool UseBackupHeaders;

	protected:
		void CopyFrom (const MountOptions &other);
	};
}

#endif // TC_HEADER_Core_MountOptions
