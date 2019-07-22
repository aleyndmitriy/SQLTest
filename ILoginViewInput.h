#pragma once
namespace DrvFtaeAlarm {
	class ILoginViewInput {
	public:
		virtual void setupInitialState() = 0;
		virtual ~ILoginViewInput() {};
	};
}