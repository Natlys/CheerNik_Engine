#ifndef GL_CORE_H
#define GL_CORE_H
#include <gl_decl.hpp>

#include <lib/utils/math_vector.h>
#include <lib/utils/math_matrix.h>

#if (defined NW_GRAPHICS)
// Functions
namespace NW
{
	inline Size SDType_GetSize(ShaderDataTypes sDataType, UInt32 unCount = 1) {
		Size szData = 0;
		switch (sDataType) {
		case SDT_BOOL:	case SDT_INT8:		case SDT_UINT8:		szData = 1;	break;
		case SDT_INT16:	case SDT_UINT16:	case SDT_SAMPLER:	szData = 4;	break;
		case SDT_INT32:	case SDT_UINT32:	case SDT_FLOAT32:	szData = 4;	break;
		case SDT_FLOAT64:										szData = 8;	break;
		default:	NW_ERR("Invalid shader data type");			szData = 0;	break;
		}
		return szData * unCount;
	}
	inline Size SDType_GetAllignedSize(ShaderDataTypes sDataType, UInt32 unCount = 1) {
		Size szAll = 0;
		switch (sDataType) {
		case SDT_BOOL:	case SDT_INT8:	case SDT_UINT8:			szAll = 4;	break;
		case SDT_INT16:	case SDT_UINT16:						szAll = 4;	break;
		case SDT_INT32:	case SDT_UINT32:	case SDT_SAMPLER:	szAll = 4;	break;
		case SDT_FLOAT32:										szAll = 4;	break;
		case SDT_FLOAT64:										szAll = 8;	break;
		default:	NW_ERR("Invalid shader data type");			szAll = 0;	break;
		}
		return szAll * (( unCount + (szAll - 1)) & ~(szAll - 1) );
	}
}
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
		bool bNormalized;
		UInt32 unOffset;
	public:
		BufferElement() : strName(""), sdType(SDT_NONE), unCount(0), bNormalized(false), unOffset(0) { }
		BufferElement(const char* sName, ShaderDataTypes shaderDataType, UInt32 Count, bool Normalized) :
			strName(""), sdType(shaderDataType), unCount(Count),
			bNormalized(Normalized), unOffset(0) { strcpy(strName, sName); }
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
			unBindPoint(BindPoint), szAll(0), szOffset(0) { strcpy(strName, sName); }
	};
	/// VertexBufLayout class
	class NW_API VertexBufLayout
	{
	public:
		VertexBufLayout() : m_unStride(0) { }
		VertexBufLayout(const DArray<BufferElement>& rBufElems) :
			m_unStride(0) { SetElements(rBufElems); }

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
				m_unStride += SDType_GetSize(rBufElem.sdType, rBufElem.unCount);
			}
		}
	};
	/// ShaderBufLayout class
	class NW_API ShaderBufLayout
	{
	public:
		ShaderBufLayout() : m_szData(0) { }
		ShaderBufLayout(const DArray<ShaderBlock>& rBlocks) :
			m_szData(0) { SetBlocks(rBlocks); }
		// --getters
		inline Size GetSize() const { return m_szData; }
		inline const DArray<BufferElement>& GetGlobals() const { return m_Globals; }
		inline const ShaderBlock& GetBlock(UInt8 unIdx) const { return m_Blocks.at(unIdx); }
		inline const DArray<ShaderBlock>& GetBlocks() const { return m_Blocks; }
		// --setters
		inline void AddGlobalElem(const BufferElement& rBufElem) { m_Globals.push_back(rBufElem); }
		inline void SetBlocks(const DArray<ShaderBlock>& rBlocks) { m_Blocks = rBlocks; Update(); }
		inline void AddBlock(const ShaderBlock& rBlock, Int8 nElems = 1) { while (nElems-- > 0) { m_Blocks.push_back(rBlock); } Update(); }
		inline void Reset() { m_szData = 0; m_Blocks.clear(); }
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
					rBlock.szAll += SDType_GetAllignedSize(rElem.sdType, rElem.unCount);
				}
				m_szData += rBlock.szAll;
			}
		}
	};
}
// Vertex
namespace NW
{
	/// Vertex for 1D objects
	/// -- XYZ coordinates for vertices
	/// -- RGBA color for vertices
	/// -- X coordinate for texture sampling
	/// -- Index for the texture slot
	struct NW_API VertexBatch1d
	{
		V3f vtxCrd;
		V4f vtxClr;
		float texCrd;
		float nTexSlot;
	};
	/// Vertex for 2D objects
	/// -- XY coordinates for vertices
	/// -- RGBA color for vertices
	/// -- UV coordinates for texture coordinates
	/// -- Index for the texture slot
	struct NW_API VertexBatch2d
	{
		V2f vtxCrd;
		V4f vtxClr;
		V2f texCrd;
		float nTexSlot;
	};
	/// Vertex for 3D objects
	/// -- XYZ coordinates for vertices
	/// -- RGBA color for vertices
	/// -- XY coordinates for texture sampling
	/// -- Index for the texture slot
	struct NW_API VertexBatch3d
	{
		V3f vtxCrd;
		V4f vtxClr;
		V2f texCrd;
		float nTexSlot;
		Mat4f m4Transform;
	};
	/// Vertex for 3D world
	/// -- XYZ coordinates for vertex coordinates
	/// -- UV coordinates for texture coordinates
	/// -- XYZ coordinates for normals
	struct NW_API VertexShape3d
	{
		V3f vtxCrd;
		V2f texCrd;
		V3f normCrd;

