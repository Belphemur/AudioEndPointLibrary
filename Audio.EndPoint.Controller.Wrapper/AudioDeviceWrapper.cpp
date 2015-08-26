#include "stdafx.h"
#include "AudioDeviceWrapper.h"

using namespace System;


void AudioEndPointControllerWrapper::AudioDeviceWrapper::SetAsDefault(Role role)
{
	(_audioDevice.get())->get()->SetDefault(static_cast<::ERole>(role));
}

bool AudioEndPointControllerWrapper::AudioDeviceWrapper::IsDefault(Role role)
{
	return (_audioDevice.get())->get()->IsDefault(static_cast<::ERole>(role));
}
