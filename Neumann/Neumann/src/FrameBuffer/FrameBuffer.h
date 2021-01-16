#ifndef FrameBuffer_h
#define FrameBuffer_h

#include <GL/glew.h>
#include <iostream>
#include "..\Renderer.h"

using namespace GlobalRend;

class FrameBuffer {
private:
	unsigned int m_RendererID;
public:
	FrameBuffer();
	~FrameBuffer();

	void CreateFrameBufferTexture();
	void CreateRenderbuffer();
	inline void Bind() const;
	inline void UnBind() const;
};

#endif