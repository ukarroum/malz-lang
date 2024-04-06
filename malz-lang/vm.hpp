#pragma once

#include <stack>

#include "chunk.hpp"

enum class InterpretResult
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

class VM
{
public:
    InterpretResult interpret(Chunk chunk);
    InterpretResult run();
    uint8_t readByte() { return *m_ip++; }
    template <typename Op> void binaryOp();
private:
    Chunk m_chunk;
    std::vector<uint8_t>::const_iterator m_ip;
    std::stack<Value> m_stack;
};