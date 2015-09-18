// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers



// TODO: reference additional headers your program requires here

#ifdef  _DEBUG
#define VERIFY(x)   _ASSERT(x)
#else   // _DEBUG
#define VERIFY(x)   (x)
#endif  // _DEBUG

#include <string>
#include <MmDeviceApi.h>
#include <vector>
#include <memory>
#include <array>

#pragma comment(lib, "comsuppw.lib")
#include <comip.h>
#include <comutil.h>
#include <mmdeviceapi.h>

#include "DefSoundError.h"
