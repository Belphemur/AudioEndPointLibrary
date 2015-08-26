#include "stdafx.h"
#include "AudioDeviceWrapper.h"
#include "DefSoundException.h"

using namespace System;


void AudioEndPointControllerWrapper::AudioDeviceWrapper::SetAsDefault(Role role)
{
	try 
	{
		(_audioDevice.get())->get()->SetDefault(static_cast<::ERole>(role));
	}
	catch (DefSound::CNotActiveError error)
	{
		throw gcnew DefSoundNotActiveException(error);
	}
	catch(DefSound::CError error)
	{
		throw gcnew DefSoundException(error);
	}
}

bool AudioEndPointControllerWrapper::AudioDeviceWrapper::IsDefault(Role role)
{
	try
	{
		return (_audioDevice.get())->get()->IsDefault(static_cast<::ERole>(role));
	}
	catch (DefSound::CNotActiveError error)
	{
		throw gcnew DefSoundNotActiveException(error);
	}
	catch (DefSound::CError error)
	{
		throw gcnew DefSoundException(error);
	}
}
