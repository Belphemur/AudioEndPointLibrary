#include "stdafx.h"
#include "AudioDevice.h"
#include <DefSoundIsDefaultEndpoint.h>


void AudioEndPoint::AudioDevice::SetDefault(ERole role) const
{
	DefSound::CEndpointCollection::SetDefaultEndpoint(endpoint, role);
}

bool AudioEndPoint::AudioDevice::IsDefault(ERole role) const
{
	return DefSound::CIsDefaultEndpoint(role)(endpoint);
}

AudioEndPoint::AudioDevice::~AudioDevice()
{
}
