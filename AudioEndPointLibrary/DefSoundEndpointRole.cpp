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
// DefSoundEndpointRole.cpp
// Audio endpoint roles
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundEndpointRole.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

namespace {

// ----------------------------------------------------------------------------

class CEndpointRoleArrayInitializer : public CEndpointRoleArray
{
public:
    CEndpointRoleArrayInitializer();
};

class CEndpointAllRoleInitializer : public CEndpointRole
{
public:
    CEndpointAllRoleInitializer();
};

// ----------------------------------------------------------------------------

CEndpointRoleArrayInitializer::CEndpointRoleArrayInitializer()
{
    at(CEndpointRole::Console).m_wszShortName = L"CON";
    at(CEndpointRole::Console).m_wszFullName = L"Console";
    at(CEndpointRole::Console).m_wszDescription = L"system notification sounds and voice commands";
    at(CEndpointRole::Console).m_RoleValue = eConsole;

    at(CEndpointRole::Multimedia).m_wszShortName = L"MMEDIA";
    at(CEndpointRole::Multimedia).m_wszFullName = L"Multimedia";
    at(CEndpointRole::Multimedia).m_wszDescription = L"multimedia";
    at(CEndpointRole::Multimedia).m_RoleValue = eMultimedia;

    at(CEndpointRole::Communications).m_wszShortName = L"VOICE";
    at(CEndpointRole::Communications).m_wszFullName = L"Communications";
    at(CEndpointRole::Communications).m_wszDescription = L"voice communications";
    at(CEndpointRole::Communications).m_RoleValue = eCommunications;
}

CEndpointAllRoleInitializer::CEndpointAllRoleInitializer()
{
    m_wszShortName = L"ALL";
    m_wszFullName = L"All Roles";
    m_wszDescription = L"all roles";
    m_RoleValue = ERole_enum_count;
}

// ----------------------------------------------------------------------------

CEndpointRoleArrayInitializer g_EndpointRoleArrayInitializer;
CEndpointAllRoleInitializer g_EndpointAllRoleInitializer;

// ----------------------------------------------------------------------------

}   // namespace <nameless>

// ----------------------------------------------------------------------------

const CEndpointRoleArray &GetEndpointRoleArray()
{
    return g_EndpointRoleArrayInitializer;
}

const CEndpointRole &GetEndpointAllRoles()
{
    return g_EndpointAllRoleInitializer;
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
