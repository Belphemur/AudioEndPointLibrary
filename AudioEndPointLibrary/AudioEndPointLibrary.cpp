// AudioEndPointLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AudioEndPointLibrary.h"
#include "DefSoundEndpointColl.h"


// This is the constructor of a class that has been exported.
// see AudioEndPointLibrary.h for the class definition
AudioEndPoint::CAudioEndPointLibrary::CAudioEndPointLibrary()
{
    return;
}

AudioEndPoint::AudioDeviceListPtr AudioEndPoint::CAudioEndPointLibrary::GetPlaybackDevices(DefSound::EDeviceState state)
{
	AudioDeviceListPtr list(new AudioDeviceList);
	auto collection = std::make_unique<DefSound::CEndpointCollection>(DefSound::CEndpointCollection(state, ::eRender));
	for(auto &endpoint : collection->Get())
	{
		list->push_back(std::make_shared<AudioDevice>(endpoint, Playback));
	}

	return list;
}

AudioEndPoint::AudioDeviceListPtr AudioEndPoint::CAudioEndPointLibrary::GetRecordingDevices(DefSound::EDeviceState state)
{
	AudioDeviceListPtr list(new AudioDeviceList);
	auto collection = std::make_unique<DefSound::CEndpointCollection>(DefSound::CEndpointCollection(state, ::eCapture));
	for (auto &endpoint : collection->Get())
	{
		list->push_back(std::make_shared<AudioDevice>(endpoint, Recording));
	}

	return list;
}