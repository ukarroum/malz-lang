#include <iostream>
#include <iomanip>
#include <format>

#include "debug.hpp"

void disChunk(const Chunk &chunk, const std::string &name)
{
    std::cout << "=== " << name << " ===" << std::endl;

    for(size_t offset = 0; offset < chunk.size(); offset++)
    {
        offset = disInstr(chunk, offset);
    }
}

size_t disInstr(const Chunk &chunk, size_t offset)
{
    std::cout << std::setfill('0') << std::setw(4) << offset << " ";

    auto instruction = chunk[offset];

    switch(instruction)
    {
        case OP_RETURN:
            return simpleInstr("OP_RETURN", offset);
        default:
            throw std::invalid_argument(std::format("Unknown opcode {}", instruction));
    }
}

size_t simpleInstr(const std::string &name, size_t offset)
{
    std::cout << name << std::endl;
    return offset + 1;
}
