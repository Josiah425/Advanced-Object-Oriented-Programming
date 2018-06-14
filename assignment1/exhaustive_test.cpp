#include<deque>
#include"Deque.hpp"
#include<cassert>
#include<cstdlib>
#include<iostream>
#include<algorithm>

#define SEED 32
#define MAX_ITERATIONS 100000
#define MAX_ELEM 1000
#define NUM_OPTIONS 2

bool int_less(const int &o1, const int &o2) {
        return o1<o2;
}

Deque_DEFINE(int)

void pushElem(std::deque<int>& stlDeq,Deque_int& myDeq,int elemCount){
    for(int i=0;i<elemCount;i++){

        int gen = rand()%2;
        if(gen == 0){
            int elem = rand()%MAX_ELEM;
            stlDeq.push_back(elem);
            myDeq.push_back(&myDeq,elem);
        }
        else{
            int elem = rand()%MAX_ELEM;
            stlDeq.push_front(elem);
            myDeq.push_front(&myDeq,elem);
        }
    }
}

void check(std::deque<int>& stlDeq,Deque_int& myDeq){

    assert(stlDeq.size() == myDeq.size(&myDeq));

    // Check front and back 
    if(stlDeq.size() > 0){
        assert(stlDeq.front() == myDeq.front(&myDeq));
        assert(stlDeq.back() == myDeq.back(&myDeq));
    }
}

int main(){
    std::deque<int> stlDeq;
    Deque_int myDeq;
    Deque_int_ctor(&myDeq, int_less);

    srand(SEED);
    for(int i=0;i<MAX_ITERATIONS;i++){

        int currentOption = rand()%7;

        if(stlDeq.size() == 0){
            assert(stlDeq.size() == myDeq.size(&myDeq));
            int elemCount = (rand()%(MAX_ELEM-1))+1;
            pushElem(stlDeq,myDeq,elemCount);
            assert(stlDeq.size() == myDeq.size(&myDeq));
        }

        check(stlDeq,myDeq);

        // push few elements
        if(currentOption == 0){
            int elemCount = rand()%100;
            pushElem(stlDeq,myDeq,elemCount);
            assert(stlDeq.size() == myDeq.size(&myDeq));
        }
        else if(currentOption == 1){ //check at option
            assert(myDeq.size(&myDeq) != 0);
            int idx = rand()%myDeq.size(&myDeq);

            for(int i=0;i<idx;++i){
                if(stlDeq.at(idx) != myDeq.at(&myDeq, idx)){
                    std::cout << idx << " ";
                }
            }

            assert(stlDeq.at(idx) == myDeq.at(&myDeq, idx));
        }
        else if(currentOption == 2){ // empty option
            assert(myDeq.empty(&myDeq) == stlDeq.empty());
        }
        else if(currentOption == 3){ // pop
            int elemCount = rand()%stlDeq.size();

            for(int i=0;i<elemCount;++i){
                int gen = rand()%2;

                if(gen == 0){
                    stlDeq.pop_back();
                    myDeq.pop_back(&myDeq);
                }
                else{
                    stlDeq.pop_front();
                    myDeq.pop_front(&myDeq);
                }

                check(stlDeq,myDeq);
            }
        }
        else if(currentOption == 4){ //clear
            myDeq.clear(&myDeq);
            stlDeq.clear();
        }
        else if(currentOption == 5){ // Create a new deque with same or different elements
                                     // and check if equal function works correctly.
            int choice = rand()%2;

            // create same Deque.
            if(choice == 0){
                Deque_int myDeq2;
                Deque_int_ctor(&myDeq2, int_less);

                Deque_int_Iterator iter1 = myDeq.begin(&myDeq);

                for (Deque_int_Iterator it = myDeq.begin(&myDeq);
                        !Deque_int_Iterator_equal(it, myDeq.end(&myDeq)); it.inc(&it)) {

                    if(myDeq2.size(&myDeq2) != myDeq.size(&myDeq))
                        assert(Deque_int_equal(myDeq, myDeq2) == false);
                    myDeq2.push_back(&myDeq2,it.deref(&it));
                }
                
                assert(myDeq.size(&myDeq) == myDeq2.size(&myDeq2));

                assert(Deque_int_equal(myDeq, myDeq2) == true);

                myDeq2.dtor(&myDeq2);
            }
            else{
                Deque_int myDeq2;
                Deque_int_ctor(&myDeq2, int_less);

                for (Deque_int_Iterator it = myDeq.begin(&myDeq);
                        !Deque_int_Iterator_equal(it, myDeq.end(&myDeq)); it.inc(&it)) {
                    if(myDeq2.size(&myDeq2) != myDeq.size(&myDeq))
                        assert(Deque_int_equal(myDeq, myDeq2) == false);
                    myDeq2.push_back(&myDeq2,it.deref(&it));
                }
                myDeq2.pop_back(&myDeq2);
                myDeq2.push_back(&myDeq2,-1);
                assert(Deque_int_equal(myDeq, myDeq2) == false);

                myDeq2.dtor(&myDeq2);
            
            }
            
        }
        else if(currentOption == 6){
            myDeq.sort(&myDeq,myDeq.begin(&myDeq),myDeq.end(&myDeq));
            std::sort(stlDeq.begin(),stlDeq.end(),int_less);
        }
    }

    myDeq.dtor(&myDeq);

}
