#ifndef NW_ATEXTURE_H
#define NW_ATEXTURE_H

#include <gl/nw_gl_core.h>

#if (defined NW_GRAPHICS)
namespace NW
{
	/// ImageInfo struct
	struct NW_API ImageInfo
	{
	public:
		UByte* ClrData;
		Int32 nWidth = 1, nHeight = 1, nDepth = 1;
		Int32 nChannels = 1;
	};
	/// TextureInfo struct
	struct NW_API TextureInfo
	{
	public:
		TextureConfigs WrapTypeS = TC_WRAP_REPEAT, WrapTypeT = TC_WRAP_REPEAT, WrapTypeR = TC_WRAP_REPEAT;
		TextureConfigs FilterMin = TC_FILTER_NEAREST, FilterMag = TC_FILTER_NEAREST;
		TextureConfigs Format = TC_FORMAT_RGBA, InterFormat = TC_FORMAT_RGBA;
	};
	/// Abstract Texture class
	/// Description:
	/// -> Set props and data -> LoadData -> MakeTexture -> Bind drawing stuff
	/// -> Enable -> Draw -> Disable
	/// --It's a wrapping image which has to wrap a mesh
	class NW_API ATexture : public ADataRes
	{
	public:
		ATexture(const char* strName);
		virtual ~ATexture();

		// -- Getters
		inline UInt32 GetRenderId() const { return m_unRId; }
		inline UInt32 GetTexSlot() const { return m_unTexSlot; }
		inline const TextureInfo& GetTexInfo() const { return m_TexInfo; }
		inline const ImageInfo& GetImgInfo() const { return m_ImgInfo; }
		// -- Setters
		virtual void SetInfo(const TextureInfo& rTexInfo) = 0;
		virtual void SetInfo(const ImageInfo& rImgInfo) = 0;

		// -- Interface Methods
		virtual void Bind(UInt32 unTexSlot) = 0;
		virtual void Unbind() = 0;
		virtual void Remake() = 0;
		// -- Data Methods
		virtual bool SaveF(const char* strFPath) = 0;
		virtual bool LoadF(const char* strFPath) = 0;
	protected:
		String m_strName;
		UInt32 m_unRId;
		UInt32 m_unTexSlot;
		TextureInfo m_TexInfo;
		ImageInfo m_ImgInfo;
		static UByte s_ClearColorData[4];
	};
	/// Abstract Texture1d class
	class NW_API ATexture1d : public ATexture
	{
	public:
		ATexture1d(const char* strName);
		virtual ~ATexture1d();

		// -- Getters
		inline UInt32 GetWidth() const { return m_ImgInfo.nWidth; }
		// -- Setters
		virtual void SetInfo(const TextureInfo& rTexInfo) = 0;
		virtual void SetInfo(const ImageInfo& rImgInfo) = 0;

		// -- Interface Methods
		virtual void Bind(UInt32 unTexSlot) = 0;
		virtual void Unbind() = 0;
		virtual void Remake() = 0;

		// -- Data Methods
		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;

		static ATexture1d* Create(const char* strName);
	};
	/// Abstract Texture2d class
	class NW_API ATexture2d : public ATexture
	{
	public:
		ATexture2d(const char* strName);
		virtual ~ATexture2d();

		// -- Getters
		inline UInt32 GetWidth() const { return m_ImgInfo.nWidth; }
		inline UInt32 GetHeight() const { return m_ImgInfo.nHeight; }
		// -- Setters
		virtual void SetInfo(const TextureInfo& rTexInfo) = 0;
		virtual void SetInfo(const ImageInfo& rImgInfo) = 0;

		// -- Interface Methods
		virtual void Bind(UInt32 unTexSlot) = 0;
		virtual void Unbind() = 0;
		virtual void Remake() = 0;

		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;

		static ATexture2d* Create(const char* strName);
	};
	/// Abstract Texture3d class
	class NW_API ATexture3d : public ATexture
	{
	public:
		ATexture3d(const char* strName);
		virtual ~ATexture3d();

		// -- Getters
		inline UInt32 GetWidth() const { return m_ImgInfo.nWidth; }
		inline UInt32 GetHeight() const { return m_ImgInfo.nHeight; }
		inline UInt32 GetDepth() const { return m_ImgInfo.nDepth; }
		// -- Setters
		virtual void SetInfo(const TextureInfo& rTexInfo) = 0;
		virtual void SetInfo(const ImageInfo& rImgInfo) = 0;

		// -- Interface Methods
		virtual void Bind(UInt32 unTexSlot) = 0;
		virtual void Unbind() = 0;
		virtual void Remake() = 0;

		virtual bool SaveF(const char* strFPath) override;
		virtual bool LoadF(const char* strFPath) override;

