#include <filesystem>

#include "camel_cards.hpp"
#include "parser.hpp"

int
main() {
	const std::filesystem::path filepath{"day7/data/7.in"};

	if (!std::filesystem::exists(filepath)) {
		std::cout << "fatal: file not found" << std::endl;
		return EXIT_FAILURE;
	}

	auto data = parser::read_file(filepath);
	auto hands = parser::parse_hands(*data);

	auto part_1_result = camel_cards::calc_total_winnings_simple(*hands);
	std::cout << "result (part one): " << part_1_result << std::endl;
	std::cout << std::endl;

	auto part_2_result = camel_cards::calc_total_winnings_complex(*hands);
	std::cout << "result (part two): " << part_2_result << std::endl;
	std::cout << std::endl;

	return EXIT_SUCCESS;
}