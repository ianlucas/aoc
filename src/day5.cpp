#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>

std::unordered_map<std::string, std::vector<std::vector<unsigned long>>> maps;

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
        std::cout << "looping through " << i << "th group...\n";
        std::string last_print = "";
        for (unsigned long j = 0; j < length; j++) {
            auto seed = start + j;
            auto location = get_seed_location(seed);
            if (lowest_location_pt2 == -1 || lowest_location_pt2 > location) {
                lowest_location_pt2 = location;
            }
            std::ostringstream print_stream;
            print_stream << "progress: " << (j * 100) / length << "\n";
            if (print_stream.str() != last_print) {
                last_print = print_stream.str();
                std::cout << last_print;
            }
        }
    }
    std::cout << "lowest location is " << lowest_location << "\n";
    std::cout << "(part 2) lowest location is " << lowest_location_pt2 << "\n";
    return 0;
}
