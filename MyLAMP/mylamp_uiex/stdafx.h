// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>



// TODO: reference additional headers your program requires here
#define MYLAMP_HEADER_EXPORT 1
#include "../mylamp_core/mylamp_header.h"

#pragma comment(lib, "dwmapi.lib") 
#include <dwmapi.h>
//TODO: check define
//#define WINVER         0x0600
//#define _WIN32_WINNT   0x0600
//#define _WIN32_IE      0x0700