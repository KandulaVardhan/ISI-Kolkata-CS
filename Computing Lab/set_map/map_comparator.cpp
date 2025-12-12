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
            return a.second > b.second;  // larger second first
        return a.first < b.first;        // smaller first first
    }
};

// 3️⃣ Custom struct key example
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
    // 1️⃣ Default map (ascending order of keys)
    // =========================================
    map<int, string> m1;
    m1[3] = "three";
    m1[1] = "one";
    m1[2] = "two";

    cout << "Map m1 (ascending):\n";
    for (auto &[k, v] : m1)
        cout << k << " -> " << v << "\n";
    cout << "\n";

    // =========================================
    // 2️⃣ Map with predefined comparator (descending)
    // =========================================
    map<int, string, greater<int>> m2 = {{3, "three"}, {1, "one"}, {2, "two"}};
    cout << "Map m2 (descending):\n";
    for (auto &[k, v] : m2)
        cout << k << " -> " << v << "\n";
    cout << "\n";

    // =========================================
    // 3️⃣ Map with custom comparator struct
    // =========================================
    map<int, string, Descending> m3;
    m3[10] = "ten";
    m3[5]  = "five";
    m3[7]  = "seven";

    cout << "Map m3 (using Descending struct):\n";
    for (auto &[k, v] : m3)
        cout << k << " -> " << v << "\n";
    cout << "\n";

    // =========================================
    // 4️⃣ Map with lambda comparator
    // =========================================
    auto cmp = [](int a, int b) {
        // Sort by even numbers first, then odd (ascending within groups)
        if ((a % 2 == 0) && (b % 2 != 0)) return true;
        if ((a % 2 != 0) && (b % 2 == 0)) return false;
        return a < b;
    };

    map<int, string, decltype(cmp)> m4(cmp);
    m4[5] = "odd";
    m4[2] = "even";
    m4[8] = "even";
    m4[3] = "odd";

    cout << "Map m4 (lambda comparator — evens first):\n";
    for (auto &[k, v] : m4)
        cout << k << " -> " << v << "\n";
    cout << "\n";

    // =========================================
    // 5️⃣ Map with pair keys
    // =========================================
    map<pair<int,int>, string, PairComp> m5;
    m5[{1,5}] = "A";
    m5[{1,7}] = "B";
    m5[{2,6}] = "C";
    m5[{2,8}] = "D";

    cout << "Map m5 (pair keys — first asc, second desc):\n";
    for (auto &[k, v] : m5)
        cout << "(" << k.first << "," << k.second << ") -> " << v << "\n";
    cout << "\n";

    // =========================================
    // 6️⃣ Map with custom struct key
    // =========================================
    map<Student, int, StudentComp> studentMap;
    studentMap[{"Vardhan", 95}] = 1;
    studentMap[{"Ankit", 80}] = 2;
    studentMap[{"Ravi", 95}] = 3;
    studentMap[{"Neha", 85}] = 4;

    cout << "Map studentMap (keys sorted by marks desc, name asc):\n";
    for (auto &[s, val] : studentMap)
        cout << s.name << " (" << s.marks << ") -> id " << val << "\n";
}
