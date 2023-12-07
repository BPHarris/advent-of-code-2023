#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "camel_cards.hpp"

namespace parser {

std::unique_ptr<std::vector<std::string>>
read_file(const std::filesystem::path &filepath) {
	auto lines = std::make_unique<std::vector<std::string>>();

	std::ifstream file(filepath);

	if (!file.is_open()) {
		std::cout << "fatal: can't open file" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string line;
	while (getline(file, line)) {
		lines->push_back(line);
	}

	return lines;
}

std::unique_ptr<std::vector<camel_cards::hand_t>>
parse_hands(const std::vector<std::string> &data) {
	auto hands = std::make_unique<std::vector<camel_cards::hand_t>>();

	for (size_t i = 0; i < data.size(); i++) {
		auto line = data.at(i);
		std::stringstream ss(line);

		std::string cards{};
		u_int64_t bid{};

		std::getline(ss, cards, ' ');
		while (ss >> bid)
			;

		hands->push_back({cards, bid, camel_cards::simple_score(cards),
		                  camel_cards::complex_score(cards)});
	}

	return hands;
}

}   // namespace parser