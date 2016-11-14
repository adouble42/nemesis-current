/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#ifndef TC_HEADER_Core_CoreException
#define TC_HEADER_Core_CoreException

#include "Platform/Platform.h"

namespace nemesis
{
	struct ElevationFailed : public ExecutedProcessFailed
	{
		ElevationFailed () { }
		ElevationFailed (const string &message, const string &command, int exitCode, const string &errorOutput)
			: ExecutedProcessFailed (message, command, exitCode, errorOutput) { }
		TC_SERIALIZABLE_EXCEPTION (ElevationFailed);
	};

	TC_EXCEPTION_DECL (RootDeviceUnavailable, SystemException);

#define TC_EXCEPTION(NAME) TC_EXCEPTION_DECL(NAME,Exception)

#undef TC_EXCEPTION_SET
#define TC_EXCEPTION_SET \
	TC_EXCEPTION_NODECL (ElevationFailed); \
	TC_EXCEPTION_NODECL (RootDeviceUnavailable); \
	TC_EXCEPTION (DriveLetterUnavailable); \
	TC_EXCEPTION (DriverError); \
	TC_EXCEPTION (EncryptedSystemRequired); \
	TC_EXCEPTION (HigherFuseVersionRequired); \
	TC_EXCEPTION (KernelCryptoServiceTestFailed); \
	TC_EXCEPTION (LoopDeviceSetupFailed); \
	TC_EXCEPTION (MountPointRequired); \
	TC_EXCEPTION (MountPointUnavailable); \
	TC_EXCEPTION (NoDriveLetterAvailable); \
	TC_EXCEPTION (TemporaryDirectoryFailure); \
	TC_EXCEPTION (UnsupportedSectorSizeHiddenVolumeProtection); \
	TC_EXCEPTION (UnsupportedSectorSizeNoKernelCrypto); \
	TC_EXCEPTION (VolumeAlreadyMounted); \
	TC_EXCEPTION (VolumeSlotUnavailable);

	TC_EXCEPTION_SET;

#undef TC_EXCEPTION
}

#endif // TC_HEADER_Core_CoreException
