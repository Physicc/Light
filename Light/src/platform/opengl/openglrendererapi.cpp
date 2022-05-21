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

	void OpenGLRendererAPI::setBlendFunc(BlendFactor src, BlendFactor dst)
	{
		GLenum sfactor = GL_NONE;
		GLenum dfactor = GL_NONE;

		switch (src)
		{
		case BlendFactor::ZERO:
			sfactor = GL_ZERO;
			break;

		case BlendFactor::ONE:
			sfactor = GL_ONE;
			break;

		case BlendFactor::SRC_COLOR:
			sfactor = GL_SRC_COLOR;
			break;

		case BlendFactor::ONE_MINUS_SRC_COLOR:
			sfactor = GL_ONE_MINUS_SRC_COLOR;
			break;

		case BlendFactor::DST_COLOR:
			sfactor = GL_DST_COLOR;
			break;

		case BlendFactor::ONE_MINUS_DST_COLOR:
			sfactor = GL_ONE_MINUS_DST_COLOR;
			break;

		case BlendFactor::SRC_ALPHA:
			sfactor = GL_SRC_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_SRC_ALPHA:
			sfactor = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case BlendFactor::DST_ALPHA:
			sfactor = GL_DST_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_DST_ALPHA:
			sfactor = GL_ONE_MINUS_DST_ALPHA;
			break;

		case BlendFactor::CONSTANT_ALPHA:
			sfactor = GL_CONSTANT_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
			sfactor = GL_ONE_MINUS_CONSTANT_ALPHA;
			break;

		default:
			break;
		}

		switch (dst)
		{
		case BlendFactor::ZERO:
			dfactor = GL_ZERO;
			break;

		case BlendFactor::ONE:
			dfactor = GL_ONE;
			break;

		case BlendFactor::SRC_COLOR:
			dfactor = GL_SRC_COLOR;
			break;

		case BlendFactor::ONE_MINUS_SRC_COLOR:
			dfactor = GL_ONE_MINUS_SRC_COLOR;
			break;

		case BlendFactor::DST_COLOR:
			dfactor = GL_DST_COLOR;
			break;

		case BlendFactor::ONE_MINUS_DST_COLOR:
			dfactor = GL_ONE_MINUS_DST_COLOR;
			break;

		case BlendFactor::SRC_ALPHA:
			dfactor = GL_SRC_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_SRC_ALPHA:
			dfactor = GL_ONE_MINUS_SRC_ALPHA;
			break;

		case BlendFactor::DST_ALPHA:
			dfactor = GL_DST_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_DST_ALPHA:
			dfactor = GL_ONE_MINUS_DST_ALPHA;
			break;

		case BlendFactor::CONSTANT_ALPHA:
			dfactor = GL_CONSTANT_ALPHA;
			break;

		case BlendFactor::ONE_MINUS_CONSTANT_ALPHA:
			dfactor = GL_ONE_MINUS_CONSTANT_ALPHA;
			break;

		default:
			break;
		}

		glBlendFunc(sfactor, dfactor);
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
