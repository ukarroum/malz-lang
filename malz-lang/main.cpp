#include"chunk.hpp"
#include "debug.hpp"
#include "vm.hpp"

int main(int argc, const char* argv[])
{
    Chunk chunk;

    chunk.writeConstant(1.2, 123);
    chunk.writeConstant(3.4, 123);

    chunk.writeChunk(OP_ADD, 123);

    chunk.writeConstant(5.6, 123);

    chunk.writeChunk(OP_DIVIDE, 123);
    chunk.writeChunk(OP_NEGATE, 123);;

    chunk.writeChunk(OP_RETURN, 123);

    VM vm;

    vm.interpret(chunk);

    return 0;
}