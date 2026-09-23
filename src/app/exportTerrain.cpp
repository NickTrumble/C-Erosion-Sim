#include "exportTerrain.hpp"

#include <array>
#include <cstdint>
#include <fstream>

namespace 
{
    std::string_view signature = "TERA";

    template <typename T>
    bool writeValue(std::ofstream& file, const T& value) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(T));
        return static_cast<bool>(file);
    }

    template <typename T>
    bool readValue(std::ifstream& file, T& value) {
        file.read(reinterpret_cast<char*>(&value), sizeof(T));
        return static_cast<bool>(file);
    }

}

void ExportTerrain::updatePath(std::filesystem::path& newPath){
    path = newPath;
}

bool ExportTerrain::saveTerrain(Heightmap& heightmap, TerrainGeneratorSettings& settings, std::filesystem::path& path){
    std::ofstream file(path, std::ios::binary);
    if (!file) {
        return false;
    }

    file.write(signature.data(), signature.size());
    writeValue(file, heightmap.getWidth());
    writeValue(file, heightmap.getHeight());

    writeValue(file, settings.seed);
    writeValue(file, settings.octaves);
    writeValue(file, settings.persistence);
    writeValue(file, settings.scale);

    auto& values = heightmap.getValues();

    for (float value : values) {
        if (!writeValue(file, value)) {
            return false;
        }
    }

    return true;
}

std::optional<LoadedTerrain> ExportTerrain::loadTerrain(std::filesystem::path& path){
    std::ifstream file(path, std::ios::binary);

    if(!file){
        return std::nullopt;
    }

    //verify file signature
    std::string fileSignature(signature.size(), (char)0);
    file.read(fileSignature.data(), static_cast<std::streamsize>(fileSignature.size()));

    if (fileSignature != signature){
        return std::nullopt;
    }

    TerrainGeneratorSettings settings;
    int width;
    int height;

    readValue(file, width);
    readValue(file, height);

    readValue(file, settings.seed);
    readValue(file, settings.octaves);
    readValue(file, settings.persistence);
    readValue(file, settings.scale);

    Heightmap heightmap(width, height);

    for(auto& value : heightmap.getValues()){
        readValue(file, value);
    }

    if (!file){
        return std::nullopt;
    }

    return LoadedTerrain(
        std::move(heightmap),
        settings
    );
}