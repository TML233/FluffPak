#pragma once

#include "Engine/System/Definition.h"
#include <atomic>

namespace Engine {
	template<typename T>
	class AtomicValue {
	public:
		explicit AtomicValue(T value = static_cast<T>(0)) {
			Set(value);
		}
		T Get() const {
			return value.load(std::memory_order_acquire);
		}
		void Set(T value) {
			this->value.store(value,std::memory_order_release);
		}
		T Exchange(T value) {
			return this->value.exchange(value, std::memory_order_acq_rel);
		}
		// Returns the value before operation.
		T FetchAdd(T value) {
			return this->value.fetch_add(value, std::memory_order_acq_rel);
		}
		// Returns the value before operation.
		T FetchSubtract(T value) {
			return this->value.fetch_sub(value, std::memory_order_acq_rel);
		}
		// Returns current value.
		T Add(T value) {
			return FetchAdd(value) + value;
		}
		// Returns current value.
		T Subtract(T value) {
			return FetchSubtract(value) - value;
		}

	private:
		std::atomic<T> value;
	};

	template<>
	class AtomicValue<bool> {
	public:
		explicit AtomicValue(bool value = false) {
			Set(value);
		}
		bool Get() const {
			return value.load(std::memory_order_acquire);
		}
		void Set(bool value) {
			this->value.store(value, std::memory_order_release);
		}
		bool Exchange(bool value) {
			return this->value.exchange(value, std::memory_order_acq_rel);
		}

	private:
		std::atomic<bool> value;
	};

	class ReferenceCount {
	public:
		ReferenceCount(uint32 count = 0) :count(count) {}

		uint32 Get() const {
			return count.Get();
		}
		uint32 Reference() {
			return count.Add(1);
		}
		uint32 Dereference() {
			return count.Subtract(1);
		}
	private:
		AtomicValue<uint32> count;
	};
}