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
// DefSoundEndpointColl.h
// Audio endpoint collection
// ----------------------------------------------------------------------------

#pragma once
#include "DefSoundDeviceState.h"
#include "DefSoundEndpoint.h"


// ----------------------------------------------------------------------------

namespace DefSound {


class CEndpointCollection
{
public:
	CEndpointCollection(EDeviceState device_state = All, EDataFlow device_flow = ::eRender);

	void Refresh();

    typedef std::vector< CEndpoint > CImpl;
    const CImpl &Get() const;

    
    // ERole_enum_count may be used (==CEndpointRole::All)

    bool IsDefault(__in size_t nIndex, __in ERole Role) const;

    void SetDefault(__in size_t nIndex, __in ERole Role) const;
	static void SetDefaultEndpoint(__in const CEndpoint &Endpoint, __in::ERole Role);
    INT_PTR SetDefaultNext(__in ERole Role) const;   // -1 == no next

private:
    std::unique_ptr< const CImpl >  m_pImpl;
	EDeviceState m_device_state;
	EDataFlow m_device_type;
};

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
