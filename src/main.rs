use image::{io::Reader, GenericImageView};
use std::fs::File;
use std::io::prelude::*;

fn main() {
    let img = Reader::open("peppers.jpg").unwrap().decode().unwrap();

    let mut r = "".to_string();
    let mut g = "".to_string();
    let mut b = "".to_string();

    for p in img.pixels() {
        r.push_str(&p.2[0].to_string());
        r += ",";

        g.push_str(&p.2[1].to_string());
        g += ",";

        b.push_str(&p.2[2].to_string());
        b += ",";
    }

    r.pop(); // Remove the last comma
    g.pop();
    b.pop();

    let mut rfile = File::create("r_buf.txt").unwrap();
    let _ = rfile.write_all(r.as_bytes());

    let mut gfile = File::create("g_buf.txt").unwrap();
    let _ = gfile.write_all(g.as_bytes());

    let mut bfile = File::create("b_buf.txt").unwrap();
    let _ = bfile.write_all(b.as_bytes());
}
