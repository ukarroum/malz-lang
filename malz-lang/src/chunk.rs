#[repr(u8)]
pub enum OpCode {
    RETURN = 0
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
impl TryFrom<u8> for OpCode {
    type Error = &'static str;

    fn try_from(value: u8) -> Result<Self, Self::Error>{
        match value {
            0 => Ok(OpCode::RETURN),
            _ => Err("Invalid opcode")
        }
    }
}