		VertexShape3d() :
			vtxCrd{}, texCrd{}, normCrd{} {}
		VertexShape3d(const V3f& vCoord, const V2f& vUV, const V3f& vNorm) :
			vtxCrd(vCoord), texCrd(vUV), normCrd(vNorm) {}
	};
}
// Drawing data
namespace NW
{
	/// DrawSceneData struct
	struct NW_API DrawSceneData
	{
		Mat4f m4Proj;
		Mat4f m4View;
	public:
		DrawSceneData() : pData(&m4Proj[0]), szData(sizeof(Mat4f) * 2) {}
		inline const void* GetData() const { return pData; }
		inline Size GetDataSize() const { return szData; }
	private:
		void* pData;
		Size szData;
	};
	/// RenderAttributes struct
	/// Description:
	/// -- Check and change renderer attributes for more or less performance/resource usage
	struct NW_API DrawEngineInfo
	{
	public:
		// Configurations
		Size szMaxVtx = 0;
		Size szMaxIdx = 0;
		Size szMaxShd = 0;
		UInt32 unMaxTex = 0;
		// Counters
		Size szVtx = 0;
		Size unVtx = 0;
		Size szIdx = 0;
		Size unIdx = 0;
		UInt32 unTex = 0;
		// Drawing
		UInt16 unDrawCalls = 0;
	public:
		// --setters
		inline void Reset() {
			szVtx = szIdx = unTex = 0;
			unDrawCalls = 0;
		}
	};
}
// Pixel
namespace NW
{
	/// Pixel structure
	/// Description:
	/// -- Represents color in rgba for float and unsigned integer formats
	struct NW_API Pixel
	{
	public:
		explicit Pixel(float red = 0, float green = 0, float blue = 0, float alpha = 0) :
			ur(static_cast<UInt8>(red * 255.0f)),
			ug(static_cast<UInt8>(green * 255.0f)),
			ub(static_cast<UInt8>(blue * 255.0f)),
			ua(static_cast<UInt8>(alpha * 255.0f)) {}
		explicit Pixel(Int32 red, Int32 green, Int32 blue, Int32 alpha = 0) :
			ur(static_cast<UInt8>(red)),
			ug(static_cast<UInt8>(green)),
			ub(static_cast<UInt8>(blue)),
			ua(static_cast<UInt8>(alpha)) {}
		explicit Pixel(Int8 rgbaColor) :
			ur(static_cast<UInt8>(rgbaColor)),
			ug(static_cast<UInt8>(rgbaColor)),
			ub(static_cast<UInt8>(rgbaColor)),
			ua(static_cast<UInt8>(rgbaColor)) {};
		explicit Pixel(V4f rgbaColor)
		{
			ur = static_cast<UInt8>(rgbaColor.r * 255.0f);
			ug = static_cast<UInt8>(rgbaColor.g * 255.0f);
			ub = static_cast<UInt8>(rgbaColor.b * 255.0f);
			ua = static_cast<UInt8>(rgbaColor.a * 255.0f);
		}
		Pixel(UInt32 unRgba) { this->unRgba = unRgba; }

