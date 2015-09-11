#pragma once
#include <list>
#include <memory>
#include <comdef.h>
#include "AudioDevice.h"

namespace AudioEndPoint {
    typedef std::shared_ptr<AudioDevice> AudioDevicePtr;
    typedef std::list<AudioDevicePtr> AudioDeviceList;
    _COM_SMARTPTR_TYPEDEF(IMMNotificationClient, __uuidof(IMMNotificationClient));
    struct AudioDevicesContainer
    {
        AudioDeviceList m_playback;
        AudioDeviceList m_recording;
        IMMNotificationClientPtr m_notif_client;
    };
}