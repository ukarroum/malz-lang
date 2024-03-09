#include "chunk.hpp"
#include "debug.hpp"

int main(int argc, const char* argv[])
{
    Chunk chunk;

    size_t constant = chunk.addConstant(1.2);
    chunk.writeChunk(OP_CONSTANT, 123);
    chunk.writeChunk(constant, 123);
    chunk.writeChunk(OP_RETURN, 123);

    disChunk(chunk, "test chunk");

    return 0;
}