		union { UInt32 unRgba; struct { UInt8 ur, ug, ub, ua; }; struct { Int8 r, g, b, a; }; };
	public:
		// -- Convertion to float
		inline const float fR() const { return static_cast<float>(ur) / 255.0f; }
		inline const float fG() const { return static_cast<float>(ug) / 255.0f; }
		inline const float fB() const { return static_cast<float>(ub) / 255.0f; }
		inline const float fA() const { return static_cast<float>(ua) / 255.0f; }
		inline operator V4f() { return V4f{ fR(), fG(), fB(), fA() }; }
		// -- Operations with other pixels
		inline Pixel operator+(const Pixel& rPixel) {
			return Pixel(static_cast<Int8>(r + rPixel.r), static_cast<Int8>(g + rPixel.g),
				static_cast<Int8>(b + rPixel.b), static_cast<Int8>(a + rPixel.a));
		}
		inline Pixel operator-(const Pixel& rPixel) {
			return Pixel(static_cast<Int8>(r - rPixel.r), static_cast<Int8>(g - rPixel.g),
				static_cast<Int8>(b - rPixel.b), static_cast<Int8>(a - rPixel.a));
		}
		inline Pixel operator*(const Pixel& rPixel) {
			return Pixel(static_cast<Int8>(r * rPixel.r), static_cast<Int8>(g * rPixel.g),
				static_cast<Int8>(b * rPixel.b), static_cast<Int8>(a * rPixel.a));
		}
		inline Pixel operator/(const Pixel& rPixel) {
			return Pixel(static_cast<Int8>(r / rPixel.r), static_cast<Int8>(g / rPixel.g),
				static_cast<Int8>(b / rPixel.b), static_cast<Int8>(a / rPixel.a));
		}
		inline Pixel operator+=(const Pixel& rPixel) { r + rPixel.r; g + rPixel.g; b + rPixel.b; a + rPixel.a; return *this; }
		inline Pixel operator-=(const Pixel& rPixel) { r - rPixel.r; g - rPixel.g; b - rPixel.b; a - rPixel.a; return *this; }
		inline Pixel operator*=(const Pixel& rPixel) { r* rPixel.r; g* rPixel.g; b* rPixel.b; a* rPixel.a; return *this; }
		inline Pixel operator/=(const Pixel& rPixel) { r / rPixel.r; g / rPixel.g; b / rPixel.b; a / rPixel.a; return *this; }
		// -- Operations with float vector
		inline Pixel operator+(const V4f& rColor) {
			Pixel rPixel(rColor);
			return Pixel(r + rPixel.r, g + rPixel.g, b + rPixel.b, a + rPixel.a);
		}
		inline Pixel operator-(const V4f& rColor)
		{
			Pixel rPixel(rColor);
			return Pixel(r - rPixel.r, g - rPixel.g, b - rPixel.b, a - rPixel.a);
		}
		inline Pixel operator*(const V4f& rColor)
		{
			Pixel rPixel(rColor);
			return Pixel(r * rPixel.r, g * rPixel.g, b * rPixel.b, a * rPixel.a);
		}
		inline Pixel operator/(const V4f& rColor)
		{
			Pixel rPixel(rColor);
			return Pixel(r / rPixel.r, g / rPixel.g, b / rPixel.b, a / rPixel.a);
		}
		inline Pixel& operator+=(const V4f& rColor)
		{
			r += static_cast<Int8>(rColor.r) * 255.0f;
			g += static_cast<Int8>(rColor.g) * 255.0f;
			b += static_cast<Int8>(rColor.b) * 255.0f;
			a += static_cast<Int8>(rColor.a) * 255.0f;
			return *this;
		}
		inline Pixel& operator-=(const V4f& rColor)
		{
			r -= static_cast<Int8>(rColor.r) * 255.0f;
			g -= static_cast<Int8>(rColor.g) * 255.0f;
			b -= static_cast<Int8>(rColor.b) * 255.0f;
			a -= static_cast<Int8>(rColor.a) * 255.0f;
			return *this;
		}
		inline Pixel& operator*=(const V4f& rColor)
		{
			r *= static_cast<Int8>(rColor.r) * 255.0f;
			g *= static_cast<Int8>(rColor.g) * 255.0f;
			b *= static_cast<Int8>(rColor.b) * 255.0f;
			a *= static_cast<Int8>(rColor.a) * 255.0f;
			return *this;
		}
		inline Pixel& operator/=(const V4f& rColor)
		{
			r /= static_cast<Int8>(rColor.r) * 255.0f;
			g /= static_cast<Int8>(rColor.g) * 255.0f;
			b /= static_cast<Int8>(rColor.b) * 255.0f;
			a /= static_cast<Int8>(rColor.a) * 255.0f;
			return *this;
		}
	};
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
#define GL_DEBUG_ERR_LOG(errType, objectID) (OGL_ErrLog_Shader(errType, objectID))
// Functions
namespace NW
{
	// Debug
	/// Clear GL error buffer
	extern void OGL_ClearErr();
	/// Return suitable error message accordingly to glGetError()
	extern bool OGL_ErrLog(const char* funcName, const char* file, int line);
	/// Get compile and linking status return true if there are errors
	extern int OGL_ErrLog_Shader(ShaderTypes ShaderType, UInt32 unShaderId);
}

#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
#endif // NW_GRAPHICS

#if (defined NW_GRAPHICS)
// Graphics Objetcs
namespace NW
{
	/// Abstract VertexBuffer Class
	/// Interface:
	/// -> Create -> If created with data in OpenGL, load dynamically,
	/// else - load statically and don't change the buffer size;
	/// -> Load Data (if it isn't)
	/// -> Give it's pointer to VertexArray EITHER
	/// Bind -> Draw -> Unbind
	class NW_API AVertexBuf
	{
	public:
		virtual ~AVertexBuf() = default;

