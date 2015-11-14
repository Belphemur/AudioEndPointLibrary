// AudioEndPointLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AudioEndPointLibrary.h"
#include "MMNotificationClient.h"
#include <DefSoundEndpointColl.h>
#include <algorithm>

namespace AudioEndPoint {

    // This is the constructor of a class that has been exported.
    // see AudioEndPointLibrary.h for the class definition
    CAudioEndPointLibrary::CAudioEndPointLibrary()
    {
        this->RegisterNotificationClient();
    }

    CAudioEndPointLibrary::~CAudioEndPointLibrary()
    {
        this->UnRegisterNotificationClient();
    }

    HRESULT CAudioEndPointLibrary::OnDeviceStateChanged(LPCWSTR pwstr_device_id, DWORD dw_new_state)
    {
        auto m_playback = this->GetPlaybackDevices(DefSound::EDeviceState::All);
        auto m_recording = this->GetRecordingDevices(DefSound::EDeviceState::All);
        auto audio_device = find_if(m_playback.begin(), m_playback.end(),[pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        auto e_device_state = static_cast<DefSound::EDeviceState>(dw_new_state);
        if(audio_device != m_playback.end())
        {
            auto prevState = (*audio_device)->GetEndPoint().m_State.state;
            (*audio_device)->GetEndPoint().m_State.state = e_device_state;
            Signals.DeviceStateChanged.Notify((*audio_device), prevState, e_device_state);
        }

        audio_device = find_if(m_recording.begin(), m_recording.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_recording.end())
        {
            auto prevState = (*audio_device)->GetEndPoint().m_State.state;
            (*audio_device)->GetEndPoint().m_State.state = e_device_state;
            Signals.DeviceStateChanged.Notify((*audio_device), prevState, e_device_state);
        }
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDeviceRemoved(LPCWSTR pwstr_device_id)
    {
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstr_default_device_id)
    {
        AudioDeviceList list;
        if(flow == ::eRender)
        {
            list = this->GetPlaybackDevices(DefSound::EDeviceState::Active);
        } 
        else if(flow == ::eCapture)
        {
            list = this->GetRecordingDevices(DefSound::EDeviceState::Active);
        } else
        {
            return S_FALSE;
        }
        auto audio_device = find_if(list.begin(), list.end(), [pwstr_default_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_default_device_id) == 0;
        });

        if (audio_device != list.end())
        {
            Signals.DeviceDefaultChanged.Notify((*audio_device), role);
        }
        
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDeviceAdded(LPCWSTR pwstr_device_id)
    {
        auto m_playback = this->GetPlaybackDevices(DefSound::EDeviceState::All);
        auto m_recording = this->GetRecordingDevices(DefSound::EDeviceState::All);
        auto audio_device = find_if(m_playback.begin(), m_playback.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_playback.end())
        {
            Signals.DeviceAdded.Notify(*audio_device);
        }

        audio_device = find_if(m_recording.begin(), m_recording.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_recording.end())
        {
            Signals.DeviceAdded.Notify(*audio_device);
        }

        return S_OK;
    }

    CAudioEndPointLibrary& CAudioEndPointLibrary::GetInstance()
    {
        static CAudioEndPointLibrary instance;
        return instance;
    }

    AudioDeviceList CAudioEndPointLibrary::GetPlaybackDevices(DefSound::EDeviceState state) const
    {
        AudioDeviceList list;
        auto collection = DefSound::CEndpointCollection(state, ::eRender);
        for (auto &endpoint : collection.Get())
        {
            list.push_back(std::make_unique<AudioDevice>(endpoint, Playback));
        }

        return list;
    }

    AudioDeviceList CAudioEndPointLibrary::GetRecordingDevices(DefSound::EDeviceState state) const
    {
        AudioDeviceList list;
        auto collection = DefSound::CEndpointCollection(state, ::eCapture);
        for (auto &endpoint : collection.Get())
        {
            list.push_back(std::make_unique<AudioDevice>(endpoint, Recording));
        }

        return list;
    }

    HRESULT CAudioEndPointLibrary::RegisterNotificationClient()
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if(!m_container.m_DeviceEnumerator)
        {          
            hr = m_container.m_DeviceEnumerator.CreateInstance(__uuidof(MMDeviceEnumerator));
            ReturnIfFailed(hr);
        }

        if (!m_container.m_notif_client) {
            m_container.m_notif_client = new CMMNotificationClient;            
            hr = m_container.m_DeviceEnumerator->RegisterEndpointNotificationCallback(m_container.m_notif_client);
            return hr;
        }
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::UnRegisterNotificationClient() const
    {
        if (m_container.m_notif_client) {
            m_container.m_notif_client->Release();
            HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            hr = m_container.m_DeviceEnumerator->UnregisterEndpointNotificationCallback(m_container.m_notif_client);
            return hr;
        }
        return S_FALSE;
    }
}
