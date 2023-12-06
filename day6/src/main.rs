use std::collections::VecDeque;
use std::fs;

mod race;

use race::parse::{parse_as_multiple_races, parse_as_one_race};
use race::types::Race;

fn product_of_winning_charge_times(races: &Vec<Race>) -> u64 {
    return races
        .iter()
        .map(|r| {
            (0..=r.time)
                .map(|t| r.is_wining_charge_time(t))
                .filter(|&won| won)
                .count() as u64
        })
        .product();
}

fn find_start_point(race: &Race) -> Option<u64> {
    // recursively splitting binary search to find any valid start point
    let mut queue = VecDeque::new();
    queue.push_back(race.time / 2);

    // since each no two tree paths will visit the same guess, if the seen counter
    // reaches the max number of possible start times we can quit (there is no solution),
    // to avoid an infinite loop
    let possible_start_times = race.time as usize + 1;
    let mut seen = 0 as usize;

    while seen < possible_start_times {
        let guess = queue.pop_front().unwrap();
        seen += 1;

        if race.is_wining_charge_time(guess) {
            return Some(guess);
        }

        let lower_guess = guess / 2;
        let upper_guess = guess + guess / 2;

        queue.push_back(lower_guess);
        queue.push_back(upper_guess);
    }

    return None;
}

fn binary_search_for_lower_bound(race: &Race) -> u64 {
    let ref_point = find_start_point(race).expect("no valid start time");

    // a recursive closure would be nice but they look tricky!
    // maybe just unwrap into loop...
    fn search(race: &Race, ref_point: u64, point: u64) -> u64 {
        if race.is_lower_bound(point) {
            return point;
        }

        if race.is_wining_charge_time(point) {
            if point < ref_point {
                return search(race, point, point / 2);
            } else {
                return search(race, ref_point, ref_point / 2);
            }
        } else {
            if point < ref_point {
                return search(race, ref_point, point + ((ref_point - point) / 2));
            } else {
                return search(race, ref_point, ref_point / 2);
            }
        }
    }

    return search(race, ref_point, ref_point / 2);
}

fn binary_search_for_upper_bound(race: &Race) -> u64 {
    let ref_point = find_start_point(race).expect("no valid start time");

    fn search(race: &Race, ref_point: u64, point: u64) -> u64 {
        if race.is_upper_bound(point) {
            return point;
        }

        if race.is_wining_charge_time(point) {
            if point > ref_point {
                return search(race, point, point + ((race.time - point) / 2));
            } else {
                return search(race, ref_point, ref_point + ((race.time - ref_point) / 2));
            }
        } else {
            if point > ref_point {
                return search(race, ref_point, ref_point + ((point - ref_point) / 2));
            } else {
                return search(race, ref_point, ref_point + ((race.time - ref_point) / 2));
            }
        }
    }

    return search(race, ref_point, ref_point + ((race.time - ref_point) / 2));
}

fn main() {
    let data = fs::read_to_string("data/6.in")
        .expect("fatal: file not found")
        .split("\n")
        .map(str::to_string)
        .collect();

    // part 1
    let races = parse_as_multiple_races(&data);

    for race in &races {
        println!("{:?}", race);
    }
    println!("");

    let part_1_result = product_of_winning_charge_times(&races);
    println!("result (part one): {}", part_1_result);

    // part 2
    println!("");

    let race = parse_as_one_race(&data);
    println!("{:?}\n", race);

    let lowest_winning_charge_time = binary_search_for_lower_bound(&race);
    println!("lower bound: {}", lowest_winning_charge_time);
    let highest_winning_charge_time = binary_search_for_upper_bound(&race);
    println!("upper bound: {}", highest_winning_charge_time);
    println!("");

    let part_2_result = highest_winning_charge_time - lowest_winning_charge_time + 1;
    println!("result (part two): {}", part_2_result);
}
