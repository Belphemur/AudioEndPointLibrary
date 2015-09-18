#pragma once
#include "mmdeviceapi.h"
namespace AudioEndPoint {
    class CMMNotificationClient :
        public CUnknown,
        public IMMNotificationClient
    {
    public:
        DECLARE_IUNKNOWN

        STDMETHODIMP OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) override;
        STDMETHODIMP OnDeviceAdded(LPCWSTR pwstrDeviceId) override;
        STDMETHODIMP OnDeviceRemoved(LPCWSTR pwstrDeviceId) override;
        STDMETHODIMP OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDeviceId) override;
        STDMETHODIMP OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) override;
        STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void** ppv);
        CMMNotificationClient();
    };
}

