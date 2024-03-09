#include "chunk.hpp"

void Chunk::writeChunk(uint8_t opcode, size_t line)
{
    m_code.push_back(opcode);
    m_lines.push_back(line);
}

size_t Chunk::addConstant(Value value)
{
    m_constants.push_back(value);
    return m_constants.size() - 1;
}
