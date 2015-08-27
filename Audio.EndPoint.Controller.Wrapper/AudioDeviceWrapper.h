#pragma once

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
		clr_scoped_ptr<std::unique_ptr<AudioDevice>> _audioDevice;
	public:
		AudioDeviceWrapper(std::unique_ptr<AudioDevice>& device) : _audioDevice(std::move(device)) {}
		~AudioDeviceWrapper() { this->!AudioDeviceWrapper(); }
		!AudioDeviceWrapper() { delete _audioDevice; }
		property String^ FriendlyName {
			String^ get()
			{
				return gcnew String((_audioDevice.get())->get()->FriendlyName);
			}
		}

		property String^ Description {
			String^ get()
			{
				return gcnew String((_audioDevice.get())->get()->Description);
			}
		}

		property String^ Id {
			String^ get()
			{
				return gcnew String((_audioDevice.get())->get()->ID);
			}
		}

		property String^ DeviceClassIconPath {
			String^ get()
			{
				return gcnew String((_audioDevice.get())->get()->DeviceClassIconPath);
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

