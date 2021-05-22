#include "platform/opengl/openglvertexarray.hpp"

#include "glad/glad.h"

namespace Light
{
	GLenum Shader2OpenGLType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4: 	return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4: 		return GL_INT;
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4: 		return GL_FLOAT;
		case ShaderDataType::Bool: 		return GL_BOOL;
		default:
			std::cerr << "Unsupported Shader data type" << std::endl;
			exit(1);
		}
	}

	VertexArray* VertexArray::create() 
	{
		return new OpenGLVertexArray();
	}

	OpenGLVertexArray::OpenGLVertexArray() 
	{
		glGenVertexArrays(1, &m_rendererId);
	}
	
	OpenGLVertexArray::~OpenGLVertexArray() = default;
	
	void OpenGLVertexArray::bind() const
	{
		glBindVertexArray(m_rendererId);
	}
	
	void OpenGLVertexArray::unbind() const
	{
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) 
	{
		if(vbo->getLayout().getElements().size() == 0)
		{
			std::cerr << "Buffer Layout not set!" << std::endl;
			exit(1);
		}

		glBindVertexArray(m_rendererId);
		vbo->bind();

		const auto& layout = vbo->getLayout();

		uint32_t index = 0;
		for(const auto& element: layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.getComponentCount(),
				Shader2OpenGLType(element.getType()),
				element.isNormalized() ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				INT2VOIDP(element.getOffset()));
			index++;
		}
		m_vertexBuffers.push_back(vbo);
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& ibo) 
	{
		glBindVertexArray(m_rendererId);
		ibo->bind();
        m_indexBuffer = ibo;
		glBindVertexArray(0);
	}
}