/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#ifndef TC_HEADER_Core_Unix_CoreService
#define TC_HEADER_Core_Unix_CoreService

#include "CoreServiceRequest.h"
#include "Platform/Stream.h"
#include "Platform/Unix/Pipe.h"
#include "Core/Core.h"

namespace nemesis
{
	// This service facilitates process forking and elevation of user privileges
	class CoreService
	{
	public:
		static void ProcessElevatedRequests ();
		static void ProcessRequests (int inputFD = -1, int outputFD = -1);
		static void RequestCheckFilesystem (shared_ptr <VolumeInfo> mountedVolume, bool repair);
		static void RequestDismountFilesystem (const DirectoryPath &mountPoint, bool force);
		static shared_ptr <VolumeInfo> RequestDismountVolume (shared_ptr <VolumeInfo> mountedVolume, bool ignoreOpenFiles = false, bool syncVolumeInfo = false);
		static uint32 RequestGetDeviceSectorSize (const DevicePath &devicePath);
		static uint64 RequestGetDeviceSize (const DevicePath &devicePath);
		static HostDeviceList RequestGetHostDevices (bool pathListOnly);
		static shared_ptr <VolumeInfo> RequestMountVolume (MountOptions &options);
		static void RequestSetFileOwner (const FilesystemPath &path, const UserId &owner);
		static void SetAdminPasswordCallback (shared_ptr <GetStringFunctor> functor) { AdminPasswordCallback = functor; }
		static void Start ();
		static void Stop ();

	protected:
		template <class T> static auto_ptr <T> GetResponse ();
		template <class T> static auto_ptr <T> SendRequest (CoreServiceRequest &request);
		static void StartElevated (const CoreServiceRequest &request);

		static shared_ptr <GetStringFunctor> AdminPasswordCallback;

		static auto_ptr <Pipe> AdminInputPipe;
		static auto_ptr <Pipe> AdminOutputPipe;

		static auto_ptr <Pipe> InputPipe;
		static auto_ptr <Pipe> OutputPipe;
		static shared_ptr <Stream> ServiceInputStream;
		static shared_ptr <Stream> ServiceOutputStream;

		static bool ElevatedPrivileges;
		static bool ElevatedServiceAvailable;
		static bool Running;

	private:
		CoreService ();
	};

#define TC_CORE_SERVICE_CMDLINE_OPTION "--core-service"
}

#endif // TC_HEADER_Core_Unix_CoreService
