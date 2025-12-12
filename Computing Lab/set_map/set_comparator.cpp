#include <bits/stdc++.h>
using namespace std;

// 1️⃣ Comparator struct for descending order
struct Descending {
    bool operator()(int a, int b) const {
        return a > b; // reverse default (ascending)
    }
};

// 2️⃣ Comparator for pair keys — sort by first ascending, second descending
struct PairComp {
    bool operator()(const pair<int,int>& a, const pair<int,int>& b) const {
        if (a.first == b.first)
            return a.second > b.second; // larger second first
        return a.first < b.first;       // smaller first first
    }
};

// 3️⃣ Custom struct example
struct Student {
    string name;
    int marks;
};

// Comparator for Student struct
struct StudentComp {
    bool operator()(const Student& a, const Student& b) const {
        if (a.marks == b.marks)
            return a.name < b.name; // if marks same → sort by name
        return a.marks > b.marks;   // higher marks first
    }
};

int main() {
    // =========================================
    // 1️⃣ Default set (ascending order)
    // =========================================
    set<int> s1 = {3, 1, 2};
    cout << "Set s1 (ascending):\n";
    for (int x : s1) cout << x << " ";
    cout << "\n\n";

    // =========================================
    // 2️⃣ Set with predefined comparator (descending)
    // =========================================
    set<int, greater<int>> s2 = {3, 1, 2};
    cout << "Set s2 (descending):\n";
    for (int x : s2) cout << x << " ";
    cout << "\n\n";

    // =========================================
    // 3️⃣ Set with custom comparator struct
    // =========================================
    set<int, Descending> s3 = {10, 5, 7};
    cout << "Set s3 (using Descending struct):\n";
    for (int x : s3) cout << x << " ";
    cout << "\n\n";

    // =========================================
    // 4️⃣ Set with lambda comparator
    // =========================================
    auto cmp = [](int a, int b) {
        // Even numbers first, then odd (ascending within each)
        if ((a % 2 == 0) && (b % 2 != 0)) return true;
        if ((a % 2 != 0) && (b % 2 == 0)) return false;
        return a < b;
    };
    set<int, decltype(cmp)> s4(cmp);
    s4.insert({5, 2, 8, 3});

    cout << "Set s4 (lambda comparator — evens first):\n";
    for (int x : s4) cout << x << " ";
    cout << "\n\n";

    // =========================================
    // 5️⃣ Set with pair elements
    // =========================================
    set<pair<int,int>, PairComp> s5;
    s5.insert({1,5});
    s5.insert({1,7});
    s5.insert({2,6});
    s5.insert({2,8});

    cout << "Set s5 (pairs — first asc, second desc):\n";
    for (auto &p : s5)
        cout << "(" << p.first << "," << p.second << ") ";
    cout << "\n\n";

    // =========================================
    // 6️⃣ Set with custom struct
    // =========================================
    set<Student, StudentComp> studentSet;
    studentSet.insert({"Vardhan", 95});
    studentSet.insert({"Ankit", 80});
    studentSet.insert({"Ravi", 95});
    studentSet.insert({"Neha", 85});

    cout << "Set studentSet (sorted by marks desc, name asc):\n";
    for (auto &s : studentSet)
        cout << s.name << " (" << s.marks << ")\n";
}
