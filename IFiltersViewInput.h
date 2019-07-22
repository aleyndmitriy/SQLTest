#pragma once

namespace DrvFtaeAlarm {
	class IFiltersViewInput {
	public:
		virtual void setupInitialState() = 0;
		virtual ~IFiltersViewInput() {};
	};
}