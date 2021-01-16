#ifndef IndexBuffer_h
#define IndexBuffer_h

#include "..\Renderer.h"

class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int *data, unsigned int count);
	~IndexBuffer();

	inline void Bind() const;
	inline void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};

#endif