use crate::race::types::Race;

// part 1: multiple races
pub fn parse_as_multiple_races(data: &Vec<String>) -> Vec<Race> {
    let times: Vec<_> = data[0]
        .split_whitespace()
        .into_iter()
        .skip(1) // skip "Time:"
        .map(|t| t.parse::<u64>().expect("fatal: not a number"))
        .collect();

    let distances: Vec<_> = data[1]
        .split_whitespace()
        .into_iter()
        .skip(1) // skip "Duration:"
        .map(|t| t.parse::<u64>().expect("fatal: not a number"))
        .collect();

    return times
        .iter()
        .zip(distances.iter())
        .map(|(&time, &distance)| Race { time, distance })
        .collect();
}

// part 2: single races ("bad kerning")
pub fn parse_as_one_race(data: &Vec<String>) -> Race {
    let mut time_string: String = data[0].chars().filter(|c| !c.is_whitespace()).collect();
    time_string = time_string.split_once(":").unwrap().1.to_string();

    let mut distance_string: String = data[1].chars().filter(|c| !c.is_whitespace()).collect();
    distance_string = distance_string.split_once(":").unwrap().1.to_string();

    let time = time_string.parse::<u64>().unwrap();
    let distance = distance_string.parse::<u64>().unwrap();
    return Race { time, distance };
}
