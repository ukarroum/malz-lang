#include "chunk.hpp"
#include "debug.hpp"

int main(int argc, const char* argv[])
{
    Chunk chunk;

    chunk.writeChunk(OP_RETURN);

    disChunk(chunk, "test chunk");

    return 0;
}