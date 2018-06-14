#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

#include <stdexcept>
#include <functional>
#include <iostream>

namespace cs540{
	class OutOfRange : public std::out_of_range {
		public:
			explicit OutOfRange() : OutOfRange("Error: Accessing array out of bounds") { }
			explicit OutOfRange(const std::string& error) : std::out_of_range(error) { }
	};

	//Apparently, this is needed according to Professor Chiu
	template <typename T, size_t... Dims>
	class Array{};

	//Base Case
	template <typename T, size_t Dim>
	class Array<T, Dim>{
		public:
			//Can't be less than 0 (size_t can't be negative but I'm checking the less than anyways)
			static_assert(Dim > 0, "This gets rid of a warning -wpedantic");

			template <size_t N>
			class NestedIndex{
				public:
					NestedIndex() : index(0) {}

					void max(){
						index = N-1;
					}

					//Doesn't need reset because were done once this hits the end
					void operator++(){
						++index;
					}

					bool operator++(int){
						if(++index == N){
							//Don't set to 0 here or it'll mess up with fmend
							return true;
						}
						return false;
					}

					friend bool operator==(const NestedIndex<N> &nested1, const NestedIndex<N> &nested2){
						return nested1.index == nested2.index;
					}

					size_t index;
			};

			class FirstDimensionMajorIterator{
				public:
					FirstDimensionMajorIterator() : array(nullptr){}

					FirstDimensionMajorIterator(NestedIndex<Dim> nestedIndex, Array<T, Dim> * other) : index(nestedIndex), array(other) {}

					FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &other){
						index = other.index;
						array = other.array;
						return *this;
					}

					friend bool operator==(const FirstDimensionMajorIterator &rhs, const FirstDimensionMajorIterator &lhs){
						return (rhs.index == lhs.index) && (rhs.array == lhs.array);
					}

					friend bool operator!=(const FirstDimensionMajorIterator &rhs, const FirstDimensionMajorIterator &lhs){
						return !(rhs == lhs);
					}				

					FirstDimensionMajorIterator &operator++(){
						index++;
						return *this;
					}

					FirstDimensionMajorIterator operator++(int){
						FirstDimensionMajorIterator temp = *this;
						index++;
						return temp;
					}

					T &operator*() const{
						return (*array)[index.index];
					}

					NestedIndex<Dim> index;
					Array<T, Dim> * array;
			};

			class LastDimensionMajorIterator{
				public:
					LastDimensionMajorIterator() : array(nullptr){}

					LastDimensionMajorIterator(NestedIndex<Dim> nestedIndex, Array<T, Dim> * other) : index(nestedIndex), array(other) {}

					LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &other){
						index = other.index;
						array = other.array;
						return *this;
					}

					friend bool operator==(const LastDimensionMajorIterator &rhs, const LastDimensionMajorIterator &lhs){
						return (rhs.index == lhs.index) && (rhs.array == lhs.array);
					}

					friend bool operator!=(const LastDimensionMajorIterator &rhs, const LastDimensionMajorIterator &lhs){
						return !(rhs == lhs);
					}				

					LastDimensionMajorIterator &operator++(){
						++index;
						return *this;
					}

					LastDimensionMajorIterator operator++(int){
						LastDimensionMajorIterator temp = *this;
						++index;
						return temp;
					}

					T &operator*() const{
						return (*array)[index.index];
					}

