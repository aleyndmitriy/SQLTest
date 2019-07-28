#pragma once
#include"IFiltersViewOutput.h"
#include"DatabaseEngine.h"
#include"StatementCondition.h"
#include<map>
namespace DrvFtaeAlarm {
	class FiltersPresenter final : public IFiltersViewOutput {
	public:
		using ConditionsVector = std::vector<StatementCondition>;
		using FiltersIterator = std::map<std::string, ConditionsVector>::iterator;
		FiltersPresenter(const std::shared_ptr<DatabaseEngine>& database);
		void viewIsReady() override;
		void SetViewInput(std::shared_ptr<IFiltersViewInput> input) override;
		void AddFilter(std::string filterName) override;
		void SelectFilter(std::string filterName) override;
		void AddCondition(StatementCondition&& condition, std::string filterName) override;
		void RemoveCondition(size_t index, std::string filterName) override;
		void SelectCondition(size_t index, std::string filterName) override;
		~FiltersPresenter();
	private:
		std::weak_ptr<IFiltersViewInput> view;
		std::shared_ptr<DatabaseEngine> _database;
		std::map<std::string, ConditionsVector> filters;
	};
}
