#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include <omp.h>

#include "almanac.hpp"

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

size_t
min_location_number(const almanac::input_almanac_maps_t &almanac_maps) {
	std::vector<size_t> end_points{};

	for (auto start_point : almanac_maps.initial_seeds) {
		end_points.push_back(almanac::follow_map_route(almanac_maps, start_point));
	}

	return *std::min_element(end_points.begin(), end_points.end());
}

size_t
min_location_number_for_seed_range(const almanac::input_almanac_maps_t &almanac_maps) {
	size_t smallest = UINT64_MAX;

	// for part 2, initial seeds come in pairs of <start> and <range>
	// break these down into somewhat similar sized chunks for parallelising
	// 10m and 20m blocks seem to be about the same speed
	std::vector<almanac::seed_range_t> seed_ranges;
	for (size_t i = 0; i < almanac_maps.initial_seeds.size();) {
		auto start = almanac_maps.initial_seeds.at(i++);
		auto range = almanac_maps.initial_seeds.at(i++);

		size_t block_size = 40000000;

		while (range > 0) {
			if (range > block_size) {
				seed_ranges.push_back({start, block_size});
				start += block_size + 1;
				range -= block_size;
			} else {
				seed_ranges.push_back({start, range});
				range = 0;
			}
		}
	}

	std::cout << "parallel blocks: " << seed_ranges.size() << std::endl;

#pragma omp parallel for reduction(min : smallest)
	for (auto seeds : seed_ranges) {
		for (size_t seed = seeds.start; seed < seeds.end(); seed++) {
			auto current = almanac::follow_map_route(almanac_maps, seed);
			if (current < smallest) {
				smallest = current;
			}
		}
	}

	// TODO TRY SPEED OF:
	// humidity-to-location map
	// this means that humidity has ranges
	//   0..55  =>   0..55
	//  56..92  =>  60..96
	//  93..96  =>  56..59
	// temperature-to-humidity map
	//  69..70  =>   0..1   =>   0..1 (in endpoints)
	//  ...

	return smallest;
}

int
main() {
	omp_set_num_threads(omp_get_max_threads());

	const std::filesystem::path filepath{"day5/data/5.in"};

	if (!std::filesystem::exists(filepath)) {
		std::cout << "fatal: file not found" << std::endl;
		return EXIT_FAILURE;
	}

	auto data = read_file(filepath);
	auto maps = almanac::parse_input_almanac_maps(*data);

	auto part_1_result = min_location_number(*maps);
	std::cout << "result (part one): " << part_1_result << std::endl;

	std::cout << std::endl << "----------" << std::endl;

	auto part_2_result = min_location_number_for_seed_range(*maps);
	std::cout << "result (part two): " << part_2_result << std::endl;

	return EXIT_SUCCESS;
}
