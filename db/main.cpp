#include <iostream>
#include <unordered_map>
#include "parser.h"
#include "trie.h"

using namespace std;

int main() {

    Parser parser;
    unordered_map<string, Trie> sets;
    string input;

    while (true) {

        cout << "\n> ";
        getline(cin, input);

        vector<string> results = parser.parse_command(input);
        string output = results[0];

        cout << output << endl;

        if (output.find("has been created") != string::npos) {
            string set_name = output.substr(4, output.find(" has") - 4);
            if (sets.count(set_name)) {
                cout << "Set " << set_name << " already exists" << endl;
            }
            else {
                sets[set_name] = Trie();
            }
        }

        else if (output.find("has been added to") != string::npos) {
            size_t pos1 = output.find(" has been added to ");
            string value = output.substr(0, pos1);
            string set_name = output.substr(pos1 + 19);
            sets[set_name].insert(value);
        }

        else if (output.find("Printing tree for") != string::npos) {
            string set_name = output.substr(18);
            if (sets.count(set_name))
                sets[set_name].print_tree();
            else
                cout << "Set not found.\n";
        }

        else if (output.find("Checking if") != string::npos) {
            size_t pos1 = output.find("Checking if ") + 12;
            size_t pos2 = output.find(" is in ");
            string value = output.substr(pos1, pos2 - pos1);
            string set_name = output.substr(pos2 + 7);

            if (sets.count(set_name))
                cout << (sets[set_name].contains(value) ? "TRUE" : "FALSE") << endl;
            else
                cout << "Set not found.\n";
        }

        else if (output.find("Searching in") != string::npos) {
            string set_name = output.substr(13);
            if (sets.count(set_name)) {
                auto words = sets[set_name].search();
                cout << "Elements in " << set_name << ": ";
                for (auto& w : words) cout << w << " ";
                cout << endl;
            }
            else {
                cout << "Set not found.\n";
            }
        }
    }

    return 0;
}
