// AudioEndPointLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AudioEndPointLibrary.h"


// This is an example of an exported variable
AUDIOENDPOINTLIBRARY_API int nAudioEndPointLibrary=0;

// This is an example of an exported function.
AUDIOENDPOINTLIBRARY_API int fnAudioEndPointLibrary(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see AudioEndPointLibrary.h for the class definition
CAudioEndPointLibrary::CAudioEndPointLibrary()
{
    return;
}
