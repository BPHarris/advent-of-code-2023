use std::fs;

trait Possibility {
    fn is_possible(&self) -> bool;
}

trait MinimumRequiredCubes {
    fn minimum_required_cubes(&self) -> Cubes;
}

trait Power {
    fn power(&self) -> u64;
}

#[derive(Debug)]
pub struct Game {
    pub id: u8,
    pub plays: Vec<Cubes>,
}

impl Possibility for Game {
    fn is_possible(&self) -> bool {
        return self.plays.iter().all(|p| p.is_possible());
    }
}

impl MinimumRequiredCubes for Game {
    fn minimum_required_cubes(&self) -> Cubes {
        return Cubes {
            red: self.plays.iter().map(|p| p.red).max().unwrap(),
            green: self.plays.iter().map(|p| p.green).max().unwrap(),
            blue: self.plays.iter().map(|p| p.blue).max().unwrap(),
        };
    }
}

#[derive(Debug)]
pub struct Cubes {
    pub red: u8,
    pub green: u8,
    pub blue: u8,
}

impl Possibility for Cubes {
    fn is_possible(&self) -> bool {
        return self.red <= 12 && self.green <= 13 && self.blue <= 14;
    }
}

impl Power for Cubes {
    fn power(&self) -> u64 {
        return self.red as u64 * self.green as u64 * self.blue as u64;
    }
}

fn parse_games(data: &String) -> Vec<Game> {
    let lines: Vec<String> = data.lines().map(|l| l.to_string()).collect();
    return lines.iter().map(|l| parse_game(l)).collect();
}

fn parse_game(data: &String) -> Game {
    // e.g.: "Game 1: 7 red, 8 blue; 6 blue, 6 red, 2 green; 2 red, 6 green, 8 blue"
    let parts: Vec<&str> = data.splitn(2, ": ").collect();
    let (game_id, game_plays) = (parts[0], parts[1]);

    // e.g.: "Game 123"
    let game_id_parts: Vec<&str> = game_id.splitn(2, " ").collect();
    let id: u8 = game_id_parts[1].parse().unwrap();

    // e.g.: "7 red, 8 blue; 6 blue, 6 red, 2 green; 2 red, 6 green, 8 blue"
    let plays_parts: Vec<&str> = game_plays.split("; ").collect();
    let plays: Vec<Cubes> = plays_parts
        .iter()
        .map(|p| parse_play(p.to_string()))
        .collect();

    return Game { id, plays };
}

fn parse_play(data: String) -> Cubes {
    // e.g.: "7 red, 8 blue, 9 green" in any order, with each component optional
    let colour_infos: Vec<&str> = data.split(", ").collect();

    let mut red: u8 = 0;
    let mut green: u8 = 0;
    let mut blue: u8 = 0;

    for colour_info in colour_infos {
        let colour_info_string = colour_info.to_string();
        let parts: Vec<&str> = colour_info_string.splitn(2, " ").collect();

        let n: u8 = parts[0].parse().unwrap();
        let colour: &str = parts[1];

        match colour {
            "red" => red = n,
            "green" => green = n,
            "blue" => blue = n,
            _ => (),
        }
    }

    return Cubes { red, green, blue };
}

fn main() {
    let data = fs::read_to_string("data/2a.in").expect("file not found");
    let games = parse_games(&data);

    let part_1_result: u64 = games
        .iter()
        .filter(|g| g.is_possible())
        .map(|g| g.id as u64)
        .sum();

    println!("result (part one): {}", part_1_result);

    let part_2_result: u64 = games
        .iter()
        .map(|g| g.minimum_required_cubes())
        .map(|c| c.power())
        .sum();

    println!("result (part two): {}", part_2_result);
}
