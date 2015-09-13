#pragma once
#include "IAudioDevice.h"
#include "DeviceState.h"
using namespace System;

namespace AudioEndPointControllerWrapper {
    public ref class DeviceEvent : public System::EventArgs
    {        
    public:
        DeviceEvent(IAudioDevice^ dev) : device(dev) {}
        IAudioDevice^ device;
    };

    public ref class DeviceAddedEvent: public DeviceEvent
    {
    public:
        DeviceAddedEvent(IAudioDevice^ dev) : DeviceEvent(dev) {}
    };

    public ref class DeviceRemovedEvent : public DeviceEvent
    {
    public:
        DeviceRemovedEvent(IAudioDevice^ dev) : DeviceEvent(dev) {}
    };

    public ref class DeviceStateChangedEvent : public DeviceEvent
    {
    public:
        DeviceStateChangedEvent(IAudioDevice^ dev, DeviceState prev, DeviceState newS) : DeviceEvent(dev), previousState(prev), newState(newS) {}
        DeviceState previousState;
        DeviceState newState;
    };

    public ref class DeviceDefaultChangedEvent : public DeviceEvent
    {
    public:
        DeviceDefaultChangedEvent(IAudioDevice^ dev, Role r) : DeviceEvent(dev), role(r) {}
        Role role;
    };

}

