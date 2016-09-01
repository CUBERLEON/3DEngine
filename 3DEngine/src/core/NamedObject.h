#pragma once

#include <string>

class NamedObject
{
public:
	NamedObject(const std::string& name);
	~NamedObject();

	inline const std::string& getName() const { return m_name; }
protected:
	std::string m_name;
private:
};

