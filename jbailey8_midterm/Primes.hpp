#ifndef CS540_PRIMES_HPP
#define CS540_PRIMES_HPP

#include <iostream>
#include <string.h>

namespace cs540{
	class Primes{
		private:
			class Node{
				friend class Primes;
				public:
					Node(unsigned long val_input) : val(val_input), next(nullptr), prev(nullptr){}
					unsigned long getValue(){
						return val;
					}
					void setValue(unsigned long val_input){
						val = val_input;
					}
					Node * getNext(){
						return next;
					}
					Node * getPrev(){
						return prev;
					}
					void setNext(Node * node_input){
						 next = node_input;
					}
					void setPrev(Node * node_input){
						prev = node_input;
					}
				
				private:
					unsigned long val;
					Node * next;
					Node * prev;
			};
	
		public:
			class Iterator{
				friend class Primes;

				public:
					friend bool operator!=(const Iterator &it1, const Iterator &it2){
						return it1.node != it2.node;
					}
					friend bool operator==(const Iterator &it1, const Iterator &it2){
						return it1.node == it2.node;
					}
					Iterator(const Iterator & it) : node{it.node} {}
					Iterator &operator++(){
						node = node->getNext();
						return *this;
					}
					Iterator operator++(int){
						Iterator temp = *this;
						node = node->getNext();
						return temp;
					}
					Iterator &operator--(){
						node = node->getPrev();
						return *this;
					}
					Iterator operator--(int){
						Iterator temp = *this;
						node = node->getPrev();
						return temp;
					}
					unsigned long operator*(){
						return node->getValue();
					}
				private:
					Iterator(Node *n) : node{n} {}
					Node * node;
			};

			Primes(unsigned long size_input) : size(size_input){
				first = new Node(2);
				Node * tmp = first;
				Node * prev = nullptr;
				for(unsigned long i = 3; i <= size_input; i++){
					tmp->setNext(new Node(i));
					prev = tmp;
					if(i != size_input) tmp = tmp->getNext();
					tmp->setPrev(prev);
				}
				last = tmp;
				Node * prime = first;
				unsigned long k = prime->getValue();
				while(prime->getNext() != nullptr){
					Node * prev = prime;
					Node * curr = prime->getNext();
					for(unsigned long i = k*prime->getValue(); i <= size; ){
						if(curr == nullptr) break;
						if(curr->getValue() >= i){
							if(curr->getValue() == i){
								prev->setNext(curr->getNext());
								if(curr->getNext() != nullptr) curr->getNext()->setPrev(prev);
								delete(curr);
								curr = prev->getNext();
							}
							i += k;
						}
						else{
							prev = curr;
							curr = curr->getNext();
						}
					}
					prime = prime->getNext();
					k = prime->getValue();
				}
				last = prime;
			}
			Iterator begin() { return Iterator(first); }
			Iterator end() { return Iterator(last); }
			void calculatePrimes(){
				Node * prime = first;
				unsigned long k = prime->getValue();
				while(prime->getNext() != nullptr){
					Node * prev = prime;
					Node * curr = prime->getNext();
					for(unsigned long i = k*prime->getValue(); i <= size; ){
						if(curr == nullptr) break;
						if(curr->getValue() >= i){
							if(curr->getValue() == i){
								prev->setNext(curr->getNext());
								if(curr->getNext() != nullptr) curr->getNext()->setPrev(prev);
								delete(curr);
								curr = prev->getNext();
							}
							i += k;
						}
						else{
							prev = curr;
							curr = curr->getNext();
						}
					}
					prime = prime->getNext();
					k = prime->getValue();
				}
				last = prime;
			}
		
			void printNodeList(){
				Node * tmp = first;
				while(tmp->getNext() != nullptr){
					std::cout << tmp->getValue() << std::endl;
					tmp = tmp->getNext();
				}
				if(tmp != nullptr){
					std::cout << tmp->getValue() << std::endl;
				}
			}

			~Primes(){
				Node * tmp = first;
				while(tmp != nullptr){
					Node * temp = tmp->getNext();
					delete(tmp);
					tmp = temp;
				}
			}
		
	
		private:
			unsigned long size;
			Node * first;
			Node * last;

	};
}


#endif
