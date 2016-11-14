/*
 Copyright (c) 2016 nemesis project/mrn@sdf.org. All rights reserved.
 http://mrn.sixbit.org/

 Governed by the BSD 2 Clause license, the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
 Based on TrueCrypt 7.1a, which was governed by the TrueCrypt license, which
 is also made available with nemesis.
*/

#include "CoreException.h"
#include "Platform/SerializerFactory.h"

namespace nemesis
{
	void ElevationFailed::Deserialize (shared_ptr <Stream> stream)
	{
		ExecutedProcessFailed::Deserialize (stream);
	}

	void ElevationFailed::Serialize (shared_ptr <Stream> stream) const
	{
		ExecutedProcessFailed::Serialize (stream);
	}

#define TC_EXCEPTION(TYPE) TC_SERIALIZER_FACTORY_ADD(TYPE)
#undef TC_EXCEPTION_NODECL
#define TC_EXCEPTION_NODECL(TYPE) TC_SERIALIZER_FACTORY_ADD(TYPE)

	TC_SERIALIZER_FACTORY_ADD_EXCEPTION_SET (CoreException);
}
