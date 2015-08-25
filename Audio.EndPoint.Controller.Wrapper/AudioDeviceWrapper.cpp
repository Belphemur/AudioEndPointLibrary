#include "stdafx.h"
#include "AudioDeviceWrapper.h"

using namespace System;


void AudioEndPointControllerWrapper::AudioDeviceWrapper::SetAsDefault(Role role)
{
	this->_audioDevice->SetDefault(static_cast<::ERole>(role));
}

bool AudioEndPointControllerWrapper::AudioDeviceWrapper::IsDefault(Role role)
{
	return this->_audioDevice->IsDefault(static_cast<::ERole>(role));
}
