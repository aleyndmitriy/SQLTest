#pragma once
#include<map>
#include<string>

namespace DrvFtaeAlarm {
	class Record {
	public:
		using FieldData = std::pair<short, std::string>;
		using const_iterator = std::map<std::string, FieldData>::const_iterator;
		Record() = default;
		Record(const Record& src) = default;
		Record& operator=(const Record& src) = default;
		Record(Record&& src) noexcept = default;
		Record& operator=(Record&& src) = default;
		std::pair<const_iterator, bool> insert(std::string colName, short dataType, std::string value);
		const FieldData& at(const std::string& key) const;
		const_iterator cbegin() const;
		const_iterator cend() const;
		const_iterator findColumnValue(const std::string& key) const;
		~Record();
		friend bool operator==(const Record& lhs, const Record& rhs);
		friend bool operator!=(const Record& lhs, const Record& rhs);
		friend bool operator<(const Record& lhs, const Record& rhs);
		friend bool operator<=(const Record& lhs, const Record& rhs);
		friend bool operator>(const Record& lhs, const Record& rhs);
		friend bool operator>=(const Record& lhs, const Record& rhs);
	private:
		std::map<std::string, FieldData> recordData;
	};

	bool operator==(const Record& lhs, const Record& rhs);
	bool operator!=(const Record& lhs, const Record& rhs);
	bool operator<(const Record& lhs, const Record& rhs);
	bool operator<=(const Record& lhs, const Record& rhs);
	bool operator>(const Record& lhs, const Record& rhs);
	bool operator>=(const Record& lhs, const Record& rhs);
}
