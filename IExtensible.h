#pragma once
class IExtensible {
public:
	virtual ~IExtensible() {}
	virtual void* GetInterface() = 0;
};