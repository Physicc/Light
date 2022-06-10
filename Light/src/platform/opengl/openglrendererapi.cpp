#include "light/platform/opengl/openglrendererapi.hpp"

#include "light/rendering/rendercommand.hpp"

#include "glad/gl.h"

namespace Light
{
	RendererAPI* RenderCommand::s_rendererApi = new OpenGLRendererAPI;

	void OpenGLRendererAPI::init()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::depthMask(bool enable)
	{
		if(enable)
		{
			glDepthMask(GL_TRUE);
		}
		else
		{
			glDepthMask(GL_FALSE);
		}
	}

	void OpenGLRendererAPI::setViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::setClearColor(glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static GLenum BlendFactor2OpenGLType(BlendFactor factor)
	{
		GLenum result = GL_NONE;

		switch (factor)
		{
		case BlendFactor::ZERO:
			result = GL_ZERO;
			break;

		case BlendFactor::ONE:
			result = GL_ONE;
			break;

		case BlendFactor::SRC_COLOR:
			result = GL_SRC_COLOR;
			break;

		case BlendFactor::ONE_MINUS_SRC_COLOR:
			result = GL_ONE_MINUS_SRC_COLOR;
			break;

		case BlendFactor::DST_COLOR:
			result = GL_DST_COLOR;
			break;

		case BlendFactor::ONE_MINUS_DST_COLOR:
			result = GL_ONE_MINUS_DST_COLOR;
			break;

		case BlendFactor::SRC_ALPHA:
			result = GL_SRC_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_SRC_ALPHA:
			result = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case BlendFactor::DST_ALPHA:
			result = GL_DST_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_DST_ALPHA:
			result = GL_ONE_MINUS_DST_ALPHA;
			break;

		case BlendFactor::CONSTANT_ALPHA:
			result = GL_CONSTANT_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
			result = GL_ONE_MINUS_CONSTANT_ALPHA;
			break;

		default:
			break;
		}

		return result;
	}

	void OpenGLRendererAPI::setBlendFunc(BlendFactor src, BlendFactor dst)
	{
		glBlendFunc(BlendFactor2OpenGLType(src), BlendFactor2OpenGLType(dst));
	}

	void OpenGLRendererAPI::setBlendFuncSeperate(BlendFactor srcRGB, BlendFactor dstRGB, BlendFactor srcAlpha, BlendFactor dstAlpha)
	{
		glBlendFuncSeparate(BlendFactor2OpenGLType(srcRGB), BlendFactor2OpenGLType(dstRGB), BlendFactor2OpenGLType(srcAlpha), BlendFactor2OpenGLType(dstAlpha));
	}

	void OpenGLRendererAPI::drawIndexed(const std::shared_ptr<VertexArray>& vao)
	{
		glDrawElements(GL_TRIANGLES, vao->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::framebufferBlit(const std::shared_ptr<Framebuffer>& src, const std::shared_ptr<Framebuffer>& dst, bool depth)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, src->getRendererId());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst->getRendererId());

		auto srcSpec = src->getSpec();
		auto dstSpec = dst->getSpec();

		if(depth)
		{
			glBlitFramebuffer(0, 0, srcSpec.width, srcSpec.height, 0, 0, dstSpec.width, dstSpec.height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		}
		else
		{
			LIGHT_CORE_ASSERT("Will be implemented later", false);
		}
	}
}
