#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>

std::unordered_map<std::string, std::vector<std::vector<unsigned long>>> maps;
std::vector<std::string> map_order = { "seed-to-soil", "soil-to-fertilizer", "fertilizer-to-water", "water-to-light", "light-to-temperature", "temperature-to-humidity", "humidity-to-location" };

unsigned long lookup(std::string map_key, unsigned long source)
{
    if (maps.find(map_key) == maps.end()) {
        return source;
    }
    auto ranges = maps.find(map_key)->second;
    for (auto range : ranges) {
        auto destination_range_start = range[0];
        auto source_range_start = range[1];
        auto range_length = range[2];
        auto source_range_end = source_range_start + range_length;
        if (source >= source_range_start && source < source_range_end) {
            auto diff = source - source_range_start;
            return destination_range_start + diff;
        }
    }
    return source;
}

unsigned long get_seed_location(unsigned long seed)
{
    auto soil = lookup("seed-to-soil", seed);
    auto fertilizer = lookup("soil-to-fertilizer", soil);
    auto water = lookup("fertilizer-to-water", fertilizer);
    auto light = lookup("water-to-light", water);
    auto temperature = lookup("light-to-temperature", light);
    auto humidity = lookup("temperature-to-humidity", temperature);
    return lookup("humidity-to-location", humidity);
}

int main()
{
    std::fstream input_file("day5.input");
    if (!input_file.is_open()) {
        std::cout << "unable to open input file.\n";
        return 1;
    }
    std::string line;
    std::vector<unsigned long> seeds;
    std::string map_key = "";
    while (std::getline(input_file, line)) {
        bool parsing_seeds = line.find("seeds") != std::string::npos;
        bool parsing_map = line.find(" map:") != std::string::npos;
        if (parsing_map) {
            map_key = line.replace(line.find(" map:"), 5, "");
            continue;
        }
        std::vector<unsigned long> numbers;
        std::string buffer = "";
        for (int i = 0; i < line.length(); i++) {
            bool eol = i == line.length() - 1;
            char c = line.at(i);
            if (isdigit(c)) {
                buffer += c;
            }
            if ((!isdigit(c) || eol) && buffer != "") {
                unsigned long number = std::stoul(buffer);
                buffer = "";
                if (parsing_seeds) {
                    seeds.push_back(number);
                } else {
                    numbers.push_back(number);
                }
            }
        }
        if (map_key != "" && numbers.size() > 0) {
            if (numbers.size() != 3) {
                std::cout << map_key << ": expected 3 numbers, got " << numbers.size() << ".\n";
                return 1;
            }
            if (maps.find(map_key) == maps.end()) {
                maps.insert({ map_key, std::vector<std::vector<unsigned long>>() });
            }
            maps.find(map_key)->second.push_back(numbers);
            numbers.clear();
        }
    }
    unsigned long lowest_location = -1;
    unsigned long lowest_location_pt2 = -1;
    for (auto seed : seeds) {
        auto location = get_seed_location(seed);
        if (lowest_location == -1 || lowest_location > location) {
            lowest_location = location;
        }
    }
    for (int i = 0; i < seeds.size(); i += 2) {
        auto start = seeds[i];
        auto length = seeds[i + 1];
        std::vector<std::vector<unsigned long>> source_ranges = { { start, start + length - 1} };
        for (auto map_key : map_order) {
            auto map = maps.find(map_key);
            if (map == maps.end()) {
                throw std::invalid_argument("map not found!");
            }
            auto ranges = source_ranges;
            source_ranges.clear();
            while (ranges.size() > 0) {
                auto range = ranges.back();
                ranges.pop_back();
                auto start = range[0];
                auto end = range[1];
                auto length = end - start;
                bool added_source_ranges = false; 
                for (auto map_range : map->second) {
                    auto destination_range_start = map_range[0];
                    auto source_range_start = map_range[1];
                    auto range_length = map_range[2];
                    auto source_range_end = source_range_start + range_length - 1;
                    auto destination_range_end = source_range_start + range_length - 1;
                    if (start >= source_range_start && start < source_range_end) {
                        auto start_offset = start - source_range_start;
                        long end_diff = source_range_end - end;
                        if (end_diff >= 0) {
                            auto a = destination_range_start + start_offset;
                            auto b = destination_range_start + start_offset + length;
                            source_ranges.push_back({ a, b });
                        } else {
                            auto a = destination_range_start + start_offset;
                            auto b = destination_range_start + start_offset + length + end_diff;
                            source_ranges.push_back({ a, b });
                            auto c = start + (b - a) + 1;
                            auto d = end;
                            ranges.push_back({ c, d });
                        }
                        added_source_ranges = true;
                        break;
                    }
                }
                if (!added_source_ranges) {
                    source_ranges.push_back({ start, end });
                }
            }
        }
        for (auto range : source_ranges) {
            auto location = range[0];
            if (lowest_location_pt2 == -1 || lowest_location_pt2 > location) {
                lowest_location_pt2 = location;
            }
        }
    }
    std::cout << "lowest location is " << lowest_location << "\n";
    std::cout << "(part 2) lowest location is " << lowest_location_pt2 << "\n";
    return 0;
}
