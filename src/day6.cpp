#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

int main()
{
    std::fstream input_file("day6.input");
    if (!input_file.is_open()) {
        std::cout << "unable to open input file.\n";        
        return 1;
    }

    std::vector<int> times;
    std::vector<int> distances;
    std::vector<long> times_pt2;
    std::vector<long> distances_pt2;

    bool is_parsing_distance_record = false;
    std::string line;
    while (std::getline(input_file, line)) {
        std::string buffer = "";
        std::string buffer_pt2 = "";
        for (int i = 0; i < line.length(); i++) {
            bool eol = i == line.length() - 1;
            auto c = line.at(i);
            if (isdigit(c)) {
                buffer += c;
                buffer_pt2 += c;
            }
            if ((!isdigit(c) || eol) && buffer != "") {
                int number = std::stoi(buffer);
                buffer = "";
                if (is_parsing_distance_record) {
                    distances.push_back(number);
                } else {
                    times.push_back(number);
                }
            }
            if (eol && buffer_pt2 != "") {
                auto number = std::stol(buffer_pt2);
                if (is_parsing_distance_record) {
                    distances_pt2.push_back(number);
                } else {
                    times_pt2.push_back(number);
                }
            }
        }
        is_parsing_distance_record = true;
    }

    std::vector<int> n_ways_to_win;
    for (int i = 0; i < times.size(); i++) {
        auto time = times[i];
        auto distance_record = distances[i];
        int ways_to_win = 0;
        for (int j = 0; j < time; j++) {
            auto speed = j;
            auto distance = speed * (time - j);
            if (distance > distance_record) {
                ways_to_win += 1;
            }
        }
        n_ways_to_win.push_back(ways_to_win);
    }
    int answer = 1;
    for (auto n_way : n_ways_to_win) {
        answer *= n_way;
    }
    auto time = times_pt2[0];
    auto distance_record = distances_pt2[0];
    long answer_pt2 = 0;
    for (long j = 0; j < time; j++) {
        auto speed = j;
        auto distance = speed * (time - j);
        if (distance > distance_record) {
            answer_pt2 += 1;
        }
    }
    std::cout << "margin of error is " << answer << "\n";
    std::cout << "(part 2) number of ways to win is " << answer_pt2 << "\n";
    return 0;
}