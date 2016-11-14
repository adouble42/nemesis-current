/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/ucred.h>
#include <sys/mount.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "CoreMacOSX.h"
#include "Driver/Fuse/FuseService.h"
#include "Core/Unix/CoreServiceProxy.h"

namespace nemesis
{
	CoreMacOSX::CoreMacOSX ()
	{
	}

	CoreMacOSX::~CoreMacOSX ()
	{
	}

	shared_ptr <VolumeInfo> CoreMacOSX::DismountVolume (shared_ptr <VolumeInfo> mountedVolume, bool ignoreOpenFiles, bool syncVolumeInfo)
	{
		if (!mountedVolume->VirtualDevice.IsEmpty() && mountedVolume->VirtualDevice.IsBlockDevice())
		{
			list <string> args;
			args.push_back ("detach");
			args.push_back (mountedVolume->VirtualDevice);

			if (ignoreOpenFiles)
				args.push_back ("-force");

			try
			{
				Process::Execute ("hdiutil", args);
			}
			catch (ExecutedProcessFailed &e)
			{
				if (!ignoreOpenFiles)
				{
					string err = e.GetErrorOutput();

					if (err.find ("couldn't unmount") != string::npos
						|| err.find ("busy") != string::npos
						|| err.find ("49153") != string::npos)
					{
						throw MountedVolumeInUse (SRC_POS);
					}
				}

				throw;
			}
		}

		if (syncVolumeInfo || mountedVolume->Protection == VolumeProtection::HiddenVolumeReadOnly)
		{
			sync();
			VolumeInfoList ml = GetMountedVolumes (mountedVolume->Path);

			if (ml.size() > 0)
				mountedVolume = ml.front();
		}

		list <string> args;
		args.push_back ("--");
		args.push_back (mountedVolume->AuxMountPoint);

		for (int t = 0; true; t++)
		{
			try
			{
				Process::Execute ("umount", args);
				break;
			}
			catch (ExecutedProcessFailed&)
			{
				if (t > 10)
					throw;
				Thread::Sleep (200);
			}
		}

		try
		{
			mountedVolume->AuxMountPoint.Delete();
		}
		catch (...)	{ }

		return mountedVolume;
	}

	void CoreMacOSX::CheckFilesystem (shared_ptr <VolumeInfo> mountedVolume, bool repair) const
	{
		list <string> args;
		args.push_back ("/Applications/Utilities/Disk Utility.app");
		Process::Execute ("open", args);
	}

	void CoreMacOSX::MountAuxVolumeImage (const DirectoryPath &auxMountPoint, const MountOptions &options) const
	{
		// Check FUSE version
		char fuseVersionString[MAXHOSTNAMELEN + 1] = { 0 };
		size_t fuseVersionStringLength = MAXHOSTNAMELEN;

//		if (sysctlbyname ("macfuse.version.number", fuseVersionString, &fuseVersionStringLength, NULL, 0) != 0)
//			throw HigherFuseVersionRequired (SRC_POS);

//		vector <string> fuseVersion = StringConverter::Split (string (fuseVersionString), ".");
//		if (fuseVersion.size() < 2)
//			throw HigherFuseVersionRequired (SRC_POS);

//		uint32 fuseVersionMajor = StringConverter::ToUInt32 (fuseVersion[0]);
//		uint32 fuseVersionMinor = StringConverter::ToUInt32 (fuseVersion[1]);

//		if (fuseVersionMajor < 1 || (fuseVersionMajor == 1 && fuseVersionMinor < 3))
//			throw HigherFuseVersionRequired (SRC_POS);

		// Mount volume image
		string volImage = string (auxMountPoint) + FuseService::GetVolumeImagePath();

		list <string> args;
		args.push_back ("attach");
		args.push_back ("-plist");
		args.push_back ("-noautofsck");
		args.push_back ("-imagekey");
		args.push_back ("diskimage-class=CRawDiskImage");
		args.push_back (volImage);

				if (!options.NoFilesystem && options.MountPoint && !options.MountPoint->IsEmpty())
		{
			// Let the system specify mount point except when the user specified a non-default one
			if (string (*options.MountPoint).find (GetDefaultMountPointPrefix()) != 0)
			{
			        args.push_back ("-mount");
			        args.push_back ("required");
				args.push_back ("-mountpoint");
				args.push_back (*options.MountPoint);
			}
					}
			if (options.NoFilesystem) {
				args.push_back ("-nomount");
			}
		if (options.Protection == VolumeProtection::ReadOnly)
			args.push_back ("-readonly");

		std::string xml;
		
		while (true)
		{
			try
			{
       			  printf("prcocessing\n");
				xml = Process::Execute ("hdiutil", args);
				break;
			}
			catch (ExecutedProcessFailed &e)
			{
				if (e.GetErrorOutput().find ("noautofsck") != string::npos)
				{
					args.remove ("-noautofsck");
					continue;
				}
				
				throw;
			}
		}

		size_t p = xml.find ("<key>dev-entry</key>");
		if (p == string::npos)
			throw ParameterIncorrect (SRC_POS);

		p = xml.find ("<string>", p);
		if (p == string::npos)
			throw ParameterIncorrect (SRC_POS);
		p += 8;

		size_t e = xml.find ("</string>", p);
		if (e == string::npos)
			throw ParameterIncorrect (SRC_POS);
		printf("stringconverter\n");
		DevicePath virtualDev = StringConverter::Trim (xml.substr (p, e - p));
		cout << "virtualDev: " << StringConverter::Trim (xml.substr (p, e - p));
		try
		{
			FuseService::SendAuxDeviceInfo (auxMountPoint, virtualDev);
		}
		catch (...)
		{
			printf("survived fuseservice\n");
		        try
			{
				list <string> args;
				args.push_back ("detach");
				args.push_back (volImage);
				args.push_back ("-force");

				Process::Execute ("hdiutil", args);
			}
			catch (ExecutedProcessFailed&) { }
			throw;
		}
	}

	auto_ptr <CoreBase> Core (new CoreServiceProxy <CoreMacOSX>);
	auto_ptr <CoreBase> CoreDirect (new CoreMacOSX);
}
