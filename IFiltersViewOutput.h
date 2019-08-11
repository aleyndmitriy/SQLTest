#pragma once

#include<memory>
#include"IFiltersViewInput.h"

namespace DrvFtaeAlarm {
	class IFiltersViewOutput {
	public:
		virtual void viewIsReady() = 0;
		virtual void SetViewInput(std::shared_ptr<IFiltersViewInput> input) = 0;
		virtual void AddFilter(std::string filterName) = 0;
		virtual void SelectFilter(std::string filterName) = 0;
		virtual void AddCondition(StatementCondition&& condition, std::string filterName) = 0;
		virtual void SelectCondition(size_t index, std::string filterName) = 0;
		virtual void RemoveCondition(size_t index, std::string filterName) = 0;
		virtual void LoadProperties() = 0;
		virtual void SaveFilters() = 0;
		virtual ~IFiltersViewOutput() {};
	};
}