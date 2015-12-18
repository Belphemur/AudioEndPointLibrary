// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

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

#include <comdef.h>
#pragma comment(lib, "comsuppw.lib")
#include <comip.h>
#include <comutil.h>

#include "ExportDll.h"

#include "IAudioDevice.h"
#include "SharedNativePtr.h"
#include "AudioDevice.h"
#include "AudioEndPointLibraryImpl.h"
#include <DefSoundError.h>
#include <list>

