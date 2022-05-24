#ifndef __APPCONFIG_H__
#define __APPCONFIG_H__

#include <string>
#include <map>
#include "cereal/cereal.hpp"

namespace cereal
{
	//! Saving for std::map<std::string, std::string>
	template <class Archive, class C, class A> inline
	void save( Archive & ar, std::map<std::string, std::string, C, A> const & map )
	{
		for( const auto & i : map )
			ar( cereal::make_nvp( i.first, i.second ) );
	}

	//! Loading for std::map<std::string, std::string>
	template <class Archive, class C, class A> inline
	void load( Archive & ar, std::map<std::string, std::string, C, A> & map )
	{
		map.clear();

		auto hint = map.begin();
		while( true )
		{
			const auto namePtr = ar.getNodeName();

			if( !namePtr )
				break;

			std::string key = namePtr;
			std::string value; ar( value );
			hint = map.emplace_hint( hint, std::move( key ), std::move( value ) );
		}
	}
} // namespace cereal

namespace Light
{

	class AppConfig
	{
	public:
		AppConfig() = default;
		~AppConfig() = default;

		bool Load(std::string filename);
		bool Save(std::string filename) const;

		bool GetBool(std::string name) const;
		int GetInt(std::string name) const;
		float GetFloat(std::string name) const;
		std::string GetString(std::string name) const;

		void SetBool(std::string name, bool value);
		void SetInt(std::string name, int value);
		void SetFloat(std::string name, float value);
		void SetString(std::string name, std::string value);

		bool Has(std::string name) const;

		std::string operator[](std::string name) const;

	private:
		std::string m_filename;
		std::map<std::string, std::string> m_config;
	};
}

#endif // __APPCONFIG_H__
