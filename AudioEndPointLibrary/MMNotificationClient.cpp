#include "stdafx.h"
#include "MMNotificationClient.h"
#include "AudioEndPointLibrary.h"

namespace AudioEndPoint {
    CMMNotificationClient::CMMNotificationClient() : m_refcount(1)
    {
    }

    STDMETHODIMP CMMNotificationClient::QueryInterface(const IID& riid, void** ppvInterface)
    {
        if (IID_IUnknown == riid)
        {
            AddRef();
            *ppvInterface = static_cast<IUnknown*>(this);
        }
        else if (__uuidof(IMMNotificationClient) == riid)
        {
            AddRef();
            *ppvInterface = static_cast<IMMNotificationClient*>(this);
        }
        else
        {
            *ppvInterface = nullptr;
            return E_NOINTERFACE;
        }
        return S_OK;
    }

    STDMETHODIMP_(ULONG) CMMNotificationClient::AddRef()
    {
        return InterlockedIncrement(&m_refcount);
    }

    STDMETHODIMP_(ULONG) CMMNotificationClient::Release()
    {
        ULONG ulRef = InterlockedDecrement(&m_refcount);
        if (0 == ulRef)
        {
            delete this;
        }
        return ulRef;
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