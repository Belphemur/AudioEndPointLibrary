// Audio.EndPoint.Controller.Wrapper.h

#pragma once
#include "AudioDeviceWrapper.h"
#include "DeviceState.h"

using namespace System;
using namespace System::Collections::Generic;

namespace AudioEndPointControllerWrapper {


	public ref class AudioController
	{
	public:
		static List<AudioDeviceWrapper^>^ GetActivePlaybackDevices();
		static List<AudioDeviceWrapper^>^ GetAllPlaybackDevices();
		static List<AudioDeviceWrapper^>^ GetPlaybackDevices(DeviceState state);
		static List<AudioDeviceWrapper^>^ GetRecordingDevices(DeviceState state);
		static List<AudioDeviceWrapper^>^ GetActiveRecordingDevices();
		static List<AudioDeviceWrapper^>^ GetAllRecordingDevices();

	};

}
