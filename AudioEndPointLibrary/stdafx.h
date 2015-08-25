// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>


#ifdef  _DEBUG
#define VERIFY(x)   _ASSERT(x)
#else   // _DEBUG
#define VERIFY(x)   (x)
#endif  // _DEBUG

#include <string>
#include <MmDeviceApi.h>
#include <vector>
#include <memory>

#pragma comment(lib, "comsuppw.lib")
#include <comip.h>
#include <comutil.h>

#include "DefSoundError.h"
#include "ExportDll.h"

