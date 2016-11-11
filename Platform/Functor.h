/*
 Copyright (c) 2008 nemesis Developers Association. All rights reserved.

 Governed by the nemesis License 3.0 the full text of which is contained in
 the file License.txt included in nemesis binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Platform_Functor
#define TC_HEADER_Platform_Functor

#include "PlatformBase.h"

namespace nemesis
{
	struct Functor
	{
		virtual ~Functor () { }
		virtual void operator() () = 0;
	};

	struct GetStringFunctor
	{
		virtual ~GetStringFunctor () { }
		virtual void operator() (string &str) = 0;
	};
}

#endif // TC_HEADER_Platform_Functor
