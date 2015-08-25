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
// DefSoundError.cpp
// DefSound exception
// ----------------------------------------------------------------------------

#include "stdafx.h"
#include "DefSoundError.h"

// ----------------------------------------------------------------------------

namespace DefSound {

// ----------------------------------------------------------------------------

CError::CError(const std::wstring &Description, DWORD nErrorCode)
    : m_Description(Description), m_nErrorCode(nErrorCode)
{
}

// ----------------------------------------------------------------------------

std::wstring MakeDefaultErrorDescription(
    __in PCWSTR wszFuctionName, 
    __in PCWSTR wszArguments /*= L"(...)"*/
)
{
    std::wstring Description = L"Call ";
    Description += wszFuctionName;
    Description += wszArguments;
    Description += L" failed";
    return Description;
}

// ----------------------------------------------------------------------------

}   // namespace DefSound

// ----------------------------------------------------------------------------
