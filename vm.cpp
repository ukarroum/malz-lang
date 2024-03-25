#include <iostream>
#include <functional>

#include "vm.hpp"
#include "debug.hpp"

#define DEBUG_TRACE_EXEC

InterpretResult VM::interpret(Chunk chunk)
{
    m_chunk = std::move(chunk);
    m_ip = m_chunk.getCode().cbegin();

    return run();
}

InterpretResult VM::run()
{
    while(true)
    {
        uint8_t instr = readByte();
#ifdef DEBUG_TRACE_EXEC
        disInstr(m_chunk, m_ip - m_chunk.getCode().cbegin() - 1);
#endif
        switch(instr)
        {
            case OP_RETURN:
            {
                Value val = m_stack.top();
                m_stack.pop();
                std::cout << val << std::endl;
                return InterpretResult::INTERPRET_OK;
            }
            case OP_CONSTANT:
            {
                Value constant = m_chunk.getConstant(readByte());
                m_stack.push(constant);
                break;
            }
            case OP_NEGATE:
            {
                Value val = m_stack.top();
                m_stack.pop();
                m_stack.push(-val);
                break;
            }
            case OP_ADD:
                binaryOp<std::plus<Value>>(); break;
            case OP_SUBTRACT:
                binaryOp<std::minus<Value>>(); break;
            case OP_MULTIPLY:
                binaryOp<std::multiplies<Value>>(); break;
            case OP_DIVIDE:
                binaryOp<std::divides<Value>>(); break;
        }
    }
}

template<typename Op>
void VM::binaryOp()
{
    Value b = m_stack.top();
    m_stack.pop();
    Value a = m_stack.top();
    m_stack.pop();

    m_stack.push(Op()(a, b));
}
