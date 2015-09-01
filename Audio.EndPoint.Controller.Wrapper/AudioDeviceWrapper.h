#pragma once

#include "IAudioDevice.h"

using namespace System;
using namespace AudioEndPoint;
namespace AudioEndPointControllerWrapper {


	public ref class AudioDeviceWrapper : IAudioDevice
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
		virtual property String^ FriendlyName {
			String^ get()
			{
				return gcnew String(_audioDevice->FriendlyName);
			}
		}

		virtual property String^ Description {
			String^ get()
			{
				return gcnew String(_audioDevice->Description);
			}
		}

		virtual property String^ Id {
			String^ get()
			{
				return gcnew String(_audioDevice->ID);
			}
		}

		virtual property String^ DeviceClassIconPath {
			String^ get()
			{
				return gcnew String(_audioDevice->DeviceClassIconPath);
			}
		}

		virtual property AudioEndPointControllerWrapper::DeviceState DeviceState {
			AudioEndPointControllerWrapper::DeviceState get()
			{
				return static_cast<AudioEndPointControllerWrapper::DeviceState>(_audioDevice->DeviceState);
			}
		}


		virtual String^ ToString() override
		{
			return FriendlyName;
		}


		virtual void SetAsDefault(Role role);
		virtual bool IsDefault(Role role);
	};
}

