#include <nw_pch.hpp>
#include "nw_gcontext.h"

#if (defined NW_GRAPHICS)
#include <glib/gcontext/nw_framebuf.h>
#include <glib/nw_gtools.h>
#endif	// NW_WINDOW
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace NW
{
	static bool s_GLinit = false;

	GContextOgl::GContextOgl(GLFWwindow* pNativeWindow) :
		m_pNativeWindow(pNativeWindow) { }
	GContextOgl::~GContextOgl() { }

	// --==<core_methods>==--
	bool GContextOgl::OnInit()
	{
		glfwMakeContextCurrent(m_pNativeWindow);
		if (!s_GLinit) { s_GLinit = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); }
		m_Info = GContextInfo(glGetString(GL_RENDERER), glGetString(GL_VERSION),
			glGetString(GL_VENDOR), glGetString(GL_SHADING_LANGUAGE_VERSION));
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_Info.nMaxTextures);
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_Info.nMaxVertexAttribs);
		std::cout << std::endl << "Graphics Context: GLFW_" << glfwGetVersionString() << std::endl <<
			"OpenGL Renderer: " << m_Info.strRenderer << std::endl <<
			"OpenGL Version: " << m_Info.strVersion << std::endl <<
			"OpenGL Vendor: " << m_Info.strVendor << std::endl <<
			"OpenGL Shading Language: " << m_Info.strShadingLanguage << "\n\n";
		
		return true;
	}
	void GContextOgl::OnQuit()
	{
		if (glfwWindowShouldClose(m_pNativeWindow) == GLFW_TRUE) { glfwTerminate(); }
	}

	void GContextOgl::SwapBuffers()
	{
		glfwSwapBuffers(m_pNativeWindow);
	}
	// --==</core_methods>==--
}
#endif	// NW_GRAPHICS