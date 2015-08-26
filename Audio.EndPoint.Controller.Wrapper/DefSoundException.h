#pragma once
using namespace System;
namespace AudioEndPoint {
	ref class DefSoundException :
		public Exception
	{
	public:
		DefSoundException(DefSound::CError error);
	};
}

