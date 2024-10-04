//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// vvis_launcher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <direct.h>
#include "tier1/strtools.h"
#include "tier0/icommandline.h"
#include "ilaunchabledll.h"
#include <immintrin.h> 


char* GetLastErrorString()
{
	static char err[2048];
	
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	strncpy( err, (char*)lpMsgBuf, sizeof( err ) );
	LocalFree( lpMsgBuf );

	err[ sizeof( err ) - 1 ] = 0;

	return err;
}


int main(int argc, char* argv[])
{
	CommandLine()->CreateCmdLine( argc, argv );

#ifdef PLATFORM_64BITS
	const char* pDLLName = nullptr;
	int cpuinfo[4];
	__cpuid(cpuinfo, 1);
	bool sse2Supported = cpuinfo[3] & (1 << 26) || false;
	bool avxSupported = cpuinfo[2] & (1 << 28) || false;

	__cpuid(cpuinfo, 7);
	bool avx2Supported = cpuinfo[1] & (1 << 5) || false;

	bool forceSSE2 = CommandLine()->FindParm("-force_sse2");
	bool forceAVX = CommandLine()->FindParm("-force_avx");
	bool forceAVX2 = CommandLine()->FindParm("-force_avx2");

	bool forcing = forceSSE2 || forceAVX || forceAVX2;

	if ((!forcing && avx2Supported) || forceAVX2)
	{
		pDLLName = "vvis_avx2.dll";
	}
	else if ((!forcing && avxSupported) || forceAVX)
	{
		pDLLName = "vvis_avx.dll";
	}
	else if ((!forcing && sse2Supported) || forceSSE2)
	{
		pDLLName = "vvis_sse2.dll";
	}
	else
	{
		printf("vvis launcher error: unsupported CPU\n");
		return 1;
	}
	printf("vvis launcher: Using %s\n", pDLLName);
#else
	const char *pDLLName = "vvis_dll.dll";
#endif
	CSysModule *pModule = Sys_LoadModule( pDLLName );
	if ( !pModule )
	{
		printf( "vvis launcher error: can't load %s\n%s", pDLLName, GetLastErrorString() );
		return 1;
	}

	CreateInterfaceFn fn = Sys_GetFactory( pModule );
	if( !fn )
	{
		printf( "vvis launcher error: can't get factory from %s\n", pDLLName );
		Sys_UnloadModule( pModule );
		return 2;
	}

	int retCode = 0;
	ILaunchableDLL *pDLL = (ILaunchableDLL*)fn( LAUNCHABLE_DLL_INTERFACE_VERSION, &retCode );
	if( !pDLL )
	{
		printf( "vvis launcher error: can't get IVVisDLL interface from %s\n", pDLLName );
		Sys_UnloadModule( pModule );
		return 3;
	}

	pDLL->main( argc, argv );
	Sys_UnloadModule( pModule );

	return 0;
}

