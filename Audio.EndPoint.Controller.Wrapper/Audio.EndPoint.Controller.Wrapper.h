// Audio.EndPoint.Controller.Wrapper.h

#pragma once
#include "IAudioDevice.h"

using namespace System;
using namespace System::Collections::Generic;

namespace AudioEndPointControllerWrapper {


	public ref class AudioController
	{
	public:
		static List<IAudioDevice^>^ GetActivePlaybackDevices();
		static List<IAudioDevice^>^ GetAllPlaybackDevices();
		static List<IAudioDevice^>^ GetPlaybackDevices(DeviceState state);
		static List<IAudioDevice^>^ GetRecordingDevices(DeviceState state);
		static List<IAudioDevice^>^ GetActiveRecordingDevices();
		static List<IAudioDevice^>^ GetAllRecordingDevices();

	};

}
