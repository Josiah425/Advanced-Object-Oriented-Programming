#ifndef _NODE_HPP_
#define _NODE_HPP_

#include <iostream>

class NodeList{
	private:
		class Node{
			friend class NodeList;
			public:
				Node(int val_input) : val(val_input), next(nullptr){}
				int getValue(){
					return val;
				}
				void setValue(int val_input){
					val = val_input;
				}
				Node * getNext(){
					return next;
				}
				void setNext(Node * node_input){
					 next = node_input;
				}
				
			private:
				int val;
				Node * next;
		};
	
	public:
		NodeList(int size_input) : size(size_input){
			first = new Node(2);
			Node * tmp = first;
			for(int i = 3; i <= size_input; i++){
				tmp->setNext(new Node(i));
				tmp = tmp->getNext();
			}
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
		}
		
		void calculatePrimes(){
			Node * prime = first;
			int k = prime->getValue();
			while(prime->getNext() != nullptr){
				Node * prev = prime;
				Node * curr = prime->getNext();
				for(int i = k*prime->getValue(); i <= size; ){
					if(curr == nullptr) break;
					if(curr->getValue() >= i){
						if(curr->getValue() == i){
							prev->setNext(curr->getNext());
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

		~NodeList(){
			Node * tmp = first;
			while(tmp != nullptr){
				Node * temp = tmp->getNext();
				delete(tmp);
				tmp = temp;
			}
		}
		
	
	private:
		int size;
		Node * first;

};

#endif
