// This is the main DLL file.

#include "stdafx.h"

#include "Audio.EndPoint.Controller.Wrapper.h"
#include "AudioEndPointLibrary.h"
#include "DefSoundException.h"
#include "AudioDeviceWrapper.h"

using namespace System;
using namespace AudioEndPoint;

namespace AudioEndPointControllerWrapper {

    static AudioController::AudioController()
    {
        CAudioEndPointLibrary::GetInstance().Signals.DeviceAdded.Register([](AudioDevicePtr device)
        {
            AudioController::DeviceAdded(nullptr, gcnew DeviceAddedEvent(gcnew AudioDeviceWrapper(device)));
        });

        CAudioEndPointLibrary::GetInstance().Signals.DeviceRemoved.Register([](AudioDevicePtr device)
        {
            AudioController::DeviceRemoved(nullptr, gcnew DeviceRemovedEvent(gcnew AudioDeviceWrapper(device)));
        });

        CAudioEndPointLibrary::GetInstance().Signals.DeviceDefaultChanged.Register([](AudioDevicePtr device, ERole role)
        {
            AudioController::DeviceDefaultChanged(nullptr, gcnew DeviceDefaultChangedEvent(gcnew AudioDeviceWrapper(device), static_cast<Role>(role)));
        });

        CAudioEndPointLibrary::GetInstance().Signals.DeviceStateChanged.Register([](AudioDevicePtr device, DefSound::EDeviceState prev, DefSound::EDeviceState cur)
        {
            AudioController::DeviceStateChanged(nullptr, gcnew DeviceStateChangedEvent(gcnew AudioDeviceWrapper(device), static_cast<DeviceState>(prev), static_cast<DeviceState>(cur)));
        });
    }

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
}