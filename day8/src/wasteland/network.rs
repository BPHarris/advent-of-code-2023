use std::collections::{HashMap, HashSet};

use crate::wasteland::maths::lcm;

#[derive(Debug)]
pub struct Network {
    pub instructions: String,
    pub nodes: HashMap<String, Node>,
}

impl Network {
    pub fn start(&self, name: &String) -> TrackPosition {
        return TrackPosition {
            name: name.clone(),
            node: self.nodes.get(name).unwrap().clone(),
            instruction_idx: 0.clone(),
            instruction_ord: 0.clone(),
            direction: self.instructions.chars().nth(0).unwrap().clone(),
        };
    }

    pub fn follow(&self, from: &TrackPosition) -> Result<TrackPosition, String> {
        return match from.direction {
            'L' | 'R' => {
                let next_name = from.node.go(from.direction).unwrap().to_string();
                let next_idx = (from.instruction_idx + 1) % self.instructions.len();
                Ok(TrackPosition {
                    name: next_name.clone(),
                    node: self.nodes.get(&next_name).unwrap().clone(),
                    instruction_idx: next_idx,
                    instruction_ord: from.instruction_ord + 1,
                    direction: self.instructions.chars().nth(next_idx).unwrap().clone(),
                })
            }
            _ => Err("no".to_string()),
        };
    }
}

#[derive(Debug, Hash, Eq, PartialEq, Clone)]
pub struct Node {
    pub left: String,
    pub right: String,
}

impl Node {
    pub fn go(&self, direction: char) -> Result<String, String> {
        return match direction {
            'L' => Ok(self.left.clone()),
            'R' => Ok(self.right.clone()),
            _ => Err("no".to_string()),
        };
    }
}

#[derive(Debug, Hash, Eq, PartialEq, Clone)]
pub struct TrackPosition {
    pub name: String,
    pub node: Node,
    pub instruction_idx: usize,
    pub instruction_ord: usize,
    pub direction: char,
}

#[derive(Debug, Hash, Eq, PartialEq, Clone)]
pub struct TrackSummary {
    pub factors: Vec<usize>,
}

pub fn calc_network_path_length(network: &Network) -> usize {
    let mut counter = 0;
    let mut track = network.start(&"AAA".to_string());

    while track.name != "ZZZ" {
        track = network.follow(&track).unwrap();
        counter += 1;
    }

    return counter;
}

pub fn find_spooky_start_points(network: &Network) -> Vec<TrackPosition> {
    return network
        .nodes
        .keys()
        .filter(|name| name.ends_with("A"))
        .map(|name| network.start(name).clone())
        .collect();
}

pub fn calc_spooky_network_path_length(network: &Network) -> usize {
    let tracks = find_spooky_start_points(network);

    let mut track_factors: HashMap<TrackPosition, Vec<u64>> = HashMap::new();
    for track in tracks.iter() {
        track_factors.insert(track.clone(), vec![]);
    }

    for track_start in tracks.iter() {
        let mut track = track_start.clone();
        let mut seen_points: HashSet<(String, usize)> =
            HashSet::from([(track_start.name.clone(), 0)]);

        loop {
            track = network.follow(&track).unwrap();
            let here = (track.name.clone(), track.instruction_idx);

            if track.name.ends_with("Z") {
                track_factors
                    .get_mut(&track_start)
                    .unwrap()
                    .push(track.instruction_ord as u64);
            }

            if seen_points.contains(&here) {
                break;
            }

            seen_points.insert(here);
        }
    }

    // Note
    // This works as there is exactly one solution between the start and the point of
    // repetition in each track for the given input, so we can just find the least
    // common multiple of each track's single factor.
    // Given more complex input s.t. there were Zs before the repetition and several Zs
    // in the repeating loop, we'd need an infinite iterator giving the possible Zs for
    // each track, progressing the lowest, until all the tracks current Zs have the same
    // ordinality.
    return lcm(track_factors
        .values()
        .map(|s| s.get(0).unwrap().clone() as u64)
        .collect::<Vec<_>>()
        .clone()) as usize;
}
