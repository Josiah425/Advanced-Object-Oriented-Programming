#ifndef _MAP_HPP_
#define _MAP_HPP_

#include <utility>
#include <stdlib.h>
#include <iostream>
#include <vector>

namespace cs540{
	template<typename Key_T, typename Mapped_T> //Has < and == operators, no default constructor or assignment operator
	class Map{
		public:
			typedef typename std::pair<const Key_T, Mapped_T> KeyValuePair;
			
			class SkipList{
				public:
					class Node{
						public:
							std::vector<Node *> forward;
							std::vector<Node *> backward;

							Node(int levels){
								for(int i = 0; i <= levels; i++){
									forward.push_back(nullptr);
									backward.push_back(nullptr);
								}
							}
					};

					class ValueNode : public Node{
						public:
							KeyValuePair key_value;

							ValueNode(const KeyValuePair &v, int levels) : Node(levels), key_value(v){}
					};

					class Sentinel : public Node{
						public:
							Sentinel() : Node(0){}
					};

				private:
					int currentLevel;
					size_t sz;
					float promoteProbability;
					Node * head;
					Node * tail;

				public:
											
					SkipList(){
						promoteProbability = .50;
						currentLevel = 0;
						sz = 0;
						head = new Sentinel();
						tail = new Sentinel();

						head->forward[0] = tail;
						tail->forward[0] = nullptr;
						head->backward[0] = nullptr;
						tail->backward[0] = head;
					}
				
					int randomLevel(){
						int value = 0;
						while((((double)std::rand()/RAND_MAX)) < promoteProbability){
							value++;
						}
						
						return value;
					}
				
					//Used https://www.geeksforgeeks.org/skip-list-set-3-searching-deletion/ as a reference for insertion
					Node * insert(KeyValuePair v){
						Node * curr = head;
						std::vector<Node *> update = head->forward;
						for(int i = currentLevel; i >= 0; i--){
							while(curr->forward[i] != tail && static_cast<typename SkipList::ValueNode *>(curr->forward[i])->key_value.first < v.first){
								curr = curr->forward[i];
							}
							update[i] = curr;
						}
						curr = curr->forward[0];
					
						if(curr == tail || !(static_cast<typename SkipList::ValueNode *>(curr)->key_value.first == v.first)){
							int randLevel = randomLevel();
							if(randLevel > currentLevel){
								for(int i = currentLevel+1; i <= randLevel; i++){
									update.push_back(head);
									head->forward.push_back(tail);
									head->backward.push_back(nullptr);
									tail->forward.push_back(nullptr);
									tail->backward.push_back(head);
								}
								currentLevel = randLevel;
							}
							Node *n = new ValueNode(v, randLevel);

							for(int i = 0; i <= randLevel; i++){
								if(update[i] == head){
									n->backward[i] = head;
									head->forward[i]->backward[i] = n;
									n->forward[i] = head->forward[i];
									head->forward[i] = n;
								}
								else{
									n->forward[i] = update[i]->forward[i];
									update[i]->forward[i]->backward[i] = n;
									n->backward[i] = update[i];
									update[i]->forward[i] = n;
								}
							}
							sz++;
							return n;
						}
						return curr;
					}

					//Used https://www.geeksforgeeks.org/skip-list-set-3-searching-deletion/ as reference for find
					Node * find(const Key_T key){
						Node * curr = head;
						for(unsigned int i = currentLevel; i-- > 0;){
							while(curr->forward[i] != tail && static_cast<typename SkipList::ValueNode *>(curr->forward[i])->key_value.first < key){
								curr = curr->forward[i];
							}
						}
						curr = curr->forward[0];

						if(curr != tail && static_cast<typename SkipList::ValueNode *>(curr)->key_value.first == key) return curr;
						return nullptr;
					}
					
					//Used https://www.geeksforgeeks.org/skip-list-set-3-searching-deletion/ as reference for erase
					void erase(const Key_T key){
						Node * curr = find(key);
						if(curr == nullptr) return;
						curr = head;
						std::vector<Node *> update = head->forward;

						for(int i = currentLevel; i >= 0; i--){
							while(curr->forward[i] != tail && static_cast<typename SkipList::ValueNode *>(curr->forward[i])->key_value.first < key){
								curr = curr->forward[i];
							}
							update[i] = curr;
						}

						curr = curr->forward[0];
						if(curr != tail && static_cast<typename SkipList::ValueNode *>(curr)->key_value.first == key){
							for(int i = 0; i <= currentLevel; i++){
								if(update[i]->forward[i] != curr) break;
								update[i]->forward[i] = curr->forward[i];
								update[i]->forward[i]->backward[i] = update[i];
							}
							delete curr;
							sz--;
						}
					}

					size_t size(){
						return sz;
					}

					void inc_size(){
						sz++;
					}

					Node * getHead(){
						return head;
					}

					Node * getTail(){
						return tail;
					}

					int getCurrentLevel(){
						return currentLevel;
					}

					void setCurrentLevel(int val){
						currentLevel = val;
					}

