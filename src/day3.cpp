#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>
#include <unordered_map>
#include <set>

int main()
{
    std::fstream input_file("day3.input");
    if (!input_file.is_open()) {
        std::cout << "unable to open input file.\n";
        return 1;
    }
    std::string schematic = "";
    std::string line;
    int height = 0;
    int width = 0;
    while (std::getline(input_file, line)) {
        schematic += line;
        height += 1;
        if (width == 0) {
            width = line.length();
        }
    }
    if (width == 0 || height == 0) {
        std::cout << "invalid schematic.\n";
        return 1;
    }
    int addends[] = { -width - 1, -width, -width + 1, -1, 1, width - 1, width, width + 1 };
    int len = schematic.length();
    std::string buffer = "";
    std::set<int> adjacents_to_check;
    std::unordered_map<int, std::vector<int>> gear_map;
    int sum = 0;
    int sum_pt2 = 0;
    for (int i = 0; i < len; i++) {
        char c = schematic.at(i);
        if (isdigit(c)) {
            buffer += c;
            for (int addend : addends) {
                int adjacent_index = i + addend;
                if (adjacent_index >= 0 && adjacent_index < len) {
                    if (
                        adjacents_to_check.find(adjacent_index) == adjacents_to_check.end()
                        && !isdigit(schematic.at(adjacent_index))
                    ) {
                        adjacents_to_check.insert(adjacent_index);
                    }
                }
            }
        }
        if ((!isdigit(c) || (i + 1) % width == 0) && buffer != "") {
            int number = std::stoi(buffer);
            bool add_to_sum = false;
            for (int adjacent_index : adjacents_to_check) {
                char c = schematic.at(adjacent_index);
                if (c != '.') {
                    add_to_sum = true;
                }
                if (c == '*') {
                    if (gear_map.count(adjacent_index) == 0) {
                        gear_map.insert({ adjacent_index, std::vector<int>() });
                    }
                    auto gear = gear_map.find(adjacent_index);
                    if (gear != gear_map.end()) {
                        gear->second.push_back(number);
                    }
                }
            }
            if (add_to_sum) {
                sum += number;
            }
            buffer = "";
            adjacents_to_check.clear();
        }
    }
    for (auto gear : gear_map) {
        if (gear.second.size() == 2) {
            sum_pt2 += gear.second.at(0) * gear.second.at(1);
        }
    }
    std::cout << "size: " << width << "x" << height << "\n";
    std::cout << "sum is " << sum << "\n";
    std::cout << "(part 2) sum is " << sum_pt2 << "\n";
    return 0;
}
