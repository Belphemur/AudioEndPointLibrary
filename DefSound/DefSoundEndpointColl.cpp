/* The MIT License (MIT)

Copyright (c) 2015 DefSound

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
// ----------------------------------------------------------------------------
// DefSoundEndpointColl.cpp
// Audio endpoint collection
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundEndpointColl.h"
#include "DefSoundEndpointRole.h"

#include "PolicyConfig.h"
#include <Functiondiscoverykeys_devpkey.h>
#include "DefSoundIsDefaultEndpoint.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

namespace {

// ----------------------------------------------------------------------------

typedef _com_ptr_t< _com_IIID<IMMDeviceEnumerator, &__uuidof(IMMDeviceEnumerator)> > CDeviceEnumeratorPtr;
typedef _com_ptr_t< _com_IIID<IMMDeviceCollection, &__uuidof(IMMDeviceCollection)> > CDeviceCollectionPtr;
typedef _com_ptr_t< _com_IIID<IMMDevice, &__uuidof(IMMDevice)> > CDevicePtr;
typedef _com_ptr_t< _com_IIID<IPropertyStore, &__uuidof(IPropertyStore)> > CPropertyStorePtr;

typedef _com_ptr_t< _com_IIID<IPolicyConfigVista, &__uuidof(IPolicyConfigVista)> > CPolicyConfigPtr;

// ----------------------------------------------------------------------------

class CPropVariant
{
public:
    CPropVariant()
    {
        ::PropVariantInit(&m_Impl);
    }
    ~CPropVariant()
    {
        ::PropVariantClear(&m_Impl);
    }

    PROPVARIANT &Get()
    {
        return m_Impl;
    }
    const PROPVARIANT &Get() const
    {
        return m_Impl;
    }

protected:
    CPropVariant(const CPropVariant &) = delete;
    CPropVariant operator=(const CPropVariant &) = delete;

private:
    PROPVARIANT m_Impl;
};

void EnumerateEndpoints(
    __in const CDeviceEnumeratorPtr &pDeviceEnumerator,
	__in EDeviceState deviceState,
	__in EDataFlow deviceFlow,
    __out CEndpointCollection::CImpl &EndpointCollectionImpl
)
{
    HRESULT Result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    CDeviceCollectionPtr pDeviceCollection;
    Result = pDeviceEnumerator->EnumAudioEndpoints(deviceFlow, deviceState, &pDeviceCollection);
    if (FAILED(Result))
        throw CError( MakeDefaultErrorDescription(L"IMMDeviceEnumerator::EnumAudioEndpoints"), Result );

    UINT nCount = 0;
    Result = pDeviceCollection->GetCount(&nCount);
    if (FAILED(Result))
        throw CError( MakeDefaultErrorDescription(L"IMMDeviceCollection::Count"), Result );

    EndpointCollectionImpl.resize(nCount);
    for (UINT i = 0; i < nCount; ++i)
    {
        CDevicePtr pDevice;
        Result = pDeviceCollection->Item(i, &pDevice);
        if (FAILED(Result))
            throw CError( MakeDefaultErrorDescription(L"IMMDeviceCollection::Item"), Result );

		DWORD dwState;
		Result = pDevice->GetState(&dwState);
		if (FAILED(Result))
			throw CError(MakeDefaultErrorDescription(L"IMMDevice::GetState"), Result);

        WCHAR *wszDeviceId = nullptr;
        Result = pDevice->GetId(&wszDeviceId);
        if (FAILED(Result))
            throw CError( MakeDefaultErrorDescription(L"IMMDevice::GetId"), Result );
        std::unique_ptr< WCHAR, decltype(&::CoTaskMemFree) > DeviceIdHolder(wszDeviceId, &::CoTaskMemFree);

        CPropertyStorePtr pPropertyStore;
        Result = pDevice->OpenPropertyStore(STGM_READ, &pPropertyStore);
        if (FAILED(Result))
            throw CError( MakeDefaultErrorDescription(L"IMMDevice::OpenPropertyStore"), Result );

        CPropVariant DeviceDesc;
        try
        {
            Result = pPropertyStore->GetValue(PKEY_Device_DeviceDesc, &DeviceDesc.Get());
            if (FAILED(Result))
                throw CError( MakeDefaultErrorDescription(L"IPropertyStore::GetValue", L"(PKEY_Device_DeviceDesc, ...)"), Result );
            if (DeviceDesc.Get().vt != VT_LPWSTR)
                throw CError( L"Unexpected type of value `PKEY_Device_DeviceDesc'", ERROR_SUCCESS );
        }
        catch (CError)
        {
            DeviceDesc.Get().pwszVal = L"";
        }

		CPropVariant FriendlyName;
	    try
	    {
			Result = pPropertyStore->GetValue(PKEY_Device_FriendlyName, &FriendlyName.Get());
			if (FAILED(Result))
				throw CError(MakeDefaultErrorDescription(L"IPropertyStore::GetValue", L"(PKEY_Device_FriendlyName, ...)"), Result);
			if (FriendlyName.Get().vt != VT_LPWSTR)
				throw CError(L"Unexpected type of value `PKEY_Device_FriendlyName'", ERROR_SUCCESS);
	    }
	    catch (CError)
	    {
			FriendlyName.Get().pwszVal = L"";
	    }	

        CPropVariant DeviceClassIconPath;
        try
        {
            Result = pPropertyStore->GetValue(PKEY_DeviceClass_IconPath, &DeviceClassIconPath.Get());
            if (FAILED(Result))
                throw CError(MakeDefaultErrorDescription(L"IPropertyStore::GetValue", L"(PKEY_DeviceClass_IconPath, ...)"), Result);
            if (DeviceClassIconPath.Get().vt != VT_LPWSTR)
                throw CError(L"Unexpected type of value `PKEY_DeviceClass_IconPath'", ERROR_SUCCESS);
        }
        catch (CError)
        {
            DeviceClassIconPath.Get().pwszVal = L"%windir%\system32\mmres.dll,-3010";
        }
        


        CEndpoint &Endpoint = EndpointCollectionImpl.at(i);

        Endpoint.m_DeviceId = wszDeviceId;
        Endpoint.m_DeviceDesc = DeviceDesc.Get().pwszVal;
		Endpoint.m_FriendlyName = FriendlyName.Get().pwszVal;
        Endpoint.m_DeviceClassIconPath = DeviceClassIconPath.Get().pwszVal;
		Endpoint.m_State.state = static_cast<EDeviceState>(dwState);

        for (auto &IsDefault : Endpoint.m_IsDefault)
            IsDefault = false;
    }
}

// ----------------------------------------------------------------------------

void MarkDefaultAudioEndpoint(
    __in const CDeviceEnumeratorPtr &pDeviceEnumerator,
    __in ::ERole Role,
	__in EDataFlow deviceFlow,
    __inout CEndpointCollection::CImpl &EndpointCollectionImpl
)
{
    HRESULT Result;

    CDevicePtr pDevice;
    Result = pDeviceEnumerator->GetDefaultAudioEndpoint(deviceFlow, Role, &pDevice);
    if (FAILED(Result))
        throw CError( MakeDefaultErrorDescription(L"IMMDeviceEnumerator::GetDefaultAudioEndpoint"), Result );
    _ASSERT(pDevice);

    WCHAR *wszDeviceId = nullptr;
    Result = pDevice->GetId(&wszDeviceId);
    if (FAILED(Result))
        throw CError( MakeDefaultErrorDescription(L"IMMDevice::GetId"), Result );
    std::unique_ptr< WCHAR, decltype(&::CoTaskMemFree) > DeviceIdHolder(wszDeviceId, &::CoTaskMemFree);

    class CPred
    {
    public:
        explicit CPred(__in WCHAR *wszDeviceId)
            : m_wszDeviceId(wszDeviceId)
        {
            _ASSERT(m_wszDeviceId);
        }
        bool operator()(const CEndpoint &Endpoint) const
        {
            return !Endpoint.m_DeviceId.compare(m_wszDeviceId);
        }
    private:
        WCHAR *m_wszDeviceId;
    };
    auto it = 
        std::find_if(
            EndpointCollectionImpl.begin(),
            EndpointCollectionImpl.end(),
            CPred(wszDeviceId));
    _ASSERT(it != EndpointCollectionImpl.end());
    if (it != EndpointCollectionImpl.end())
        it->m_IsDefault[Role] = true;
}

// ----------------------------------------------------------------------------

void SetDefaultEndpointOneRole(__in PCWSTR wszDeviceId, __in ::ERole Role)
{
    CPolicyConfigPtr pPolicyConfig;
    HRESULT Result = pPolicyConfig.CreateInstance(__uuidof(CPolicyConfigVistaClient));
    if (FAILED(Result))
        throw CError( L"Create instance of CPolicyConfig failed", Result );

    Result = pPolicyConfig->SetDefaultEndpoint(wszDeviceId, Role);
    if (FAILED(Result))
        throw CError( MakeDefaultErrorDescription(L"IPolicyConfig::SetDefaultEndpoint"), Result );
}

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

CEndpointCollection::CEndpointCollection(EDeviceState device_state, EDataFlow device_flow)
{
	m_device_state = device_state;
	m_device_type = device_flow;
	Refresh();
}

	// ----------------------------------------------------------------------------

void CEndpointCollection::Refresh()
{
    CDeviceEnumeratorPtr pDeviceEnumerator;
	HRESULT Result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	Result = pDeviceEnumerator.CreateInstance(__uuidof(MMDeviceEnumerator));
    if (FAILED(Result))
        throw CError( L"Create instance of MMDeviceEnumerator failed", Result );

    CEndpointCollection::CImpl Impl;
    EnumerateEndpoints(pDeviceEnumerator, m_device_state, m_device_type, Impl);

    for (const auto &EndpointRole : GetEndpointRoleArray()) {
        try
        {
            MarkDefaultAudioEndpoint(pDeviceEnumerator, EndpointRole.m_RoleValue, m_device_type, Impl);
        }
        catch (...)
        {
            printf("Can't get default device role %ls", EndpointRole.m_wszFullName);
        }       
    }

    m_pImpl.reset( new CEndpointCollection::CImpl(std::move(Impl)) );
}

// ----------------------------------------------------------------------------

const CEndpointCollection::CImpl &CEndpointCollection::Get() const
{
    return *m_pImpl.get();
}

// ----------------------------------------------------------------------------

bool CEndpointCollection::IsDefault(__in size_t nIndex, __in ERole Role) const
{
    return CIsDefaultEndpoint(Role)(m_pImpl->at(nIndex));
}

// ----------------------------------------------------------------------------

void CEndpointCollection::SetDefault(__in size_t nIndex, ERole Role) const
{
    SetDefaultEndpoint(m_pImpl->at(nIndex), Role);
}

void CEndpointCollection::SetDefaultEndpoint(const CEndpoint& Endpoint, ::ERole Role)
{
	if(Endpoint.m_State.state != Active)
		throw CNotActiveError(L"Device is not in active state", Endpoint.m_State.state);
	
	if (GetEndpointAllRoles().m_RoleValue != Role)
	{
		SetDefaultEndpointOneRole(Endpoint.m_DeviceId.c_str(), Role);
		return;
	}
	for (const auto &itRole : GetEndpointRoleArray())
		SetDefaultEndpointOneRole(Endpoint.m_DeviceId.c_str(), itRole.m_RoleValue);
}

    // ----------------------------------------------------------------------------

INT_PTR CEndpointCollection::SetDefaultNext(__in ERole Role) const
{
    if (m_pImpl->size() < 2)
        return -1;

    auto it = std::find_if(m_pImpl->begin(), m_pImpl->end(), CIsDefaultEndpoint(Role));
    if (it == m_pImpl->end())
        return -1;

    ++it;
    if (it == m_pImpl->end())
    {
        SetDefaultEndpoint(*m_pImpl->begin(), Role);
        return 0;
    }

    SetDefaultEndpoint(*it, Role);
    return std::distance(m_pImpl->begin(), it);
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
