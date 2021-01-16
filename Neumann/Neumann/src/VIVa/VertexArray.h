#ifndef VertexArray_h
#define VertexArray_h

#include "VertexBuffer.h"
#include "..\Renderer.h"
#include "VertexBufferLayout.h"

class VertexArray {
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
	inline void Bind() const;
	inline void UnBind() const;
};

#endif