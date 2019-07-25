#pragma once
#include<map>
#include<string>

namespace DrvFtaeAlarm {
	class Record {
	public:
		using FieldData = std::pair<short, std::string>;
		using iterator = std::map<std::string, FieldData>::iterator;
		Record() = default;
		Record(const Record& src) = default;
		Record& operator=(const Record& src) = default;
		Record(Record&& src) noexcept = default;
		Record& operator=(Record&& src) = default;
		std::pair<iterator, bool> insert(std::string colName, short dataType, std::string value);
		FieldData operator[](std::string key);
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
