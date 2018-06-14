#include <cstddef>
#include <vector>
#include <iostream>
#include <array>
#include <type_traits>
#include <tuple>

template <size_t... Ts>
class Extractor{};

template <size_t T>
class Extractor<T>{
	public:
		Extractor() : index(T) {}

		Extractor &operator=(const Extractor &other){
			index = other.index;
		}

		template <typename K>
		K operator()(K& t){
			return t[index];
		}
		
		size_t index;
};

template <size_t T, size_t... Ts>
class Extractor<T, Ts...>{
	public:
		Extractor() : index(T), ex() {}

		Extractor &operator=(const Extractor &other){
			index = other.index;
		}

		template <typename K>
		std::array<K,sizeof...(Ts)+1> operator()(K & t){
			std::array<K, sizeof...(Ts)+1> retVal;
			size_t i = 0;
			for(i = 0; i < sizeof...(Ts)+1; i++){
				retVal.at(i) = t.at(index);
				index = ex.index;
				ex = ex;
			}
			return retVal;
		}

		size_t index;
		Extractor<Ts...> ex;
};

int
main() {
    
    // An Extractor extracts the corresponding elements from a native array, a
    // std::vector, or a std::array, and returns a std::array with the
    // corresponding elements in it.
    
    // This extractor will extract the elements with the corresponding indices
    // from the given sequence container, and return a std::array of the
    // appropriate type with the specified elements.
    
    // The extractor should work for any size !!! 
    // including any size of template arguments and any size of the argument array
    // Otherwise, you will lose some points    
    Extractor<1, 2, 2, 0> ex;

    int a1[] = {11, 22, 33};
    std::array<int, 3> aa1{11, 22, 33};
    std::vector<int> v1{11, 22, 33};

    double a2[] = {1.1, 2.2, 3.3, 4.4};
    std::array<double, 4> aa2{1.1, 2.2, 3.3, 4.4};
    std::vector<double> v2{1.1, 2.2, 3.3, 4.4};

    const char *a3[] = {"1", "22", "333", "4444", "55555"};
    std::array<const char *, 5> aa3{"1", "22", "333", "4444", "55555"};
    std::vector<const char *> v3{"1", "22", "333", "4444", "55555"};

    std::cout << "Below should be 22, 33, 33, 11." << std::endl;
    /*for (auto e : ex(a1)) {
        std::cout << e.at(0) << std::endl;
    }
    std::cout << "Below should be 22, 33, 33, 11." << std::endl;
    for (auto e : ex(aa1)) {
        std::cout << e.at(0) << std::endl;
    }
    std::cout << "Below should be 22, 33, 33, 11." << std::endl;
    for (auto e : ex(v1)) {
        std::cout << e << std::endl;
    }

    std::cout << "Below should be 2.2, 3.3, 3.3, 1.1." << std::endl;
    for (auto e : ex(a2)) {
        std::cout << e << std::endl;
    }
    std::cout << "Below should be 2.2, 3.3, 3.3, 1.1." << std::endl;
    for (auto e : ex(aa2)) {
        std::cout << e << std::endl;
    }
    std::cout << "Below should be 2.2, 3.3, 3.3, 1.1." << std::endl;
    for (auto e : ex(v2)) {
        std::cout << e << std::endl;
    }

    std::cout << "Below should be 22, 333, 333, 1." << std::endl;
    for (auto e : ex(a3)) {
        std::cout << e << std::endl;
    }
    std::cout << "Below should be 22, 333, 333, 1." << std::endl;
    for (auto e : ex(aa3)) {
        std::cout << e << std::endl;
    }
    std::cout << "Below should be 22, 333, 333, 1." << std::endl;
    for (auto e : ex(v3)) {
        std::cout << e << std::endl;
    }*/
}