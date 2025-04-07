use crate::value::Value;

#[repr(u8)]
pub enum OpCode {
    RETURN = 0,
    CONSTANT = 1
}

pub struct Chunk {
    pub code: Vec<u8>,
    pub constants: Vec<Value>
}

impl Chunk {
    pub fn new() -> Self {
        Self { code: Vec::new(), constants: Vec::new() }
    }
    pub fn write(&mut self, byte: u8){
        self.code.push(byte);
    }
    pub fn add_constant(&mut self, val Value) -> usize {
        self.constants.push(val);

        self.constants.len() - 1
    }
}
impl TryFrom<u8> for OpCode {
    type Error = &'static str;

    fn try_from(value: u8) -> Result<Self, Self::Error>{
        match value {
            0 => Ok(OpCode::RETURN),
            1 => Ok(OpCode::CONSTANT),
            _ => Err("Invalid opcode")
        }
    }
}

