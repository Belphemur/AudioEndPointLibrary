// This is the main DLL file.

#include "stdafx.h"

#include "Audio.EndPoint.Controller.Wrapper.h"
#include "AudioEndPointLibrary.h"
#include "DefSoundException.h"
#include "AudioDeviceWrapper.h"

using namespace System;
using namespace AudioEndPointControllerWrapper;
using namespace AudioEndPoint;


List<IAudioDevice^>^ GetAudioDevicesWrapperList(AudioDeviceListPtr& audioDeviceList)
{
	List<IAudioDevice^>^ list = gcnew List<IAudioDevice^>();
	for (AudioDeviceList::iterator i = audioDeviceList->begin(); i != audioDeviceList->end(); i++)
	{
		IAudioDevice^ wrapper = gcnew AudioDeviceWrapper(*i);
		list->Add(wrapper);
	}
	return list;
}

List<IAudioDevice^>^ AudioEndPointControllerWrapper::AudioController::GetActivePlaybackDevices()
{
	return AudioController::GetPlaybackDevices(DeviceState::Active);
}

List<IAudioDevice^>^ AudioEndPointControllerWrapper::AudioController::GetAllPlaybackDevices()
{
	return AudioController::GetPlaybackDevices(DeviceState::All);
}


List<IAudioDevice^>^ AudioEndPointControllerWrapper::AudioController::GetPlaybackDevices(DeviceState state)
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

List<IAudioDevice^>^ AudioEndPointControllerWrapper::AudioController::GetRecordingDevices(DeviceState state)
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

List<IAudioDevice^>^ AudioEndPointControllerWrapper::AudioController::GetActiveRecordingDevices()
{
	return AudioController::GetRecordingDevices(DeviceState::Active);
}

List<IAudioDevice^>^ AudioEndPointControllerWrapper::AudioController::GetAllRecordingDevices()
{
	return AudioController::GetRecordingDevices(DeviceState::All);
}
