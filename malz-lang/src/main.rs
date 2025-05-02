mod chunk;
mod debug;
mod value;
mod vm;

use crate::chunk::Chunk;
use crate::chunk::OpCode;
use crate::debug::dis_chunk;
use crate::vm::VM;

fn main() {
    let mut code_chunk = Chunk::new();

    code_chunk.write_constant(1.2, 123);

    code_chunk.write(OpCode::RETURN as u8, 123);

    dis_chunk(&code_chunk, "test chunk");
    let mut code_vm = VM::new(code_chunk);
    code_vm.interpret();
}