		static ATexture3d* Create(const char* strName);
	};
}
namespace NW
{
	/// SubTexture1d struct
	struct NW_API SubTexture1d
	{
	public:
		Int32 nTexCrd = 0;
		Int32 nTexSize = 1;
		ATexture1d* pOverTex = nullptr;
	public:
		// -- Getters
		inline float GetTexCoord_0_1() const {
			return static_cast<float>(nTexCrd) / static_cast<float>(pOverTex->GetWidth());
		}
		inline float GetTexSize_0_1() const {
			return static_cast<float>(nTexSize) / static_cast<float>(pOverTex->GetWidth());
		}
	};
	/// SubTexture2d struct
	struct NW_API SubTexture2d
	{
	public:
		V2i xyTexCrd = { 0, 0 };
		V2i whTexSize = { 1, 1 };
		ATexture2d* pOverTex = nullptr;
	public:
		// -- Getters
		inline V2f GetTexCoord_0_1() const {
			return V2f{ static_cast<float>(xyTexCrd.x) / static_cast<float>(pOverTex->GetWidth()),
				static_cast<float>(xyTexCrd.y) / static_cast<float>(pOverTex->GetHeight()) };
		}
		inline V2f GetTexSize_0_1() const {
			return V2f{ static_cast<float>(whTexSize.x) / static_cast<float>(pOverTex->GetWidth()),
				static_cast<float>(whTexSize.y) / static_cast<float>(pOverTex->GetHeight()) };
		}
	};
	/// SubTexture3d struct
	struct NW_API SubTexture3d
	{
	public:
		V3i xyzTexCrd = { 0, 0, 0 };
		V3i whdTexSize = { 1, 1, 1};
		ATexture3d* pOverTex = nullptr;
	public:
		// -- Getters
		inline V3f GetTexCoord_0_1() const {
			return V3f{ static_cast<float>(xyzTexCrd.x) / static_cast<float>(pOverTex->GetWidth()),
				static_cast<float>(xyzTexCrd.y) / static_cast<float>(pOverTex->GetHeight()),
				static_cast<float>(xyzTexCrd.z) / static_cast<float>(pOverTex->GetDepth()) };
		}
		inline V3f GetTexSize_0_1() const {
			return V3f{ static_cast<float>(whdTexSize.x) / static_cast<float>(pOverTex->GetWidth()),
				static_cast<float>(whdTexSize.y) / static_cast<float>(pOverTex->GetHeight()),
				static_cast<float>(whdTexSize.z) / static_cast<float>(pOverTex->GetDepth()) };
		}
	};
}
#endif	// NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_OGL)
namespace NW
{
	/// Texture1dOgl Class
	class NW_API Texture1dOgl : public ATexture1d
	{
	public:
		Texture1dOgl(const char* strName);
		~Texture1dOgl();

		// -- Setters
		virtual void SetInfo(const TextureInfo& rTexImfo) override;
		virtual void SetInfo(const ImageInfo& rImgInfo) override;

		// -- Interface Methods
		virtual void Bind(UInt32 unTexSlot) override;
		virtual void Unbind() override;
		virtual void Remake() override;
	};
	/// Texture2dOgl Class
	class NW_API Texture2dOgl : public ATexture2d
	{
	public:
		Texture2dOgl(const char* strName);
		~Texture2dOgl();

		// -- Setters
		virtual void SetInfo(const TextureInfo& rTexImfo) override;
		virtual void SetInfo(const ImageInfo& rImgInfo) override;
		
		// -- Interface Methods
		virtual void Bind(UInt32 unTexSlot) override;
		virtual void Unbind() override;
		virtual void Remake() override;
	};
	/// Texture3dOgl Class
	class NW_API Texture3dOgl : public ATexture3d
	{
	public:
		Texture3dOgl(const char* strName);
		~Texture3dOgl();

		// -- Setters
		virtual void SetInfo(const TextureInfo& rTexImfo) override;
		virtual void SetInfo(const ImageInfo& rImgInfo) override;

		// -- Interface Methods
		virtual void Bind(UInt32 unTexSlot) override;
		virtual void Unbind() override;
		virtual void Remake() override;
	};
}
#endif // NW_GRAPHICS
#if (NW_GRAPHICS & NW_GRAPHICS_COUT)
// ATexture
namespace NW
{
	/// Texture2dOgl Class
	class NW_API Texture2dCout : public ATexture2d
	{
	public:
		Texture2dCout();
		~Texture2dCout();

		// -- Getters
		// -- Setters

		// -- Interface Methods
		virtual void Bind(UInt32 unTexSlot) const override;
		virtual void Unbind() const override;
		virtual bool MakeTexture(UInt32 bitMask = 0) override;
	private:
		UInt32 m_unRId;
		UInt32 m_unTexSlot;
	};
}

#endif // NW_GRAPHICS

#endif // NW_ATEXTURE_H