#pragma once

#include "debug.h"
#include <GL/glew.h>
#include <iostream>
#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	VertexBufferElement(unsigned int t, unsigned int c, bool n):count(c),type(t),normalized(n) {}

	static unsigned int GetSizeofType(unsigned int type) {

		switch (type)
		{
			case GL_FLOAT:		   return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout 
{
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;

public:
	VertexBufferLayout()
		: m_stride(0) {}

	template<typename T>
	void Push(unsigned int count) {

		std::runtime_error(false);
	}	
	
	template<>
	void Push<float>(unsigned int count) {

		m_elements.push_back({ GL_FLOAT, count, false });
		m_stride += count * VertexBufferElement::GetSizeofType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count) {

		m_elements.push_back({ GL_UNSIGNED_INT, count, false });
		m_stride += count * VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count) {

		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }
};

// Forward declaration of Renderer class
class Renderer;
