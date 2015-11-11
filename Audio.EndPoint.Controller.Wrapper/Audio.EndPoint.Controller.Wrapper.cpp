// This is the main DLL file.

#include "stdafx.h"

#include "Audio.EndPoint.Controller.Wrapper.h"
#include "AudioEndPointLibrary.h"
#include "DefSoundException.h"
#include "AudioDeviceWrapper.h"
#include "EventWrapper.h"

using namespace System;
using namespace AudioEndPoint;

namespace AudioEndPointControllerWrapper {
#ifndef APPVEYOR //AppVeyor don't like to do anything with Audio. (Windows Server)
    static EventWrapper wrapper;
#endif

    List<IAudioDevice^>^ GetAudioDevicesWrapperList(AudioDeviceList& audioDeviceList)
    {
        List<IAudioDevice^>^ list = gcnew List<IAudioDevice^>();
        for (AudioDeviceList::iterator i = audioDeviceList.begin(); i != audioDeviceList.end(); i++)
        {
            IAudioDevice^ wrapper = gcnew AudioDeviceWrapper(*i);
            list->Add(wrapper);
        }
        return list;
    }

    List<IAudioDevice^>^ AudioController::GetActivePlaybackDevices()
    {
        return AudioController::GetPlaybackDevices(DeviceState::Active);
    }

    List<IAudioDevice^>^ AudioController::GetAllPlaybackDevices()
    {
        return AudioController::GetPlaybackDevices(DeviceState::All);
    }


    List<IAudioDevice^>^ AudioController::GetPlaybackDevices(DeviceState state)
    {
        try
        {
            AudioDeviceList audioDeviceList = CAudioEndPointLibrary::GetInstance().GetPlaybackDevices(static_cast<DefSound::EDeviceState>(state));
            return GetAudioDevicesWrapperList(audioDeviceList);
        }
        catch (DefSound::CError error)
        {
            throw gcnew DefSoundException(error);
        }
    }

    List<IAudioDevice^>^ AudioController::GetRecordingDevices(DeviceState state)
    {
        try
        {
            AudioDeviceList audioDeviceList = CAudioEndPointLibrary::GetInstance().GetRecordingDevices(static_cast<DefSound::EDeviceState>(state));
            return GetAudioDevicesWrapperList(audioDeviceList);
        }
        catch (DefSound::CError error)
        {
            throw gcnew DefSoundException(error);
        }
    }

    List<IAudioDevice^>^ AudioController::GetActiveRecordingDevices()
    {
        return AudioController::GetRecordingDevices(DeviceState::Active);
    }

    List<IAudioDevice^>^ AudioController::GetAllRecordingDevices()
    {
        return AudioController::GetRecordingDevices(DeviceState::All);
    }

    bool AudioController::IsNotificationAvailable()
    {
#ifndef APPVEYOR
        return wrapper.IsLoaded();
#else
        return false;
#endif
    }

    void AudioController::RaiseAdded(DeviceAddedEvent ^ devEvent)
    {
        DeviceAdded(nullptr, devEvent);
    }
    void AudioController::RaiseRemoved(DeviceRemovedEvent ^ devEvent)
    {
        DeviceRemoved(nullptr, devEvent);
    }
    void AudioController::RaiseState(DeviceStateChangedEvent ^ devEvent)
    {
        DeviceStateChanged(nullptr, devEvent);
    }
    void AudioController::RaiseDefault(DeviceDefaultChangedEvent ^ devEvent)
    {
        DeviceDefaultChanged(nullptr, devEvent);
    }
}