#pragma once
namespace DrvFtaeAlarm {
	class ISettingsViewInput {
	public:
		virtual void setupInitialState() = 0;
		virtual ~ISettingsViewInput() {};
	};
}