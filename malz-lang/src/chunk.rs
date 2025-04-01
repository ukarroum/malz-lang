use std::vec;

#[repr(u8)]
pub enum OpCode {
    RETURN = 1
}

pub struct Chunk {
    pub code: Vec<u8>
}

impl Chunk {
    pub fn new() -> Self {
        Self { code: Vec::new() }
    }
    pub fn write(&mut self, byte: u8){
        self.code.push(byte);
    }
}

