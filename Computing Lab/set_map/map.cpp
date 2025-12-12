#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===============================
    // 1️⃣ Declaration & Initialization
    // ===============================
    map<int, string> m; // empty map (sorted ascending by key)
    map<int, string, greater<int>> mdesc; // descending order of keys
    map<int, string> m2 = {{1, "One"}, {2, "Two"}, {3, "Three"}};

    cout << "Initial map m2:\n";
    for (auto &p : m2)
        cout << p.first << " => " << p.second << "\n";
    cout << "\n";

    // ===============================
    // 2️⃣ Insertion
    // ===============================
    m.insert({10, "Ten"});
    m.insert({30, "Thirty"});
    m.insert({20, "Twenty"});
    m.insert({10, "Duplicate"}); // ignored because key exists

    // Emplace (faster than insert)
    m.emplace(25, "Twenty Five");
    m.emplace(5, "Five");

    cout << "After insertion:\n";
    for (auto &p : m)
        cout << p.first << " => " << p.second << "\n";
    cout << "\n";

    // ===============================
    // 3️⃣ Access using []
    // ===============================
    m[15] = "Fifteen";      // inserts if key doesn’t exist
    m[20] = "Updated 20";   // overwrites value for existing key

    cout << "After using [] operator:\n";
    for (auto &p : m)
        cout << p.first << " => " << p.second << "\n";
    cout << "\n";

    // ===============================
    // 4️⃣ Check size, empty
    // ===============================
    cout << "Size = " << m.size() << ", Empty? " << (m.empty() ? "Yes" : "No") << "\n\n";

    // ===============================
    // 5️⃣ Find, Count
    // ===============================
    int key = 20;
    auto it = m.find(key);
    if (it != m.end())
        cout << key << " found => " << it->second << "\n";
    else
        cout << key << " not found\n";

    cout << "Count(20) = " << m.count(20) << "\n\n";

    // ===============================
    // 6️⃣ Erase operations
    // ===============================
    m.erase(25); // erase by key
    cout << "After erase(25):\n";
    for (auto &p : m) cout << p.first << " => " << p.second << "\n";
    cout << "\n";

    it = m.begin();
    advance(it, 2);
    m.erase(it); // erase by iterator
    cout << "After erase by iterator:\n";
    for (auto &p : m) cout << p.first << " => " << p.second << "\n";
    cout << "\n";

    // ===============================
    // 7️⃣ lower_bound / upper_bound
    // ===============================
    m = {{10, "Ten"}, {20, "Twenty"}, {30, "Thirty"}, {40, "Forty"}};

    auto lb = m.lower_bound(25); // >= 25
    auto ub = m.upper_bound(25); // > 25

    cout << "lower_bound(25): ";
    if (lb != m.end()) cout << lb->first << " => " << lb->second << "\n"; else cout << "none\n";
    cout << "upper_bound(25): ";
    if (ub != m.end()) cout << ub->first << " => " << ub->second << "\n"; else cout << "none\n";
    cout << "\n";

    // ===============================
    // 8️⃣ Swap two maps
    // ===============================
    map<int, string> m3 = {{100, "Hundred"}, {200, "Two Hundred"}};
    cout << "Before swap, m3:\n";
    for (auto &p : m3) cout << p.first << " => " << p.second << "\n";

    m.swap(m3);
    cout << "\nAfter swap:\n";
    cout << "m: ";
    for (auto &p : m) cout << p.first << " => " << p.second << ", ";
    cout << "\n";
    cout << "m3: ";
    for (auto &p : m3) cout << p.first << " => " << p.second << ", ";
    cout << "\n\n";

    // ===============================
    // 9️⃣ Iterate in reverse
    // ===============================
    cout << "Reverse order of m:\n";
    for (auto it = m.rbegin(); it != m.rend(); ++it)
        cout << it->first << " => " << it->second << "\n";
    cout << "\n";

    // ===============================
    // 🔟 Copy / Range constructor
    // ===============================
    map<int, string> copyMap(m3.begin(), m3.end());
    cout << "Copied map from m3:\n";
    for (auto &p : copyMap) cout << p.first << " => " << p.second << "\n";
    cout << "\n";

    // ===============================
    // 1️⃣1️⃣ Clear
    // ===============================
    m3.clear();
    cout << "After clear(), m3 size = " << m3.size() << "\n\n";

    // ===============================
    // 1️⃣2️⃣ Custom comparator (descending)
    // ===============================
    map<int, string, greater<int>> descMap = {{1, "A"}, {3, "C"}, {2, "B"}};
    cout << "Descending map:\n";
    for (auto &p : descMap) cout << p.first << " => " << p.second << "\n";
    cout << "\n";

    // ===============================
    // 1️⃣3️⃣ Multimap (duplicate keys)
    // ===============================
    multimap<int, string> mm;
    mm.insert({1, "A"});
    mm.insert({1, "Alpha"});
    mm.insert({2, "B"});
    mm.insert({3, "C"});
    mm.insert({2, "Beta"});

    cout << "Multimap contents:\n";
    for (auto &p : mm) cout << p.first << " => " << p.second << "\n";
    cout << "\n";

    cout << "Count of key 2 = " << mm.count(2) << "\n";

    // Equal range: get all values with same key
    auto range = mm.equal_range(2);
    cout << "All values for key 2: ";
    for (auto it = range.first; it != range.second; ++it)
        cout << it->second << " ";
    cout << "\n";

    // Erase all with key 1
    mm.erase(1);
    cout << "After erase(1): ";
    for (auto &p : mm) cout << p.first << " => " << p.second << ", ";
    cout << "\n";

    // ===============================
    // 1️⃣4️⃣ Summary
    // ===============================
    cout << "\n✅ All map and multimap operations practiced successfully.\n";

    return 0;
}
