// Audio.EndPoint.Controller.Wrapper.h

#pragma once
#include "AudioDeviceWrapper.h"

using namespace System;
using namespace System::Collections::Generic;

namespace AudioEndPointControllerWrapper {

	/*
	#define DEVICE_STATE_ACTIVE      0x00000001
	#define DEVICE_STATE_DISABLED    0x00000002
	#define DEVICE_STATE_NOTPRESENT  0x00000004
	#define DEVICE_STATE_UNPLUGGED   0x00000008
	#define DEVICE_STATEMASK_ALL     0x0000000f
	*/
	[Flags]
	public enum class DeviceState {
		Active = DEVICE_STATE_ACTIVE,
		Disabled = DEVICE_STATE_DISABLED,
		NotPresent = DEVICE_STATE_NOTPRESENT,
		Unplugged = DEVICE_STATE_UNPLUGGED,
		All = DEVICE_STATEMASK_ALL
	};

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