					~SkipList(){
						Node * node = head;
						while(node->forward[0]){
							Node * tmp = node;
							node = node->forward[0];
							delete(tmp);
						}
						delete(node);
						sz = 0;
						currentLevel = 0;
					}

			};
			
			class Iterator{
				public:
					typename SkipList::Node * node;

					Iterator(typename SkipList::Node * in){
						this->node = in;
					}

					friend bool operator !=(const typename Map<Key_T, Mapped_T>::Iterator &lhs, const typename Map<Key_T, Mapped_T>::Iterator& rhs){
						return lhs.node != rhs.node;
					}

					friend bool operator !=(const typename Map<Key_T, Mapped_T>::Iterator &lhs, const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
						return lhs.node != rhs.node;
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::Iterator &lhs, const typename Map<Key_T, Mapped_T>::Iterator& rhs){
						return lhs.node == rhs.node;
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::Iterator &lhs, const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
						return lhs.node == rhs.node;
					}

					Iterator& operator=(const Iterator &it){
						this->node = it.node;
						return *this;
					}

					Iterator &operator++(){
						this->node = node->forward[0];
						return *this;
					}

					Iterator operator++(int){
						Iterator temp = *this;
						this->node = node->forward[0];
						return temp;
					}

					Iterator &operator--(){
						this->node = node->backward[0];
						return *this;
					}

					Iterator operator--(int){
						Iterator temp = *this;
						this->node = node->backward[0];
						return temp;
					}

					KeyValuePair &operator*() const{
						return static_cast<typename SkipList::ValueNode *>(node)->key_value;
					}

					KeyValuePair *operator->() const{
						return &static_cast<typename SkipList::ValueNode *>(node)->key_value;
					}

					Iterator() = delete;
					
			};
		
			class ConstIterator{
				public:
					typename SkipList::Node * node;

					ConstIterator(const Iterator & iterator){
						this->node = iterator.node;
					}

					ConstIterator& operator=(const ConstIterator &const_it){
						this->node = const_it.node;
					}

					friend bool operator !=(const typename Map<Key_T, Mapped_T>::ConstIterator &lhs, const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
						return lhs.node != rhs.node;
					}

					friend bool operator !=(const typename Map<Key_T, Mapped_T>::ConstIterator &lhs, const typename Map<Key_T, Mapped_T>::Iterator& rhs){
						return lhs.node != rhs.node;
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::ConstIterator &lhs, const typename Map<Key_T, Mapped_T>::ConstIterator& rhs){
						return lhs.node == rhs.node;
					}

					friend bool operator==(const typename Map<Key_T, Mapped_T>::ConstIterator &lhs, const typename Map<Key_T, Mapped_T>::Iterator& rhs){
						return lhs.node == rhs.node;
					}

					ConstIterator &operator++(){
						this->node = node->forward[0];
						return *this;
					}

					ConstIterator operator++(int){
						ConstIterator temp = *this;
						this->node = node->forward[0];
						return temp;
					}

					ConstIterator &operator--(){
						this->node = node->backward[0];
						return *this;
					}

					ConstIterator operator--(int){
						ConstIterator temp = *this;
						this->node = node->backward[0];
						return temp;
					}
					const KeyValuePair &operator*() const{
						return static_cast<typename SkipList::ValueNode *>(node)->key_value;
					}

					const KeyValuePair *operator->() const{
						return &static_cast<typename SkipList::ValueNode *>(node)->key_value;
					}

					ConstIterator() = delete;
			};

			class ReverseIterator{
				public:
					typename SkipList::Node * node;
					
					ReverseIterator(typename SkipList::Node * in){
						this->node = in;
					}

					ReverseIterator& operator=(const ReverseIterator &it){
						this->node = it.node;
					}

					friend bool operator !=(const typename Map<Key_T, Mapped_T>::ReverseIterator &lhs, const typename Map<Key_T, Mapped_T>::ReverseIterator& rhs){
						return lhs.node != rhs.node;
					}

					ReverseIterator &operator++(){
						this->node = node->backward[0];
						return *this;
					}

					ReverseIterator operator++(int){
						ReverseIterator temp = *this;
						this->node = node->backward[0];
						return temp;
					}

					ReverseIterator &operator--(){
						this->node = node->forward[0];
						return *this;
					}

					ReverseIterator operator--(int){
						ReverseIterator temp = *this;
						this->node = node->forward[0];
						return temp;
					}

					KeyValuePair &operator*() const{
						return static_cast<typename SkipList::ValueNode *>(node)->key_value;
					}

					KeyValuePair *operator->() const{
						return &static_cast<typename SkipList::ValueNode *>(node)->key_value;
					}


					ReverseIterator() = delete;
			};

	private:
		SkipList *skipList;

	public:

		Map(){
			skipList = new SkipList();
		}

		Map(const Map &m){
			skipList = new SkipList();
			for(auto iterator = m.begin(); iterator != m.end(); ++iterator) skipList->insert(static_cast<typename SkipList::ValueNode *>(iterator.node)->key_value);
		}

