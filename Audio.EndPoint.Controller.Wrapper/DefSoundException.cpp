#include "stdafx.h"
#include "DefSoundException.h"


AudioEndPoint::DefSoundException::DefSoundException(DefSound::CError error) : Exception{gcnew String(error.m_Description.c_str())}
{
}

AudioEndPoint::DefSoundNotActiveException::DefSoundNotActiveException(DefSound::CNotActiveError error) : DefSoundException{error}
{
	
}
