#ifndef CN_LOAD_MASTER_H
#define CN_LOAD_MASTER_H

#include <cn_core.hpp>
#include <gl/cn_gl_lib.hpp>

namespace CN
{
	/// LoadMaster Singleton class
	/// --Deals with loading of
	/// -->textures and meshes(obj/dae)
	/// -->shaders, text files and file streams
	/// -->binary objects
	class CN_API LoadMaster
	{
	public : // Constants&Macroses
#define MAX_LOAD_SIZE 1024 * 100
	public: // Enums
		enum class MeshFormats {
			DEFAULT = 0, OBJ = 1, DAE
		};
		enum class ImgFormats {
			DEFAULT = 0, PNG = 1, JPG, DDS
		};
		enum class ShaderFormats {
			DEFAULT = 0, VERTEX = 1, PIXEL, GEOMETRY
		};
	public: // Methods
		// Destructor
		~LoadMaster();

		// Accessors
		static LoadMaster& get() { static LoadMaster s_instance; return s_instance; }
		
		/// Returns resources directory of the project
		std::string getDir() const { return m_resDir; };
		UInt getLoadedCount() const { return m_loadedCount; }
		UInt getLoadedBytes() const { return m_bytesCount; }

		// Saving
		/// Save the data with the given size into the file by given path
		bool saveIt_bin(const std::string& path,
			void* data, size_t bytes);
		// Loading
		/// Load bytes of the given size from the file into the given data
		bool loadIt_bin(const std::string& path,
			void* data, size_t bytes);
		/// Load image bytes from the file and initialize GL texture struct
		bool loadIt_tex2d(const std::string& path,
			GL::Texture* texPtr);
		/// Load vertex and material data from the file and initialize GL mesh class
		bool loadIt_mesh(const std::string& path,
			GL::Mesh* meshPtr);
		/// Load material and image data from the file and initialize GL sprite class
		bool loadIt_sprite(const std::string& path,
			GL::Sprite* spritePtr);
		/// Load shader state from the file and initialize GL shader class
		bool loadIt_Shader(const std::string& path,
			GL::Shader* shaderPtr);
	private: // Members
		std::string m_resDir;
		size_t m_bytesCount;
		UInt m_loadedCount;
	private: // Implementation Functions
		// Private singleton Constructor
		LoadMaster();
		/// -Parse mesh file depending on it's format
		/// --Write vertex and material data into the mesh struct
		void load_mesh(const std::string& path,
			GL::Mesh* meshPtr, MeshFormats format);
		/// -Load binary information from the file and parse it depending on file format
		/// --Return byte array with written pixel data
		UChar* load_img(const std::string& path,
			MeshFormats format);
	};
}

#endif