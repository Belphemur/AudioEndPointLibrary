// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define ReturnIfFailed(hr) if (FAILED(hr)) return hr;

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <streams.h>
#include <combase.h>
#include <wxutil.h>

#include "ExportDll.h"

