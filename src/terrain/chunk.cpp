#include "chunk.hpp"

Chunk::Chunk(int chunkX, int chunkY, int size, int resolution):
    chunkX(chunkX),
    chunkY(chunkY),
    size(size),
    resolution(resolution)
    {}

std::pair<int, int> Chunk::getChunkCoords() const {
    return std::pair<int, int>(chunkX, chunkY);
}

bool Chunk::isReady() const{
    return ready;
}