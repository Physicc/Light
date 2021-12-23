#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "core/base.hpp"
#include "core/logging.hpp"

namespace Light
{

	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Int, Int2, Int3, Int4,
		Mat3, Mat4,
		Bool
	};

	struct BufferElement
	{
		BufferElement(ShaderDataType type, std::string name, bool normalized = false)
			:	m_type(type), m_name(name), m_offset(0), m_size(getSize(type)),
				m_componentCount(getComponentCount(type)), m_normalized(normalized)
		{}

		void setOffset(uint32_t offset)
		{
			this->m_offset = offset;
		}

		uint32_t getSize() const
		{
			return m_size;
		}

		uint32_t getComponentCount() const
		{
			return m_componentCount;
		}
		
		uint32_t getOffset() const
		{
			return m_offset;
		}

		ShaderDataType getType() const
		{
			return m_type;
		}

		bool isNormalized() const
		{
			return m_normalized;
		}

	private:
		ShaderDataType m_type;
		std::string m_name;
		uint32_t m_offset;
		uint32_t m_size;
		uint32_t m_componentCount;
		bool m_normalized;

	private:
		inline uint32_t getSize(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float: 	return 4;
			case ShaderDataType::Float2: 	return 4 * 2;
			case ShaderDataType::Float3: 	return 4 * 3;
			case ShaderDataType::Float4: 	return 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2: 		return 4 * 2;
			case ShaderDataType::Int3: 		return 4 * 3;
			case ShaderDataType::Int4: 		return 4 * 4;
			case ShaderDataType::Mat3: 		return 4 * 3 * 3;
			case ShaderDataType::Mat4: 		return 4 * 4 * 4;
			case ShaderDataType::Bool: 		return 1;
			default:
				LIGHT_CORE_WARN("Unsupported Shader data type");
				return 0;
			}
		}

		inline uint32_t getComponentCount(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Float: 	return 1;
			case ShaderDataType::Float2: 	return 2;
			case ShaderDataType::Float3: 	return 3;
			case ShaderDataType::Float4: 	return 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2: 		return 2;
			case ShaderDataType::Int3: 		return 3;
			case ShaderDataType::Int4: 		return 4;
			case ShaderDataType::Mat3: 		return 3 * 3;
			case ShaderDataType::Mat4: 		return 4 * 4;
			case ShaderDataType::Bool: 		return 1;
			default:
				LIGHT_CORE_WARN("Unsupported Shader data type");
				return 0;
			}
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(std::initializer_list<BufferElement> elements) : m_elements(elements)
		{
            m_stride = 0;
			for(auto &element : this->m_elements)
			{
				element.setOffset(m_stride);
                m_stride += element.getSize();
			}
		}

		inline uint32_t getStride() const
		{
			return m_stride;
		}

		inline const std::vector<BufferElement>& getElements() const { return m_elements; }

		std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_stride;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() = default;
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		inline void setLayout(BufferLayout layout)
		{
			this->m_layout = layout;
		}

		inline const BufferLayout& getLayout()
		{
			return m_layout;
		}

		static VertexBuffer* create(float* vertices, uint32_t size);

	protected:
		BufferLayout m_layout;
	};


	class IndexBuffer
	{
	public:
		IndexBuffer() = default;
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		static IndexBuffer* create(uint32_t* indices, uint32_t count);
	};
}

#endif // __BUFFER_H__