#include "stdafx.h"
#include "AudioDevice.h"


void AudioEndPoint::AudioDevice::SetDefault(ERole role)
{
	for (SIZE_T i = 0; i < sizeof(isDefault); i++)
	{
		isDefault[i] = false;
	}
	isDefault[role] = true;
}

AudioEndPoint::AudioDevice::AudioDevice()
{
	isDefault = new bool[::ERole_enum_count];
}


AudioEndPoint::AudioDevice::~AudioDevice()
{
	delete isDefault;
}
