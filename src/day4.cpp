#include <fstream>
#include <iostream>
#include <cctype>
#include <set>
#include <unordered_map>
#include <cmath>

const int GET_CARD_ID = 0;
const int GATHER_WINNING_NUMBERS = 1;
const int GATHER_NUMBERS_YOU_HAVE = 2;

int main()
{
    std::fstream file_input("day4.input");
    if (!file_input.is_open()) {
        std::cout << "unable to open input file.\n";
        return 1;
    }
    std::string line;
    int sum = 0;
    int sum_pt2 = 0;
    int total_cards = 0;
    std::unordered_map<int, int> card_store;
    while (std::getline(file_input, line)) {
        total_cards += 1;
        std::string buffer = "";
        int state = GET_CARD_ID;
        int number;
        int card_num;
        std::set<int> winning_numbers;
        int matching_numbers = 0;
        for (auto i = 0; i < line.length(); i++) {
            char c = line.at(i);
            if (isdigit(c)) {
                buffer += c;
                if (i != line.length() - 1) {
                    continue;
                }
            }
            if (buffer != "") {
                number = std::stoi(buffer);
                switch (state) {
                    case GET_CARD_ID:
                        card_num = number;
                        break;
                    case GATHER_WINNING_NUMBERS:
                        if (winning_numbers.find(number) == winning_numbers.end()) {
                            winning_numbers.insert(number);
                        }
                        break;
                    case GATHER_NUMBERS_YOU_HAVE:
                        if (winning_numbers.find(number) != winning_numbers.end()) {
                            matching_numbers += 1;
                        }
                        break;
                }
                buffer = "";
            }
            if (c == ':') {
                state = GATHER_WINNING_NUMBERS;
            }
            if (c == '|') {
                state = GATHER_NUMBERS_YOU_HAVE;
            }
        }
        int at_hand = 1 + (
            card_store.find(card_num) != card_store.end()
            ? card_store.find(card_num)->second
            : 0
        );
        for (auto j = 0; j < at_hand; j++) {
            for (auto k = 0; k < matching_numbers; k++) {
                int copy_card_num = card_num + k + 1;
                if (card_store.find(copy_card_num) == card_store.end()) {
                    card_store.insert({ copy_card_num, 0 });
                }
                card_store.find(copy_card_num)->second += 1;
            }
        }
        sum += matching_numbers > 0 ? pow(2, matching_numbers - 1) : 0;
    }
    sum_pt2 += total_cards;
    for (auto card : card_store) {
        sum_pt2 += card.second;
    }
    std::cout << "sum is " << sum << "\n";
    std::cout << "(part 2) sum is " << sum_pt2 << "\n";
    file_input.close();
    return 0;
}
