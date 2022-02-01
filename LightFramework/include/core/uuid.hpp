#ifndef __UUID_H__
#define __UUID_H__

#include <string>

extern "C"
{
	#if defined _WIN32
	#include <Rpc.h>
	#else
	#include <uuid/uuid.h>
	#endif
}

inline std::string newUUID()
{
#ifdef _WIN32
	UUID uuid;
	UuidCreate ( &uuid );

	unsigned char * str;
	UuidToStringA ( &uuid, &str );

	std::string s( ( char* ) str );

	RpcStringFreeA ( &str );
#else
	uuid_t uuid;
	uuid_generate_random ( uuid );
	char s[37];
	uuid_unparse ( uuid, s );
#endif
	return s;
}

#endif // __UUID_H__