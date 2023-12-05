#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

int main()
{
    std::string words[9] { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    std::fstream file_input("day1.input");
    if (!file_input.is_open()) {
        std::cout << "unable to load input file.\n";
        return 1;
    }
    int sum = 0;
    int sum_pt2 = 0;
    std::string line;
    while (std::getline(file_input, line)) {
        std::string first = "";
        std::string last = "";
        std::string first_pt2 = "";
        std::string last_pt2 = "";
        for (auto i = 0; i < line.length(); i++) {
            char c = line.at(i);
            if (isdigit(c)) {
                last = c;
                last_pt2 = c;
                if (first == "") {
                    first = c;
                }
                if (first_pt2 == "") {
                    first_pt2 = c;
                }
            } else {
                for (auto j = 0; j < 9; j++) {
                    std::string &word = words[j];
                    std::string digit = "";
                    if (word == line.substr(i, i + word.length())) {
                        digit = std::to_string(j + 1);
                    }
                    if (digit != "") {
                        last_pt2 = digit;
                        if (first_pt2 == "") {
                            first_pt2 = digit;
                        }
                        i++;
                        break;
                    }
                }
            }
        }
        sum += std::stoi(first) * 10 + std::stoi(last);
        sum_pt2 += std::stoi(first_pt2) * 10 + std::stoi(last_pt2);
    }
    std::cout << "sum is " << sum << "\n";
    std::cout << "(part 2) sum is " << sum_pt2 << "\n";
    return 0;
}
