// AudioEndPointLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AudioEndPointLibrary.h"
#include "MMNotificationClient.h"
#include <DefSoundEndpointColl.h>
#include <algorithm>
#include <mutex>

namespace AudioEndPoint {

    struct CAudioEndPointLibrary::AudioEndPointLibraryImpl
    {
        IMMDeviceEnumeratorPtr m_DeviceEnumerator;
        IMMNotificationClientPtr m_notif_client;
    };

    struct CAudioEndPointLibrary::AudioEndPointLibraryDevicesImpl
    {
        AudioDeviceList m_playback;
        AudioDeviceList m_recording;
        bool m_need_update = true;
        std::mutex m_lists_mutex;
    };


    // This is the constructor of a class that has been exported.
    // see AudioEndPointLibrary.h for the class definition
    CAudioEndPointLibrary::CAudioEndPointLibrary() : m_container(new AudioEndPointLibraryImpl()), 
        m_signals(new AudioEndPointLibrarySignals()), 
        m_devices_lists(new AudioEndPointLibraryDevicesImpl())
    {
        m_devices_lists->m_need_update = true;
        this->RegisterNotificationClient();
    }


    CAudioEndPointLibrary::~CAudioEndPointLibrary()
    {
        delete m_signals;
        delete m_devices_lists;
        this->UnRegisterNotificationClient();
        delete m_container;
    }

    HRESULT CAudioEndPointLibrary::OnDeviceStateChanged(LPCWSTR pwstr_device_id, DWORD dw_new_state) const
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
            Signals->DeviceStateChanged.Notify((*audio_device), prevState, e_device_state);
        }

        audio_device = find_if(m_recording.begin(), m_recording.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_recording.end())
        {
            auto prevState = (*audio_device)->GetEndPoint().m_State.state;
            (*audio_device)->GetEndPoint().m_State.state = e_device_state;
            Signals->DeviceStateChanged.Notify((*audio_device), prevState, e_device_state);
        }
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDeviceRemoved(LPCWSTR pwstr_device_id) const
    {
        m_devices_lists->m_need_update = true;
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstr_default_device_id) const
    {
        AudioDeviceList list;
        if(flow == ::eRender)
        {
            list = this->GetPlaybackDevices(DefSound::EDeviceState::All);
        } 
        else if(flow == ::eCapture)
        {
            list = this->GetRecordingDevices(DefSound::EDeviceState::All);
        } else
        {
            return S_FALSE;
        }

        AudioDevicePtr deviceFound = nullptr;
        for (auto& device : list)
        {
            if (wcscmp(device->ID, pwstr_default_device_id) == 0)
            {
                device->GetEndPoint().m_IsDefault[role] = true;
                deviceFound = device;
            }
            else
            {
                device->GetEndPoint().m_IsDefault[role] = false;
            }
        }
        Signals->DeviceDefaultChanged.Notify(deviceFound, role);
        
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDeviceAdded(LPCWSTR pwstr_device_id) const
    {
        m_devices_lists->m_need_update = true;
        auto m_playback = this->GetPlaybackDevices(DefSound::EDeviceState::All);
        auto m_recording = this->GetRecordingDevices(DefSound::EDeviceState::All);
        auto audio_device = find_if(m_playback.begin(), m_playback.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_playback.end())
        {
            Signals->DeviceAdded.Notify(*audio_device);
        }

        audio_device = find_if(m_recording.begin(), m_recording.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (audio_device != m_recording.end())
        {
            Signals->DeviceAdded.Notify(*audio_device);
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
        if(m_devices_lists->m_need_update)
        {
            Refresh();
        }

        if(state == DefSound::EDeviceState::All)
        {
            return m_devices_lists->m_playback;
        }
        std::lock_guard<std::mutex> lock(m_devices_lists->m_lists_mutex);
        AudioDeviceList list;
        for (auto &endpoint : m_devices_lists->m_playback)
        {
            if (endpoint->GetDeviceState() == state)
                list.push_back(endpoint);
        }

        return list;

     
    }

    AudioDeviceList CAudioEndPointLibrary::GetRecordingDevices(DefSound::EDeviceState state) const
    {
        if (m_devices_lists->m_need_update)
        {
            Refresh();
        }

        if (state == DefSound::EDeviceState::All)
        {
            return m_devices_lists->m_recording;
        }
        std::lock_guard<std::mutex> lock(m_devices_lists->m_lists_mutex);
        AudioDeviceList list;
        for (auto &endpoint : m_devices_lists->m_recording)
        {
            if (endpoint->GetDeviceState() == state)
                list.push_back(endpoint);
        }

        return list;
    }

    HRESULT CAudioEndPointLibrary::RegisterNotificationClient() const
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if(!m_container->m_DeviceEnumerator)
        {          
            hr = m_container->m_DeviceEnumerator.CreateInstance(__uuidof(MMDeviceEnumerator));
            ReturnIfFailed(hr);
        }

        if (!m_container->m_notif_client) {
            m_container->m_notif_client = new CMMNotificationClient;            
            hr = m_container->m_DeviceEnumerator->RegisterEndpointNotificationCallback(m_container->m_notif_client);
            return hr;
        }
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::UnRegisterNotificationClient() const
    {
        if (m_container->m_notif_client) {
            m_container->m_notif_client->Release();
            HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
            hr = m_container->m_DeviceEnumerator->UnregisterEndpointNotificationCallback(m_container->m_notif_client);
            return hr;
        }
        return S_FALSE;
    }

    void CAudioEndPointLibrary::Refresh() const
    {   
        std::lock_guard<std::mutex> lock(m_devices_lists->m_lists_mutex);

        m_devices_lists->m_playback.clear();
        auto collectionPlayback = DefSound::CEndpointCollection(DefSound::All, ::eRender);
        for (auto &endpoint : collectionPlayback.Get())
        {
            m_devices_lists->m_playback.push_back(std::make_shared<AudioDevice>(endpoint, Playback));
        }

        m_devices_lists->m_recording.clear();
        auto collectionRecording = DefSound::CEndpointCollection(DefSound::All, ::eCapture);
        for (auto &endpoint : collectionRecording.Get())
        {
            m_devices_lists->m_recording.push_back(std::make_shared<AudioDevice>(endpoint, Recording));
        }
        m_devices_lists->m_need_update = false;
    }
}
