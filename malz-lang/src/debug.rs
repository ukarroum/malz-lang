use crate::chunk::Chunk;
use crate::chunk::OpCode;

pub fn dis_chunk(code_chunk: &Chunk, name: &str){
    println!("==== {} ====", &name);

    let mut offset = 0;

    while offset < code_chunk.code.len() {
        offset = dis_instr(&code_chunk, offset);
    }
}
pub fn dis_instr(chunk: &Chunk, offset: usize) -> usize{
    print!("{}", format!("{:0>4} ", offset.to_string()));
    print!("[ {} ] ", chunk.get_line(offset));
    
    match OpCode::try_from(chunk.code[offset]).unwrap() {
        OpCode::RETURN => simple_instr("RETURN", offset),
        OpCode::CONSTANT => constant_instr("CONSTANT", &chunk, offset),
        OpCode::CONSTANT_LONG => long_constant_instr("CONSTANT_LONG", &chunk, offset)
    }
}
pub fn simple_instr(name: &str, offset: usize) -> usize{
    println!("{}", &name);
    offset + 1
}

pub fn constant_instr(name: &str, chunk: &Chunk, offset: usize) -> usize{
    let constant = chunk.code[offset + 1];
    
    println!("{}", format!("{} {:0>4} ", &name, constant.to_string()));

    offset + 2
}

pub fn long_constant_instr(name: &str, chunk: &Chunk, offset: usize) -> usize{
    println!(
        "{}", 
        format!(
            "{} {:0>4} {:0>4} {:0>4}",
            &name,
            chunk.code[offset + 1].to_string(),
            chunk.code[offset + 2].to_string(),
            chunk.code[offset + 3].to_string()
        )
    );

    offset + 4
}
