// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define ReturnIfFailed(hr) if (FAILED(hr)) return hr;
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

#define SAFE_DELETE(punk)  \
              if ((punk) != NULL)  \
                { delete (punk); (punk) = NULL; }

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "ExportDll.h"

