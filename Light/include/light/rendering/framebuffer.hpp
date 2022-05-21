#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include "core/base.hpp"

namespace Light
{
	enum class FramebufferTextureFormat
	{
		None,

		// Color Buffers
		RGBA8,
		RED_INTEGER,

		// Depth Buffers
		DEPTH24_STENCIL8,


		// Default depth
		Depth = DEPTH24_STENCIL8,

		/**
		 * @brief First index of Depth Type attachment
		 * @example Use like if(fmt < FramebufferAttachmentFormat::DepthType) { // Code for color buffers }
		 */
		DepthTypes = DEPTH24_STENCIL8
	};

	enum class TextureWrap
	{
		None,

		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_BORDER,
		CLAMP_TO_EDGE
	};

	struct FramebufferTextureSpec
	{
		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
		TextureWrap wrapFormat = TextureWrap::None;

		FramebufferTextureSpec() = default;
		FramebufferTextureSpec(
			FramebufferTextureFormat format,
			TextureWrap wrap) : textureFormat(format), wrapFormat(wrap) {}
	};

	struct FramebufferAttachmentsSpec
	{
		std::vector<FramebufferTextureSpec> attachments;

		FramebufferAttachmentsSpec() = default;
		FramebufferAttachmentsSpec(
			std::initializer_list<FramebufferTextureSpec> attachmentList) : attachments(attachmentList) {}

		std::vector<FramebufferTextureSpec>::iterator begin() { return attachments.begin(); }
		std::vector<FramebufferTextureSpec>::iterator end() { return attachments.end(); }
		std::vector<FramebufferTextureSpec>::const_iterator begin() const { return attachments.begin(); }
		std::vector<FramebufferTextureSpec>::const_iterator end() const { return attachments.end(); }
	};

	struct FramebufferSpec
	{
		uint32_t width, height;
		uint32_t samples = 1;

		FramebufferAttachmentsSpec attachments;

		bool swapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer() = default;
		virtual ~Framebuffer() = default;

		virtual const FramebufferSpec& getSpec() const = 0;

		virtual uint32_t getColorAttachmentRendererId(uint32_t attachmentIndex = 0) const = 0;
		virtual uint32_t getRendererId() const = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual int readPixelInt(uint32_t attachmentIndex, uint32_t x, uint32_t y) = 0;
		virtual glm::vec4 readPixelVec4(uint32_t attachmentIndex, uint32_t x, uint32_t y) = 0;

		virtual void clearAttachment(uint32_t attachmentIndex, int clearValue) = 0;
		virtual void clearAttachment(uint32_t attachmentIndex, glm::vec4 clearValue) = 0;
		virtual void clearDepthAttachment() = 0;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void bindAttachmentTexture(uint32_t attachmentIndex, uint32_t slot) = 0;
		virtual void bindDepthAttachmentTexture(uint32_t slot) = 0;

		static std::shared_ptr<Framebuffer> create(const FramebufferSpec& spec);
	};

}

#endif // __FRAMEBUFFER_H__
