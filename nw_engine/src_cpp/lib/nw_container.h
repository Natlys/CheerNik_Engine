#ifndef NW_COLLECTION_H
#define NW_COLLECTION_H

namespace NW
{
	/// SArrayId class
	/// Description:
	/// -- Contains a static array with booleans and sets flags to true if they are owned
	/// -- Counts from 1 to IdCount
	template<UInt32 IdCount> class SArrayId
	{
	public:
		SArrayId() = default;
		SArrayId(SArrayId<IdCount>& rIdList) = default;
		~SArrayId() = default;

		// -- Getters
		inline UInt32 GetFreeId() {
			for (UInt32 idi = 1; idi < IdCount; idi++) {
				if (IsFree(idi)) return idi;
			}
		}
		inline UInt32 GetLastOwned() { return m_unLastOwned; }
		inline UInt32 GetSize() { return IdCount; }
		// -- Setters
		inline void SetFree(UInt32 unId) {
			m_bOwnedIds[unId - 1] = false;
		}
		inline void SetOwned(UInt32 unId) {
			m_bOwnedIds[unId - 1] = true;
		}
		inline UInt32 SetFreeAll() {
			m_unLast = IdCount;
			UInt32 unCurr = 1;
			while (unCurr < IdCount) {
				m_bOwnedIds[unCurr - 1] = false;
			}
		}
		inline UInt32 SetOwnedAll() {
			m_unLast = IdCount;
			UInt32 unCurr = 1;
			while (unCurr < IdCount) {
				m_bOwnedIds[unCurr - 1] = true;
			}
		}
		// -- Predicates
		inline bool IsFree(UInt32 unId) { return unId > IdCount ? false : !m_bOwnedIds[unId]; }
		inline bool IsOwned(UInt32 unId) { return unId > IdCount ? false : m_bOwnedIds[unId]; }
	private:
		UInt32 m_unLast = 0;
		bool m_bOwnedIds[IdCount]{ false };
	};
#if (NW_LIBS & NW_LIBS_NATIVE_COLLECTION)
	template <typename Type, int size>
	class NW_API SArray
	{
	public:
		SArray()
		{
			//
		}
		SArray(UInt16 size)
		{
			//
		}
		SArray(SArray& saCopied)
		{
			//
		}
		SArray(SArray&& saMoved)
		{
			//
		}
		~SArray() {}
		
		// Getters
		inline const UInt32 GetCount() const
		{
			return m_unCount;
		}
		inline const UInt32 GetSlots() const
		{
			return m_unSlots;
		}
		// Setters
		inline void SetCount()
		{
			//
		}
		
		// Interface Methods
		inline void PushBack(Type)
		{
			//
		}
		inline void Clear()
		{
			//
		}

		// Operators
		SArray& operator=(SArray& saCopied)
		{
		}
		SArray& operator=(SArray&& saMoved)
		{
		}
		Type& operator[](int index)
		{
		}
		const Type& operator[](int index) const
		{
		}
	private:
		Type* m_tElems;
		UInt m_unCount;
		UInt m_unSlots;
	};
	template <typename Type>
	class NW_API DArray
	{
	public:
		DArray()
		{
			//
		}
		explicit DArray(UInt16 size)
		{
			//
		}
		DArray(DArray& daCopied)
		{
			//
		}
		DArray(DArray&& daMoved)
		{
			//
		}
		~DArray()
		{
			//
		}

		// Getters
		inline const UInt32 GetCount() const
		{
			return m_unCount;
		}
		inline const UInt32 GetSlots() const
		{
			return m_unSlots;
		}
		// Setters
		inline void SetCount()
		{
			//
		}
		inline void SetSlots()
		{
			//
		}

		// Interface Methods
		inline void PushBack(Type)
		{
			//
		}
		inline void Clear()
		{
			//
		}

		// Operators
		DArray& operator=(DArray& daCopied)
		{
		}
		DArray& operator=(DArray&& daMoved)
		{
		}
		DArray& operator[](int index)
		{
		}
	private:
		Type* m_tElems;
		UInt32 m_unCount;
		UInt32 m_unSlots;
	};
#elif (NW_LIBS & NW_LIBS_STD_COLLECTION)
#endif // NW_LIBS
}
#endif // NW_COLLECTION_H