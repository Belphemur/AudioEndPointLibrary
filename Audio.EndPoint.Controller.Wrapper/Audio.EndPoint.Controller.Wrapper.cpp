// This is the main DLL file.

#include "stdafx.h"

#include "Audio.EndPoint.Controller.Wrapper.h"
#include "AudioEndPointLibrary.h"
#include "DefSoundException.h"

using namespace System;
using namespace AudioEndPointControllerWrapper;
using namespace AudioEndPoint;


List<AudioDeviceWrapper^>^ GetAudioDevicesWrapperList(AudioDeviceListPtr& audioDeviceList)
{
	List<AudioDeviceWrapper^>^ list = gcnew List<AudioDeviceWrapper^>();
	for (AudioDeviceList::iterator i = audioDeviceList->begin(); i != audioDeviceList->end(); i++)
	{
		AudioDeviceWrapper^ wrapper = gcnew AudioDeviceWrapper(std::move(*i));
		list->Add(wrapper);
	}
	return list;
}

List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::GetActivePlaybackDevices()
{
	return AudioController::GetPlaybackDevices(DeviceState::Active);
}

List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::GetAllPlaybackDevices()
{
	return AudioController::GetPlaybackDevices(DeviceState::All);
}


List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::GetPlaybackDevices(DeviceState state)
{
	try 
	{
		AudioDeviceListPtr audioDeviceList = CAudioEndPointLibrary::GetPlaybackDevices(static_cast<DefSound::EDeviceState>(state));
		return GetAudioDevicesWrapperList(audioDeviceList);
	} catch(DefSound::CError error)
	{
		throw gcnew DefSoundException(error);
	}
}

List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::GetRecordingDevices(DeviceState state)
{
	try
	{
		AudioDeviceListPtr audioDeviceList = CAudioEndPointLibrary::GetRecordingDevices(static_cast<DefSound::EDeviceState>(state));
		return GetAudioDevicesWrapperList(audioDeviceList);
	}
	catch (DefSound::CError error)
	{
		throw gcnew DefSoundException(error);
	}
}

List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::GetActiveRecordingDevices()
{
	return AudioController::GetRecordingDevices(DeviceState::Active);
}

List<AudioDeviceWrapper^>^ AudioEndPointControllerWrapper::AudioController::GetAllRecordingDevices()
{
	return AudioController::GetRecordingDevices(DeviceState::All);
}
