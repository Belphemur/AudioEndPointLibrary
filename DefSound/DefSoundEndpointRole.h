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
// DefSoundEndpointRole.h
// Audio endpoint roles
// ----------------------------------------------------------------------------

#pragma once
#include <array>

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

struct CEndpointRole
{
    enum
    {
        Console,
        Multimedia,
        Communications,
        All,

        CountOf
    };

    PCWSTR m_wszShortName;
    PCWSTR m_wszFullName;
    PCWSTR m_wszDescription;
    ::ERole m_RoleValue;
};

// ----------------------------------------------------------------------------

static_assert(CEndpointRole::Console == ::eConsole, "CEndpointRole::Console == ::eConsole");
static_assert(CEndpointRole::Multimedia == ::eMultimedia, "CEndpointRole::Multimedia == ::eMultimedia");
static_assert(CEndpointRole::Communications == ::eCommunications, "CEndpointRole::Communications == ::eCommunications");

// ----------------------------------------------------------------------------

typedef std::array<CEndpointRole, ERole_enum_count > CEndpointRoleArray;

// ----------------------------------------------------------------------------

const CEndpointRoleArray &GetEndpointRoleArray();
const CEndpointRole &GetEndpointAllRoles();

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
