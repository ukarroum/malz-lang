mod chunk;
mod debug;
mod value;

use crate::chunk::Chunk;
use crate::chunk::OpCode;
use crate::debug::dis_chunk;

fn main() {
    let mut code_chunk = Chunk::new();

    let constant = code_chunk.add_constant(1.2);
    code_chunk.write(OpCode::CONSTANT as u8, 123);
    code_chunk.write(constant as u8, 123);

    code_chunk.write(OpCode::RETURN as u8, 123);

    dis_chunk(&code_chunk, "test chunk");
}