					NestedIndex<Dim> index;
					Array<T, Dim> * array;
			};

			

			typedef T ValueType;
			Array(){
				size = Dim;
				//std::cout << "Base Case Array()" << std::endl;
			}

			Array(const Array &other){
				//std::cout << "Base Case Array(const Array &other)" << std::endl;
				size = Dim;
				for(size_t i = 0; i < Dim; i++) array[i] = other[i];
			}

			template <typename U>
			Array(const Array<U, Dim> &other) : size(Dim) {
				//std::cout << "Base Case Array(const Array<U, Dim> &other)" << std::endl;
				for(size_t i = 0; i < Dim; i++) array[i] = other[i];
			}

			Array &operator=(const Array &other){
				//std::cout << "Base Case Array &operator=(const Array &other)" << std::endl;
				if(size != other.size) throw OutOfRange("Out Of Bounds");
				if(this != &other){
					for(size_t i = 0; i < size; i++) array[i] = other[i];
				}
				return *this;
			}

			template <typename U>
			Array &operator=(const Array<U, Dim> &other){
				//std::cout << "Base Case Array &operator=(const Array<U, Dim> &other)" << std::endl;
				if(size != other.size) throw OutOfRange("Out Of Bounds");
				if(this != (Array<T, Dim>*)&other){
					for(size_t i = 0; i < size; i++) array[i] = other[i];
				}
				return *this;
			}

			T &operator[](size_t i){
				//std::cout << "Base Case T &operator[](size_t i)" << std::endl;
				if(i < 0 || i >= Dim){
					throw OutOfRange("Index inaccessible");
				}
				return array[i];
			}

			const T &operator[](size_t i) const{
				//std::cout << "Base Case const T &operator[](size_t i) const" << std::endl;
				if(i < 0 || i >= Dim){
					throw OutOfRange("Index inaccessible");
				}
				return array[i];
			}

			FirstDimensionMajorIterator fmbegin(){
				//std::cout << "Base Case FirstDimensionMajorIterator fmbegin()" << std::endl;
				NestedIndex<Dim> index;
				return FirstDimensionMajorIterator(index, this);
			}

			FirstDimensionMajorIterator fmend(){
				//std::cout << "Base Case FirstDimensionMajorIterator fmend()" << std::endl;
				NestedIndex<Dim> index;
				index.max();
				//Has to go one past the max so we get that last print out
				index.index++;
				return FirstDimensionMajorIterator(index, this);
			}

			LastDimensionMajorIterator lmbegin(){
				//std::cout << "Base Case LastDimensionMajorIterator lmbegin()" << std::endl;
				NestedIndex<Dim> index;
				return LastDimensionMajorIterator(index, this);
			}

			LastDimensionMajorIterator lmend(){
				//std::cout << "Base Case LastDimensionMajorIterator lmend()" << std::endl;
				NestedIndex<Dim> index;
				index.max();
				//Has to go one past the max so we get that last print out
				index.index++;
				return LastDimensionMajorIterator(index, this);
			}


			T array[Dim];
			size_t size;
	};

	//General Case
	template <typename T, size_t Dim, size_t... Dims>
	class Array<T, Dim, Dims...>{
		public:
			//I know size_t can't be negative but I still am checking the greater than
			static_assert(sizeof...(Dims) > 0, "This gets rid of a warning -wpedantic");

			typedef T ValueType;
			Array(){
				size = Dim;
				//std::cout << "General Case Array()" << std::endl;
			}

			template <typename U>
			Array(const Array &other){
				//std::cout << "General Case Array(const Array &other)" << std::endl;
				for(size_t i = 0; i < (size_t)Dim; i++) array[i] = other[i];
			}

			template <typename U>
			Array(const Array<U, Dim, Dims...> &other){
				//std::cout << "General Case Array(const Array<U, Dim, Dims...> &other)" << std::endl;
				for(size_t i = 0; i < (size_t)Dim; i++) array[i] = other[i];
			}

			Array &operator=(const Array &other){
				//std::cout << "General Case Array &operator=(const Array &other)" << std::endl;
				if(this != &other){
					size = other.size;
					for(size_t i = 0; i < (size_t)Dim; i++) array[i] = other[i];
				}
				return *this;
			}

			template <typename U>
			Array &operator=(const Array<U, Dim, Dims...> &other){
				//std::cout << "General Case Array &operator=(const Array<U, Dim, Dims...> &other)" << std::endl;
				for(size_t i = 0; i < (size_t)Dim; i++) array[i] = other[i];
				return *this;
			}

			Array<T, Dims...> &operator[](size_t i){
				//std::cout << "General Case Array<T, Dims...> &operator[](size_t i)" << std::endl;
				if(i < 0 || i >= Dim){
					throw OutOfRange("Index inaccessible");
				}
				return array[i];
			}

			const Array<T, Dims...> &operator[](size_t i) const{
				//std::cout << "General Case const Array<T, Dims...> &operator[](size_t i) const" << std::endl;
				if(i < 0 || i >= Dim){
					throw OutOfRange("Index inaccessible");
				}
				return array[i];
			}

			template <size_t N, size_t... M>
			class NestedIndex{
				public:
					NestedIndex() : index(0){}
					
					void max(){
						index = N-1;
						nestedIndex.max();
					}
					
					//Last Dimension Major
					void operator++(){
						if(++index == N){
							index = 0;
							++nestedIndex;
						}
					}					

					//First Dimension Major (hard to see, but essentially
					//I check if the increment at the level below hits it's 
					//Max dimension or not, if it does, we reset the index below to 0
					//And then increment the indexes at the current dimension,
					//This recurses down the line
					bool operator++(int){
						if(nestedIndex++ == false) return false;
						nestedIndex.index = 0;
						if(++index == N){
							return true;
						}
						return false;
					}

					friend bool operator==(const NestedIndex<N, M...> &nested1, const NestedIndex<N, M...> &nested2){
						if(nested1.index != nested2.index) return false;
						return nested1.nestedIndex == nested2.nestedIndex;
					}

					size_t index;
					//Nested index at dimension below current
					typename Array<T, M...>::template NestedIndex<M...> nestedIndex;
			};

			class FirstDimensionMajorIterator{
				public:
					FirstDimensionMajorIterator() : array(nullptr) {}

					FirstDimensionMajorIterator(NestedIndex<Dim, Dims...> otherIndex, Array<T, Dim, Dims...> *other) : nestedIndex(otherIndex), array(other) {}

					FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &other){
						nestedIndex = other.nestedIndex;
						array = other.array;
						return *this;
					}

					friend bool operator==(const FirstDimensionMajorIterator &rhs, const FirstDimensionMajorIterator &lhs){
						return (rhs.nestedIndex == lhs.nestedIndex) && (rhs.array == lhs.array);
					}

					friend bool operator!=(const FirstDimensionMajorIterator &rhs, const FirstDimensionMajorIterator &lhs){
						return !(rhs == lhs);
					}				

					FirstDimensionMajorIterator &operator++(){
						nestedIndex++;
						return *this;
					}

					FirstDimensionMajorIterator operator++(int){
						FirstDimensionMajorIterator temp = *this;
						nestedIndex++;
						return temp;
					}

					//Tricky to see, but recursively calls self until we get to base case
					T &operator*() const{
						Array<T, Dims...> *other = &(array->array[nestedIndex.index]);
						typename Array<T, Dims...>::FirstDimensionMajorIterator retVal(nestedIndex.nestedIndex, other);
						return *retVal;
					}

					NestedIndex<Dim, Dims...> nestedIndex;
					Array<T, Dim, Dims...> *array;
			};

			class LastDimensionMajorIterator{
				public:
					LastDimensionMajorIterator() : array(nullptr) {}

					LastDimensionMajorIterator(NestedIndex<Dim, Dims...> otherIndex, Array<T, Dim, Dims...> *other) : nestedIndex(otherIndex), array(other) {}

					LastDimensionMajorIterator&operator=(const LastDimensionMajorIterator &other){
						nestedIndex = other.nestedIndex;
						array = other.array;
						return *this;
					}

					friend bool operator==(const LastDimensionMajorIterator &rhs, const LastDimensionMajorIterator &lhs){
						return (rhs.nestedIndex == lhs.nestedIndex) && (rhs.array == lhs.array);
					}

					friend bool operator!=(const LastDimensionMajorIterator &rhs, const LastDimensionMajorIterator &lhs){
						return !(rhs == lhs);
					}				

					LastDimensionMajorIterator &operator++(){
						++nestedIndex;
						return *this;
					}

					LastDimensionMajorIterator operator++(int){
						LastDimensionMajorIterator temp = *this;
						++nestedIndex;
						return temp;
					}

					//Tricky to see, but recursively calls self until we get to base case
					T &operator*() const{
						Array<T, Dims...> *other = &(array->array[nestedIndex.index]);
						typename Array<T, Dims...>::LastDimensionMajorIterator retVal(nestedIndex.nestedIndex, other);
						return *retVal;
					}

					NestedIndex<Dim, Dims...> nestedIndex;
					Array<T, Dim, Dims...> *array;
			};

			FirstDimensionMajorIterator fmbegin(){
				//std::cout << "General Case FirstDimensionMajorIterator fmbegin()" << std::endl;
				NestedIndex<Dim, Dims...> nestedIndex;
				return FirstDimensionMajorIterator(nestedIndex, this);
			}

			/* Dim is the size of the arrays current dimension */
			FirstDimensionMajorIterator fmend(){
				//std::cout << "General Case FirstDimensionMajorIterator fmend()" << std::endl;
				NestedIndex<Dim, Dims...> nestedIndex;
				nestedIndex.max();
				//Has to go one past the max so we get that last print out
				nestedIndex++;
				return FirstDimensionMajorIterator(nestedIndex, this);
			}

			LastDimensionMajorIterator lmbegin(){
				//std::cout << "General Case LastDimensionMajorIterator lmbegin()" << std::endl;
				NestedIndex<Dim, Dims...> nestedIndex;
				return LastDimensionMajorIterator (nestedIndex, this);
			}

			/* Dim is the size of the arrays current dimension */
			LastDimensionMajorIterator lmend(){
				//std::cout << "General Case LastDimensionMajorIterator lmend()" << std::endl;
				NestedIndex<Dim, Dims...> nestedIndex;
				nestedIndex.max();
				//Has to go one past the max so we get that last print out
				++nestedIndex;
				return LastDimensionMajorIterator(nestedIndex, this);
			}




			size_t size;
			Array<T, Dims...> array[Dim];
	};
}


#endif
