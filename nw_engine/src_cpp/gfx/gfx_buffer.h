#ifndef NW_GFX_BUFFER_H
#define NW_GFX_BUFFER_H

#include <gfx/gfx_tools.h>

#if (defined NW_GAPI)
// VertexBufLayout
namespace NW
{
	/// BufferElement structure
	/// --Contains all relevant data for shader usage of vertex buffer data
	struct NW_API BufferElement
	{
	public:
		Char strName[128];
		ShaderDataTypes sdType;
		UInt32 unCount;
		Bit bNormalized;
		UInt32 unOffset;
	public:
		BufferElement() : strName(""), sdType(ShaderDataTypes::SDT_DEFAULT), unCount(0), bNormalized(false), unOffset(0) { }
		BufferElement(const char* sName, ShaderDataTypes dataType, UInt32 Count, bool Normalized) :
			strName(""), sdType(dataType), unCount(Count),
			bNormalized(Normalized), unOffset(0) {
			strcpy(strName, sName);
		}
	};
	struct NW_API ShaderBlock
	{
	public:
		Char strName[128];
		UInt8 unBindPoint;
		DArray<BufferElement> BufElems;
		Size szOffset;
		Size szAll;
	public:
		ShaderBlock() : strName(""), unBindPoint(0), szOffset(0), szAll(0) {};
		ShaderBlock(const char* sName, UInt8 BindPoint) :
			strName(""),
			unBindPoint(BindPoint), szAll(0), szOffset(0) {
			strcpy(strName, sName);
		}
	};
	/// VertexBufLayout class
	class NW_API VertexBufLayout
	{
	public:
		VertexBufLayout() : m_unStride(1) { }
		VertexBufLayout(const DArray<BufferElement>& rBufElems) :
			m_unStride(1) { SetElements(rBufElems); }

