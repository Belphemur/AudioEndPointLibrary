#pragma once
using namespace System;
namespace AudioEndPoint {
	public ref class DefSoundException :
		public Exception
	{
	public:
		DefSoundException(DefSound::CError error);
		String^ InnerMessage;
		virtual String^ ToString() override
		{
			return gcnew String("InnerMessage: "+ InnerMessage + " \n" + __super::ToString() );
		}
	};
}

