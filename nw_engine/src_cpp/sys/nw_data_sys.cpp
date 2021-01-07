#include <nw_pch.hpp>
#include "nw_data_sys.h"

#include <core/nw_core_state.h>
#include <ecs/nw_scene.h>
#include <lua/nw_lua_vm.h>

#include <gl/gcontext/nw_window.h>
#include <gl/vision/nw_gmaterial.h>

#include <sys/nw_ev_sys.h>
#include <sys/nw_mem_sys.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#if (defined NW_PLATFORM_WINDOWS)
#include <commdlg.h>
#if (NW_WINDOW & NW_WINDOW_GLFW)
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif  // NW_WINDOW
#endif  // NW_PLATFORM

NW::DataSys::ADRs NW::DataSys::s_ADRs;
NW::String NW::DataSys::s_strRscDir = "D:\\dev\\native_world\\bin\\resources";
std::stringstream NW::DataSys::s_strStream;
std::fstream NW::DataSys::s_fStream;

namespace NW
{
    // --getters
    template <> static inline DataSys::DRs<Scene>& DataSys::GetDataResources() = delete;
    template <> Scene* DataSys::GetDataRes<Scene>(const char* strName) = delete;

    template <> Scene* DataSys::GetDataRes<Scene>(UInt32 unId) = delete;
    template <> AShader* DataSys::GetDataRes<AShader>(UInt32 unId) {
        if (AShader* pShader = dynamic_cast<AShader*>(GetADataRes(unId))) {
            return pShader;
        }
        return nullptr;
    }
    // --setters

    void DataSys::AddADataRes(ADataRes* pDataRes) {
        if (pDataRes == nullptr) return;
        s_ADRs[pDataRes->GetId()] = (pDataRes);
    }
    void DataSys::RemoveADataRes(UInt32 unId) {
        ADRs::iterator itDR = s_ADRs.find(unId);
        if (s_ADRs.size() == 0) return;
        if (itDR == s_ADRs.end()) return;
        s_ADRs.erase(itDR);
    }

    // --==<core_methods>==--=
    bool DataSys::OnInit()
    {
        s_fStream.exceptions(std::ios::badbit | std::ios::failbit);

        if (true) {
            AShader::Create("shd_batch_3d");
            GetDataRes<AShader>("shd_batch_3d")->LoadF("D:/dev/native_world/nw_engine/src_glsl/batch_3d.glsl");
        }
        if (true) {
            ATexture2d::Create("tex_white_solid");
            GetDataRes<ATexture2d>("tex_white_solid")->LoadF("");
            ATexture2d::Create("tex_white_frame");
            GetDataRes<ATexture2d>("tex_white_frame")->LoadF("D:/dev/native_world/bin/resources/graphics/images/tex_white_frame.png");
            ATexture2d::Create("tex_white_circle");
            GetDataRes<ATexture2d>("tex_white_circle")->LoadF("D:/dev/native_world/bin/resources/graphics/images/tex_white_circle.png");
            ATexture2d::Create("fnt_cheer0");
            GetDataRes<ATexture2d>("fnt_cheer0")->LoadF("D:/dev/native_world/bin/resources/graphics/fonts/fnt_cheer0.png");
            ATexture2d::Create("spt_ground0");
            GetDataRes<ATexture2d>("spt_ground0")->LoadF("D:/dev/native_world/bin/resources/graphics/images/spt_iso_ground0_0.png");
        }
        if (true) {
            GMaterial* pGMtl = MemSys::NewT<GMaterial>("gmt_batch_3d");
        }
        return true;
    }

