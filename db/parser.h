#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
#include <cctype>

using namespace std;

class Parser {
public:

    vector<string> parse_command(const string& input) {
        vector<string> results;

        auto preprocess = [&](string input) -> string {

            size_t pos = input.find(';');

            if (pos == string::npos)
                throw runtime_error("Command must end with ';'");
            input = input.substr(0, pos + 1);

            while (!input.empty() && isspace((unsigned char)input.front())) 
                input.erase(input.begin());

            while (!input.empty() && isspace((unsigned char)input.back())) 
                input.pop_back();

            return input;
            };

        auto get_command_word = [](const string& cmd) {
            stringstream ss(cmd);
            string word;
            ss >> word;
            return word;
            };

        auto to_upper = [](const string& s) {
            string res = s;
            transform(res.begin(), res.end(), res.begin(), ::toupper);
            return res;
            };

        try {

            string cleaned = preprocess(input);
            string word = get_command_word(cleaned);
            string command_word = to_upper(word);

            if (command_word == "CREATE") { results.push_back(create(cleaned)); }

            else if (command_word == "INSERT") { results.push_back(insert(cleaned)); }

            else if (command_word == "CONTAINS") { results.push_back(contains(cleaned)); }

            else if (command_word == "SEARCH") { results.push_back(search(cleaned)); }

            else if (command_word == "PRINT_TREE") { results.push_back(print_tree(cleaned)); }

            else { throw runtime_error("Unknown command: " + command_word); }

        }

        catch (const exception& e) {
            results.push_back(e.what());
        }

        return results;
    }

private:

    string create(const string& cmd) {
        smatch match;
        regex re(R"(^\s*CREATE\s+([a-zA-Z][a-zA-Z0-9_]*)\s*;$)", regex::icase);

        if (regex_search(cmd, match, re)) {
            string set_name = match[1];
            return "Set " + set_name + " has been created";
        }
        throw runtime_error("The set_name is incorrect or missing");
    }

    string insert(const string& cmd) {
        smatch match;
        regex re(R"(^\s*INSERT\s+([a-zA-Z][a-zA-Z0-9_]*)\s*\"([^\"]*)\"\s*;$)", regex::icase);

        if (regex_search(cmd, match, re)) {
            string set_name = match[1];
            string value = match[2];
            return value + " has been added to " + set_name;
        }
        throw runtime_error("The set_name or value is incorrect or missing");
    }

    string contains(const string& cmd) {
        smatch match;
        regex re(R"(^\s*CONTAINS\s+([a-zA-Z][a-zA-Z0-9_]*)\s*\"([^\"]*)\"\s*;$)", regex::icase);

        if (regex_search(cmd, match, re)) {
            string set_name = match[1];
            string value = match[2];
            return " Checking if " + value + " is in " + set_name;
        }
        throw runtime_error("The set_name or value is incorrect or missing");
    }

    string print_tree(const string& cmd) {
        smatch match;
        regex re(R"(^\s*PRINT_TREE\s+([a-zA-Z][a-zA-Z0-9_]*)\s*;$)", regex::icase);

        if (regex_search(cmd, match, re)) {
            string set_name = match[1];
            return "Printing tree for: " + set_name;
        }
        throw runtime_error("The set_name is incorrect or missing");
    }

    string search(const string& cmd) {
        smatch match;
        regex re_search(R"(^\s*SEARCH\s+([a-zA-Z][a-zA-Z0-9_]*)\s*(WHERE\s+(.+?))?\s*(ASC|DESC)?\s*;$)", regex::icase);
        if (regex_search(cmd, match, re_search)) {
            string set_name = match[1];
            string query = match[3];          
            string order = match[4];          

            if (!order.empty()) {
                transform(order.begin(), order.end(), order.begin(), ::toupper);
            }
            else {
                order = "ASC";
            }

            if (!query.empty()) {
                
                smatch m_between;
                regex re_between(R"(BETWEEN\s*\"([^\"]*)\"\s*,\s*\"([^\"]*)\")", regex::icase);
                if (regex_search(query, m_between, re_between)) {
                    string from = m_between[1];
                    string to = m_between[2];
                    return "Search_between " + set_name + " " + from + " " + to + " " + order;
                }

                smatch m_match;
                regex re_match(R"(MATCH\s*\"([^\"]*)\")", regex::icase);
                if (regex_search(query, m_match, re_match)) {
                    string pattern = m_match[1];
                    return "Search_match " + set_name + " " + pattern + " " + order;
                }

                throw runtime_error("The condition after WHERE is incorrect");
            }
            else {

                return "Search " + set_name + " " + order;
            }
        }
        throw runtime_error("The set_name is incorrect or missing");
    }
};