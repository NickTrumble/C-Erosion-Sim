#pragma once

#include "../terrain/heightmap.hpp"
#include "../terrain/terrain_generator.hpp"
#include <filesystem>
#include <optional>


struct LoadedTerrain{
    Heightmap heightmap;
    TerrainGeneratorSettings settings;
};

class ExportTerrain{
    public:
        bool saveTerrain(
            Heightmap& heightmap,
            TerrainGeneratorSettings& settings,
            std::filesystem::path& path
        );

        std::optional<LoadedTerrain> loadTerrain(std::filesystem::path& path);

        std::filesystem::path getPath() const;
        void updatePath(std::filesystem::path& newPath);

    private:
        std::filesystem::path path;
    
};
