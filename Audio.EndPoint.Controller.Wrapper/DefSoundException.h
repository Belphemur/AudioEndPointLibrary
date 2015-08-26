#pragma once
using namespace System;
namespace AudioEndPoint {
	public ref class DefSoundException :
		public Exception
	{
	public:
		DefSoundException(DefSound::CError error);
	};

	public ref class DefSoundNotActiveException :
		public DefSoundException
	{
	public:
		DefSoundNotActiveException(DefSound::CNotActiveError error);
	};
}

