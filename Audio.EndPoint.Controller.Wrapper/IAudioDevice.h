#pragma once

#include "DeviceState.h"

namespace AudioEndPointControllerWrapper {

	public enum class Role {
		Console = ::eConsole,
		Multimedia = ::eMultimedia,
		Communications = ::eCommunications,
		All
	};

	public interface class IAudioDevice
	{

	public:

		property String^ FriendlyName {String^ get(); }

		property String^ Description {String^ get(); }

		property String^ Id {String^ get(); }

		property String^ DeviceClassIconPath {String^ get(); }

		property AudioEndPointControllerWrapper::DeviceState DeviceState {AudioEndPointControllerWrapper::DeviceState get(); }

		void SetAsDefault(Role role);
		bool IsDefault(Role role);
	};
}