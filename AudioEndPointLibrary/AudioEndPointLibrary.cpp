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
    }

    HRESULT CAudioEndPointLibrary::OnDeviceStateChanged(LPCWSTR pwstr_device_id, DWORD dw_new_state)
    {
        auto audio_device = find_if(m_container.m_playback.begin(), m_container.m_playback.end(),[pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        auto e_device_state = static_cast<DefSound::EDeviceState>(dw_new_state);
        if(audio_device != m_container.m_playback.end())
        {
            auto prevState = (*audio_device)->GetEndPoint().m_State.state;
            (*audio_device)->GetEndPoint().m_State.state = e_device_state;
            Signals.DeviceStateChanged.Notify((*audio_device), prevState, e_device_state);
        }

        audio_device = find_if(m_container.m_recording.begin(), m_container.m_recording.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_container.m_recording.end())
        {
            auto prevState = (*audio_device)->GetEndPoint().m_State.state;
            (*audio_device)->GetEndPoint().m_State.state = e_device_state;
            Signals.DeviceStateChanged.Notify((*audio_device), prevState, e_device_state);
        }
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDeviceRemoved(LPCWSTR pwstr_device_id)
    {
        auto audio_device = find_if(m_container.m_playback.begin(), m_container.m_playback.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_container.m_playback.end())
        {
            Signals.DeviceRemoved.Notify(*audio_device);
            m_container.m_playback.remove(*audio_device);
        }

        audio_device = find_if(m_container.m_recording.begin(), m_container.m_recording.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_container.m_recording.end())
        {
            Signals.DeviceRemoved.Notify(*audio_device);
            m_container.m_playback.remove(*audio_device);
        }
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstr_default_device_id)
    {
        AudioDeviceList* list = nullptr;
        if(flow == ::eRender)
        {
            list = &m_container.m_playback;
        } 
        else if(flow == ::eCapture)
        {
            list = &m_container.m_recording;
        } else
        {
            return S_FALSE;
        }

        for(auto& device : *list)
        {
            if(wcscmp(device->ID, pwstr_default_device_id) == 0)
            {
                device->GetEndPoint().m_IsDefault[role] = true;
                Signals.DeviceDefaultChanged.Notify(device, role);
            } 
            else
            {
                device->GetEndPoint().m_IsDefault[role] = false;
            }
        }
        
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDeviceAdded(LPCWSTR pwstr_device_id)
    {
        Refresh();
        auto audio_device = find_if(m_container.m_playback.begin(), m_container.m_playback.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_container.m_playback.end())
        {
            Signals.DeviceAdded.Notify(*audio_device);
        }

        audio_device = find_if(m_container.m_recording.begin(), m_container.m_recording.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_container.m_recording.end())
        {
            Signals.DeviceAdded.Notify(*audio_device);
        }

        return S_OK;
    }

    CAudioEndPointLibrary::~CAudioEndPointLibrary()
    {
       UnRegisterNotificationClient();
    }

    CAudioEndPointLibrary& CAudioEndPointLibrary::GetInstance()
    {
        static CAudioEndPointLibrary instance;
        return instance;
    }

    AudioDeviceList CAudioEndPointLibrary::GetPlaybackDevices(DefSound::EDeviceState state) const
    {
        if(state == DefSound::All)
        {
            return m_container.m_playback;
        }

        AudioDeviceList list;
        for (auto &endpoint : m_container.m_playback)
        {
            if(endpoint->GetDeviceState() == state)
                list.push_back(endpoint);
        }

        return list;
    }

    AudioDeviceList CAudioEndPointLibrary::GetRecordingDevices(DefSound::EDeviceState state) const
    {
        if (state == DefSound::All)
        {
            return m_container.m_recording;
        }

        AudioDeviceList list;
        for (auto &endpoint : m_container.m_recording)
        {
            if (endpoint->GetDeviceState() == state)
                list.push_back(endpoint);
        }
        return list;
    }

    HRESULT CAudioEndPointLibrary::RegisterNotificationClient()
    {
        if(!m_container.m_DeviceEnumerator)
        {
            ReturnIfFailed(CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_container.m_DeviceEnumerator)));
        }

        if (!m_container.m_notif_client) {
            m_container.m_notif_client = new CMMNotificationClient;            
            return m_container.m_DeviceEnumerator->RegisterEndpointNotificationCallback(m_container.m_notif_client);
        }
        return S_FALSE;
    }

    HRESULT CAudioEndPointLibrary::UnRegisterNotificationClient() const
    {
        if (m_container.m_notif_client) {
            HRESULT hr = m_container.m_DeviceEnumerator->UnregisterEndpointNotificationCallback(m_container.m_notif_client);
            if(SUCCEEDED(hr))
            {
                m_container.m_notif_client->Release();
            }
            return hr;
        }
        return S_FALSE;
    }

    void CAudioEndPointLibrary::Refresh()
    {
        m_container.m_recording.clear();
        m_container.m_playback.clear();

        auto playbackPtr = DefSound::CEndpointCollection(DefSound::CEndpointCollection(DefSound::All, ::eRender));
        for (auto &endpoint : playbackPtr.Get())
        {
            m_container.m_playback.push_back(std::make_shared<AudioDevice>(endpoint, Playback));
        }

        auto recordingPtr = DefSound::CEndpointCollection(DefSound::CEndpointCollection(DefSound::All, ::eCapture));
        for (auto &endpoint : recordingPtr.Get())
        {
            m_container.m_recording.push_back(std::make_shared<AudioDevice>(endpoint, Recording));
        }
    }
}
