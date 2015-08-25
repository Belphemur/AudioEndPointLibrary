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

// ----------------------------------------------------------------------------

class CIsDefaultEndpoint
{
public:
    explicit CIsDefaultEndpoint(__in ::ERole Role)
        : m_Role(Role)
    {
    }
    bool operator()(const CEndpoint &Endpoint) const
    {
        if (GetEndpointAllRoles().m_RoleValue != m_Role)
            return Endpoint.m_IsDefault[m_Role];

        for (const auto &itRole : GetEndpointRoleArray())
        {
            if (!Endpoint.m_IsDefault[itRole.m_RoleValue])
                return false;
        }
        return true;
    }
private:
    ::ERole m_Role;
};

// ----------------------------------------------------------------------------

void EnumerateEndpoints(
    __in const CDeviceEnumeratorPtr &pDeviceEnumerator,
    __out CEndpointCollection::CImpl &EndpointCollectionImpl
)
{
    HRESULT Result;

    CDeviceCollectionPtr pDeviceCollection;
    Result = pDeviceEnumerator->EnumAudioEndpoints(::eRender, DEVICE_STATE_ACTIVE, &pDeviceCollection);
    if (S_OK != Result)
        throw CError( MakeDefaultErrorDescription(L"IMMDeviceEnumerator::EnumAudioEndpoints"), Result );

    UINT nCount = 0;
    Result = pDeviceCollection->GetCount(&nCount);
    if (S_OK != Result)
        throw CError( MakeDefaultErrorDescription(L"IMMDeviceCollection::Count"), Result );

    EndpointCollectionImpl.resize(nCount);
    for (UINT i = 0; i < nCount; ++i)
    {
        CDevicePtr pDevice;
        Result = pDeviceCollection->Item(i, &pDevice);
        if (S_OK != Result)
            throw CError( MakeDefaultErrorDescription(L"IMMDeviceCollection::Item"), Result );

        WCHAR *wszDeviceId = nullptr;
        Result = pDevice->GetId(&wszDeviceId);
        if (S_OK != Result)
            throw CError( MakeDefaultErrorDescription(L"IMMDevice::GetId"), Result );
        std::unique_ptr< WCHAR, decltype(&::CoTaskMemFree) > DeviceIdHolder(wszDeviceId, &::CoTaskMemFree);

        CPropertyStorePtr pPropertyStore;
        Result = pDevice->OpenPropertyStore(STGM_READ, &pPropertyStore);
        if (S_OK != Result)
            throw CError( MakeDefaultErrorDescription(L"IMMDevice::OpenPropertyStore"), Result );

        CPropVariant DeviceDesc;
        Result = pPropertyStore->GetValue(PKEY_Device_DeviceDesc, &DeviceDesc.Get());
        if (S_OK != Result)
            throw CError( MakeDefaultErrorDescription(L"IPropertyStore::GetValue", L"(PKEY_Device_DeviceDesc, ...)"), Result );
        if (DeviceDesc.Get().vt != VT_LPWSTR)
            throw CError( L"Unexpected type of value `PKEY_Device_DeviceDesc'", ERROR_SUCCESS );

        CPropVariant DeviceClassIconPath;
        Result = pPropertyStore->GetValue(PKEY_DeviceClass_IconPath, &DeviceClassIconPath.Get());
        if (S_OK != Result)
            throw CError( MakeDefaultErrorDescription(L"IPropertyStore::GetValue", L"(PKEY_DeviceClass_IconPath, ...)"), Result );
        if (DeviceClassIconPath.Get().vt != VT_LPWSTR)
            throw CError( L"Unexpected type of value `PKEY_DeviceClass_IconPath'", ERROR_SUCCESS );

        CEndpoint &Endpoint = EndpointCollectionImpl.at(i);

        Endpoint.m_DeviceId = wszDeviceId;
        Endpoint.m_DeviceDesc = DeviceDesc.Get().pwszVal;
        Endpoint.m_DeviceClassIconPath = DeviceClassIconPath.Get().pwszVal;

        for (auto &IsDefault : Endpoint.m_IsDefault)
            IsDefault = false;
    }
}

// ----------------------------------------------------------------------------

void MarkDefaultAudioEndpoint(
    __in const CDeviceEnumeratorPtr &pDeviceEnumerator,
    __in ::ERole Role,
    __inout CEndpointCollection::CImpl &EndpointCollectionImpl
)
{
    HRESULT Result;

    CDevicePtr pDevice;
    Result = pDeviceEnumerator->GetDefaultAudioEndpoint(::eRender, Role, &pDevice);
    if (S_OK != Result)
        throw CError( MakeDefaultErrorDescription(L"IMMDeviceEnumerator::GetDefaultAudioEndpoint"), Result );
    _ASSERT(pDevice);

    WCHAR *wszDeviceId = nullptr;
    Result = pDevice->GetId(&wszDeviceId);
    if (S_OK != Result)
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
    if (S_OK != Result)
        throw CError( L"Create instance of CPolicyConfig failed", Result );

    Result = pPolicyConfig->SetDefaultEndpoint(wszDeviceId, Role);
    if (S_OK != Result)
        throw CError( MakeDefaultErrorDescription(L"IPolicyConfig::SetDefaultEndpoint"), Result );
}

// ----------------------------------------------------------------------------

void SetDefaultEndpoint(__in const CEndpoint &Endpoint, __in ::ERole Role)
{
    if (GetEndpointAllRoles().m_RoleValue != Role)
    {
        SetDefaultEndpointOneRole(Endpoint.m_DeviceId.c_str(), Role);
        return;
    }
    for (const auto &itRole : GetEndpointRoleArray())
        SetDefaultEndpointOneRole(Endpoint.m_DeviceId.c_str(), itRole.m_RoleValue);
}

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

CEndpointCollection::CEndpointCollection()
{
    Refresh();
}

// ----------------------------------------------------------------------------

void CEndpointCollection::Refresh()
{
    CDeviceEnumeratorPtr pDeviceEnumerator;
    HRESULT Result = pDeviceEnumerator.CreateInstance(__uuidof(MMDeviceEnumerator));
    if (S_OK != Result)
        throw CError( L"Create instance of MMDeviceEnumerator failed", Result );

    CEndpointCollection::CImpl Impl;
    EnumerateEndpoints(pDeviceEnumerator, Impl);

    for (const auto &EndpointRole : GetEndpointRoleArray())
        MarkDefaultAudioEndpoint(pDeviceEnumerator, EndpointRole.m_RoleValue, Impl);

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
