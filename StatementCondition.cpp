#include"StatementCondition.h"

DrvFtaeAlarm::StatementCondition::StatementCondition(CombineOperation combineOperation, std::string property, PropertyType propertyType, ConditionType conditionType, std::string value1, std::string value2):
	_combineOperation(combineOperation),_property(property),_propertyType(propertyType),_conditionType(conditionType),_value1(value1),_value2(value2)
{

}

DrvFtaeAlarm::StatementCondition::StatementCondition() :StatementCondition(CombineOperation::COMBINEOP_OR, std::string(), PropertyType::PROPTYPE_NUMERIC, ConditionType::CONDTYPE_EQUAL, std::string(), std::string())
{

}

DrvFtaeAlarm::StatementCondition::~StatementCondition()
{
	_property.clear();
	_value1.clear();
	_value2.clear();
}

DrvFtaeAlarm::CombineOperation DrvFtaeAlarm::StatementCondition::GetCombineOperation() const
{
	return _combineOperation;
}

std::string DrvFtaeAlarm::StatementCondition::GetProperty() const
{
	return _property;
}

DrvFtaeAlarm::ConditionType DrvFtaeAlarm::StatementCondition::GetConditionType() const
{
	return _conditionType;
}

DrvFtaeAlarm::PropertyType DrvFtaeAlarm::StatementCondition::GetPropertyType() const
{
	return _propertyType;
}

std::string DrvFtaeAlarm::StatementCondition::GetValue1() const
{
	return _value1;
}

std::string DrvFtaeAlarm::StatementCondition::GetValue2() const
{
	return _value2;
}

void DrvFtaeAlarm::StatementCondition::SetCombineOperation(CombineOperation operation)
{
	_combineOperation = operation;
}

void DrvFtaeAlarm::StatementCondition::SetConditionType(ConditionType conditionType)
{
	_conditionType = conditionType;
}

void DrvFtaeAlarm::StatementCondition::SetProperty(std::string property)
{
	_property = property;
}

void DrvFtaeAlarm::StatementCondition::SetPropertyType(PropertyType propertyType)
{
	_propertyType = propertyType;
}

void DrvFtaeAlarm::StatementCondition::SetValue1(std::string value1)
{
	_value1 = value1;
}

void DrvFtaeAlarm::StatementCondition::SetValue2(std::string value2)
{
	_value2 = value2;
}


bool DrvFtaeAlarm::operator==(const DrvFtaeAlarm::StatementCondition& lhs, const DrvFtaeAlarm::StatementCondition& rhs)
{
	return lhs.GetProperty() == rhs.GetProperty();
}

bool DrvFtaeAlarm::operator!=(const DrvFtaeAlarm::StatementCondition& lhs, const DrvFtaeAlarm::StatementCondition& rhs)
{
	return lhs.GetProperty() != rhs.GetProperty();
}

bool DrvFtaeAlarm::operator<(const DrvFtaeAlarm::StatementCondition& lhs, const DrvFtaeAlarm::StatementCondition& rhs)
{
	return lhs.GetProperty() < rhs.GetProperty();
}

bool DrvFtaeAlarm::operator<=(const DrvFtaeAlarm::StatementCondition& lhs, const DrvFtaeAlarm::StatementCondition& rhs)
{
	return lhs.GetProperty() <= rhs.GetProperty();
}

bool DrvFtaeAlarm::operator>(const DrvFtaeAlarm::StatementCondition& lhs, const DrvFtaeAlarm::StatementCondition& rhs)
{
	return lhs.GetProperty() > rhs.GetProperty();
}

bool DrvFtaeAlarm::operator>=(const DrvFtaeAlarm::StatementCondition& lhs, const DrvFtaeAlarm::StatementCondition& rhs)
{
	return lhs.GetProperty() >= rhs.GetProperty();
}