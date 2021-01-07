#include <nw_pch.hpp>
#include "nw_gl_core.h"

#include <sys/nw_mem_sys.h>

#pragma warning (disable: 4312)

#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
// Functions
namespace NW
{
	void OGL_ClearErr()
	{// glGetError gets last message and clears errorLog
		while (glGetError() != GL_NO_ERROR);
	}

	bool OGL_ErrLog(const char* funcName, const char* file, int line)
	{// Get error and match error message for it
		UInt32 errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case NW_INVALID_ENUM:
				error = "INVALID_ENUM";
				break;
			case NW_INVALID_VALUE:
				error = "INVALID_VALUE";
				break;
			case NW_INVALID_OPERATION:
				error = "INVALID_OPERATION";
				break;
			case NW_STACK_OVERFLOW:
				error = "STACK_OVERFLOW";
				break;
			case NW_STACK_UNDERFLOW:
				error = "STACK_UNDERFLOW";
				break;
			case NW_OUT_OF_MEMORY:
				error = "OUT_OF_MEMORY";
				break;
			}
			printf("NW_ERR_LOG::FILE_%s::LINE_%d:\n%s\n",
				funcName, line, error.c_str());
			return false;
		}
		return true;
	}

	Int32 OGL_ErrLog_Shader(ShaderTypes shaderType, UInt32 shaderID)
	{
		if (shaderID == 0) return NW_ERR_UNKNOWN_ID;
		Int32 success = 0;
		Int32 errLogSize = 0;
		String errLog;
		std::string strType = shaderType == NW_SHADER_VERTEX ?
			"SHADER_VERTEX" : shaderType == NW_SHADER_PIXEL ?
			"SHADER_PIXEL" : "SHADER_PROG";
		if (shaderType != ST_SHADER)
		{
			glGetShaderiv(shaderID, NW_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errLogSize);
				errLog.resize(errLogSize);

				glGetShaderInfoLog(shaderID, errLogSize, NULL, &errLog[0]);
				NW_ERR(errLog);
				return NW_ERR_SHADER_COMPILE;
			}
		}
		else
		{
			glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &errLogSize);
				errLog.resize(errLogSize);

				glGetProgramInfoLog(shaderID, errLogSize, NULL, &errLog[0]);
				NW_ERR(&errLog[0]);
				return NW_ERR_SHADER_LINK;
			}
		}
		return NW_OK;
	}
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
#endif	// NW_GRAPHICS

#include <sys/nw_mem_sys.h>
#include <sys/nw_data_sys.h>

#if (defined NW_GRAPHICS)
#include <core/nw_graph_engine.h>
// Buffers
namespace NW
{
	AVertexBuf* AVertexBuf::Create(Size szAlloc, void* pVtxData)
	{
		AVertexBuf* pVB = nullptr;
		switch (GraphEngine::GetGApi()->GetType()) {
		#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GApiTypes::GAPI_COUT: break;
		#endif	// NW_GRAPHICS
		#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: pVB = MemSys::NewT<VertexBufOgl>(); break;
		#endif	// NW_GRAPHICS
		default:
			NW_ERR("There is no accessible API");
			break;
		}
		pVB->SetData(szAlloc, pVtxData);
		return pVB;
	}
	AIndexBuf* AIndexBuf::Create(Size szAlloc, void* pIndData)
	{
		AIndexBuf* pIB = nullptr;
		switch (GraphEngine::GetGApi()->GetType()) {
		#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GApiTypes::GAPI_COUT: break;
		#endif // NW_GRAPHICS
		#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: pIB = MemSys::NewT<IndexBufOgl>();
			break;
		#endif // NW_GRAPHICS
		default:
			NW_ERR("Graphics API is not defined");
			break;
		}
		pIB->SetData(szAlloc, pIndData);
		return pIB;
	}
	AShaderBuf* AShaderBuf::Create(Size szAlloc, void* pIndData) {
		AShaderBuf* pSB = nullptr;
		switch (GraphEngine::GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
		case GApiTypes::GAPI_COUT: break;
	#endif // NW_GRAPHICS
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: pSB = MemSys::NewT<ShaderBufOgl>();
			break;
	#endif // NW_GRAPHICS
		default:
			NW_ERR("Graphics API is not defined");
			break;
		}
		pSB->SetData(szAlloc, pIndData);
		return pSB;
	}
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
namespace NW
{
	// VertexBufOglfer
	VertexBufOgl::VertexBufOgl() :
		m_unRIdVB(0), m_unRIdVA(0), m_szData(0)
	{
		glGenVertexArrays(1, &m_unRIdVA);
	}
	VertexBufOgl::~VertexBufOgl()
	{
		SetData(0, nullptr);
		glDeleteVertexArrays(1, &m_unRIdVA);
	}

