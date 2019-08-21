#pragma once
#include"IFiltersViewOutput.h"
#include"DatabaseEngine.h"
#include"StatementCondition.h"
#include"ISettingsDataSource.h"
#include "DatabaseInfoDAO.h"
#include<map>

namespace DrvFtaeAlarm {
	class FiltersPresenter final : public IFiltersViewOutput {
	public:
		using ConditionsVector = std::vector<StatementCondition>;
		using FiltersIterator = std::map<std::string, ConditionsVector>::iterator;
		FiltersPresenter(const std::shared_ptr<DatabaseEngine>& database, const std::shared_ptr<DatabaseInfoDAO>& databaseInfoDAO, const std::shared_ptr<ISettingsDataSource>& settingsDataSource);
		void viewIsReady() override;
		void SetViewInput(std::shared_ptr<IFiltersViewInput> input) override;
		void AddFilter(std::string filterName) override;
		void RemoveFilter(std::string filterName) override;
		void SelectFilter(std::string filterName) override;
		void AddCondition(StatementCondition&& condition, std::string filterName) override;
		void RemoveCondition(size_t index, std::string filterName) override;
		void RemoveAllConditions(std::string filterName) override;
		void SelectCondition(size_t index, std::string filterName) override;
		void SaveFilters() override;
		void LoadProperties() override;
		~FiltersPresenter();
	private:
		std::weak_ptr<IFiltersViewInput> view;
		std::shared_ptr<DatabaseEngine> _database;
		std::shared_ptr<DatabaseInfoDAO> _databaseInfoDAO;
		std::shared_ptr<ISettingsDataSource> _settingsDataSource;
		std::map<std::string, ConditionsVector> filters;
	};
}