		Map &operator=(const Map &m){
			if(&m != this){
				delete(skipList);
				skipList = new SkipList();
				for(auto iterator = m.begin(); iterator != m.end(); ++iterator) skipList->insert(static_cast<typename SkipList::ValueNode *>(iterator.node)->key_value);
			}
			return *this;
		}

		Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> v){
			skipList = new SkipList();
			for(auto pair : v) skipList->insert(pair);
		}

		~Map(){
			delete(skipList);
		}

		size_t size() const{
			return skipList->size();
		}

		bool empty() const{
			if(size() == 0) return true;
			return false;
		}

		Iterator begin(){
			Iterator it(skipList->getHead()->forward[0]);
			return it;
		}

		Iterator end(){
			Iterator it(skipList->getTail());
			return it;
		}

		ReverseIterator rbegin(){
			ReverseIterator reverse_it(skipList->getTail()->backward[0]);
			return reverse_it;
		}

		ReverseIterator rend(){
			ReverseIterator reverse_it(skipList->getHead());
			return reverse_it;
		}

		ConstIterator begin() const{
			ConstIterator const_it(skipList->getHead()->forward[0]);
			return const_it;
		}

		ConstIterator end() const{
			ConstIterator const_it(skipList->getTail());
			return const_it;
		}

		Iterator find(const Key_T &key){
			Iterator it(skipList->find(key));
			if(it.node == nullptr) return end();
			return it;
		}

		ConstIterator find(const Key_T &key) const{
			ConstIterator const_it(skipList->find(key));
			if(const_it.node == nullptr) return end();
			return const_it;
		}

		Mapped_T &at(const Key_T &key){
			typename SkipList::Node *node = skipList->find(key);
			if(node == nullptr){
				throw std::out_of_range("Error Accessing element that doesn't exist in the map");
			}
			return static_cast<typename SkipList::ValueNode *>(node)->key_value.second;
		}

		const Mapped_T &at(const Key_T &key) const{
			typename SkipList::Node *node = skipList->find(key);
			if(node == nullptr){
				throw std::out_of_range("Error Accessing element that doesn't exist in the map");
			}
			return static_cast<typename SkipList::ValueNode *>(node)->key_value.second;
		}

		Mapped_T &operator[](const Key_T & key){
			typename SkipList::Node * node = skipList->insert(std::make_pair(key, Mapped_T()));
			return static_cast<typename SkipList::ValueNode *>(node)->key_value.second;
		}

		std::pair<Iterator, bool> insert(const KeyValuePair &v){
			Iterator it = find(v.first);
			if(it != end()){
				return std::make_pair(it, false);
			}
			skipList->insert(v);
			it = find(v.first);
			return std::make_pair(it, true);
		}

		template <typename IT_T>
		void insert(IT_T beg, IT_T end){
			for(auto it = beg; it != end; ++it) skipList->insert(*it);
		}

		void erase(Iterator iterator){
			if(iterator.node == skipList->getTail() || iterator.node == skipList->getHead()) return;
			skipList->erase(static_cast<typename SkipList::ValueNode *>(iterator.node)->key_value.first);
		}

		void erase(const Key_T & key){
			typename SkipList::Node * node = skipList->find(key);
			if(node == nullptr){
				throw std::out_of_range("Map tried to erase value not inside");
			} else{
				skipList->erase(key);
			}
		}

		void clear(){
			typename SkipList::Node * curr = skipList->getHead();
			while(curr->forward[0] != skipList->getTail()){
				skipList->erase(static_cast<typename SkipList::ValueNode *>(curr)->key_value.first);
				curr = curr->forward[0];
			}
		}

		friend bool operator==(const Map &lhs, const Map &rhs){
			typename SkipList::Node * currLhs = lhs.skipList->getHead();
			typename SkipList::Node * currRhs = rhs.skipList->getHead();

			if(lhs.size() != rhs.size()) return false;

			while(currLhs->forward[0] != lhs.skipList->getTail()){
				currLhs = currLhs->forward[0];
				currRhs = currRhs->forward[0];
				if(!(static_cast<typename SkipList::ValueNode *>(currLhs)->key_value.first == static_cast<typename SkipList::ValueNode *>(currRhs)->key_value.first) && !(static_cast<typename SkipList::ValueNode *>(currLhs)->key_value.second == static_cast<typename SkipList::ValueNode *>(currRhs)->key_value.second)) return false;
			}

			return true;
		}

		friend bool operator!=(const Map &lhs, const Map &rhs){
			return !(lhs == rhs);
		}

		friend bool operator<(const Map &lhs, const Map &rhs){
			if(lhs.skipList->size() < rhs.skipList->size()) return false;
			typename Map<Key_T, Mapped_T>::ConstIterator lhsIterator = lhs.begin();
			typename Map<Key_T, Mapped_T>::ConstIterator rhsIterator = rhs.begin();

			while(lhsIterator != lhs.end()){
				if(lhsIterator.node->key_value.second < rhsIterator.node->key_value.second){
					++lhsIterator;
					++rhsIterator;
				} else return false;
			}

			return true;
		}
	};
}

#endif
