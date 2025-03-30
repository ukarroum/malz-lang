mod chunk;
mod debug;

pub use crate::chunk::chunk::Chunk;
pub use crate::chunk::chunk::OpCode;
pub use crate::debug::debug::disChunk;

fn main() {
    let mut code_chunk = Chunk::new();

    code_chunk.write(OpCode::RETURN as u8);

    disChunk(&chunk, "test chunk");
}
