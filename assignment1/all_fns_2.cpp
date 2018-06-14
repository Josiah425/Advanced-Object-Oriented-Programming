#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <random>
#include <unistd.h>
#include "Deque.hpp"

struct MyClass {
    int id;
  char name[10];
};

bool
MyClass_less_by_id(const MyClass &o1, const MyClass &o2) {
    return o1.id < o2.id;
}

void
MyClass_print(const MyClass *o) {
    printf("%d\n", o->id);
    printf("%s\n", o->name);
}

Deque_DEFINE(MyClass)

/*
 * Test for int.
 */

    bool
    int_less(const int &o1, const int &o2) {
        return o1 < o2;
    }
Deque_DEFINE(int)

int
main() {
    FILE *devnull = fopen("/dev/null", "w");
    assert(devnull != 0);

    // Test equality.  It is undefined behavior if the two deques were constructed with different
    // comparison functions.
    {
        Deque_int deq1, deq2;
        Deque_int_ctor(&deq1, int_less);
        Deque_int_ctor(&deq2, int_less);

        deq1.push_back(&deq1, 1);
        deq1.push_back(&deq1, 2);
        deq1.push_back(&deq1, 3);
        deq2.push_front(&deq2, 3);
        deq2.push_front(&deq2, 2);
        deq2.push_front(&deq2, 1);

        assert(Deque_int_equal(deq1, deq2));

        deq1.pop_back(&deq1);
        assert(!Deque_int_equal(deq1, deq2));
        deq1.push_back(&deq1, 4);
        assert(!Deque_int_equal(deq1, deq2));

        deq1.dtor(&deq1);
        deq2.dtor(&deq2);
    }

}
