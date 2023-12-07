#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace camel_cards {

const std::vector<char> simple_card_ordering{'2', '3', '4', '5', '6', '7', '8',
                                             '9', 'T', 'J', 'Q', 'K', 'A'};
const std::vector<char> complex_card_ordering{'J', '2', '3', '4', '5', '6', '7',
                                              '8', '9', 'T', 'Q', 'K', 'A'};

const std::unordered_map<int, std::string> _dbg_score_to_name = {
    {7, "five of a kind"},  {6, "four of a kind"}, {5, "full house"},
    {4, "three of a kind"}, {3, "two-pair"},       {2, "one-pair"},
    {1, "high card"},
};

typedef struct Hand {
	std::string cards;
	u_int64_t bid;
	u_int64_t score_simple;
	u_int64_t score_complex;
} hand_t;

inline std::optional<u_int64_t>
simple_value(const char card) {

	for (size_t i = 0; i < simple_card_ordering.size(); i++) {
		if (simple_card_ordering.at(i) == card) {
			return i;
		}
	}
	return std::nullopt;
}

inline std::optional<u_int64_t>
simple_strength(const std::string &cards) {
	std::vector<size_t> card_counts(simple_card_ordering.size(), 0);

	for (auto c : cards) {
		auto v = simple_value(c);

		if (!v.has_value()) {
			return std::nullopt;
		}

		card_counts[v.value()]++;
	}

	// todo: generalise me! seems simple but i am going to bed now...

	size_t n_three_of_a_kinds = 0;
	size_t n_two_of_a_kinds = 0;

	for (auto count : card_counts) {
		if (count == 5) {
			return 7;   // five of a kind
		}
		if (count == 4) {
			return 6;   // four of a kind
		}
		if (count == 3) {
			n_three_of_a_kinds++;
		}
		if (count == 2) {
			n_two_of_a_kinds++;
		}
	}

	if (n_three_of_a_kinds == 1) {
		if (n_two_of_a_kinds == 1) {
			return 5;   // full house
		} else {
			return 4;   // three of a kind
		}
	}

	if (n_two_of_a_kinds == 2) {
		return 3;   // two pair
	}

	if (n_two_of_a_kinds == 1) {
		return 2;   // one pair
	}

	return 1;   // high-card
}

inline u_int64_t
simple_score(const std::string &cards) {
	u_int64_t combined_score = 0;

	// strength       <= 7      => 3 bits
	// each tie break <= 12     => 4 bits (4 x 5 = 20 bits)
	// .: we can store the entire score in 23 bits
	// I shall do each as 4 bits as it is neater
	combined_score += simple_strength(cards).value();
	combined_score = combined_score << 4;

	for (auto card : cards) {
		combined_score += simple_value(card).value();
		combined_score = combined_score << 4;
	}

	return combined_score;
}

inline std::optional<u_int64_t>
complex_value(const char card) {

	for (size_t i = 0; i < complex_card_ordering.size(); i++) {
		if (complex_card_ordering.at(i) == card) {
			return i;
		}
	}
	return std::nullopt;
}

inline std::optional<u_int64_t>
complex_strength(const std::string &cards) {
	// handle jokers separately
	u_int64_t n_jokers = 0;
	std::vector<size_t> card_counts(complex_card_ordering.size(), 0);

	for (auto c : cards) {
		if (c == 'J') {
			n_jokers++;
			continue;
		}

		auto v = complex_value(c);

		if (!v.has_value()) {
			return std::nullopt;
		}

		card_counts[v.value()]++;
	}

	size_t n_five_of_a_kinds = 0;
	size_t n_four_of_a_kinds = 0;
	size_t n_three_of_a_kinds = 0;
	size_t n_two_of_a_kinds = 0;
	size_t n_one_of_a_kinds = 0;

	for (auto count : card_counts) {
		switch (count) {
		case 5:
			n_five_of_a_kinds++;
			break;
		case 4:
			n_four_of_a_kinds++;
			break;

		case 3:
			n_three_of_a_kinds++;
			break;
		case 2:
			n_two_of_a_kinds++;
			break;
		case 1:
			n_one_of_a_kinds++;
			break;

		default:
			break;
		}
	}

	// todo: the pattern here is obvious (for n-of-a-kind), refactor it as for-loops
	// full house and two-pair may need to be exceptions

	if (n_five_of_a_kinds == 1 || (n_four_of_a_kinds == 1 && n_jokers >= 1) ||
	    (n_three_of_a_kinds == 1 && n_jokers == 2) ||
	    (n_two_of_a_kinds == 1 && n_jokers == 3) ||
	    (n_one_of_a_kinds == 1 && n_jokers == 4) || (n_jokers == 5)) {
		return 7;   // five of a kind
	}

	if (n_four_of_a_kinds == 1 || (n_three_of_a_kinds == 1 && n_jokers >= 1) ||
	    (n_two_of_a_kinds == 1 && n_jokers >= 2) ||
	    (n_one_of_a_kinds == 1 && n_jokers >= 3) || n_jokers >= 3) {
		return 6;   // four of a kind
	}

	if ((n_three_of_a_kinds == 1 && n_two_of_a_kinds == 1) ||
	    (n_three_of_a_kinds == 1 && n_jokers >= 1) ||
	    (n_two_of_a_kinds == 2 && n_jokers == 1) ||
	    (n_two_of_a_kinds == 1 && n_jokers >= 2)) {
		return 5;   // full house
	}

	if (n_three_of_a_kinds == 1 || (n_two_of_a_kinds == 1 && n_jokers >= 1) ||
	    n_jokers >= 2) {
		return 4;   // three of a kind
	}

	if (n_two_of_a_kinds == 2 || (n_two_of_a_kinds == 1 && n_jokers >= 1)) {
		return 3;   // two pair
	}

	if (n_two_of_a_kinds == 1 || (n_jokers >= 1)) {
		return 2;   // one pair
	}

	return 1;   // high-card
}

inline u_int64_t
complex_score(const std::string &cards) {
	u_int64_t combined_score = 0;

	// see simple_score
	combined_score += complex_strength(cards).value();
	combined_score = combined_score << 4;

	for (auto card : cards) {
		combined_score += complex_value(card).value();
		combined_score = combined_score << 4;
	}

	return combined_score;
}

u_int64_t
calc_total_winnings_simple(std::vector<camel_cards::hand_t> &hands) {
	std::vector<camel_cards::hand_t> hands_(hands);

	std::sort(hands_.begin(), hands_.end(),
	          [](auto lhs, auto rhs) { return lhs.score_simple < rhs.score_simple; });

	u_int64_t total = 0;

	for (size_t i = 0; i < hands_.size(); i++) {
		total += (i + 1) * hands_.at(i).bid;
	}

	return total;
}

u_int64_t
calc_total_winnings_complex(std::vector<camel_cards::hand_t> &hands) {
	std::vector<camel_cards::hand_t> hands_(hands);

	std::sort(hands_.begin(), hands_.end(),
	          [](auto lhs, auto rhs) { return lhs.score_complex < rhs.score_complex; });

	u_int64_t total = 0;

	for (size_t i = 0; i < hands_.size(); i++) {
		total += (i + 1) * hands_.at(i).bid;
	}

	return total;
}
}   // namespace camel_cards
