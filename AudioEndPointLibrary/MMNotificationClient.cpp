#include "stdafx.h"
#include "MMNotificationClient.h"
#include "AudioEndPointLibrary.h"

namespace AudioEndPoint {
    CMMNotificationClient::CMMNotificationClient() : m_cRef(0)
    {
    }


    CMMNotificationClient::~CMMNotificationClient()
    {
    }

    HRESULT CMMNotificationClient::QueryInterface(const IID& riid, void** ppvObject)
    {
        if (IID_IUnknown == riid)
        {
            AddRef();
            *ppvObject = static_cast<IUnknown*>(this);
        }
        else if (__uuidof(IMMNotificationClient) == riid)
        {
            AddRef();
            *ppvObject = static_cast<IMMNotificationClient*>(this);
        }
        else
        {
            *ppvObject = nullptr;
            return E_NOINTERFACE;
        }
        return S_OK;
    }

    ULONG CMMNotificationClient::AddRef()
    {
        return InterlockedIncrement(&m_cRef);
    }

    ULONG CMMNotificationClient::Release()
    {
        ULONG ulRef = InterlockedDecrement(&m_cRef);
        if (0 == ulRef)
        {
            delete this;
        }
        return ulRef;
    }

    HRESULT CMMNotificationClient::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
    {
        return CAudioEndPointLibrary::GetInstance().OnDeviceStateChanged(pwstrDeviceId, dwNewState);
    }

    HRESULT CMMNotificationClient::OnDeviceAdded(LPCWSTR pwstrDeviceId)
    {
        return CAudioEndPointLibrary::GetInstance().OnDeviceAdded(pwstrDeviceId);
    }

    HRESULT CMMNotificationClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
    {
        return CAudioEndPointLibrary::GetInstance().OnDeviceRemoved(pwstrDeviceId);
    }

    HRESULT CMMNotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId)
    {
        return CAudioEndPointLibrary::GetInstance().OnDefaultDeviceChanged(flow, role, pwstrDefaultDeviceId);
    }

    HRESULT CMMNotificationClient::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
    {
        return S_OK;
    }
};