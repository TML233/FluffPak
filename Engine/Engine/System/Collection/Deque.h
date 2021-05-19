#pragma once
#include "Engine/System/Definition.h"
#include "Engine/System/Collection/List.h"
#include "Engine/System/Memory/SharedPtr.h"

namespace Engine {
	template<typename T>
	class Deque final {
	public:
		~Deque() {
			Clear();
		}

		void PushFront(const T& value) {
			int32 chunkIndex = PrepareChunk(GetChunkIndex(frontIndex));
			SharedPtr<ElementChunk> chunk = chunks.Get(chunkIndex);
			
			T* ptr = chunk->elements + GetElementIndexInChunk(frontIndex);
			Memory::Construct(ptr, value);

			frontIndex -= 1;
		}
		void PushBack(const T& value) {
			int32 chunkIndex = PrepareChunk(GetChunkIndex(backIndex));
			SharedPtr<ElementChunk> chunk = chunks.Get(chunkIndex);

			T* ptr = chunk->elements + GetElementIndexInChunk(backIndex);
			Memory::Construct(ptr, value);

			backIndex += 1;
		}
		bool TryPopFront(T& result) {
			if (GetCount() <= 0) {
				return false;
			}
			frontIndex += 1;
			
			int32 chunkIndex = GetChunkIndex(frontIndex);
			SharedPtr<ElementChunk> chunk = chunks.Get(chunkIndex);
			result = chunk->elements[GetElementIndexInChunk(frontIndex)];

			Memory::Destruct(chunk->elements + GetElementIndexInChunk(frontIndex));
			return true;
		}
		bool TryPopBack(T& result) {
			if (GetCount() <= 0) {
				return false;
			}
			backIndex -= 1;

			int32 chunkIndex = GetChunkIndex(backIndex);
			SharedPtr<ElementChunk> chunk = chunks.Get(chunkIndex);
			result = chunk->elements[GetElementIndexInChunk(backIndex)];

			Memory::Destruct(chunk->elements + GetElementIndexInChunk(backIndex));
			return true;
		}
		T PopFront() {
			T result;
			bool succeeded = TryPopFront(result);
			ERR_ASSERT(succeeded, u8"Cannot pop from front!", return T());
			return result;
		}
		T PopBack() {
			T result;
			bool succeeded = TryPopBack(result);
			ERR_ASSERT(succeeded, u8"Cannot pop from back!", return T());
			return result;
		}
		int32 GetCount() const {
			return backIndex - frontIndex - 1;
		}
		void Clear() {
			int32 lastChunkIndex = -1;
			SharedPtr<ElementChunk> lastChunk;
			for (int i = frontIndex + 1; i < backIndex; i += 1) {
				int32 chunkIndex = GetChunkIndex(i);
				if (lastChunkIndex != chunkIndex) {
					lastChunkIndex = chunkIndex;
					lastChunk = chunks.Get(chunkIndex);
				}
				Memory::Destruct(lastChunk->elements + GetElementIndexInChunk(i));
			}

			frontIndex = -1;
			backIndex = 0;
		}

		static inline constexpr int32 ChunkSize = 8;

	private:
		struct ElementChunk {
			ElementChunk() {
				elements = (T*)Memory::Allocate(sizeof(T) * ChunkSize);
			}
			~ElementChunk() {
				Memory::Deallocate(elements);
			}
			T* elements;
		};
		//                                  CenterChunk
		// [ *  *  *  *  *  *  *  * ] [ *  *  *  *  *  *  *  * ]
		//  -8 -7 -6 -5 -4 -3 -2 -1     0  1  2  3  4  5  6  7
		List<SharedPtr<ElementChunk>> chunks{};

		int32 frontIndex = -1;
		int32 backIndex = 0;
		int32 centerChunk = -1;


		int32 GetChunkIndex(int32 elementIndex) const {
			if (elementIndex >= 0) {
				return elementIndex / ChunkSize + centerChunk;
			} else {
				return (elementIndex + 1) / ChunkSize - 1 + centerChunk;
			}
		}
		int32 GetElementIndexInChunk(int32 elementIndex) const {
			if (elementIndex >= 0) {
				return elementIndex % ChunkSize;
			} else {
				return (ChunkSize - 1) - ((-elementIndex - 1) % ChunkSize);
			}
		}
		int32 PrepareChunk(int32 chunkIndex) {
			while (chunkIndex < 0 || chunkIndex >= chunks.GetCount()) {
				if (chunkIndex < 0) {
					chunks.Insert(0, SharedPtr<ElementChunk>::Create());
					centerChunk += 1;
					chunkIndex += 1;
				} else if (chunkIndex >= chunks.GetCount()) {
					chunks.Add(SharedPtr<ElementChunk>::Create());
				}
			}
			return chunkIndex;
		}
	};
}