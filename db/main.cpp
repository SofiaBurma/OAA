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
            string set_name = output.substr(19);
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

        else if (output.find("Search") == 0) {

            vector<string> words;
            string order = "ASC"; 

            if (output.find("Search_between ") == 0) {
                size_t pos1 = 15; 
                size_t pos2 = output.find(' ', pos1);
                size_t pos3 = output.find(' ', pos2 + 1);
                size_t pos4 = output.find(' ', pos3 + 1);

                string set_name = output.substr(pos1, pos2 - pos1);
                string from = output.substr(pos2 + 1, pos3 - pos2 - 1);
                string to = output.substr(pos3 + 1, (pos4 == string::npos ? string::npos : pos4 - pos3 - 1));
                if (pos4 != string::npos) order = output.substr(pos4 + 1);

                if (!sets.count(set_name)) {
                    cout << "Set not found.\n";
                    continue;
                }

                words = sets[set_name].search_between(from, to);
            }
            else if (output.find("Search_match ") == 0) {
                size_t pos1 = 13; 
                size_t pos2 = output.find(' ', pos1);
                size_t pos3 = output.find(' ', pos2 + 1); 

                string set_name = output.substr(pos1, pos2 - pos1);
                string pattern = output.substr(pos2 + 1, (pos3 == string::npos ? string::npos : pos3 - pos2 - 1));
                if (pos3 != string::npos) order = output.substr(pos3 + 1);

                if (!sets.count(set_name)) {
                    cout << "Set not found.\n";
                    continue;
                }

                try {
                    words = sets[set_name].search_match(pattern);
                }
                catch (const exception& e) {
                    cout << e.what() << endl;
                    continue;
                }
            }
            else if (output.find("Search ") == 0) {
                size_t pos1 = 7;
                size_t pos2 = output.find(' ', pos1);
                string set_name = output.substr(pos1, pos2 - pos1);
                if (pos2 != string::npos) order = output.substr(pos2 + 1);

                if (!sets.count(set_name)) {
                    cout << "Set not found.\n";
                    continue;
                }

                words = sets[set_name].search_all();
            }

            if (order == "ASC") sort(words.begin(), words.end());
            else sort(words.begin(), words.end(), greater<string>());

            cout << "Elements in " << output.substr(7, output.find(' ', 7) - 7) << ": ";
            for (auto& w : words) cout << w << " ";
            cout << endl;
        }

    }

    return 0;
}
