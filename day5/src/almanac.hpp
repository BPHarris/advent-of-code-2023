#pragma once

#include <memory>
#include <numeric>
#include <sstream>
#include <vector>

namespace almanac {

// NOTE
// I set many members to size_t as they are used as indicies... But that has led to all
// of the members being size_t, in hindsight they should be uint64_t with implicit casts
// to size_t. Too bad!

typedef struct input_map_rule {
		const size_t source;
		const size_t dest;
		const size_t range;

		inline size_t max_source_index() {
			return source + range - 1;
		};

		inline size_t max_dest_index() {
			return dest + range - 1;
		};

		inline bool is_in_range(const size_t index) {
			return source <= index && index <= max_source_index();
		};

		inline size_t map(const size_t index) {
			return dest - source + index;
		};
} input_map_rule_t;

typedef struct input_map {
		const std::vector<input_map_rule_t> rules;

		inline size_t map(const size_t index) {
			for (ssize_t i = rules.size() - 1; i >= 0; i--) {
				auto rule = rules.at(i);
				if (rule.is_in_range(index)) {
					return rule.map(index);
				}
			}
			return index;
		};
} input_map_t;

typedef struct input_almanac_maps {
		const std::vector<size_t> initial_seeds;
		const input_map_t seed_to_soil_map;
		const input_map_t soil_to_fertilizer_map;
		const input_map_t fertilizer_to_water_map;
		const input_map_t water_to_light_map;
		const input_map_t light_to_temperature_map;
		const input_map_t temperature_to_humidity_map;
		const input_map_t humidity_to_location_map;
} input_almanac_maps_t;

typedef struct seed_range {
		const size_t start;
		const size_t range;

		inline size_t end() {
			return start + range - 1;
		};
} seed_range_t;

std::unique_ptr<std::vector<size_t>>
parse_seeds(const std::string &line, const size_t offset = 0) {
	// "seeds: 79 14 55 13"
	// or
	// "79 14 55 13"
	// specified by offset
	auto seeds = std::make_unique<std::vector<size_t>>();

	std::istringstream stream(line.substr(offset));

	size_t seed;
	while (stream >> seed) {
		seeds->push_back(seed);
	}

	return seeds;
}

std::unique_ptr<std::vector<input_map_rule_t>>
parse_input_map_rules(const std::vector<std::string> &data, const size_t from,
                      size_t *upto) {
	// parse
	//  seed-to-soil map:
	//  50 98 2
	//  ...
	size_t index = from + 1;
	auto rules = std::make_unique<std::vector<input_map_rule_t>>();

	std::string line;
	do {
		line = data.at(index);
		auto seed_info = parse_seeds(line);

		if (seed_info->size() != 3) {
			std::cout << "fatal: expected exactly 3 seed numbers in map entry"
			          << std::endl;
			exit(EXIT_FAILURE);
		}

		rules->push_back({.source = seed_info->at(1),
		                  .dest = seed_info->at(0),
		                  .range = seed_info->at(2)});

		index++;
	} while (index < data.size() && data.at(index).size() > 0);

	*upto = index;

	return rules;
}

std::unique_ptr<input_almanac_maps_t>
parse_input_almanac_maps(const std::vector<std::string> &data) {
	// "seeds: 79 14 55 13"
	auto seeds = parse_seeds(data.at(0), std::string("seeds: ").length());

	// "seed-to-soil map:" starts on line index 2
	// then (++offset) consume the empty line between blocks
	size_t offset = 2;
	auto maps = std::make_unique<input_almanac_maps_t>(
	    input_almanac_maps_t{*seeds,
	                         {*parse_input_map_rules(data, offset, &offset)},
	                         {*parse_input_map_rules(data, (++offset), &offset)},
	                         {*parse_input_map_rules(data, (++offset), &offset)},
	                         {*parse_input_map_rules(data, (++offset), &offset)},
	                         {*parse_input_map_rules(data, (++offset), &offset)},
	                         {*parse_input_map_rules(data, (++offset), &offset)},
	                         {*parse_input_map_rules(data, (++offset), &offset)}});

	return maps;
}

size_t
follow_map_route(const input_almanac_maps_t &almanac_maps, const size_t start_idx) {
	auto idx = start_idx;

	auto seed_to_soil_map = almanac_maps.seed_to_soil_map;
	idx = seed_to_soil_map.map(idx);

	auto soil_to_fertilizer_map = almanac_maps.soil_to_fertilizer_map;
	idx = soil_to_fertilizer_map.map(idx);

	auto fertilizer_to_water_map = almanac_maps.fertilizer_to_water_map;
	idx = fertilizer_to_water_map.map(idx);

	auto water_to_light_map = almanac_maps.water_to_light_map;
	idx = water_to_light_map.map(idx);

	auto light_to_temperature_map = almanac_maps.light_to_temperature_map;
	idx = light_to_temperature_map.map(idx);

	auto temperature_to_humidity_map = almanac_maps.temperature_to_humidity_map;
	idx = temperature_to_humidity_map.map(idx);

	auto humidity_to_location_map = almanac_maps.humidity_to_location_map;
	idx = humidity_to_location_map.map(idx);

	return idx;
}

}   // namespace almanac
