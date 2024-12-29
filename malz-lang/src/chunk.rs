use std::Vec;

pub mod chunk {
    pub enum OpCode {
        RETURN
    }

    pub struct Chunk {
        code: Vec<u8>
    }
}

