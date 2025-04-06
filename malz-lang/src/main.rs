mod chunk;
mod debug;

use crate::chunk::Chunk;
use crate::chunk::OpCode;
use crate::debug::dis_chunk;

fn main() {
    let mut code_chunk = Chunk::new();

    code_chunk.write(OpCode::RETURN as u8);

    dis_chunk(&code_chunk, "test chunk");
}
