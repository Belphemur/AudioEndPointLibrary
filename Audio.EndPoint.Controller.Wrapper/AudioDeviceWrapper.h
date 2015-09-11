#pragma once

using namespace System;
using namespace AudioEndPoint;
namespace AudioEndPointControllerWrapper {


	public ref class AudioDeviceWrapper : IAudioDevice
	{
	private:
	    SharedNativePtr<AudioDevice> _audioDevice;
	public:
		AudioDeviceWrapper(std::shared_ptr<AudioDevice> device)
		{
            _audioDevice.Reset(device);
		}
        ~AudioDeviceWrapper() { _audioDevice.Reset(); }
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

		virtual property AudioDeviceType Type {
			AudioDeviceType get()
			{
				return static_cast<AudioDeviceType>(_audioDevice->Type);
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

