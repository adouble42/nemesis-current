/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#ifndef TC_HEADER_Core_FatFormatter
#define TC_HEADER_Core_FatFormatter

#include "Platform/Platform.h"

namespace nemesis
{
	class FatFormatter
	{
	public:
		struct WriteSectorCallback
		{
			virtual ~WriteSectorCallback () { }
			virtual bool operator() (const BufferPtr &sector) = 0;
		};

		static void Format (WriteSectorCallback &writeSector, uint64 deviceSize, uint32 clusterSize, uint32 sectorSize);
	};
}

#endif // TC_HEADER_Core_FatFormatter
