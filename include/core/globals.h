#pragma once

#ifdef __WINDOWS__
constexpr auto SCREEN_WIDTH = 500;
constexpr auto SCREEN_HEIGHT = 800;
#else
constexpr auto SCREEN_WIDTH = 1024;
constexpr auto SCREEN_HEIGHT = 512;
#endif

constexpr auto MAX_COMPONENTS = 1024;
constexpr auto MAX_ENTITIES = 1024;
constexpr auto MAX_BLOCKS = 1024;
constexpr auto tickRate = 1.0 / 60.0;

namespace age
{
	class Engine;
	class ResourceManager;
} // namespace age

namespace Global
{
	inline age::Engine* Engine = nullptr;
	inline age::ResourceManager* ResourceManager = nullptr;
} // namespace Global
