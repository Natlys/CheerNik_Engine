#include <nw_pch.hpp>
#include "nw_gbuffer.h"

#pragma warning(disable : 4312)

#if (defined NW_GRAPHICS)
#include <core/nw_core_engine.h>
#include <glib/nw_gapi.h>
// Buffers
namespace NW
{
	// --==<AVertexBuf>==--
	AVertexBuf::AVertexBuf() :
		m_bIsBound(false), m_szData(0), m_bufLayout(VertexBufLayout()) {}
	// --core_methods
	AVertexBuf* AVertexBuf::Create(Size szAlloc, const Ptr pVtxData)
	{
		AVertexBuf* pVB = nullptr;
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: pVB = NewT<VertexBufOgl>(CoreEngine::Get().GetMemory()); break;
	#endif	// NW_GRAPHICS
		default: NWL_ERR("There is no accessible API"); break;
		}
		pVB->SetData(szAlloc, pVtxData);
		return pVB;
	}
	void AVertexBuf::Create(Size szAlloc, const Ptr pIdxData, RefOwner<AVertexBuf>& rVtxBuf)
	{
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: rVtxBuf.MakeRef<VertexBufOgl>(); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		rVtxBuf->SetData(szAlloc, pIdxData);
	}
	void AVertexBuf::Create(Size szAlloc, const Ptr pIdxData, RefKeeper<AVertexBuf>& rVtxBuf)
	{
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: rVtxBuf.MakeRef<VertexBufOgl>(); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		rVtxBuf->SetData(szAlloc, pIdxData);
	}
	// --==</AVertexBuf>==--
	// --==<AIndexBuf>==--
	AIndexBuf::AIndexBuf() :
		m_bIsBound(false), m_szData(0) {}
	// --core_methods
	AIndexBuf* AIndexBuf::Create(Size szAlloc, const Ptr pIdxData)
	{
		AIndexBuf* pIB = nullptr;
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: pIB = CoreEngine::Get().NewT<IndexBufOgl>(); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		pIB->SetData(szAlloc, pIdxData);
		return pIB;
	}
	void AIndexBuf::Create(Size szAlloc, const Ptr pIdxData, RefOwner<AIndexBuf>& rIdxBuf)
	{
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: rIdxBuf.MakeRef<IndexBufOgl>(); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		rIdxBuf->SetData(szAlloc, pIdxData);
	}
	void AIndexBuf::Create(Size szAlloc, const Ptr pIdxData, RefKeeper<AIndexBuf>& rIdxBuf)
	{
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: rIdxBuf.MakeRef<IndexBufOgl>(); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		rIdxBuf->SetData(szAlloc, pIdxData);
	}
	// --==</AIndexBuf>==--
	// --==<AShaderBuf>==--
	AShaderBuf::AShaderBuf() :
		m_bIsBound(false), m_szData(0), m_bufLayout(ShaderBufLayout()) {}
	// --core_methods
	AShaderBuf* AShaderBuf::Create(Size szAlloc, const Ptr pIdxData) {
		AShaderBuf* pSB = nullptr;
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: pSB = NewT<ShaderBufOgl>(CoreEngine::Get().GetMemory()); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		pSB->SetData(szAlloc, pIdxData);
		return pSB;
	}
	void AShaderBuf::Create(Size szAlloc, const Ptr pIdxData, RefOwner<AShaderBuf>& rShdBuf)
	{
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: rShdBuf.MakeRef<ShaderBufOgl>(); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		rShdBuf->SetData(szAlloc, pIdxData);
	}
	void AShaderBuf::Create(Size szAlloc, const Ptr pIdxData, RefKeeper<AShaderBuf>& rShdBuf)
	{
		switch (CoreEngine::Get().GetGApi()->GetType()) {
	#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
		case GApiTypes::GAPI_OPENGL: rShdBuf.MakeRef<ShaderBufOgl>(); break;
	#endif // NW_GRAPHICS
		default: NWL_ERR("Graphics API is not defined"); break;
		}
		rShdBuf->SetData(szAlloc, pIdxData);
	}
	// --==</AShaderBuf>==--
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#include <glad/glad.h>
// VertexBufferOgl
namespace NW
	// VertexBufferOgl
{
	VertexBufOgl::VertexBufOgl() :
		AVertexBuf(),
		m_unRIdVB(0), m_unRIdVA(0)
	{
		glGenVertexArrays(1, &m_unRIdVA);
	}
	VertexBufOgl::~VertexBufOgl()
	{
		SetData(0, nullptr);
		glDeleteVertexArrays(1, &m_unRIdVA);
	}

	// --setters
	void VertexBufOgl::SetData(Size szData, const Ptr pVtxData) {
		m_szData = szData;
		if (m_unRIdVB != 0) { glDeleteBuffers(1, &m_unRIdVB); m_unRIdVB = 0; }
		if (szData == 0) { return; }
		glGenBuffers(1, &m_unRIdVB);

		glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		glBufferData(GL_ARRAY_BUFFER, szData, pVtxData,
			pVtxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBufOgl::SetSubData(Size szData, const Ptr pVtxData, Size szOffset) {
		glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		glBufferSubData(GL_ARRAY_BUFFER, szOffset, szData, pVtxData);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBufOgl::SetLayout(const VertexBufLayout& rBufLayout) {
		glBindVertexArray(m_unRIdVA); glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		for (UInt32 ati = 0; ati < m_bufLayout.GetElems().size(); ati++) { glDisableVertexAttribArray(ati); }
		m_bufLayout = rBufLayout;
		for (UInt32 ati = 0; ati < rBufLayout.GetElems().size(); ati++) {
			const BufferElement& rBufElem = m_bufLayout.GetElem(ati);
			glVertexAttribPointer(ati, rBufElem.unCount,
				rBufElem.sdType, rBufElem.bNormalized ? GL_TRUE : GL_FALSE,
				m_bufLayout.GetStride(), reinterpret_cast<void*>(rBufElem.unOffset));
			glEnableVertexAttribArray(ati);
		}
		glBindVertexArray(0); glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// --core_methods
	void VertexBufOgl::Bind() const {
		if (m_bIsBound) { return; }
		glBindVertexArray(m_unRIdVA); glBindBuffer(GL_ARRAY_BUFFER, m_unRIdVB);
		m_bIsBound = true;
	}
	void VertexBufOgl::Unbind() const {
		if (!m_bIsBound) { return; }
		glBindVertexArray(0); glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_bIsBound = false;
	}
}
// IndexBufferOgl
namespace NW
{
	IndexBufOgl::IndexBufOgl() :
		AIndexBuf(),
		m_unRId(0) { }
	IndexBufOgl::~IndexBufOgl() { SetData(0, nullptr); }

	// --setters
	void IndexBufOgl::SetData(Size szAlloc, const Ptr pIdxData) {
		m_szData = szAlloc;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szAlloc == 0) { return; }
		glGenBuffers(1, &m_unRId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, szAlloc, pIdxData,
			pIdxData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void IndexBufOgl::SetSubData(Size szData, const Ptr pIdxData, Size szOffset)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, szOffset, szData, pIdxData);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// --core_methods
	void IndexBufOgl::Bind() const {
		if (m_bIsBound) { return; }
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_unRId);
		m_bIsBound = true;
	}
	void IndexBufOgl::Unbind() const {
		if (!m_bIsBound) { return; }
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_bIsBound = false;
	}
}
// ShaderBufferOgl
namespace NW
{
	ShaderBufOgl::ShaderBufOgl() :
		AShaderBuf(),
		m_unRId(0) { }
	ShaderBufOgl::~ShaderBufOgl() { SetData(0, nullptr); }

	// --setters
	void ShaderBufOgl::SetData(Size szAlloc, const Ptr pShdData) {
		m_szData = szAlloc;
		if (m_unRId != 0) { glDeleteBuffers(1, &m_unRId); m_unRId = 0; }
		if (szAlloc == 0) { return; }
		glGenBuffers(1, &m_unRId);

		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBufferData(GL_UNIFORM_BUFFER, szAlloc, pShdData,
			pShdData == nullptr ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	void ShaderBufOgl::SetSubData(Size szData, const Ptr pShdData, Size szOffset) {
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBufferSubData(GL_UNIFORM_BUFFER, szOffset, szData, pShdData);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void ShaderBufOgl::SetLayout(const ShaderBufLayout& rBufLayout) {
		m_bufLayout = rBufLayout;
		if (m_szData < rBufLayout.GetSize()) { SetData(rBufLayout.GetSize()); }
		for (auto& rBlock : rBufLayout.GetBlocks()) { Bind(rBlock.unBindPoint, rBlock.szAll, rBlock.szOffset); }
	}

	// --core_methods
	void ShaderBufOgl::Bind() const
	{
		if (m_bIsBound) { return; }
		for (auto& rBlock : m_bufLayout.GetBlocks()) { Bind(rBlock.unBindPoint, rBlock.szAll, rBlock.szOffset); }
		m_bIsBound = true;
	}
	void ShaderBufOgl::Bind(UInt32 unPoint) const {
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBindBufferBase(GL_UNIFORM_BUFFER, unPoint, m_unRId);
	}
	void ShaderBufOgl::Bind(UInt32 unPoint, Size szData, Size szOffset) const {
		glBindBuffer(GL_UNIFORM_BUFFER, m_unRId);
		glBindBufferRange(GL_UNIFORM_BUFFER, unPoint, m_unRId, szOffset, szData);
	}
	void ShaderBufOgl::Unbind() const {
		if (!m_bIsBound) { return; }
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		m_bIsBound = false;
	}
}
#endif	// NW_GRAPHICS