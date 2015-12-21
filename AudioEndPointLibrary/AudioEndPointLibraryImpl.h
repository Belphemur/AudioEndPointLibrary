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

    _COM_SMARTPTR_TYPEDEF(IMMDeviceEnumerator, __uuidof(IMMDeviceEnumerator));
    _COM_SMARTPTR_TYPEDEF(IMMNotificationClient, __uuidof(IMMNotificationClient));
    struct AudioEndPointLibrarySignals
    {
        DeviceSignal DeviceRemoved;
        DeviceSignal DeviceAdded;
        DeviceStateSignal DeviceStateChanged;
        DeviceDefaultSignal DeviceDefaultChanged;
    };
}