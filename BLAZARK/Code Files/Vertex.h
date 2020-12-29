#pragma once

#include "glad/glad.h"

#include <vector>
#include <map>

class VertexBuffer {
public:
	template<typename dataType>
	VertexBuffer(const std::vector<dataType>& data, GLint typeLen) {
		glGenBuffers(1, &m_bufferID);

		m_typeLen = typeLen;

		UpdateBufferData(data);
	}

	VertexBuffer(const VertexBuffer&) = delete;

	~VertexBuffer() {
		glDeleteBuffers(1, &m_bufferID);
	}

	template<typename dataType>
	void UpdateBufferData(const std::vector<dataType>& data) {
		m_typeSize = sizeof(dataType);
		m_bufferLen = (GLsizei)data.size();

		glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
		glBufferData(GL_ARRAY_BUFFER, m_typeSize * m_bufferLen, &(data[0]), GL_STATIC_DRAW);
	}

	// Getters //
	GLuint getID() const {
		return m_bufferID;
	}

	GLint getTypeLen() const {
		return m_typeLen;
	}

	GLsizei getTypeSize() const {
		return m_typeSize;
	}

	GLsizei getBufferLen() const {
		return m_bufferLen;
	}

private:
	GLuint m_bufferID;

	GLint m_typeLen;

	GLsizei m_typeSize;
	GLsizei m_bufferLen;

	//bool m_dynamicVBO;
};

class VertexArray {
public:
	VertexArray() {
		glGenVertexArrays(1, &m_arrayID);
	}

	~VertexArray() {
		glDeleteVertexArrays(1, &m_arrayID);
	}

	void BindBuffer(const VertexBuffer& vbo, GLuint bufferAttrib) {
		m_vboDict[bufferAttrib] = &vbo;

		glBindVertexArray(m_arrayID);
		glEnableVertexAttribArray(bufferAttrib);
		glBindBuffer(GL_ARRAY_BUFFER, vbo.getID());
		glVertexAttribPointer(bufferAttrib, vbo.getTypeLen(), GL_FLOAT, GL_FALSE, 0, (const void*)0);
	}

	void DrawArray() {
		glBindVertexArray(m_arrayID);
		glDrawArrays(GL_TRIANGLES, 0, m_vboDict.begin()->second->getBufferLen());
	}

private:
	GLuint m_arrayID;

	std::map<GLint, const VertexBuffer*> m_vboDict;
};