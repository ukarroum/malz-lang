#include <iostream>
#include <iomanip>
#include <format>

#include "debug.hpp"

void disChunk(const Chunk &chunk, const std::string &name)
{
    std::cout << "=== " << name << " ===" << std::endl;

    size_t offset = 0;

    while(offset < chunk.size())
    {
        offset = disInstr(chunk, offset);
    }
}

size_t disInstr(const Chunk &chunk, size_t offset)
{
    std::cout << std::setfill('0') << std::setw(4) << offset << " ";
    std::cout << "[ " << chunk.getLine(offset) << " ] ";

    auto instruction = chunk[offset];

    switch(instruction)
    {
        case OP_RETURN:
            return simpleInstr("OP_RETURN", offset);
        case OP_CONSTANT:
            return constInstr("OP_CONSTANT", chunk, offset);
        case OP_CONSTANT_LONG:
            return longConstInstr("OP_CONSTANT_LONG", chunk, offset);
        default:
            throw std::invalid_argument(std::format("Unknown opcode {}", instruction));
    }
}

size_t simpleInstr(const std::string &name, size_t offset)
{
    std::cout << name << std::endl;
    return offset + 1;
}

size_t constInstr(const std::string &name, const Chunk &chunk, size_t offset)
{
    uint8_t constant = chunk[offset + 1];

    // If we remove the "+" in +constant we get the char representation of the constant id which we don't want
    std::cout << name << " " << +constant << " ";
    std::cout << "'" << chunk.getConstant(constant) << "'" << std::endl;

    return offset + 2;
}

size_t longConstInstr(const std::string &name, const Chunk &chunk, size_t offset)
{
    uint16_t constant = (chunk[offset + 1] << 8) + (chunk[offset + 2]);

    std::cout << name << " " << +constant << " ";
    std::cout << "'" << chunk.getConstant(constant) << "'" << std::endl;

    return offset + 3;

}
