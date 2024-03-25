#pragma once

#include <cstdint>
#include <vector>

#include "value.hpp"

enum OpCode: uint8_t
{
    OP_RETURN,
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE
};

class Chunk
{
public:
    void writeChunk(uint8_t opcode, size_t line);
    void writeConstant(Value value, int line);
    size_t addConstant(Value value);

    [[nodiscard]] size_t size() const { return m_code.size(); }
    uint8_t operator[](size_t i) const { return m_code[i]; }
    [[nodiscard]] Value getConstant(size_t i) const { return m_constants[i]; }
    [[nodiscard]] size_t getLine(size_t i) const;
    [[nodiscard]] const std::vector<uint8_t>& getCode() { return m_code; }
private:
    std::vector<uint8_t> m_code;
    std::vector<Value> m_constants;
    std::vector<std::pair<size_t, size_t>> m_lines;
};