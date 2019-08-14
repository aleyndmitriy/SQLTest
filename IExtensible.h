#pragma once
class IExtensible {
public:
	virtual ~IExtensible() {}
	virtual void* GetInterface(int nIfcId) = 0;
};