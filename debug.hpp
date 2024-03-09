#pragma once

#include <string>

#include "chunk.hpp"

void disChunk(const Chunk& chunk, const std::string& name);
size_t disInstr(const Chunk& chunk, size_t offset);
size_t simpleInstr(const std::string& name, size_t offset);
size_t constInstr(const std::string& name, const Chunk& chunk, size_t offset);