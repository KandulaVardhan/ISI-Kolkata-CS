#include <bits/stdc++.h>
using namespace std;

int main() {
    // ===============================
    // 1️⃣ Declaration & Initialization
    // ===============================
    set<int> s;                         // Empty set
    set<int, greater<int>> sdesc;       // Descending order
    set<int> s2 = {1, 2, 3, 4, 5};      // Initialize using list

    cout << "Initial set s2: ";
    for (int x : s2) cout << x << " ";
    cout << "\n\n";

    // ===============================
    // 2️⃣ Insert elements (unique)
    // ===============================
    s.insert(10);
    s.insert(30);
    s.insert(20);
    s.insert(40);
    s.insert(10);  // Duplicate ignored

    cout << "Elements after insertion: ";
    for (int x : s) cout << x << " ";
    cout << "\n";

    // ===============================
    // 3️⃣ Using emplace (faster than insert)
    // ===============================
    s.emplace(25);
    s.emplace(5);

    cout << "After emplace: ";
    for (int x : s) cout << x << " ";
    cout << "\n";

    // ===============================
    // 4️⃣ Size, Empty, Clear
    // ===============================
    cout << "Size = " << s.size() << "\n";
    cout << "Is empty? " << (s.empty() ? "Yes" : "No") << "\n";

    // ===============================
    // 5️⃣ Erase elements
    // ===============================
    s.erase(25); // erase by value
    cout << "After erase(25): ";
    for (int x : s) cout << x << " ";
    cout << "\n";

    auto it = s.begin();
    advance(it, 2); // move iterator to 3rd element
    s.erase(it);    // erase by iterator
    cout << "After erase by iterator: ";
    for (int x : s) cout << x << " ";
    cout << "\n";

    // ===============================
    // 6️⃣ Find element
    // ===============================
    if (s.find(20) != s.end())
        cout << "20 found in set\n";
    else
        cout << "20 not found\n";

    // ===============================
    // 7️⃣ Count (useful for multiset)
    // ===============================
    cout << "Count of 10 = " << s.count(10) << "\n"; // 1 or 0 for set

    // ===============================
    // 8️⃣ Lower and Upper Bound
    // ===============================
    s = {5, 10, 15, 20, 25, 30};
    cout << "\nSet s: ";
    for (int x : s) cout << x << " ";
    cout << "\n";

    auto lb = s.lower_bound(15); // >= 15
    auto ub = s.upper_bound(15); // > 15
    if (lb != s.end()) cout << "lower_bound(15): " << *lb << "\n";
    if (ub != s.end()) cout << "upper_bound(15): " << *ub << "\n";

    // ===============================
    // 9️⃣ Swap sets
    // ===============================
    set<int> s3 = {100, 200};
    cout << "\nBefore swap: s3 = ";
    for (int x : s3) cout << x << " ";
    cout << "\n";

    s.swap(s3);

    cout << "After swap:\n";
    cout << "s = ";
    for (int x : s) cout << x << " ";
    cout << "\n";
    cout << "s3 = ";
    for (int x : s3) cout << x << " ";
    cout << "\n";

    // ===============================
    // 🔟 Iterate in reverse order
    // ===============================
    cout << "\nReverse order traversal of s3: ";
    for (auto it = s3.rbegin(); it != s3.rend(); ++it)
        cout << *it << " ";
    cout << "\n";

    // ===============================
    // 1️⃣1️⃣ Copy / Assign
    // ===============================
    set<int> copySet(s3.begin(), s3.end());
    cout << "Copy of s3: ";
    for (int x : copySet) cout << x << " ";
    cout << "\n";

    // ===============================
    // 1️⃣2️⃣ Custom Comparator
    // ===============================
    set<int, greater<int>> descSet = {10, 40, 20, 30};
    cout << "Descending order set: ";
    for (int x : descSet) cout << x << " ";
    cout << "\n";

    // ===============================
    // 1️⃣3️⃣ Multiset (allows duplicates)
    // ===============================
    multiset<int> ms = {10, 20, 20, 30, 30, 30};

    cout << "\nMultiset ms: ";
    for (int x : ms) cout << x << " ";
    cout << "\n";

    cout << "Count of 20 = " << ms.count(20) << "\n";
    ms.erase(ms.find(20)); // Erase one occurrence
    cout << "After erasing one 20: ";
    for (int x : ms) cout << x << " ";
    cout << "\n";

    // ===============================
    // 1️⃣4️⃣ Range Erase (iterator range)
    // ===============================
    auto start = ms.find(30);
    auto end = ms.end();
    ms.erase(start, end); // erase all 30s
    cout << "After erasing all 30s: ";
    for (int x : ms) cout << x << " ";
    cout << "\n";

    // ===============================
    // 1️⃣5️⃣ Clear
    // ===============================
    s.clear();
    cout << "\nAfter clear(), size of s = " << s.size() << "\n";

    cout << "\n✅ All set operations practiced successfully.\n";
    return 0;
}
