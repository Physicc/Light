#include "light/rendering/shader.hpp"

#include <iostream>

namespace Light
{
	void ShaderLibrary::add(const std::shared_ptr<Shader>& shader) 
	{
		auto& name = shader->getName();

		add(name, shader);
	}

	void ShaderLibrary::add(const std::string& name, const std::shared_ptr<Shader>& shader) 
	{
		if(m_shaders.find(name) != m_shaders.end())
		{
			std::cerr << "Shader already exists!" << std::endl;
			exit(1);
		}

        m_shaders[name] = shader;
	}
	
	std::shared_ptr<Shader> ShaderLibrary::load(const std::string& filepath) 
	{
		std::shared_ptr<Shader> shader = Shader::create(filepath.c_str());
		add(shader);
		return shader;
	}
	
	std::shared_ptr<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath) 
	{
		std::shared_ptr<Shader> shader = Shader::create(filepath.c_str());
		add(name, shader);
		return shader;
	}
	
	std::shared_ptr<Shader> ShaderLibrary::get(const std::string& name) 
	{
		if(m_shaders.find(name) == m_shaders.end())
		{
			std::cerr << "Shader does not exist!" << std::endl;
			exit(1);
		}

		return m_shaders[name];
	}
}

