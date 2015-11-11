#include "stdafx.h"
#include "AudioDeviceWrapper.h"
#include "Audio.EndPoint.Controller.Wrapper.h"
#include "DefSoundException.h"

using namespace System;

namespace AudioEndPointControllerWrapper {
    bool AudioDeviceWrapper::Equals(Object ^ o)
    {
        if (o == (Object^)nullptr)
            return false;

        if (o->GetType() != this->GetType())
        {
            return false;
        }

        AudioDeviceWrapper^ that = static_cast<AudioDeviceWrapper^>(o);
        return that->Id == Id;
    }

    int AudioDeviceWrapper::GetHashCode()
    {
        return this->Id->GetHashCode();
    }

    void AudioDeviceWrapper::SetAsDefault(Role role)
    {
        try
        {
            _audioDevice->SetDefault(static_cast<::ERole>(role));
            if (!AudioController::IsNotificationAvailable())
            {
                AudioController::RaiseDefault(gcnew DeviceDefaultChangedEvent(this, role));
            }
        }
        catch (DefSound::CNotActiveError error)
        {
            throw gcnew DefSoundNotActiveException(error);
        }
        catch (DefSound::CError error)
        {
            throw gcnew DefSoundException(error);
        }
    }

    bool AudioDeviceWrapper::IsDefault(Role role)
    {
        try
        {
            return _audioDevice->IsDefault(static_cast<::ERole>(role));
        }
        catch (DefSound::CNotActiveError error)
        {
            throw gcnew DefSoundNotActiveException(error);
        }
        catch (DefSound::CError error)
        {
            throw gcnew DefSoundException(error);
        }
    }
}