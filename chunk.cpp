#include <ranges>

#include "chunk.hpp"

void Chunk::writeChunk(uint8_t opcode, size_t line)
{
    m_code.push_back(opcode);

    if(m_lines.empty() || m_lines.back().second != line)
        m_lines.emplace_back(1, line);
    else
        m_lines.back().first++;
}

size_t Chunk::addConstant(Value value)
{
    m_constants.push_back(value);
    return m_constants.size() - 1;
}

size_t Chunk::getLine(size_t i) const
{
    size_t curr = 0, idx = 0;

    while(curr < i)
        curr += m_lines[idx++].first;

    if(curr == i)
        return m_lines[idx].second;
    else
        return m_lines[idx - 1].second;
}