		// --getters
		virtual inline Size GetDataSize() const = 0;
		virtual inline const VertexBufLayout& GetLayout() = 0;
		// --setters
		virtual void SetData(Size szData, const void* pVtxData = nullptr) = 0;
		virtual void SetSubData(Size szData, const void* pVtxData, Size szOffset = 0) = 0;
		virtual void SetLayout(const VertexBufLayout& rBufLayout) = 0;
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static AVertexBuf* Create(Size szAlloc, const void* pVtxData = nullptr);
	};
	/// Abstract IndexBuffer Class
	/// Interface:
	/// -> Create -> If created with data in OpenGL, load dynamically,
	/// else - load statically and don't change the buffer size;
	/// -> Load Data (if it isn't)
	/// -> Give it's pointer to VertexArray EITHER
	/// Bind -> Bind VertexBuffer -> Draw -> Unbind both Buffers
	class NW_API AIndexBuf
	{
	public:
		virtual ~AIndexBuf() = default;

		// --getters
		virtual inline Size GetDataSize() const = 0;
		// --setters
		virtual void SetData(Size szAlloc, const void* pIdxData = nullptr) = 0;
		virtual void SetSubData(Size szData, const void* pIdxData, Size szOffset = 0) = 0;
		// --core_methods
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		static AIndexBuf* Create(Size szAlloc, const void* pIdxData = nullptr);
	};
	/// Abstract ShaderBuffer class
	/// Description:
	/// -- Is used by shaders as opengl uniform setter, or as directx constant buffer
	class NW_API AShaderBuf
	{
	public:
		virtual ~AShaderBuf() = default;

		// --getters
		virtual inline Size GetDataSize() const = 0;
		virtual inline const ShaderBufLayout& GetLayout() = 0;
		// --setters
		virtual void SetData(Size szData, const void* pData = nullptr) = 0;
		virtual void SetSubData(Size szData, const void* pData, Size szOffset = 0) = 0;
		virtual void SetLayout(const ShaderBufLayout& rShdLayout) = 0;
		// --core_methods
		virtual void Bind(UInt32 unPoint) const = 0;
		virtual void Bind(UInt32 unPoint, Size szData, Size szOffset = 0) const = 0;
		virtual void Unbind() const = 0;

		static AShaderBuf* Create(Size szAlloc, const void* pShaderData = nullptr);
	};
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
namespace NW
{
	/// VertexBufOgl
	class NW_API VertexBufOgl : public AVertexBuf
	{
	public:
		VertexBufOgl();
		~VertexBufOgl();

