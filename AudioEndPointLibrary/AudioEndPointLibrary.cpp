// AudioEndPointLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "AudioEndPointLibrary.h"
#include "DefSoundEndpointColl.h"

namespace AudioEndPoint {
    // This is the constructor of a class that has been exported.
    // see AudioEndPointLibrary.h for the class definition
    CAudioEndPointLibrary::CAudioEndPointLibrary()
    {
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


    CAudioEndPointLibrary::~CAudioEndPointLibrary()
    {
        m_container.m_recording.clear();
        m_container.m_playback.clear();
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
}