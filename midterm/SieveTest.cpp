#include "NodeList.hpp"
#include <iostream>

int main(){
	NodeList m(150000);
	m.calculatePrimes();
	m.printNodeList();
}