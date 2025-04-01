mod chunk;
mod debug;

use crate::chunk::Chunk;
use crate::chunk::OpCode;
use crate::debug::disChunk;

fn main() {
    let mut code_chunk = Chunk::new();

    code_chunk.write(OpCode::RETURN as u8);

    disChunk(&code_chunk, "test chunk");
}
