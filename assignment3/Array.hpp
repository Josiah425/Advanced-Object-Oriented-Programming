#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <iostream>
#include <chrono>
#include <random>
#include <assert.h>
#include "MyInt.hpp"

namespace cs540{
	class Array{
		public:
			Array(){
				arr = nullptr;
				size = 0;
			}

			Array(std::initializer_list<int> list){
				size = list.size();
				arr = new MyInt[size];
				size_t index = 0;
				for(int e : list) {
					arr[index] = e;
					MyInt::count -= 1;
					index++;
				}
			}

			//Move constructor
			Array(Array && other){
				size = other.size;
				arr = other.arr;
				other.arr = nullptr;
				other.size = 0;
			}

			//Copy constructor
			Array(const Array & other){
				size = other.size;
				arr = new MyInt[size];
				for(size_t i = 0; i < size; i++){
					arr[i] = other.arr[i];
				}
			}

			Array& operator=(const Array& other){
				if(this == &other) return *this;
				size = other.size;
				if(arr != nullptr) delete[] arr;
				arr = new MyInt[size];
				for(size_t i = 0; i < size; i++) arr[i] = other.arr[i];
				return *this;
			}

			Array& operator=(Array&& other){
				if(this == &other) return *this;
				if(arr != nullptr) delete [] arr;
				size = other.size;
				arr = other.arr;
				other.arr = nullptr;
				other.size = 0;
				return *this;
			}

			~Array(){
				delete [] arr;
			}

			friend std::ostream& operator<<(std::ostream& os, const Array& a){
				for(size_t j = 0; j < a.size; j++){
					os << a.arr[j];
				}
				return os;
			}

			void static move_performance_test(){
				auto seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine gen(seed);

				using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
				using Milli = std::chrono::duration<double, std::ratio<1,1000>>;
				using namespace std::chrono;

				size_t elements = 10000000;

				TimePoint begin1, begin2, begin3, begin4, end1, end2, end3, end4;

				Array arr1;
				arr1.arr = new MyInt[elements];

				for(size_t i = 0; i < elements; ++i) {
					MyInt g = rand() % 10000;
					arr1.arr[i] = g;
					arr1.size++;
				}

				std::cout << "Done Inserting" << std::endl;

				//Copy constructor test
				begin1 = system_clock::now();
				Array arr2{arr1};
				end1 = system_clock::now();
				Milli time1 = end1 - begin1;
		
				//Move construtcor test
				begin2 = system_clock::now();
				Array arr3{std::move(arr1)};
				end2 = system_clock::now();
				Milli time2 = end2 - begin2;

				//Assignment operator test
				begin3 = system_clock::now();
				Array arr4 = arr2;
				end3 = system_clock::now();
				Milli time3 = end3 - begin3;

				//Move assignment operator test
				begin4 = system_clock::now();
				Array arr5 = std::move(arr2);
				end4 = system_clock::now();
				Milli time4 = end4 - begin4;

				std::cerr << "Copy constructor: " << elements << " elements: "  << time1.count() << " milliseconds" << std::endl;
				std::cerr << "Move constructor: " << elements << " elements: "  << time2.count() << " milliseconds" << std::endl;
				std::cerr << "Assignment operator: " << elements << " elements: "  << time3.count() << " milliseconds" << std::endl;
				std::cerr << "Move assignment operator: " << elements << " elements: "  << time4.count() << " milliseconds" << std::endl;
			}


		private:
			MyInt * arr;
			size_t size;
	};
}

#endif