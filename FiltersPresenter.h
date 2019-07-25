#pragma once
#include"IFiltersViewOutput.h"
#include"DatabaseEngine.h"
#include"StatementCondition.h"
#include<map>
namespace DrvFtaeAlarm {
	class FiltersPresenter final : public IFiltersViewOutput {
	public:
		FiltersPresenter(const std::shared_ptr<DatabaseEngine>& database);
		void viewIsReady() override;
		void SetViewInput(std::shared_ptr<IFiltersViewInput> input) override;
		~FiltersPresenter();
	private:
		std::weak_ptr<IFiltersViewInput> view;
		std::shared_ptr<DatabaseEngine> _database;
		std::map< std::string, std::vector<StatementCondition> > filters;
		std::string selectedFilter;
		std::vector<StatementCondition> editableCondition;
		size_t selectedCondition;
	};
}
