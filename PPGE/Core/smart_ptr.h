#pragma once
#include <memory>

namespace PPGE {

	// Shortened unquie_ptr under PPGE API
	template<typename T>
	using Unique = std::unique_ptr<T>;
	// Templated make_uinique function
	// Variadic template is used for arbitrary number of inputs
	// constexpr is used for compile time evaluations
	template<typename T, typename ... Args>
	constexpr Unique<T> CreateUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// Shortened shared_ptr under PPGE API
	template<typename T>
	using Shared = std::shared_ptr<T>;
	// Templated make_shared function
	// Variadic template is used for arbitrary number of inputs
	// constexpr is used for compile time evaluations
	template<typename T, typename ... Args>
	constexpr Shared<T> CreateShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}