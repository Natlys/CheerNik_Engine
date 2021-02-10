#ifndef NW_GFX_TOOLS_H
#define NW_GFX_TOOLS_H

#include <gfx_core.hpp>

#if (defined NW_GAPI)
// Functions
namespace NW
{
	inline Size SdTypeGetSize(ShaderDataTypes sDataType, UInt32 unCount = 1) {
		Size szData = 0;
		switch (sDataType) {
		case SDT_BOOL:	case SDT_INT8:		case SDT_UINT8:		szData = 1;	break;
		case SDT_INT16:	case SDT_UINT16:	case SDT_SAMPLER:	szData = 4;	break;
		case SDT_INT32:	case SDT_UINT32:	case SDT_FLOAT32:	szData = 4;	break;
		case SDT_FLOAT64:										szData = 8;	break;
		default:	NWL_ERR("Invalid shader data type");		szData = 0;	break;
		}
		return szData * unCount;
	}
	inline Size SdTypeGetAllignedSize(ShaderDataTypes sDataType, UInt32 unCount = 1) {
		Size szAll = 0;
		switch (sDataType) {
		case SDT_BOOL:	case SDT_INT8:	case SDT_UINT8:			szAll = 4;	break;
		case SDT_INT16:	case SDT_UINT16:						szAll = 4;	break;
		case SDT_INT32:	case SDT_UINT32:	case SDT_SAMPLER:	szAll = 4;	break;
		case SDT_FLOAT32:										szAll = 4;	break;
		case SDT_FLOAT64:										szAll = 8;	break;
		default:	NWL_ERR("Invalid shader data type");		szAll = 0;	break;
		}
		return szAll * ((unCount + (szAll - 1)) & ~(szAll - 1));
	}
	inline const char* SdTypeGetStr(ShaderDataTypes sdType) {
		return sdType == SDT_BOOL ? "boolean" :
			sdType == SDT_INT8 ? "byte" : sdType == SDT_UINT8 ? "unsigned byte" :
			sdType == SDT_INT16 ? "short" : sdType == SDT_UINT16 ? "unsigned short" :
			sdType == SDT_INT32 ? "integer" : sdType == SDT_UINT32 ? "unsigned integer" :
			sdType == SDT_FLOAT32 ? "float" : sdType == SDT_FLOAT64 ? "double" :
			"unknown";
	}
}
namespace NW
{
	/// DrawTools struct
	/// Description:
	/// -- Check and change renderer attributes for more or less performance/resource usage
	struct NW_API DrawTools {
	public:
		// --vertex_data
		UByte* pVtxData = nullptr;
		UByte* pVtxIter = nullptr;
		Size szVtxData = 0;
		// --index_data
		UInt32* pIdxIter = nullptr;
		UInt32* pIdxData = nullptr;
		Size szIdxData = 0;
		// --shader_data
		UByte* pShdData = nullptr;
		UByte* pShdIter = nullptr;
		Size szShdData = 0;
		// --texture_data
		Texture* pTextures[1];
		UInt8 unTexCount = 0;
		// --objects
		Shader* pShader = nullptr;
		GfxMaterial* pgMtl = nullptr;
		VertexBuf* pVtxBuf = nullptr;
		IndexBuf* pIdxBuf = nullptr;
		ShaderBuf* pShdBuf = nullptr;
	public:
		inline void ResetData() {
			pVtxIter = &pVtxData[0];
			szVtxData = 0;
			pIdxIter = &pIdxData[0];
			szIdxData = 0;
			pShdIter = &pShdData[0];
			szShdData = 0;
			for (UInt8 txi = 0; txi < unTexCount; txi++) { pTextures[txi] = nullptr; }
			unTexCount = 0;
		}
	};
}
#endif	// NW_GAPI
#if (NW_GAPI & NW_GAPI_OGL)
// --functions
namespace NW
{
	// Debug
	/// Clear GL error buffer
	extern void OglClearErr();
	/// Return suitable error message accordingly to glGetError()
	extern bool OglErrLog(const char* funcName, const char* file, int line);
	/// Get compile and linking status return true if there are errors
	extern int OglErrLogShader(ShaderTypes ShaderType, UInt32 unShaderId);
}
	#if (defined NW_DEBUG)
		#if (NW_GAPI & NW_GAPI_OGL)
			#define OGL_CALL(function) OglClearErr();\
				function;\
				NWL_ASSERT(OglErrLog(#function, NWL_FNAME_APATH((std::string)__FILE__), __LINE__), "GL_ERROR: ")
			#define NWL_OGL_SHADER_ERR_LOG(errType, objectID) (OglErrLogShader(errType, objectID))
			#define NWL_OGL_SHADER_ERR_LOG(errType, objectID) (OglErrLogShader(errType, objectID))
		#endif // NW_GAPI
	#else
		#define OGL_CALL(function);
#endif
#endif	// NW_GAPI
#endif	// NW_GFX_TOOLS_H