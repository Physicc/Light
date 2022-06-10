#include "config/appconfig.hpp"
#include "cereal/archives/json.hpp"

#include <fstream>

namespace Light
{
	bool AppConfig::Load(std::string filename)
	{
		std::ifstream ifs(filename);
		cereal::JSONInputArchive archive(ifs);
		archive(cereal::make_nvp("config", m_config));

		// Check if loaded correctly
		if (m_config.empty())
		{
			return false;
		}

		return true;
	}

	bool AppConfig::Save(std::string filename) const
	{
		std::ofstream ofs(filename);
		cereal::JSONOutputArchive archive(ofs);
		archive(cereal::make_nvp("config", m_config));

		return true;
	}

	bool AppConfig::GetBool(std::string name) const
	{
		return m_config.at(name) == "true";
	}

	int AppConfig::GetInt(std::string name) const
	{
		return std::stoi(m_config.at(name));
	}

	float AppConfig::GetFloat(std::string name) const
	{
		return std::stof(m_config.at(name));
	}

	std::string AppConfig::GetString(std::string name) const
	{
		return m_config.at(name);
	}

	void AppConfig::SetBool(std::string name, bool value)
	{
		m_config[name] = value ? "true" : "false";
	}

	void AppConfig::SetInt(std::string name, int value)
	{
		m_config[name] = std::to_string(value);
	}

	void AppConfig::SetFloat(std::string name, float value)
	{
		m_config[name] = std::to_string(value);
	}

	void AppConfig::SetString(std::string name, std::string value)
	{
		m_config[name] = value;
	}

	bool AppConfig::Has(std::string name) const
	{
		return m_config.find(name) != m_config.end();
	}

	std::string AppConfig::operator[](std::string name) const
	{
		return m_config.at(name);
	}
}