		// --setters
		virtual inline Size GetDataSize() const override { return m_szData; }
		virtual inline VertexBufLayout& GetLayout() override { return m_BufLayout; }
		// --setters
		virtual void SetData(Size szAlloc, const void* pVtxData = nullptr) override;
		virtual void SetSubData(Size szAlloc, const void* pVtxData, Size szOffset = 0) override;
		virtual void SetLayout(const VertexBufLayout& rBufLayout) override;
		
		// --core_methods
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		UInt32 m_unRIdVB;
		UInt32 m_unRIdVA;
		Size m_szData;

		VertexBufLayout m_BufLayout;
	};
	/// IndexBufOgl class
	class NW_API IndexBufOgl : public AIndexBuf
	{
	public:
		IndexBufOgl();
		~IndexBufOgl();

		// --getters
		virtual inline Size GetDataSize() const override { return m_szData; }
		// --setters

		// --core_methods
		virtual void SetData(Size szData, const void* pIdxData = nullptr) override;
		virtual void SetSubData(Size szData, const void* pIdxData, Size szOffset = 0) override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		UInt32 m_unRId;
		Size m_szData;
	};
	/// ShaderBufOgl class
	class NW_API ShaderBufOgl : public AShaderBuf
	{
	public:
		ShaderBufOgl();
		~ShaderBufOgl();

		// --getters
		virtual inline Size GetDataSize() const override { return m_szData; }
		virtual inline const ShaderBufLayout& GetLayout() override { return m_BufLayout; }
		// --setters
		virtual void SetData(Size szAlloc, const void* pVtxData = nullptr) override;
		virtual void SetSubData(Size szAlloc, const void* pVtxData, Size szOffset = 0) override;
		virtual void SetLayout(const ShaderBufLayout& rBufLayout) override;
		// --core_methods
		virtual void Bind(UInt32 unPoint) const override;
		virtual void Bind(UInt32 unPoint, Size szData, Size szOffset = 0) const override;
		virtual void Unbind() const override;
	private:
		UInt32 m_unRId;
		Size m_szData;

		ShaderBufLayout m_BufLayout;
	};
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
namespace NW
{
	/// VertexBufCout
	/// Description:
	/// -- Stores all the vertex data we need for drawing
	/// -- Takes raw pointer and size of the data and loads it into own binary "storage".
	/// -- This can be used by GraphicsApiCout or can be pointed by CoutVertexArray
	/// -- CoutVertexArray uses buffer layout to define what the buffer data actually is
	class NW_API VertexBufCout : public AVertexBuf
	{
	public:
		/// Without the given data it will be loaded dynamically
		VertexBufCout(UInt32 uiSizeInBytes);
		/// With the given data it will be loaded statically
		VertexBufCout(UInt32 uiSizeInBytes, void* vDataPtr);
		~VertexBufCout();

		// Getters
		virtual inline UInt32 GetSize() const override
		{
			return m_daVData.capacity() * sizeof(char);
		}
		virtual inline VertexBufLayout& GetLayout() override
		{
			return m_bufLayout;
		}
		inline const DArray<char>& GetVData() const
		{
			return m_daVData;
		}
		// Setters
		virtual inline void SetLayout(const VertexBufLayout& bLayout) override
		{
			m_bufLayout = bLayout;
		}
		// Interface methods
		virtual void SetData(UInt32 unSizeInBytes, void* pVData, UInt32 unOffsetSize = 0) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		VertexBufLayout m_bufLayout;
		DArray<char> m_daVData;
	};
	/// IndexBufCout class
	class NW_API IndexBufCout : public AIndexBuf
	{
	public:
		/// Without the given data it will be loaded dynamically
		IndexBufCout(UInt32 unCount);
		/// With the given data it will be loaded statically
		IndexBufCout(UInt32 unCount, UInt32* pIndices);
		~IndexBufCout();

		// Getters
		virtual inline UInt32 GetCount() const override { return m_unIData.capacity(); }
		inline const DArray<UInt32>& GetIndices() const { return m_unIData; }
		// Setters

		// Interface methods
		virtual void SetData(UInt32 unCount, UInt32* punIndices, UInt32 unOffsetSize = 0) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		DArray<UInt32> m_unIData;
	};
}
#endif // NW_GRAPHICS

#endif // GL_LIB_TYPES_H