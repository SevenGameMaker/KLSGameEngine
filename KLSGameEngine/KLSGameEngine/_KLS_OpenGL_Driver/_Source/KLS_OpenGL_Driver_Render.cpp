// indlcude the needed header files
#include <glad/gl.h>
#include "_KLS_OpenGL_Driver.h"
#include "KLS_Window.h"
#include "KLS_Mesh.h"
#include "KLS_Material.h"
#include "KLS_Pipeline.h"


// unclutter the global namespace
namespace KLS
{
	void KLS_OpenGL_Driver::beginFrame()
	{
		KLS_Driver::beginFrame();
	}

	void KLS_OpenGL_Driver::endFrame()
	{
		KLS_Driver::endFrame();
	}

	void KLS_OpenGL_Driver::setClearColor(glm::vec4 color)
	{
		KLS_Driver::setClearColor(color);
		glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	}

	void KLS_OpenGL_Driver::clear(KLS_CLEARFLAG flags)
	{
		KLS_Driver::clear(flags);
		int flag = 0;
		if (flags & KLSCF_COLOR) flag += GL_COLOR_BUFFER_BIT;
		if (flags & KLSCF_DEPTH) flag += GL_DEPTH_BUFFER_BIT;
		if (flags & KLSCF_STENCIL) flag += GL_STENCIL_BUFFER_BIT;
		glClear(flag);
	}

	void KLS_OpenGL_Driver::setDepth(bool value)
	{
		if (value)
		{
			glEnable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void KLS_OpenGL_Driver::setTransparency(bool value)
	{
		if (value)
		{
			glEnable(GL_BLEND);
		}
		else glDisable(GL_BLEND);
	}

	void KLS_OpenGL_Driver::setViewport(int width, int height)
	{
		KLS_Driver::setViewport(width, height);
		glViewport(0, 0, width, height);
	}

	void KLS_OpenGL_Driver::resetFBO(KLS_CLEARFLAG flags)
	{
		KLS_Driver::resetFBO(flags);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// draw calls 
	void KLS_OpenGL_Driver::drawMesh(KLS_Mesh* mesh, glm::mat4& modeltransform)
	{
		// if there is no mesh then bail
		if (!mesh) return;

		// run through the meshbuffers
		for (uint32_t x = 0; x < mesh->getMeshBufferCount(); x++)
		{
			// get a pointer to the meshbuffer
			KLS_MeshBuffer* mb = mesh->getMeshBuffer(x);

			// set the material to the current meshbuffer material
			setMaterial(mb->getMaterial());

			// pass in the shader uniforms
			if (m_Pipeline) m_Pipeline->setShaderParameters(m_Material, modeltransform);

			// bind the meshbuffer
			mb->bind();

			// check of we need wireframe mode
			glPolygonMode(GL_FRONT_AND_BACK, m_Material.Wireframe ? GL_LINE : GL_FILL);

			GLsizei indexcount = mb->getVAO()->getIndexBuffer()->getCount();

			// call the correct render command based on the material primitive type
			switch (m_Material.Primitive)
			{
				// regular triangles, call render VAO with indexbuffer
			case KLS_PT_TRIANGLE:
			{
				// Draw the current row using trianglea
				glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
			} break;

			// points, call render VAO with indexbuffer
			case KLS_PT_POINT:
			{
				// Draw the current row using trianglea
				glDrawElements(GL_POINTS, indexcount, GL_UNSIGNED_INT, 0);
			} break;

			// lines, call render VAO with indexbuffer
			case KLS_PT_LINES:
			{
				// Draw the current row using trianglea
				glDrawElements(GL_LINES, indexcount, GL_UNSIGNED_INT, 0);
			} break;

			// triangle strips 
			case KLS_PT_TRIANGLE_STRIP:
			{
			} break;

			// instanced 
			case KLS_PT_INSTANCED:
			{
				glDrawElementsInstanced(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0, m_Material.InstanceCount);
			} break;
			}
		}
	}

} // end namespace