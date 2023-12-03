#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <ranges>
#include <string>

#define digit(d) static_cast<u_int64_t>(d - '0')

const std::map<const std::string, const char> digits = {
    {"zero", '0'}, {"one", '1'}, {"two", '2'},   {"three", '3'}, {"four", '4'},
    {"five", '5'}, {"six", '6'}, {"seven", '7'}, {"eight", '8'}, {"nine", '9'},
};

bool
has_digit_name_at_start(const std::string str) {
	for (const auto &[name, _] : digits) {
		if (str.find(name) == 0) {
			return true;
		}
	}
	return false;
}

const std::optional<const char>
digit_from_name_at_start(const std::string str) {
	for (const auto &[name, digit] : digits) {
		if (str.find(name) == 0) {
			return digit;
		}
	}
	return std::nullopt;
}

const std::optional<const char>
first_digit_in_line(const std::string line) {
	for (size_t i = 0; i < line.length(); i++) {
		if (const char c = line.at(i); isdigit(c)) {
			return c;
		}

		auto substr = line.substr(i, std::string::npos);
		if (has_digit_name_at_start(substr)) {
			return digit_from_name_at_start(substr).value();
		}
	}

	return std::nullopt;
}

const std::optional<const char>
last_digit_in_line(const std::string line) {
	for (ssize_t i = line.length() - 1; i >= 0; i--) {
		if (const char c = line.at(i); isdigit(c)) {
			return c;
		}

		auto substr = line.substr(i, std::string::npos);
		if (has_digit_name_at_start(substr)) {
			return digit_from_name_at_start(substr).value();
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
