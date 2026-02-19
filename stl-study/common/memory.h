#pragma once
#include <type_traits>
#include <memory>

namespace mst
{
	template <typename T>
	void SafeDelete(T& pointer) noexcept
	{
		static_assert(std::is_pointer_v<T>, "T must be a pointer type.");

		if (pointer != nullptr)
		{
			delete pointer;
			pointer = nullptr;
		}
	}

	template <typename T>
	void SafeFree(T& pointer) noexcept
	{
		static_assert(std::is_pointer_v<T>, "T must be a pointer type.");

		if (pointer != nullptr)
		{
			free(pointer);
			pointer = nullptr;
		}
	}

	template <typename T>
	void SafeMove(T* src, T* dst, size_t size)
	{
		if (src == nullptr || dst == nullptr)
		{
			return;
		}

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::memcpy(src, dst, sizeof(T) * size);
		}
		else
		{
			for (size_t i = 0; i < size; ++i)
			{
				if constexpr (std::is_nothrow_move_constructible_v<T>)
				{
					std::construct_at(&src[i], std::move(dst[i]));
				}
				else
				{
					std::construct_at(&src[i], dst[i]);
				}
			}
		}

		for (size_t i = 0; i < size; ++i)
		{
			std::destroy_at(&dst[i]);
		}
	}

	template <typename T>
	void SafeCopy(T* src, T* dst, size_t size)
	{
		if (src == nullptr || dst == nullptr)
		{
			return;
		}

		if constexpr (std::is_trivially_copyable_v<T>)
		{
			std::memcpy(src, dst, sizeof(T) * size);
		}
		else
		{
			for (size_t i = 0; i < size; ++i)
			{
				src[i] = dst[i];
			}
		}
	}

}