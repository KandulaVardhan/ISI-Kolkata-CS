#include <iostream>
#include "ll_alt.h"  // Include the header file for the linked list
// g++ -o q1 q1.cpp ll_implementation.o
extern "C" {
    #include "ll_implementation.c"  // Include the C file directly
}
using namespace std;
int nthLast(LIST *l, int n){
    int count = (l->length)-n;
    int curr = (l->head);
    for(int i=0;i<count;i++){
        curr = (l->elements[curr].next);
    }
    return l->elements[curr].data;
}
int main() {
    LIST myList;

    // Initialize the list with a capacity of 5
    init_list(&myList, 5);

    // Insert some elements into the list
    insert(&myList, 0, 10);  // Insert 10 at index 0
    insert(&myList, 1, 20);  // Insert 20 at index 1
    insert(&myList, 2, 30);  // Insert 30 at index 2
    insert(&myList, 3, 35);  
    //insert(&myList, 4, 50);

    int n = 2;
    cout<<nthLast(&myList, n);
    // Print the list
    //cout << "List after insertions:" << std::endl;
    //print_list(&myList);

    return 0;
}
