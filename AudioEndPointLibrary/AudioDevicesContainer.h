#pragma once
#include <list>
#include <memory>
#include "AudioDevice.h"

namespace AudioEndPoint {
    typedef std::shared_ptr<AudioDevice> AudioDevicePtr;
    typedef std::list<AudioDevicePtr> AudioDeviceList;
    struct AudioDevicesContainer
    {
        AudioDeviceList m_playback;
        AudioDeviceList m_recording;
    };
}