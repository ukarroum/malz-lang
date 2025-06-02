use crate::value::Value;

#[repr(u8)]
pub enum OpCode {
    RETURN = 0,
    CONSTANT = 1,
    CONSTANT_LONG = 2
}

pub struct Chunk {
    pub code: Vec<u8>,
    pub constants: Vec<Value>,
    pub lines: Vec<(i32, usize)>
}

impl Chunk {
    pub fn new() -> Self {
        Self { code: Vec::new(), constants: Vec::new(), lines: Vec::new() }
    }

    pub fn write(&mut self, byte: u8, line: i32){
        self.code.push(byte);

        match self.lines.last_mut() {
            None => self.lines.push((line, 1)),
            Some((o_line, _nb_offset)) => if *o_line == line {
                self.lines.last_mut().unwrap().1 += 1;
            }
            else {
                self.lines.push((line, 1));
            }
        }
    }

    pub fn write_constant(&mut self, val: Value, line: i32){
        let idx = self.add_constant(val);

        if idx >= 256 {
            self.write(OpCode::CONSTANT_LONG as u8, line);
            self.write((idx >> 16) as u8, line);
            self.write((idx & 0xff00 >> 8) as u8, line);
            self.write((idx & 0xff) as u8, line);
        }
        else {
            self.write(OpCode::CONSTANT as u8, line);
            self.write(idx as u8, line);
        }
    }

    pub fn add_constant(&mut self, val: Value) -> usize {
        self.constants.push(val);
        self.constants.len() - 1
    }

    pub fn get_line(&self, offset: usize) -> i32 {
        let mut idx: usize = 0;

        let mut offset = offset;

        while offset != 0 {
            if offset <= self.lines[idx].1 {
                return self.lines[idx].0;
            }
            else {
                offset -= self.lines[idx].1;
            }

            idx += 1;
        }

        if offset == 0 {
            return self.lines[idx].0;
        }
        else {
            panic!("lines and offset mismatch");
        }
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

