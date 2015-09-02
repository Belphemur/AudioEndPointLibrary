#pragma once

#include "DeviceState.h"
#include "AudioDeviceType.h"

namespace AudioEndPointControllerWrapper {

	public enum class Role {
		Console = ::eConsole,
		Multimedia = ::eMultimedia,
		Communications = ::eCommunications,
		All
	};

	public enum class AudioDeviceType {
		Playback = AudioEndPoint::Playback,
		Recording = AudioEndPoint::Recording
	};

	public interface class IAudioDevice
	{

	public:

		property String^ FriendlyName {String^ get(); }

		property String^ Description {String^ get(); }

		property String^ Id {String^ get(); }

		property String^ DeviceClassIconPath {String^ get(); }

		property AudioEndPointControllerWrapper::DeviceState DeviceState {AudioEndPointControllerWrapper::DeviceState get(); }

		property AudioDeviceType Type {AudioDeviceType get(); }

		void SetAsDefault(Role role);
		bool IsDefault(Role role);
	};
}