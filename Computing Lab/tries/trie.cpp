#include <bits/stdc++.h>
using namespace std;

// ============================
// Trie Node Definition
// ============================
struct TrieNode {
    TrieNode* children[26];
    int wordCount;     // how many times this exact word appears
    int prefixCount;   // how many words share this prefix

    TrieNode() {
        wordCount = 0;
        prefixCount = 0;
        for (int i = 0; i < 26; i++)
            children[i] = nullptr;
    }
};

// ============================
// Trie Class
// ============================
class Trie {
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    // ========================
    // Insert a Word
    // ========================
    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!isalpha(c)) continue; // skip non-alphabets
            c = tolower(c);
            int idx = c - 'a';
            if (!node->children[idx])
                node->children[idx] = new TrieNode();
            node = node->children[idx];
            node->prefixCount++;
        }
        node->wordCount++;
    }

    // ========================
    // Search Word Frequency
    // ========================
    int search(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!isalpha(c)) continue;
            c = tolower(c);
            int idx = c - 'a';
            if (!node->children[idx])
                return 0;
            node = node->children[idx];
        }
        return node->wordCount;
    }

    // ========================
    // Count Words with Given Prefix
    // ========================
    int countPrefix(const string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!isalpha(c)) continue;
            c = tolower(c);
            int idx = c - 'a';
            if (!node->children[idx])
                return 0;
            node = node->children[idx];
        }
        return node->prefixCount;
    }

    // ========================
    // Delete a Word
    // ========================
    bool deleteWord(const string& word) {
        return deleteHelper(root, word, 0);
    }

    // ========================
    // DFS Traversal (Print all words)
    // ========================
    void dfsTraversal() {
        string word;
        cout << "\nAll words in the Trie:\n";
        dfs(root, word);
    }

private:
    bool deleteHelper(TrieNode* node, const string& word, int depth) {
        if (!node)
            return false;

        if (depth == (int)word.size()) {
            if (node->wordCount == 0)
                return false; // word not found
            node->wordCount--;
            node->prefixCount--;
            return node->wordCount == 0 && isEmpty(node);
        }
        
        char c = tolower(word[depth]);
        int idx = c - 'a';
        if (!node->children[idx])
            return false;

        bool shouldDeleteChild = deleteHelper(node->children[idx], word, depth + 1);

        node->children[idx]->prefixCount--;
        if (node->children[idx]->prefixCount < 0)
            node->children[idx]->prefixCount = 0;

        if (shouldDeleteChild) {
            delete node->children[idx];
            node->children[idx] = nullptr;
            return node->wordCount == 0 && isEmpty(node);
        }

        return false;
    }

    bool isEmpty(TrieNode* node) {
        for (int i = 0; i < 26; i++)
            if (node->children[i])
                return false;
        return true;
    }

    void dfs(TrieNode* node, string& word) {
        if (!node) return;

        if (node->wordCount > 0)
            cout << word << " (" << node->wordCount << ")\n";

        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                word.push_back('a' + i);
                dfs(node->children[i], word);
                word.pop_back();
            }
        }
    }
};

// ============================
// Test Code
// ============================
int main() {
    Trie trie;

    trie.insert("apple");
    trie.insert("app");
    trie.insert("ape");
    trie.insert("banana");
    trie.insert("band");
    trie.insert("band");
    trie.insert("bat");

    cout << "apple count: " << trie.search("apple") << "\n"; // 1
    cout << "band count: " << trie.search("band") << "\n";   // 2
    cout << "Words with prefix 'ba': " << trie.countPrefix("ba") << "\n"; // 4

    cout << "\nDeleting 'band' once...\n";
    trie.deleteWord("band");

    cout << "band count: " << trie.search("band") << "\n";   // 1
    cout << "Words with prefix 'ban': " << trie.countPrefix("ban") << "\n"; // 2

    trie.dfsTraversal();

    return 0;
}
