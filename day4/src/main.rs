use std::fs;

#[derive(Debug)]
pub struct ScratchCard {
    pub id: u8,
    pub winning_numbers: Vec<u8>,
    pub actual_numbers: Vec<u8>,
}

trait Scoring {
    fn number_of_wins(&self) -> u8;
    fn score(&self) -> u64;
}

impl Scoring for ScratchCard {
    fn number_of_wins(&self) -> u8 {
        let matching: Vec<u8> = self
            .actual_numbers
            .iter()
            .filter(|n| self.winning_numbers.contains(n))
            .cloned()
            .collect();
        return matching.len() as u8;
    }

    fn score(&self) -> u64 {
        if self.number_of_wins() == 0 {
            return 0;
        }
        return (2 as u64).pow(self.number_of_wins() as u32 - 1);
    }
}

fn parse_scratch_cards(data: &String) -> Vec<ScratchCard> {
    let lines: Vec<String> = data.lines().map(|l| l.to_string()).collect();
    return lines.iter().map(|l| parse_scratch_card(l)).collect();
}

fn parse_scratch_card(line: &String) -> ScratchCard {
    // e.g.: "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53"
    let parts: Vec<&str> = line.splitn(2, ": ").collect();

    // card id
    // e.g.: "Card    34"
    let card_id_info: String = parts[0].chars().filter(|c| c.is_digit(10)).collect();
    let id: u8 = card_id_info
        .parse()
        .expect(&format!("id is not an int: '{}'", card_id_info));

    // card numbers
    // e.g. "41 48 83 86 17 | 83 86  6 31 17  9 48 53"
    let card_numbers_info: Vec<&str> = parts[1].splitn(2, " | ").collect();

    // ... "41 48 83 86 17" (lhs: winning numbers)
    let winning_numbers_info: Vec<&str> = card_numbers_info[0].split_whitespace().collect();
    let winning_numbers: Vec<u8> = winning_numbers_info
        .iter()
        .map(|s| {
            let digits: String = s.chars().filter(|c| c.is_digit(10)).collect();
            return digits.parse().expect(&format!("not an int: '{}'", digits));
        })
        .collect();

    // ... "83 86  6 31 17  9 48 53" (rhs: actual numbers)
    let actual_numbers_strings: Vec<&str> = card_numbers_info[1].split_whitespace().collect();
    let actual_numbers: Vec<u8> = actual_numbers_strings
        .iter()
        .map(|s| {
            let digits: String = s.chars().filter(|c| c.is_digit(10)).collect();
            return digits.parse().expect(&format!("not an int: '{}'", digits));
        })
        .collect();

    return ScratchCard {
        id,
        winning_numbers,
        actual_numbers,
    };
}

fn calc_part_two_winnings(scratch_cards: &Vec<ScratchCard>) -> Vec<u64> {
    let mut winnings: Vec<u64> = vec![1; scratch_cards.len()];

    for card in scratch_cards {
        let score = card.number_of_wins() as usize;

        if score == 0 {
            continue;
        }

        let card_id = card.id as usize;
        let lower_card_id = card_id + 1;
        let upper_card_id = card_id + score;

        for won_id in lower_card_id..=upper_card_id {
            // cards are 1-indexed :|
            winnings[won_id-1] += winnings[card_id-1];
        }
    }

    return winnings;
}

fn main() {
    let data = fs::read_to_string("data/4.in").expect("file not found");
    let scratch_cards = parse_scratch_cards(&data);

    let part_1_result: u64 = scratch_cards.iter().map(|s| s.score()).sum();
    println!("result (part one): {}", part_1_result);

    let part_2_result: u64 = calc_part_two_winnings(&scratch_cards).iter().sum();
    println!("result (part two): {}", part_2_result);
}
