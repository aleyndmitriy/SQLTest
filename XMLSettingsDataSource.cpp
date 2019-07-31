#include "XMLSettingsDataSource.h"
#include"pugixml.hpp"

bool DrvFtaeAlarm::XMLSettingsDataSource::Save(const std::map<std::pair<std::string, bool>, std::vector<StatementCondition> >& filters)
{
	pugi::xml_document doc;
	pugi::xml_node rootNode = doc.append_child("Filters");
	for (std::map<std::pair<std::string, bool>, std::vector<StatementCondition> >::const_iterator filterItr = filters.cbegin(); filterItr != filters.cend(); ++filterItr)
	{
		pugi::xml_node alarmNode = rootNode.append_child("AlarmFilter");
		alarmNode.append_attribute("Name").set_value(filterItr->first.first.c_str());
		alarmNode.append_attribute("Selected").set_value(filterItr->first.second);
		for (std::vector<StatementCondition>::const_iterator conditionItr = filterItr->second.cbegin(); conditionItr != filterItr->second.cend(); ++conditionItr) {
			pugi::xml_node conditionNode = alarmNode.append_child("Condition");
			conditionNode.append_attribute("CombineOperation").set_value(static_cast<long>(conditionItr->GetCombineOperation()));
			conditionNode.append_attribute("Property").set_value(conditionItr->GetProperty().c_str());
			conditionNode.append_attribute("PropertyType").set_value(static_cast<long>(conditionItr->GetPropertyType()));
			conditionNode.append_attribute("ConditionType").set_value(static_cast<long>(conditionItr->GetConditionType()));
			if (conditionItr->GetValue1().empty() == false) {
				conditionNode.append_attribute("Value1").set_value(conditionItr->GetValue1().c_str());
			}
			if (conditionItr->GetValue2().empty() == false) {
				conditionNode.append_attribute("Value2").set_value(conditionItr->GetValue2().c_str());
			}
		}
	}
	doc.save_file("Filters.xml");
	return true;
}

bool DrvFtaeAlarm::XMLSettingsDataSource::Save(const ConnectionAttributes& attributes)
{
	pugi::xml_document doc;
	pugi::xml_node rootNode = doc.append_child("Settings");
	pugi::xml_node connectionNode = rootNode.append_child("Connection");
	connectionNode.append_attribute("Version").set_value(attributes.driver.c_str());
	connectionNode.append_attribute("Server").set_value(attributes.serverName.c_str());
	connectionNode.append_attribute("ConfigDataBase").set_value(attributes.databaseName.c_str());
	connectionNode.append_attribute("User").set_value(attributes.loginName.c_str());
	connectionNode.append_attribute("Password").set_value(attributes.password.c_str());
	doc.save_file("Settings.xml");
	return true;
}

bool DrvFtaeAlarm::XMLSettingsDataSource::Load(std::map<std::pair<std::string, bool>, std::vector<StatementCondition> >& filters)
{
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file("Filters.xml");
	if (!res) {
		return false;
	}
	pugi::xml_node rootNode = doc.child("Filters");
	for (pugi::xml_node alarmNode = rootNode.child("AlarmFilter"); alarmNode; alarmNode = alarmNode.next_sibling("AlarmFilter"))
	{
		std::vector<StatementCondition> conditions;
		for (pugi::xml_node conditionNode = alarmNode.child("Condition"); conditionNode; conditionNode = conditionNode.next_sibling("Condition"))
		{
			CombineOperation combine{conditionNode.attribute("CombineOperation").as_int()};
			PropertyType propertyType{conditionNode.attribute("PropertyType").as_int()};
			ConditionType conditionType{conditionNode.attribute("ConditionType").as_int()};
			conditions.push_back(StatementCondition(combine, std::string(conditionNode.attribute("Property").as_string()),propertyType,conditionType, std::string(conditionNode.attribute("Value1").as_string()), std::string(conditionNode.attribute("Value2").as_string())));
		}
		std::pair<std::pair<std::string, bool>, std::vector<StatementCondition> > mapVal =
			std::make_pair<std::pair<std::string, bool>, std::vector<StatementCondition> >(std::make_pair<std::string, bool>(std::string(alarmNode.attribute("Name").as_string()), alarmNode.attribute("Selected").as_bool()), std::vector<StatementCondition>(conditions));
		filters.insert(mapVal);
	}
	return true;
}


bool DrvFtaeAlarm::XMLSettingsDataSource::Load(ConnectionAttributes& attributes)
{
	pugi::xml_document doc;
	pugi::xml_parse_result res = doc.load_file("Settings.xml");
	if (!res) {
		return false;
	}
	pugi::xml_node rootNode = doc.child("Settings");
	pugi::xml_node connectionNode = rootNode.child("Connection");
	attributes.driver = std::string(connectionNode.attribute("Version").as_string());
	attributes.serverName = std::string(connectionNode.attribute("Server").as_string());
	attributes.databaseName = std::string(connectionNode.attribute("ConfigDataBase").as_string());
	attributes.loginName = std::string(connectionNode.attribute("User").as_string());
	attributes.password = std::string(connectionNode.attribute("Password").as_string());
	return true;
}



