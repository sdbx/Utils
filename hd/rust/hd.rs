use std::io::{self, BufRead};

fn main() -> io::Result<()> {
  let stdin = io::stdin();
  let mut handle = stdin.lock();
  
  loop {
    let mut s = String::new();
    let bytes = handle.read_line(&mut s)?;
    if bytes == 0 {
      break;
    }
    let s = s.trim();
    if s.len() != 0 && !s.starts_with("# ") {
      print!("{}", "# ");
    }
    println!("{}", s);
  }

  Ok(())
}
