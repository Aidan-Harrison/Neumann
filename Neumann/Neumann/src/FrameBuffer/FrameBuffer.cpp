#include "FrameBuffer.h"

FrameBuffer::FrameBuffer() { glGenFramebuffers(1, &m_RendererID); }

void FrameBuffer::CreateFrameBufferTexture() {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}

void FrameBuffer::CreateRenderbuffer() {
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

inline void FrameBuffer::Bind() const {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Frame buffer succeeded\n";
	}
	else {
		std::cerr << "Failed to bind frame buffer!\n";
	}
}

inline void FrameBuffer::UnBind() const { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }
FrameBuffer::~FrameBuffer() { GLCall(glDeleteFramebuffers(1, &m_RendererID)); }