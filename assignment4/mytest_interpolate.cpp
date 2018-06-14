#include "Interpolate.hpp"
#include <iostream>

int main(){
	std::cout << cs540::Interpolate("i:%, j:%, k:%", 1, 1.3, "foo");
}