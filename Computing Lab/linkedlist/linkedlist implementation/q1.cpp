#include <iostream>
#include "ll_alt.h"  // Include the header file for the linked list
// g++ -o q1 q1.cpp ll_implementation.o
extern "C" {
    #include "ll_implementation.c"  // Include the C file directly
}
using namespace std;
void middle(LIST *l){
    int slow = l->head;
    int fast = l->elements[l->head].next;
    while((slow!=(-1)) && (fast!=(-1))){
        slow = l->elements[slow].next;
        fast = l->elements[fast].next;
        if(fast!=-1)
        fast = l->elements[fast].next;
    }
    cout<<(l->elements[slow].data);
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

    middle(&myList);
    // Print the list
    //cout << "List after insertions:" << std::endl;
    //print_list(&myList);

    return 0;
}
