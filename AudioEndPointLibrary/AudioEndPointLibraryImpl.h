#pragma once
#include <list>
#include <memory>
#include <comdef.h>
#include "AudioDevice.h"
#include "Signal.h"

namespace AudioEndPoint {
    EXPIMP_TEMPLATE class AUDIOENDPOINTLIBRARY_API  std::mutex;
    EXPIMP_TEMPLATE template class AUDIOENDPOINTLIBRARY_API  std::shared_ptr<AudioDevice>;
    typedef std::shared_ptr<AudioDevice> AudioDevicePtr;
    EXPIMP_TEMPLATE template class AUDIOENDPOINTLIBRARY_API std::list<AudioDevicePtr>;
    typedef std::list<AudioDevicePtr> AudioDeviceList;
    typedef Signal<AudioDevicePtr> DeviceSignal;
    typedef Signal<AudioDevicePtr, DefSound::EDeviceState, DefSound::EDeviceState> DeviceStateSignal;
    typedef Signal<AudioDevicePtr, ERole> DeviceDefaultSignal;

    _COM_SMARTPTR_TYPEDEF(IMMDeviceEnumerator, __uuidof(IMMDeviceEnumerator));
    _COM_SMARTPTR_TYPEDEF(IMMNotificationClient, __uuidof(IMMNotificationClient));

    struct AudioEndPointLibraryImpl
    {
        IMMDeviceEnumeratorPtr m_DeviceEnumerator;
        IMMNotificationClientPtr m_notif_client;     
    };

    struct AUDIOENDPOINTLIBRARY_API AudioEndPointLibraryDevicesImpl
    {
        AudioDeviceList m_playback;
        AudioDeviceList m_recording;
        bool m_need_update = true;
        std::mutex m_lists_mutex;
    };

    struct AudioEndPointLibrarySignals
    {
        DeviceSignal DeviceRemoved;
        DeviceSignal DeviceAdded;
        DeviceStateSignal DeviceStateChanged;
        DeviceDefaultSignal DeviceDefaultChanged;
    };
}