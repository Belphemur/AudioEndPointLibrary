// AudioEndPointLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AudioEndPointLibrary.h"
#include "DefSoundEndpointColl.h"
#include "MMNotificationClient.h"
#include <algorithm>

namespace AudioEndPoint {
    // This is the constructor of a class that has been exported.
    // see AudioEndPointLibrary.h for the class definition
    CAudioEndPointLibrary::CAudioEndPointLibrary()
    {
        Refresh();

        auto pNotifclient = new(std::nothrow) AudioEndPoint::CMMNotificationClient;
        pNotifclient->AddRef();
        pNotifclient->QueryInterface(IID_PPV_ARGS(&m_container.m_notif_client));
        pNotifclient->Release();
    }

    void CAudioEndPointLibrary::notify(const Event& event, AudioDevicePtr device) const
    {
        for (const auto& obs : m_container.m_observers.at(event)) obs(device);
    }

    HRESULT CAudioEndPointLibrary::OnDeviceStateChanged(LPCWSTR pwstr_device_id, DWORD dw_new_state)
    {
        auto foundPlayback = find_if(m_container.m_playback.begin(), m_container.m_playback.end(),[pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if(foundPlayback != m_container.m_playback.end())
        {
            (*foundPlayback)->GetEndPoint().m_State.state = static_cast<DefSound::EDeviceState>(dw_new_state);
            notify(STATE_CHANGED, (*foundPlayback));
        }

        auto foundRecording = find_if(m_container.m_recording.begin(), m_container.m_recording.end(), [pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        if (foundRecording != m_container.m_recording.end())
        {
            (*foundRecording)->GetEndPoint().m_State.state = static_cast<DefSound::EDeviceState>(dw_new_state);
            notify(STATE_CHANGED, (*foundRecording));
        }
        return S_OK;
    }

    HRESULT CAudioEndPointLibrary::OnDeviceRemoved(LPCWSTR pwstr_device_id)
    {
        m_container.m_playback.remove_if([pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });

        m_container.m_recording.remove_if([pwstr_device_id](AudioDevicePtr device) {
            return wcscmp(device->ID, pwstr_device_id) == 0;
        });
        notify(REMOVED, nullptr);
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
        notify(ADDED, nullptr);
        return S_OK;
    }

    CAudioEndPointLibrary::~CAudioEndPointLibrary()
    {
        m_container.m_observers.clear();
        m_container.m_recording.clear();
        m_container.m_playback.clear();
        m_container.m_notif_client.Release();
    }

    CAudioEndPointLibrary& CAudioEndPointLibrary::GetInstance()
    {
        static CAudioEndPointLibrary instance;
        return instance;
    }

    AudioDeviceList CAudioEndPointLibrary::GetPlaybackDevices(DefSound::EDeviceState state) const
    {
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
        AudioDeviceList list;
        for (auto &endpoint : m_container.m_recording)
        {
            if (endpoint->GetDeviceState() == state)
                list.push_back(endpoint);
        }
        return list;
    }

    IMMNotificationClientPtr& CAudioEndPointLibrary::GetNotifClient()
    {
        return m_container.m_notif_client;
    }

    void CAudioEndPointLibrary::Refresh()
    {
        m_container.m_recording.clear();
        m_container.m_playback.clear();

        auto playbackPtr = std::make_unique<DefSound::CEndpointCollection>(DefSound::CEndpointCollection(DefSound::All, ::eRender));
        for (auto &endpoint : playbackPtr->Get())
        {
            m_container.m_playback.push_back(std::make_shared<AudioDevice>(endpoint, Playback));
        }

        auto recordingPtr = std::make_unique<DefSound::CEndpointCollection>(DefSound::CEndpointCollection(DefSound::All, ::eCapture));
        for (auto &endpoint : recordingPtr->Get())
        {
            m_container.m_recording.push_back(std::make_shared<AudioDevice>(endpoint, Recording));
        }
    }
}