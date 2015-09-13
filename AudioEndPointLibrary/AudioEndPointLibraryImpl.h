#pragma once
#include <list>
#include <memory>
#include <comdef.h>
#include "AudioDevice.h"
#include "Signal.h"

namespace AudioEndPoint {
    typedef std::shared_ptr<AudioDevice> AudioDevicePtr;
    typedef std::list<AudioDevicePtr> AudioDeviceList;
    typedef Signal<AudioDevicePtr> DeviceSignal;
    typedef Signal<AudioDevicePtr, DefSound::EDeviceState, DefSound::EDeviceState> DeviceStateSignal;
    typedef Signal<AudioDevicePtr, ERole> DeviceDefaultSignal;

    _COM_SMARTPTR_TYPEDEF(IMMNotificationClient, __uuidof(IMMNotificationClient));

    struct AudioEndPointLibraryImpl
    {
        AudioDeviceList m_playback;
        AudioDeviceList m_recording;
        IMMNotificationClientPtr m_notif_client;
     
    };

    struct AudioEndPointLibrarySignals
    {
        DeviceSignal DeviceRemoved;
        DeviceSignal DeviceAdded;
        DeviceStateSignal DeviceStateChanged;
        DeviceDefaultSignal DeviceDefaultChanged;
    };
}