    void DataSys::OnQuit()
    {
        if (true) {
            MemSys::DelT<AShader>(GetDataRes<AShader>("shd_batch_3d"));
        }
        if (true) {
            MemSys::DelT<ATexture2d>(GetDataRes<ATexture2d>("tex_white_solid"));
            MemSys::DelT<ATexture2d>(GetDataRes<ATexture2d>("tex_white_frame"));
            MemSys::DelT<ATexture2d>(GetDataRes<ATexture2d>("tex_white_circle"));
            MemSys::DelT<ATexture2d>(GetDataRes<ATexture2d>("fnt_cheer0"));
            MemSys::DelT<ATexture2d>(GetDataRes<ATexture2d>("spt_ground0"));
        }
        if (true) {
            MemSys::DelT<GMaterial>(GetDataRes<GMaterial>("gmt_batch_3d"));
        }
    }
    // -- File Dialogs
    String DataSys::FDialog_save(const char* strFilter)
    {
    #if (defined NW_PLATFORM_WINDOWS)
        constexpr Int32 nMaxChars = 256;
        char strRes[nMaxChars]{ 0 };
        OPENFILENAMEA ofn{ 0 };
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)(EvSys::s_pAppWindow->GetNativeWindow()));
        ofn.lpstrFile = &strRes[0];
        ofn.nMaxFile = sizeof(strRes);
        ofn.lpstrFilter = strFilter;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetSaveFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        } else { return String(); }
    #endif  // NW_PLATFORM
    }
    String DataSys::FDialog_load(const char* strFilter)
    {
    #if (defined NW_PLATFORM_WINDOWS)
        constexpr Int32 nMaxChars = 256;
        char strRes[nMaxChars] { 0 };
        OPENFILENAMEA ofn{ 0 };
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)(EvSys::s_pAppWindow->GetNativeWindow()));
        ofn.lpstrFile = &strRes[0];
        ofn.nMaxFile = nMaxChars;
        ofn.lpstrFilter = strFilter;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
        if (GetOpenFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        } else { return ""; }
    #endif  // NW_PLATFORM
    }

    // -- BinaryData
    bool DataSys::SaveF_data(const char *filePath,
        void* pData, UInt64 unBytes)
    {
        try {
            s_fStream.open(filePath, std::ios::out | std::ios::binary);
            s_fStream.write(static_cast<char*>(pData), unBytes);
            s_fStream.close();
            return true;
        } catch (std::ios_base::failure ex) {
            return false;
        }
    }
    bool DataSys::SaveF_data(const char* directory, const char* name, const char* format,
        void* pData, UInt64 unBytes)
    {
        try {
            s_strStream << directory << name << "." << format;
            s_fStream.open(s_strStream.str().c_str(), std::ios::out | std::ios::binary);
            s_fStream.write(static_cast<char*>(pData), unBytes);
            s_fStream.close();
            s_strStream = std::stringstream();
            return true;
        } catch (std::ios_base::failure ex) {
            s_strStream = std::stringstream();
            return false;
        }
    }
    bool DataSys::LoadF_data(const char* filePath,
        void* pData, UInt64 unBytes)
    {
        try {
            s_fStream.open(filePath, std::ios::in | std::ios::binary);
            s_fStream.read(static_cast<char*>(pData), unBytes);
            s_fStream.close();
            return true;
        } catch (std::ios_base::failure ex)
        {
            return false;
        }
    }
    bool DataSys::LoadF_data(const char* directory, const char* name, const char* format,
        void* pData, UInt64 unBytes)
    {
        try {
            s_strStream << directory << name << "." << format;
            s_fStream.open(s_strStream.str(), std::ios::in | std::ios::binary);
            s_fStream.read(static_cast<char*>(pData), unBytes);
            s_fStream.close();
            s_strStream = std::stringstream();
            return true;
        } catch (std::ios_base::failure ex) {
            s_strStream = std::stringstream();
            return false;
        }
    }

    // --Strings
    bool DataSys::SaveF_string(const char *strFPath, const char* strSrc, UInt64 unBytes)
    {
        try {
            s_fStream.open(strFPath, std::ios::out, std::ios::binary);
            s_fStream.write(&strSrc[0], unBytes);
            s_fStream.close();
            s_strStream = std::stringstream();
            return true;
        } catch (std::ios_base::failure ex) {
            s_strStream = std::stringstream();
            return false;
        }
    }
    bool DataSys::LoadF_string(const char* strFPath, String& strDest)
    {
        try {
            s_fStream.open(strFPath, std::ios::in, std::ios::binary);
            s_fStream.seekg(0, std::ios::end);
            strDest.resize(s_fStream.tellg());
            s_fStream.seekg(0, std::ios::beg);
            s_strStream << s_fStream.rdbuf();
            strcpy(&strDest[0], &s_strStream.str()[0]);
            s_fStream.close();
            s_strStream = std::stringstream();
            return true;
        } catch (std::ios_base::failure ex) {
            s_strStream = std::stringstream();
            return false;
        }
    }

    // -- Objects
    UByte* DataSys::LoadF_image(const char* strFPath,
        Int32* pnW, Int32* pnH, Int32* pnChannels)
    {
        return stbi_load(strFPath, pnW, pnH, pnChannels, 0);
    }
    bool DataSys::LoadF_image(const char* strFPath,
        UByte* pClrDataBuf, Int32* pnW, Int32* pnH, Int32* pnChannels)
    {
        if (pClrDataBuf != nullptr) return false;
        pClrDataBuf = stbi_load(strFPath, pnW, pnH, pnChannels, 0);
        if (pClrDataBuf == nullptr) return false;
        return true;
    }
    bool DataSys::LoadF_image(const char* strFPath, ImageInfo* pImage)
    {
        if (pImage == nullptr) return false;

        pImage->ClrData = LoadF_image(strFPath, &pImage->nWidth, &pImage->nHeight, &pImage->nChannels);
        if (pImage->ClrData == nullptr) return false;

        return true;
    }

    // -- Mesh data
    bool DataSys::LoadF_mesh(const String& strFilePath, DArray<VertexShape3d>* pVtxData, DArray<UInt32>* punIndData)
    {
        UInt16 dotPos = strFilePath.rfind('.') + 1;
        String format = strFilePath.substr(dotPos, strFilePath.size() - dotPos);
        try {
            s_fStream.open(strFilePath, std::ios::in, std::ios::binary);
            s_strStream << s_fStream.rdbuf();
            if (format == "obj") { LoadF_mesh_obj(s_strStream.str(), *pVtxData, *punIndData); }
            else if (format == "dae") {}
            else { throw std::exception("Unknown format"); }
            s_fStream.close();
            s_fStream.clear();
            s_strStream = std::stringstream();
            return true;
        } catch (std::exception ex) {
            NW_ERR("Failed to load a file by path " + strFilePath + "\n" + ex.what());
            s_fStream.clear();
            s_fStream.close();
            s_strStream = std::stringstream();
            return false;
        }
    }
    // --==</core_methods>==--=

    // --==<Implementation Methods>==--=
    bool DataSys::LoadF_mesh_obj(const String& strFileData, DArray<UInt32>& arrIndicesDest,
        DArray<float>& vtxCoordsDest, DArray<float>& texCoordsDest, DArray<float>& normCoordsDest)
    {
        String
            nameToken = "g ",
            vtxToken = "v ",
            uvToken = "vt ",
            normToken = "vn ",
            elemsToken = "f ",
            mtlSrcToken = "mtllib ",
            mtlUseToken = "usemtl ";
        String
            nameFormat = nameToken + "%s",
            vtxFormat = vtxToken + "%f %f %f",
            uvFormat = uvToken + "%f %f",
            normFormat = normToken + "%f %f %f",
            elemsFormat = elemsToken + "%d/%d/%d %d/%d/%d %d/%d/%d";
        // Count |objects_amount|, |vertices|, |uvs|, |normals| and |indices|
        std::unordered_map<const char*, UInt32> name_counter
        { std::make_pair("nameCount", 0),
            std::make_pair("vtxCount", 0),
            std::make_pair("uvCount", 0),
            std::make_pair("normCount", 0),
            std::make_pair("indexCount", 0)
        };
        float fTemp[3]{ 0.0f, 0.0f, 0.0f };
        UInt32 uiTemp[3]{ 0, 0, 0 };
        
        Int32 currPos = 0, nextPos = 0;
        Int32 currMesh = 0, nextMesh = 0;
        currMesh = strFileData.find(nameToken, currPos);
        nextMesh = strFileData.find(nameToken, currPos + nameToken.size());
        while (currMesh != -1)   // Find the line position where the mesh name is
        {
            currPos = nextPos = 0;
            String strData = strFileData.substr(currMesh, nextMesh - currMesh);
            // mesh name is found
            char cMeshName[128];
            name_counter["nameCount"]++;
            // Copy the mesh name : |currPos|_meshName|lineEnd|
            sscanf(&strFileData.c_str()[currMesh], nameFormat.c_str(), cMeshName);
            printf("NW::LOAD_MASTER::LOAD_MESH_DATA_OBJ: loading the \"%s\" mesh\n", cMeshName);
            
            do { nextPos = strData.find(vtxToken, currPos);  // Find from the end of the last line
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], vtxFormat.c_str(), &fTemp[0], &fTemp[1], &fTemp[2]))
                {
                    vtxCoordsDest.push_back(fTemp[0]);
                    vtxCoordsDest.push_back(fTemp[1]);
                    vtxCoordsDest.push_back(fTemp[2]);
                    name_counter["vtxCount"]++;
                    currPos = nextPos + vtxToken.size();    // Curr = end of token; next = beginning
                }
            } while (true);
            
            do { nextPos = strData.find(uvToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], uvFormat.c_str(), &fTemp[0], &fTemp[1]))
                {
                    texCoordsDest.push_back(fTemp[0]);
                    texCoordsDest.push_back(fTemp[1]);
                    name_counter["uvCount"]++;
                    currPos = nextPos + uvToken.size();
                }
            } while (true);
            
            do { nextPos = strData.find(normToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], normFormat.c_str(), &fTemp[0], &fTemp[1], &fTemp[2]))
                {
                    normCoordsDest.push_back(fTemp[0]);
                    normCoordsDest.push_back(fTemp[1]);
                    normCoordsDest.push_back(fTemp[2]);
                    name_counter["normCount"]++;
                    currPos = nextPos + normToken.size();
                }
            } while (true);
            
            do { nextPos = strData.find(elemsToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], elemsFormat.c_str(),
                    &uiTemp[0], &uiTemp[0], &uiTemp[0],
                    &uiTemp[1], &uiTemp[1], &uiTemp[1],
                    &uiTemp[2], &uiTemp[2], &uiTemp[2]))
                {
                    arrIndicesDest.push_back(uiTemp[0]);
                    arrIndicesDest.push_back(uiTemp[1]);
                    arrIndicesDest.push_back(uiTemp[2]);
                    name_counter["indexCount"]+=3;
                    currPos = nextPos + elemsToken.size();
                }
            } while (true);
            if (nextMesh == -1) break;
             (currMesh = (strFileData.find(nameToken, nextMesh)));
             nextMesh = strFileData.find(nameToken, currMesh + nameToken.size());
        }
        return true;
    }
    bool DataSys::LoadF_mesh_obj(const String& strFileData, DArray<VertexShape3d>& rVtxData, DArray<UInt32>& runIndData)
    {
        String
            nameToken = "g ",
            vtxToken = "v ",
            uvToken = "vt ",
            normToken = "vn ",
            elemsToken = "f ",
            mtlSrcToken = "mtllib ",
            mtlUseToken = "usemtl ";
        String
            nameFormat = nameToken + "%s",
            vtxFormat = vtxToken + "%f %f %f",
            uvFormat = uvToken + "%f %f",
            normFormat = normToken + "%f %f %f",
            elemsFormat = elemsToken + "%d/%d/%d %d/%d/%d %d/%d/%d";
        // Count |objects_amount|, |vertices|, |uvs|, |normals| and |indices|
        std::unordered_map<const char*, UInt32> name_counter
        { std::make_pair("nameCount", 0),
            std::make_pair("vtxCount", 0),
            std::make_pair("uvCount", 0),
            std::make_pair("normCount", 0),
            std::make_pair("indexCount", 0)
        };
        float fTemp[3]{ 0.0f, 0.0f, 0.0f };
        UInt32 uiTemp[3]{ 0, 0, 0 };

        DArray<V3f> VtxCoords;
        DArray<V2f> TexCoords;
        DArray<V3f> NormCoords;
        DArray<UInt32> unIndices;
        VtxCoords.resize(strFileData.size() / 150);
        TexCoords.resize(strFileData.size() / 150);
        NormCoords.resize(strFileData.size() / 150);
        unIndices.resize(strFileData.size() / 50);

        Int32 currPos = 0, nextPos = 0;
        Int32 currMesh = 0, nextMesh = 0;
        currMesh = strFileData.find(nameToken, currPos);
        nextMesh = strFileData.find(nameToken, currPos + nameToken.size());
        while (currMesh != -1)   // Find the line position where the mesh name is
        {
            currPos = nextPos = 0;
            String strData = strFileData.substr(currMesh, nextMesh - currMesh);
            // mesh name is found
            char cMeshName[128];
            name_counter["nameCount"]++;
            // Copy the mesh name : |currPos|_meshName|lineEnd|
            sscanf(&strFileData.c_str()[currMesh], nameFormat.c_str(), cMeshName);
            printf("NW::LOAD_MASTER::LOAD_MESH_DATA_OBJ: loading the \"%s\" mesh\n", cMeshName);

            do {
                nextPos = strData.find(vtxToken, currPos);  // Find from the end of the last line
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], vtxFormat.c_str(), &fTemp[0], &fTemp[1], &fTemp[2]))
                {
                    VtxCoords[name_counter["vtxCount"]] = (V3f{fTemp[0], fTemp[1], fTemp[2] });
                    name_counter["vtxCount"]++;
                    currPos = nextPos + vtxToken.size();    // Curr = end of token; next = beginning
                }
            } while (true);

            do {
                nextPos = strData.find(uvToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], uvFormat.c_str(), &fTemp[0], &fTemp[1]))
                {
                    TexCoords[name_counter["uvCount"]] = (V2f{fTemp[0], fTemp[1]});
                    name_counter["uvCount"]++;
                    currPos = nextPos + uvToken.size();
                }
            } while (true);

            do {
                nextPos = strData.find(normToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], normFormat.c_str(), &fTemp[0], &fTemp[1], &fTemp[2]))
                {
                    NormCoords[name_counter["normCount"]] = (V3f{fTemp[0], fTemp[1], fTemp[2] });
                    name_counter["normCount"]++;
                    currPos = nextPos + normToken.size();
                }
            } while (true);

            do {
                nextPos = strData.find(elemsToken, currPos);
                if (nextPos == -1) break;
                if (sscanf(&strData.c_str()[nextPos], elemsFormat.c_str(),
                    &uiTemp[0], &uiTemp[0], &uiTemp[0],
                    &uiTemp[1], &uiTemp[1], &uiTemp[1],
                    &uiTemp[2], &uiTemp[2], &uiTemp[2]))
                {
                    unIndices[name_counter["indexCount"]] = (uiTemp[0]);
                    name_counter["indexCount"] ++;
                    unIndices[name_counter["indexCount"]] = (uiTemp[1]);
                    name_counter["indexCount"] ++;
                    unIndices[name_counter["indexCount"]] = (uiTemp[2]);
                    name_counter["indexCount"] ++;
                    currPos = nextPos + elemsToken.size();
                }
            } while (true);
            if (nextMesh == -1) break;
            (currMesh = (strFileData.find(nameToken, nextMesh)));
            nextMesh = strFileData.find(nameToken, currMesh + nameToken.size());
        }

        UInt32 unMaxVtx = 0;
        for (auto itCount = name_counter.begin(); itCount != name_counter.end(); advance(itCount, 1))
            unMaxVtx = itCount->second > unMaxVtx ? itCount->second : unMaxVtx;
        rVtxData.reserve(unMaxVtx);
        for (UInt32 vti = 0; vti < unMaxVtx; vti++)
        {
            rVtxData.emplace_back(VertexShape3d{VtxCoords[vti], TexCoords[vti], NormCoords[vti]});
        }
        unIndices.reserve(name_counter["indexCount"]);
        for (UInt32 idi = 0; idi < name_counter["indexCount"]; idi++)
        {
            runIndData.emplace_back(unIndices[idi]);
        }
        return true;

    }
    // --==</Implementation Methods>==--=
}