		// --getters
		inline const BufferElement& GetElem(UInt8 unIdx) const { return m_BufElems.at(unIdx); }
		inline const DArray<BufferElement>& GetElems() const { return m_BufElems; }
		inline const UInt32 GetStride() const { return m_unStride; }
		// --setters
		inline void SetElements(const DArray<BufferElement>& rBufElems) { m_BufElems = rBufElems; Update(); }
		inline void AddElement(const BufferElement& rBufElem, Int8 nElems = 1) { while (nElems-- > 0) { m_BufElems.push_back(rBufElem); } Update(); }
		inline void AddElement(const char* strName, ShaderDataTypes sdType, UInt8 unCount, Int8 nElems = 1) {
			AddElement(BufferElement(strName, sdType, unCount, false), nElems);
		}
		inline void Reset() { m_unStride = 0; m_BufElems.clear(); }
	private:
		UInt32 m_unStride;
		DArray<BufferElement> m_BufElems;
	private:
		inline void Update() {
			m_unStride = 0;
			for (auto& rBufElem : m_BufElems) {
				rBufElem.unOffset = m_unStride;
				m_unStride += SdTypeGetSize(rBufElem.sdType, rBufElem.unCount);
			}
		}
	};
	/// ShaderBufLayout class
	class NW_API ShaderBufLayout
	{
	public:
		ShaderBufLayout() : m_szData(0) { }
		ShaderBufLayout(const DArray<ShaderBlock>& rBlocks) :
			m_szData(0) {
			SetBlocks(rBlocks);
		}
		// --getters
		inline Size GetSize() const { return m_szData; }
		inline const DArray<BufferElement>& GetGlobals() const { return m_Globals; }
		inline const ShaderBlock& GetBlock(UInt8 unIdx) const { return m_Blocks.at(unIdx); }
		inline const DArray<ShaderBlock>& GetBlocks() const { return m_Blocks; }
		// --setters
		inline void AddGlobalElem(const BufferElement& rBufElem) { m_Globals.push_back(rBufElem); }
		inline void SetBlocks(const DArray<ShaderBlock>& rBlocks) { m_Blocks = rBlocks; Update(); }
		inline void AddBlock(const ShaderBlock& rBlock, Int8 nElems = 1) { while (nElems-- > 0) { m_Blocks.push_back(rBlock); } Update(); }
		inline void Reset() { m_szData = 0; m_Blocks.clear(); m_Globals.clear(); }
	private:
		DArray<BufferElement> m_Globals;
		DArray<ShaderBlock> m_Blocks;
		Size m_szData;
	private:
		inline void Update() {
			m_szData = 0;
			for (UInt8 bi = 0; bi < m_Blocks.size(); bi++) {
				auto& rBlock = m_Blocks[bi];
				rBlock.unBindPoint = bi;
				rBlock.szOffset = m_szData;
				for (auto& rElem : rBlock.BufElems) {
					rElem.unOffset += rBlock.szAll;
					rBlock.szAll += SdTypeGetAllignedSize(rElem.sdType, rElem.unCount);
				}
				m_szData += rBlock.szAll;
			}
		}
	};
}
#if (NW_GAPI & NW_GAPI_OGL)
// --graphics_pbjetcs
namespace NW
{
	/// GraphicsBuffer class
	/// Description:
	/// -- Specifies the basic interface for graphics buffers
	class NW_API AGfxBuffer
	{
	public:
		AGfxBuffer(GfxBufferTypes gbType);
		AGfxBuffer(const AGfxBuffer& rCpy) = delete;
		virtual ~AGfxBuffer();
		// --getters
		inline Size GetRenderId() const { return m_unRId; }
		inline Size GetDataSize() const { return m_szData; }
		// --setters
		void SetData(Size szData, const Ptr pVtxData = nullptr);
		void SetSubData(Size szData, const Ptr pVtxData, Size szOffset = 0);
		// --predicates
		inline Bit IsBound() const { return m_bIsBound; }
		// --operators
		inline void operator=(const AGfxBuffer& rCpy) = delete;
		inline void operator delete(Ptr pBlock) = delete;
		inline void operator delete[](Ptr pBlock) = delete;
		// --core_methods
		void Bind() const;
		void Unbind() const;
	protected:
		UInt32 m_unRId;
		const GfxBufferTypes m_gbType;
		mutable Bit m_bIsBound;
		Size m_szData;
	};
}
namespace NW
{
	/// VertexBuffer Class
	class NW_API VertexBuf : public AGfxBuffer
	{
	public:
		VertexBuf();
		virtual ~VertexBuf();
	};
	/// Abstract IndexBuffer Class
	class NW_API IndexBuf : public AGfxBuffer
	{
	public:
		IndexBuf();
		virtual ~IndexBuf();
	};
	/// Abstract ShaderBuffer class
	/// Description:
	/// -- Is used by shaders as opengl uniform setter, or as directx constant buffer
	class NW_API ShaderBuf : public AGfxBuffer
	{
	public:
		ShaderBuf();
		virtual ~ShaderBuf();
		// --core_methods
		void Bind() const;
		void Bind(UInt32 unPoint) const;
		void Bind(UInt32 unPoint, Size szData, Size szOffset = 0) const;
		void Remake(const ShaderBufLayout& rShdLayout);
	};
}
namespace NW
{
	/// VertexArray class
	class VertexArr
	{
	public:
		VertexArr();
		VertexArr(const VertexArr& rCpy) = delete;
		~VertexArr();
		// --getters
		inline DArray<RefKeeper<VertexBuf>>& GetVtxBuffers() { return m_vtxBufs; }
		inline VertexBuf* GetVtxBuffer(UInt32 unIdx = 0) { return m_vtxBufs[unIdx].GetRef(); }
		inline IndexBuf* GetIdxBuffer() { return m_idxBuf.GetRef(); }
		inline VertexBufLayout& GetLayout() { return m_vtxLayout; }
		inline GfxPrimitiveTypes GetDrawPrimitive() const { return m_gpType; }
		// --setters
		inline void AddVtxBuffer(RefKeeper<VertexBuf>& rvtxBuf) { m_vtxBufs.push_back(rvtxBuf); }
		inline void RmvVtxBuffer(UInt32 unIdx) { m_vtxBufs.erase(m_vtxBufs.begin() + unIdx); }
		inline void SetIdxBuffer(RefKeeper<IndexBuf>& ridxBuf) { m_idxBuf.SetRef(ridxBuf); }
		inline void SetDrawPrimitive(GfxPrimitiveTypes gpType) { m_gpType = gpType; }
		// --predicates
		inline Bit IsBound() { return m_bIsBound; }
		// --operators
		inline void operator=(const VertexArr& rCpy) = delete;
		inline void operator delete(Ptr pBlock) = delete;
		inline void operator delete[](Ptr pBlock) = delete;
		// --core_methods
		void Bind() const;
		void Unbind() const;
		void Remake(const VertexBufLayout& rvtxLayout);
		void CreateVtxBuffer();
		void CreateIdxBuffer();
	private:
		UInt32 m_unRId;
		mutable Bit m_bIsBound;
		DArray<RefKeeper<VertexBuf>> m_vtxBufs;
		RefKeeper<IndexBuf> m_idxBuf;
		VertexBufLayout m_vtxLayout;
		GfxPrimitiveTypes m_gpType;
	};
}
#endif
#endif	// NW_GAPI
#endif	// NW_GFX_BUFFER_H