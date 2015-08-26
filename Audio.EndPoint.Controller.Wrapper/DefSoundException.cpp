#include "stdafx.h"
#include "DefSoundException.h"


AudioEndPoint::DefSoundException::DefSoundException(DefSound::CError error)
{
	InnerMessage = gcnew String(error.m_Description.c_str());
}
