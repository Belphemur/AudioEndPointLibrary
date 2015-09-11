#pragma once
#include <list>
#include <memory>
#include <comdef.h>
#include "AudioDevice.h"
#include <functional>
#include <map>

namespace AudioEndPoint {
    typedef std::shared_ptr<AudioDevice> AudioDevicePtr;
    typedef std::list<AudioDevicePtr> AudioDeviceList;
    _COM_SMARTPTR_TYPEDEF(IMMNotificationClient, __uuidof(IMMNotificationClient));

    enum Event {ADDED, REMOVED, STATE_CHANGED};
    struct AudioEndPointLibraryImpl
    {
        AudioDeviceList m_playback;
        AudioDeviceList m_recording;
        IMMNotificationClientPtr m_notif_client;
        std::map<Event, std::vector<std::function<void(AudioDevice*)>>> m_observers;
    };
}