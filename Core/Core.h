/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#ifndef TC_HEADER_Core_Core
#define TC_HEADER_Core_Core

#include "CoreBase.h"

namespace nemesis
{
	extern auto_ptr <CoreBase> Core;
	extern auto_ptr <CoreBase> CoreDirect;
}

#endif // TC_HEADER_Core_Core
