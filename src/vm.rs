use crate::chunk::Chunk;
use crate::chunk::OpCode;

pub struct VM {
    pub chunk: Chunk,
    ip: usize
}

pub enum InterpretResult {
    OK,
    COMPILE_ERROR,
    RUNTIME_ERROR
}

impl VM {
    pub fn new(chunk: Chunk) -> Self {
        Self { chunk: chunk, ip: 0 }
    }
    pub fn interpret(&mut self) -> InterpretResult {
        self.ip = 0;

        self.run()
    }
    pub fn run(&mut self) -> InterpretResult {
        loop {
            self.ip += 1;

            match OpCode::try_from(self.chunk.code[self.ip - 1]).unwrap() {
                OpCode::RETURN => return InterpretResult::OK,
                OpCode::CONSTANT => {
                    println!("{}", self.chunk.constants[self.chunk.code[self.ip] as usize].to_string());
                    self.ip += 1;
                }
                OpCode::CONSTANT_LONG => {
                    println!("{}", self.chunk.constants[(((self.chunk.code[self.ip] as i64) << 16) + ((self.chunk.code[self.ip + 1] as i64) << 8) + (self.chunk.code[self.ip + 3] as i64)) as usize].to_string());
                    self.ip += 2;
                }
            }
        }
    }
}
