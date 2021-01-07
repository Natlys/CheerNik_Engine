#ifndef LUA_CORE_H
#define LUA_CORE_H

#include <lua_decl.hpp>

namespace NW
{
	/// LuaVirtualMachieneInfo struct
	/// Description:
	/// -- Contains strings and numbers with the information about lua stack:
	/// ==| size
	/// ==| types
	/// ==| last level of search
	struct NW_API LuaVMInfo
	{
	public:
		using StackTypesInfo = SArray<Int8, 20>;
	private:
		UInt8 unLevel = 0;
		StackTypesInfo StackTypes;
	public:
		LuaVMInfo() { memset(&StackTypes[0], LT_NON, sizeof(Int8)); }
		// --getters
		inline Int8 GetStackType(UInt8 unIndex) { return StackTypes[unIndex]; }
		inline Int8 GetLastType() { return unLevel == 0 ? LT_NIL : StackTypes[unLevel - 1]; }
		inline UInt8 GetLevel() { return unLevel; }
		// --setters
		inline void AddType(Int8 nType) { if (unLevel > StackTypes.size()) RemoveType(1); StackTypes[unLevel++] = nType; }
		inline void RemoveType(UInt8 unCount) { while (unCount > 0 && unLevel > 0) { unCount--; unLevel--; StackTypes[unLevel] = LT_NON; } }
		inline void Reset() { RemoveType(unLevel); }
		// -- Operators
		inline OutStream& operator<<(OutStream& rOStream) const {
			rOStream << std::endl << "LuaVMInfo::" << std::endl <<
				"The top of the stack:" << std::endl;
			for (UInt8 ti = 0; ti < StackTypes.size(); ti++) {
				rOStream << static_cast<Int16>(ti + 1) << "th type: ";
				switch (StackTypes[ti]) {
				case LT_NON:			rOStream << ("none");				break;
				case LT_NIL:			rOStream << ("nil");				break;
				case LT_LUD:			rOStream << ("light user data");	break;
				case LT_BOL:			rOStream << ("boolean");			break;
				case LT_NUM:			rOStream << ("number");				break;
				case LT_STR:			rOStream << ("string");				break;
				case LT_FNC:			rOStream << ("function");			break;
				case LT_TAB:			rOStream << ("table");				break;
				case LT_USD:				rOStream << ("user data");			break;
				case LT_THR:			rOStream << ("thread");				break;
				case LT_UPV:			rOStream << ("up value");			break;
				}
			rOStream << std::endl;
			}
			rOStream << "The bottom of the stack" << std::endl;
			return rOStream;
		}
	};
	/// LuaRegistrableFunction struct
	/// Description:
	/// -- This is a handler of lua for a function
	/// -- It contains the function adress pointer or lua code (one of that)
	/// -- Also, it contains upvalues adresses, points to the owner table
	struct NW_API LuaRegFunc
	{
	public:
		const char* strName;
		const char* strLoc;
		const LuaNativeFunc fnNative;
		LuaUpValues UpValues;
	public:
		LuaRegFunc(const char* sName, const LuaNativeFunc fNative) :
			strName(sName), strLoc(""),
			fnNative(fNative) { }
		LuaRegFunc(const char* sLoc, const char* sName, const LuaNativeFunc fNative) :
			strName(sName), strLoc(sLoc),
			fnNative(fNative) { }
		LuaRegFunc(const char* sLoc, const char* sName, const LuaNativeFunc fNative, const LuaUpValues& rUpVals) :
			strName(sName), strLoc(sLoc),
			fnNative(fNative), UpValues(rUpVals) { }
		LuaRegFunc(const LuaRegFunc& rCpy) :
			strName(rCpy.strName), strLoc(rCpy.strLoc),
			fnNative(rCpy.fnNative), UpValues(rCpy.UpValues) { }
	};
	/// LuaRegistrableTable struct
	/// Description:
	/// -- This is an information about a table in lua
	/// -- Contains:
	/// ==| the name;
	/// ==| over table - where it is allocated;
	/// ==| metatable - which methods are inherited;
	/// ==| default methods
	/// ==| registrated methods
	struct NW_API LuaRegTable
	{
	public:
		const char* strName;
		const char* strLoc;

		DArray<LuaRegFunc*> lrfMethods;
		LuaRegTable* lrtMeta;
	public:
		LuaRegTable(const char* sName) :
			strName(sName), strLoc(""),
			lrtMeta(nullptr) { }
		LuaRegTable(const char* sName, LuaRegTable* pMetaTable) :
			strName(sName), strLoc(""),
			lrtMeta(pMetaTable) { }

		LuaRegTable(const char* sLoc, const char* sName) :
			strName(sName), strLoc(sLoc),
			lrtMeta(nullptr) { }
		LuaRegTable(const char* sLoc, const char* sName, LuaRegTable* pMetaTable) :
			strName(sName), strLoc(sLoc),
			lrtMeta(pMetaTable) { }

		LuaRegTable(const LuaRegTable& rCpy) :
			strName(rCpy.strName), strLoc(rCpy.strLoc),
			lrfMethods(rCpy.lrfMethods),
			lrtMeta(rCpy.lrtMeta) { }
	};
	/// LuaRegistrableType struct
	struct NW_API LuaRegType
	{
	public:
		LuaRegTable Table;
		LuaRegTable MetaTable;

		LuaNativeFunc lnfCreate;
		LuaUpValues CreateUpValues;
	};
	/// LuaReaderState struct
	struct NW_API LuaReaderState
	{
		const String* strChunk = nullptr;
		bool bIsRead = false;
	};
}

#endif	// LUA_CORE_H

/// Destroy function must to:
/// -> Get the object via "lua_touserdata(LuaNative, -1)"
/// -> Call the destructor of the object
/// Index function must to:
/// -> Get the object via "lua_touserdata(LuaNative, -2)"
/// -> Get the index value via "lua_tostring(LuaNative, -1)"
/// -> Switch between index values and behave depending on it
/// -> If there is no such index - get values from the script stack
/// -> "lua_getuservalue(-1)"
/// if it is - pushvalue(2), gettable(-2)
/// else "getglobal(RegTableName)", push index string
/// -> rawget(-2)