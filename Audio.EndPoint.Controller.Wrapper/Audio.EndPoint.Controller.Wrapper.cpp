// This is the main DLL file.

#include "stdafx.h"

#include "Audio.EndPoint.Controller.Wrapper.h"
#include "AudioEndPointLibrary.h"
#include "DefSoundException.h"

using namespace System;
using namespace AudioEndPointControllerWrapper;
using namespace AudioEndPoint;


List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::getAvailableAudioDevices()
{
	return AudioController::getAudioDevices(DeviceState::Active);
}

List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::getAllAudioDevices()
{
	return AudioController::getAudioDevices(DeviceState::All);
}

List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::getAudioDevices(DeviceState state)
{
	try 
	{
		AudioDeviceListPtr audioDeviceList = CAudioEndPointLibrary::GetAudioDevices((DefSound::EDeviceState)state);

		List<AudioDeviceWrapper^>^ list = gcnew List<AudioDeviceWrapper^>();
		for (AudioDeviceList::iterator i = audioDeviceList->begin(); i != audioDeviceList->end(); i++)
		{
			AudioDeviceWrapper^ wrapper = gcnew AudioDeviceWrapper(std::move(*i));
			list->Add(wrapper);
		}
		return list;
	} catch(DefSound::CError error)
	{
		throw gcnew DefSoundException(error);
	}
}
