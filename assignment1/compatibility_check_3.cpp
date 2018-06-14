// Aim of this test case is to check:
//
// 1. Check if macro guards are present or not.
// 2. Deque.hpp should be self sufficient in terms
//    header files. 
// Compile with -Werror 


#include "Deque.hpp"
#undef Deque_DEFINE
#define Deque_DEFINE foo
#include "Deque.hpp"

int main(){
    return 0;
}
