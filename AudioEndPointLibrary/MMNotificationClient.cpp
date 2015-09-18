#include "stdafx.h"
#include "MMNotificationClient.h"
#include "AudioEndPointLibrary.h"

namespace AudioEndPoint {
    CMMNotificationClient::CMMNotificationClient() : CUnknown("AudioEndPoint::MMNotificationClient", nullptr)
    {
    }


    STDMETHODIMP CMMNotificationClient::NonDelegatingQueryInterface(REFIID riid, void** ppv)
    {
        if (riid == __uuidof(IMMNotificationClient))
            return GetInterface(static_cast<IMMNotificationClient*>(this), ppv);

        return CUnknown::NonDelegatingQueryInterface(riid, ppv);
    }


    STDMETHODIMP CMMNotificationClient::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
    {
        return CAudioEndPointLibrary::GetInstance().OnDeviceStateChanged(pwstrDeviceId, dwNewState);
    }

    STDMETHODIMP CMMNotificationClient::OnDeviceAdded(LPCWSTR pwstrDeviceId)
    {
        return CAudioEndPointLibrary::GetInstance().OnDeviceAdded(pwstrDeviceId);
    }

    STDMETHODIMP CMMNotificationClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
    {
        return CAudioEndPointLibrary::GetInstance().OnDeviceRemoved(pwstrDeviceId);
    }

    STDMETHODIMP CMMNotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId)
    {
        return CAudioEndPointLibrary::GetInstance().OnDefaultDeviceChanged(flow, role, pwstrDefaultDeviceId);
    }

    STDMETHODIMP CMMNotificationClient::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
    {
        return S_OK;
    }
};