#include <fstream>
#include <iostream>
#include <optional>
#include <ranges>
#include <string>

#define digit(d) static_cast<u_int64_t>(d - '0')

const std::optional<const char>
first_digit_in_line(const std::string line) {
	for (auto c : line) {
		if (isdigit(c)) {
			return c;
		}
	}
	return std::nullopt;
}

const std::optional<const char>
last_digit_in_line(const std::string line) {
	for (auto c : std::views::reverse(line)) {
		if (isdigit(c)) {
			return c;
		}
	}
	return std::nullopt;
}

u_int64_t
calc_calibration_value(const std::string line) {
	return 10 * digit(first_digit_in_line(line).value_or('0')) +
	       digit(last_digit_in_line(line).value_or('0'));
}

int
main() {
	std::string line;
	std::ifstream file("day1/data/1.in");

	u_int64_t summation = 0;

	if (!file.is_open()) {
		std::cout << "fatal: file not found" << std::endl;
		return EXIT_FAILURE;
	}

	while (std::getline(file, line)) {
		summation += calc_calibration_value(line);
	}
	file.close();

	std::cout << "result: " << summation << std::endl;

	return EXIT_SUCCESS;
}
