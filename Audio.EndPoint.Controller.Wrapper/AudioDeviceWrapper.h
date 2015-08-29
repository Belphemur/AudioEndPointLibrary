#pragma once

#include "DeviceState.h"

using namespace System;
using namespace AudioEndPoint;
namespace AudioEndPointControllerWrapper {

	public enum class Role {
		Console = ::eConsole,
		Multimedia = ::eMultimedia,
		Communications = ::eCommunications,
		All
	};

	public ref class AudioDeviceWrapper
	{
	private:
		AudioDevice* _audioDevice;
	public:
		AudioDeviceWrapper(std::unique_ptr<AudioDevice>& device)
		{
			_audioDevice = device.release();
		}
		~AudioDeviceWrapper() { this->!AudioDeviceWrapper(); }
		!AudioDeviceWrapper() { delete _audioDevice; }
		property String^ FriendlyName {
			String^ get()
			{
				return gcnew String(_audioDevice->FriendlyName);
			}
		}

		property String^ Description {
			String^ get()
			{
				return gcnew String(_audioDevice->Description);
			}
		}

		property String^ Id {
			String^ get()
			{
				return gcnew String(_audioDevice->ID);
			}
		}

		property String^ DeviceClassIconPath {
			String^ get()
			{
				return gcnew String(_audioDevice->DeviceClassIconPath);
			}
		}

		property AudioEndPoint::DeviceState DeviceState {
			AudioEndPoint::DeviceState get()
			{
				return static_cast<AudioEndPoint::DeviceState>(_audioDevice->DeviceState);
			}
		}


		virtual String^ ToString() override
		{
			return FriendlyName;
		}


		void SetAsDefault(Role role);
		bool IsDefault(Role role);
	};
}