	// --setters
	void VertexBufOgl::SetData(Size szData, void* pVtxData) {
		m_szData = szData;
		if (m_unRIdVB != 0) { glDeleteBuffers(1, &m_unRIdVB); m_unRIdVB = 0; }
		if (szData == 0) { return; }
		glGenBuffers(1, &m_unRIdVB);

		glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		glBufferData(GL_ARRAY_BUFFER, szData, pVtxData,
			pVtxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBufOgl::SetSubData(Size szData, void* pVtxData, Size szOffset) {
		glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		glBufferSubData(GL_ARRAY_BUFFER, szOffset, szData, pVtxData);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBufOgl::SetLayout(const BufferLayout& rBufLayout) {
		glBindVertexArray(m_unRIdVA); glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		for (UInt32 ati = 0; ati < m_BufLayout.GetElems().size(); ati++) { glDisableVertexAttribArray(ati); }
		m_BufLayout = rBufLayout;
		for (UInt32 ati = 0; ati < rBufLayout.GetElems().size(); ati++) {
			const BufferElement& rBufElem = m_BufLayout.GetElem(ati);
			glVertexAttribPointer(ati, rBufElem.unCount,
				rBufElem.sdType, rBufElem.bNormalized ? GL_TRUE : GL_FALSE,
				m_BufLayout.GetStride(), reinterpret_cast<void*>(rBufElem.unOffset));
			glEnableVertexAttribArray(ati);
		}
		glBindVertexArray(0); glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// --core_methods
	void VertexBufOgl::Bind() const { glBindVertexArray(m_unRIdVA); glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB); }
	void VertexBufOgl::Unbind() const { glBindVertexArray(0); glBindBuffer(GL_ARRAY_BUFFER, 0); }
}
// IndexBufferOgl
namespace NW
{
	IndexBufOgl::IndexBufOgl() :
		m_unRId(0), m_szData(0) { }
	IndexBufOgl::~IndexBufOgl() { SetData(0, nullptr); }

	// --setters
	void IndexBufOgl::SetData(Size szAlloc, void* pIndData) {
		m_szData = szAlloc;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szAlloc == 0) { return; }
		glGenBuffers(1, &m_unRId);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, szAlloc, pIndData,
			pIndData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void IndexBufOgl::SetSubData(Size szData, void* pIndData, Size szOffset)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, szOffset, szData, pIndData);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// --core_methods
	void IndexBufOgl::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId); }
	void IndexBufOgl::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
}
// ShaderBufferOgl
namespace NW
{
	ShaderBufOgl::ShaderBufOgl() :
		m_unRId(0), m_szData(0) { }
	ShaderBufOgl::~ShaderBufOgl() { SetData(0, nullptr); }

	// --setters
	void ShaderBufOgl::SetData(Size szAlloc, void* pIndData) {
		m_szData = szAlloc;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szAlloc == 0) { return; }
		glGenBuffers(1, &m_unRId);

		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBufferData(GL_UNIFORM_BUFFER, szAlloc, pIndData,
			pIndData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void ShaderBufOgl::SetSubData(Size szData, void* pIndData, Size szOffset)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBufferSubData(GL_UNIFORM_BUFFER, szOffset, szData, pIndData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	// --core_methods
	void ShaderBufOgl::Bind(UInt32 unPoint) const {
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBindBufferBase(GL_UNIFORM_BUFFER, unPoint, m_unRId);
	}
	void ShaderBufOgl::Bind(UInt32 unPoint, Size szData, Size szOffset) const {
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBindBufferRange(GL_UNIFORM_BUFFER, unPoint, m_unRId, szOffset, szData);
	}
	void ShaderBufOgl::Unbind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
// VertexBufCout
namespace NW
{
	VertexBufCout::VertexBufCout(UInt32 unSizeInBytes)
	{
		m_daVData.reserve(unSizeInBytes);
	}
	VertexBufCout::VertexBufCout(UInt32 unSizeInBytes, void* pVData)
	{
		m_daVData.reserve(unSizeInBytes);
		memcpy(&m_daVData[0], pVData, unSizeInBytes * sizeof(char));
	}
	VertexBufCout::~VertexBufCout()
	{
	}

	// Interface Methods
	void VertexBufCout::SetData(UInt unSizeInBytes, void* pVData, UInt unOffsetSize)
	{
		if (unSizeInBytes > GetSize())
		{
			NW_ERR("Too much bytes!");
			return;
		}
		for (UInt32 i = unOffsetSize; i < unSizeInBytes; i++)
			m_daVData.push_back(((char*)(pVData))[i]);
	}

	void VertexBufCout::Bind() const
	{
	}
	void VertexBufCout::Unbind() const
	{
	}
}
// IndexBufferCout
namespace NW
{
	IndexBufCout::IndexBufCout(UInt32 unCount)
	{
		m_unIData.reserve(unCount);
	}
	IndexBufCout::IndexBufCout(UInt32 unCount, UInt32* pIndices)
	{
		m_unIData.reserve(unCount);
		for (UInt32 i = 0; i < unCount; i++)
			m_unIData.push_back(pIndices[i]);
	}
	IndexBufCout::~IndexBufCout()
	{
	}

	// Interface Methods
	void IndexBufCout::SetData(UInt32 unCount, UInt32* pIndices, UInt32 unOffsetSize)
	{
		if (unCount > GetCount())
		{
			NW_ERR("Too much indices!");
			return;
		}
		for (UInt32 i = unOffsetSize; i < unCount; i++)
			m_unIData.push_back(pIndices[i]);
	}
	void IndexBufCout::Bind() const
	{
	}
	void IndexBufCout::Unbind() const
	{
	}
}
#endif // NW_GRAPHICS