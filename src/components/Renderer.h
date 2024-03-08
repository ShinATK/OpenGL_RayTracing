#pragma once

#include <glad/glad.h>

class Renderer 
{
private:
	GLuint m_FBO;
	GLuint m_Texture;
	const GLuint SHADOW_WIDTH;
	const GLuint SHADOW_HEIGHT;
public:
	Renderer(const GLuint width, const GLuint height) : SHADOW_WIDTH(width), SHADOW_HEIGHT(height)
	{
		glGenFramebuffers(1, &m_FBO);
		glGenTextures(1, &m_Texture);
	}
	~Renderer()
	{
		glDeleteFramebuffers(1, &m_FBO);
	}

	void InitShadowMap()
	{
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_FBO, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void UpdateShadowMap()
	{
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	
	void DrawShadowMap(const GLuint SCR_WIDTH, const GLuint SCR_HEIGHT)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

};


