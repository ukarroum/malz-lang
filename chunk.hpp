#pragma once

#include <cstdint>
#include <vector>

enum OpCode: uint8_t
{
    OP_RETURN,
};

class Chunk
{
public:
    void writeChunk(uint8_t opcode) { m_code.push_back(opcode); };
    size_t size() const { return m_code.size(); }
    uint8_t operator[](size_t i) const { return m_code[i]; };
private:
    std::vector<uint8_t> m_code;
};