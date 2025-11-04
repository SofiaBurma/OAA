#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

struct TrieNode {

    bool is_end = false;
    map<char, unique_ptr<TrieNode>> children;
};

class Trie {
private:
    unique_ptr<TrieNode> root;

    void collect_words(TrieNode* node, string current, vector<string>& result) const {
        if (node->is_end)
            result.push_back(current);
        for (auto& kv : node->children)
            collect_words(kv.second.get(), current + kv.first, result);
    }

public:

    Trie() {
        root = make_unique<TrieNode>();
    }

    void insert(const string& word) {
        TrieNode* current = root.get();
        for (char c : word) {
            if (!current->children.count(c)) {
                current->children[c] = make_unique<TrieNode>();
            }
            current = current->children[c].get();
        }
        current->is_end = true;
    }

    bool contains(const string& word) const {
        TrieNode* current = root.get();
        for (char c : word) {
            if (!current->children.count(c))
                return false;
            current = current->children.at(c).get();
        }
        return current->is_end;
    }

    void print_tree(TrieNode* node = nullptr, const string& prefix = "", bool is_last = true) const {
        if (node == nullptr) {
            cout << "[root]" << endl;
            node = root.get();
        }

        vector<pair<char, TrieNode*>> children;
        for (auto& kv : node->children)
            children.push_back({ kv.first, kv.second.get() });

        for (size_t i = 0; i < children.size(); ++i) {
            bool last_child = (i == children.size() - 1);
            string new_prefix;

            if (!prefix.empty()) {
                new_prefix = prefix.substr(0, prefix.size() - 1);
                new_prefix += (last_child ? "+--  " : "|-- ");
            }
            else {
                new_prefix = last_child ? " +-- " : "|-- ";
            }

            cout << new_prefix << children[i].first << endl;

            string child_prefix = (prefix.empty() ? "" : prefix) + (last_child ? "    " : " |   ");
            print_tree(children[i].second, child_prefix, last_child);
        }
    }

    vector<string> search() const {
        vector<string> result;
        collect_words(root.get(), "", result);
        return result;
    }
};

