#include <string>
#include <cctype>
#include <unordered_map>
#include <fstream>
#include <iostream>

class CStringParser {
    public:
        std::string str;
        int len;
        int i = 0;

        CStringParser(std::string str) {
            this->str = str;
            len = str.length();
        }

        bool next_int(int &integer) {
            std::string buf = "";
            for (; i < len; i++) {
                char c = str.at(i);
                if (isdigit(c)) {
                    buf += c;
                } else if (buf != "") {
                    break;
                }
            }
            integer = buf != "" ? std::stoi(buf) : -1;
            return integer > -1;
        }

        bool next_word(std::string &buf) {
            buf = "";
            for (; i < len; i++) {
                char c = str.at(i);
                if (c == ' ' || c == ',' || c == ';') {
                    if (buf != "") {
                        break;
                    }
                    continue;
                }
                buf += c;
            }
            return buf != "" ? true : false;
        }
};

int main()
{
    std::unordered_map<std::string, int> bag = {
        { "red", 12 },
        { "green", 13 },
        { "blue", 14 }
    };
    std::fstream file_input("day2.input");
    if (!file_input.is_open()) {
        std::cout << "unable to open input file.\n";
        return 1;
    }
    std::string line;
    int sum = 0;
    int sum_pt2 = 0;
    while (std::getline(file_input, line)) {
        CStringParser parser(line);
        int game_id;
        bool possible = true;
        std::unordered_map<std::string, int> minimum_bag = {
            { "red", 0 },
            { "green", 0 },
            { "blue", 0 }
        };
        if (parser.next_int(game_id)) {
            int quantity;
            while (parser.next_int(quantity)) {
                std::string color;
                if (parser.next_word(color)) {
                    auto hand = bag.find(color);
                    if (hand != bag.end()) {
                        if (quantity > hand->second) {
                            possible = false;
                        }
                        auto minimum_hand = minimum_bag.find(color);
                        if (quantity > minimum_hand->second) {
                            minimum_hand->second = quantity;
                        }
                    }
                }
            }
            if (possible) {
                sum += game_id;
            }
            int product = 1;
            for (auto hand : minimum_bag) {
                product *= hand.second;
            }
            sum_pt2 += product;
        }
    }
    std::cout << "sum is " << sum << "\n";
    std::cout << "(part 2) sum is " << sum_pt2 << "\n";
    return 0;
}
