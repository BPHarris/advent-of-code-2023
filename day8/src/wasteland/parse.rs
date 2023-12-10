use std::collections::HashMap;

use super::network::{Network, Node};

pub fn parse_network(data: &Vec<String>) -> Network {
    let instructions = data[0].clone();
    let mut nodes = HashMap::new();

    for line in data.iter().skip(2) {
        let parts = line.split_once(" = ").unwrap();
        let node = parts.0.to_string();
        let targets: Vec<_> = parts
            .1
            .replace("(", "")
            .replace(")", "")
            .split(", ")
            .map(|t| t.to_string())
            .collect();

        nodes.insert(
            node,
            Node {
                left: targets[0].clone(),
                right: targets[1].clone(),
            },
        );
    }

    return Network {
        instructions,
        nodes,
    };
}
