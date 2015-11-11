#pragma once
#include "mmdeviceapi.h"
namespace AudioEndPoint {
    class CMMNotificationClient :
        public IMMNotificationClient
    {
    public:
        STDMETHODIMP QueryInterface(const IID& riid, void** ppvObject) override;
        STDMETHODIMP_(ULONG) AddRef() override;
        STDMETHODIMP_(ULONG) Release() override;

        virtual ~CMMNotificationClient()
        {
        }

        STDMETHODIMP OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) override;
        STDMETHODIMP OnDeviceAdded(LPCWSTR pwstrDeviceId) override;
        STDMETHODIMP OnDeviceRemoved(LPCWSTR pwstrDeviceId) override;
        STDMETHODIMP OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId) override;
        STDMETHODIMP OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) override;
        CMMNotificationClient();
    private:
        ULONG m_refcount;
    };
}

