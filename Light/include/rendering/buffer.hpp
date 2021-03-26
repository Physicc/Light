#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "core/base.hpp"

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
			: type(type), name(name), normalized(normalized), offset(0),
				size(getSize(type)), componentCount(getComponentCount(type))
		{}

		void setOffset(uint32_t offset)
		{
			this->offset = offset;
		}

		uint32_t getSize() const
		{
			return size;
		}

		uint32_t getComponentCount() const
		{
			return componentCount;
		}
		
		uint32_t getOffset() const
		{
			return offset;
		}

		ShaderDataType getType() const
		{
			return type;
		}

		bool isNormalized() const
		{
			return normalized;
		}

	private:
		ShaderDataType type;
		std::string name;
		uint32_t offset;
		uint32_t size;
		uint32_t componentCount;
		bool normalized;

	private:
		uint32_t getSize(ShaderDataType type)
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
				std::cerr << "Unsupported Shader data type" << std::endl;
				exit(1);
			}
		}

		uint32_t getComponentCount(ShaderDataType type)
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
				std::cerr << "Unsupported Shader data type" << std::endl;
				exit(1);
			}
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements) : elements(elements)
		{
			stride = 0;
			for(auto &element : this->elements)
			{
				element.setOffset(stride);
				stride += element.getSize();
			}
		}

		uint32_t getStride() const
		{
			return stride;
		}

		inline const std::vector<BufferElement>& getElements() const { return elements; }

		std::vector<BufferElement>::iterator begin() { return elements.begin(); }
		std::vector<BufferElement>::iterator end() { return elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

	private:
		std::vector<BufferElement> elements;
		uint32_t stride;
	};

	class VertexBuffer
	{
	public:
		VertexBuffer() {}
		virtual ~VertexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		void setLayout(BufferLayout layout)
		{
			this->layout = layout;
		}

		const BufferLayout& getLayout()
		{
			return layout;
		}

		static VertexBuffer* create(float* vertices, uint32_t size);

	protected:
		BufferLayout layout;
	};


	class IndexBuffer
	{
	public:
		IndexBuffer() {}
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		static IndexBuffer* create(uint32_t* indices, uint32_t count);
	};
}

#endif // __BUFFER_H__