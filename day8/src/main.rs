use std::fs;

mod wasteland;

use wasteland::network::{calc_network_path_length, calc_spooky_network_path_length};
use wasteland::parse::parse_network;

fn main() {
    let data: Vec<_> = fs::read_to_string("data/8.in")
        .expect("fatal: file not found")
        .split("\n")
        .map(str::to_string)
        .collect();

    let network = parse_network(&data);

    // println!("{:?}", network);
    // println!("");

    let part_1_result = calc_network_path_length(&network);
    println!("result (part one): {}", part_1_result);
    println!("");

    let part_2_result = calc_spooky_network_path_length(&network);
    println!("result (part two): {}", part_2_result);
    println!("");
}
