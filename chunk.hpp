#pragma once

#include <cstdint>
#include <vector>

#include "value.hpp"

enum OpCode: uint8_t
{
    OP_RETURN,
    OP_CONSTANT
};

class Chunk
{
public:
    void writeChunk(uint8_t opcode, size_t line);
    size_t addConstant(Value value);

    [[nodiscard]] size_t size() const { return m_code.size(); }
    uint8_t operator[](size_t i) const { return m_code[i]; }
    [[nodiscard]] Value getConstant(size_t i) const { return m_constants[i]; }
    [[nodiscard]] size_t getLine(size_t i) const { return m_lines[i]; }
private:
    std::vector<uint8_t> m_code;
    std::vector<Value> m_constants;
    std::vector<size_t> m_lines;
};