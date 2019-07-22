#pragma once
#include<memory>
#include"ConnectionAttributes.h"
#include"IEnvironment.h"

namespace DrvFtaeAlarm {
	class Connection : public IExtensible {
	public:
		Connection() = delete;
		Connection(const std::shared_ptr<IEnvironment>& environment, const ConnectionAttributes& attributes);
		Connection(std::shared_ptr<IEnvironment>&& environment, ConnectionAttributes&& attributes);
		Connection(const Connection& src) = default;
		Connection& operator=(const Connection& rhs) = default;
		Connection(Connection&& src) = delete;
		Connection& operator=(Connection&& rhs) = delete;
		virtual ~Connection() override;
	protected:
		std::shared_ptr<IEnvironment> ptrEnvironment;
		ConnectionAttributes connectionAttributes;
	};
}
