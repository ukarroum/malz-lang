#include "chunk.hpp"
#include "debug.hpp"

int main(int argc, const char* argv[])
{
    Chunk chunk;

    for(int i = 0; i <= 256; i++)
        chunk.writeConstant(1.2, 123);

    chunk.writeChunk(OP_RETURN, 123);

    disChunk(chunk, "test chunk");

    return 0;
}