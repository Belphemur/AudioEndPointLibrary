#include "stdafx.h"
#include "EventWrapper.h"
#include "AudioEndPointLibrary.h"
#include "Audio.EndPoint.Controller.Wrapper.h"
#include "Events.h"
#include "AudioDeviceWrapper.h"

using namespace AudioEndPoint;

namespace AudioEndPointControllerWrapper {
    EventWrapper::EventWrapper()
    {
        try
        {
            m_added = CAudioEndPointLibrary::GetInstance().Signals->DeviceAdded.Register([](AudioDevicePtr device)
            {
                AudioController::RaiseAdded(gcnew DeviceAddedEvent(gcnew AudioDeviceWrapper(device)));
            });

            m_removed = CAudioEndPointLibrary::GetInstance().Signals->DeviceRemoved.Register([](AudioDevicePtr device)
            {
                AudioController::RaiseRemoved(gcnew DeviceRemovedEvent(gcnew AudioDeviceWrapper(device)));
            });

            m_default = CAudioEndPointLibrary::GetInstance().Signals->DeviceDefaultChanged.Register([](AudioDevicePtr device, ERole role)
            {
                AudioController::RaiseDefault(gcnew DeviceDefaultChangedEvent(gcnew AudioDeviceWrapper(device), static_cast<Role>(role)));
            });

            m_state = CAudioEndPointLibrary::GetInstance().Signals->DeviceStateChanged.Register([](AudioDevicePtr device, DefSound::EDeviceState cur)
            {
               AudioController::RaiseState(gcnew DeviceStateChangedEvent(gcnew AudioDeviceWrapper(device), static_cast<DeviceState>(cur)));
            });
            m_loaded = true;
        } catch(...)
        {
            m_loaded = false;
        }
    }
    EventWrapper::~EventWrapper()
    {
        m_added.m_signal->Unregister(m_added);
        m_removed.m_signal->Unregister(m_removed);
        m_default.m_signal->Unregister(m_default);
        m_state.m_signal->Unregister(m_state);
      
    }
}
