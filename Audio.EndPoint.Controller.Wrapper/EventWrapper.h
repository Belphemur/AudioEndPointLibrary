#pragma once
#include "AudioEndPointLibraryImpl.h"

using namespace AudioEndPoint;

namespace AudioEndPointControllerWrapper {
    class EventWrapper
    {
    public:
        EventWrapper();
        ~EventWrapper();
        bool IsLoaded() const
        {
            return m_loaded;
        }
    private:
        DeviceSignal::FunctionInfo m_added;
        DeviceSignal::FunctionInfo m_removed;
        DeviceStateSignal::FunctionInfo m_state;
        DeviceDefaultSignal::FunctionInfo m_default;
        bool m_loaded = false;
    };
}

