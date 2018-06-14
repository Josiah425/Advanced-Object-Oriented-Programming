#include <array>
#include <type_traits>
#include <tuple>
#include <iostream>
#include <vector>

int main(){
	decltype(int) x;
	x = 3;
	std::cout << x << std::endl;
}