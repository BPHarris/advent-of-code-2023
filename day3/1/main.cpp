#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

typedef struct Position {
		const u_int8_t col;
		const u_int8_t row;
} position_t;

typedef struct Symbol {
		const char c;
		const position_t pos;
} symbol_t;

typedef struct Number {
		const uint16_t v;
		const position_t start;
		const position_t end;
} number_t;

typedef struct Schematic {
		std::vector<symbol_t> symbols;
		std::vector<number_t> numbers;
} schematic_t;

const bool
is_noop(const char c) {
	return c == '.';
}

const bool
is_symbol(const char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '$' ||
	       c == '=' || c == '&' || c == '@' || c == '#';
}

const bool
is_digit(const char c) {
	// I could use isdigit(...) but this is consistent with the others
	return '0' <= c && c <= '9';
}

std::unique_ptr<const schematic_t>
parse(const std::vector<std::string> &data) {
	auto schematic = std::make_unique<schematic_t>();

	for (u_int8_t row = 0; row < data.size();) {
		auto line = data.at(row);

		for (uint8_t col = 0; col < line.length();) {
			char c = line.at(col);

			if (is_noop(c)) {
				col++;

			} else if (is_symbol(c)) {
				schematic->symbols.push_back({c, {col, row}});
				col++;

			} else if (is_digit(c)) {
				uint8_t length = 1;
				while (length <= 3 && col + length < line.length() &&
				       is_digit(line.at(col + length))) {
					length++;
				}

				char buf[4]{'\0'};
				line.copy(buf, length, col);

				const uint16_t v = (uint16_t) atoi(buf);
				const uint8_t col_ = col + length - 1;
				schematic->numbers.push_back({v, {col, row}, {col_, row}});

				col += length;

			} else {
				std::cout << "unrecognised symbol: " << c << std::endl;
				col++;
			}
		}

		row++;
	}

	return std::move(schematic);
}

std::optional<std::unique_ptr<std::vector<std::string>>>
read_file(const std::string filename) {
	std::ifstream file(filename);

	if (!file.is_open()) {
		return std::nullopt;
	}

	std::string line;
	auto lines = std::make_unique<std::vector<std::string>>();

	while (getline(file, line)) {
		lines->push_back(line);
	}

	return std::move(lines);
}

const bool
is_engine_part(const schematic_t &schematic, const number_t n) {
	int16_t x0 = (int16_t) n.start.col - 1;
	int16_t y0 = (int16_t) n.start.row - 1;
	int16_t x1 = (int16_t) n.end.col + 1;
	int16_t y1 = (int16_t) n.end.row + 1;

	for (auto s : schematic.symbols) {
		if ((x0 <= s.pos.col && s.pos.col <= x1) &&
		    (y0 <= s.pos.row && s.pos.row <= y1)) {
			return true;
		}
	}
	return false;
}

std::unique_ptr<std::vector<number_t>>
find_engine_parts(const schematic_t &schematic) {
	auto parts = std::make_unique<std::vector<number_t>>();

	for (auto n : schematic.numbers) {
		if (is_engine_part(schematic, n)) {
			parts->push_back(n);
		}
	}

	return std::move(parts);
}

const uint64_t
sum_engine_part_values(const std::vector<number_t> &parts) {
	uint64_t sum{0};

	for (auto p : parts) {
		sum += (uint64_t) p.v;
	}

	return sum;
}

int
main() {
	auto possible_data = read_file("day3/data/3.in");

	if (!possible_data.has_value()) {
		std::cout << "fatal: file not found" << std::endl;
		return EXIT_FAILURE;
	}

	auto data = std::move(possible_data.value());

	auto schematic = parse(*data);
	auto engine_parts = find_engine_parts(*schematic);

	auto part_1_result = sum_engine_part_values(*engine_parts);

	std::cout << "total number of parts   : " << schematic->numbers.size() << std::endl;
	std::cout << "  of which engine parts : " << engine_parts->size() << std::endl;
	std::cout << std::endl;
	std::cout << "result: " << part_1_result << std::endl;

	return EXIT_SUCCESS;
}
