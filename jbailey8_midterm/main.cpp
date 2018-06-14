/*
 * Write a fixed-point number class that always maintains 2 digits to the right
 * of the decimal point.
 
 * The suggested internal representation is simply as an long integer.  Then:
 *     1234 means 12.34
 *       12 means   .12
 *       20 means   .20
 *      122 means  1.22
 * etc.
 *
 * You can output integers with leading 0's with:
 *
 *     #include <iostream>
 *     #include <iomanip>
 *     std::cout << std::setw(4) << std::setfill('0') << 30 << std::endl;
 *
 * Note that width (set by std::setw()) will be reset with every output
 * operation, so you will need to reset it.
 */        

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <assert.h>

// You can modifiy the class internally as needed, except that you can't
// implement any assignment operators.
class Fixed {
    public:
    	Fixed(double n) : digits(int(n)), decimals(round(n*100 - digits*100)) { }
	Fixed(long dig, int dec) : digits(dig), decimals(dec) {}
	friend std::ostream &operator<<(std::ostream &os, const Fixed &f){
		if(f.decimals < 10) return os << f.digits << ".0" << f.decimals;
		return os << f.digits << '.' << f.decimals;
	}
	friend Fixed operator+(const Fixed& f1, const Fixed& f2){
		long dig = f1.digits + f2.digits;
		int dec = f1.decimals + f2.decimals;
		if(dec > 100){
			dig++;
			dec -= 100;
		}
		Fixed f(dig, dec);
		return f;
	}
	friend Fixed operator*(const Fixed& f1, const Fixed& f2){
		long f1Digits;
		long f2Digits;
		long times = 1;
		while(times <= f1.decimals) times *= 10;
		if(f1.decimals < 10) times *= 10;
		if(f1.decimals == 0) times *= 10;
		f1Digits = f1.digits * times + f1.decimals;
		times = 1;
		while(times <= f2.decimals) times *= 10;
		if(f2.decimals < 10) times *= 10;
		if(f2.decimals == 0) times *= 10;
		f2Digits = f2.digits * times + f2.decimals;
		double val1 = (double)f1Digits / 100.0;
		double val2 = (double)f2Digits / 100.0;
		Fixed f(val1 * val2);
		return f;
	}
	friend Fixed operator/(const Fixed& f1, const Fixed& f2){
		long f1Digits;
		long f2Digits;
		long times = 1;
		while(times <= f1.decimals) times *= 10;
		if(f1.decimals < 10) times *= 10;
		if(f1.decimals == 0) times *= 10;
		f1Digits = f1.digits * times + f1.decimals;
		times = 1;
		while(times <= f2.decimals) times *= 10;
		if(f2.decimals < 10) times *= 10;
		if(f2.decimals == 0) times *= 10;
		f2Digits = f2.digits * times + f2.decimals;
		double val1 = (double)f1Digits / 100.0;
		double val2 = (double)f2Digits / 100.0;
		Fixed f(val1 / val2);
		return f;
	}
	explicit operator int() const{
		return digits;
	}
	explicit operator double() const{
		double val = digits;
		val += (decimals)/100.0;
		return val;
	}
    private:
        //Fixed &operator=(int);  // Must not implement.
        //Fixed &operator=(double);  // Must not implement.
    private:
        // Suggested implementation representation, but not required.
        long digits;
	int decimals;
};

#define CHECK(f, out) check(f, out, __LINE__)

void
check(const Fixed &f, const std::string &out, const int ln) {
    std::stringstream ss;
    ss << f;
    if (ss.str() != out) {
        std::cerr << "At line " << ln << ": ";
        std::cerr << "Should output " << out << ", but instead output " << ss.str() << "." << std::endl;
    }
}

int
main() {

    Fixed f1(2); // Becomes 2.00.
    f1 = 1.27;

    // Prints out 1.27.
    std::cout << f1 << std::endl;
    CHECK(f1, "1.27");

    // Prints out 3.27.
    std::cout << f1 + 2 << std::endl;
    CHECK(f1 + 2, "3.27");

    // Prints out 3.28.
    std::cout << f1 + 2.01 << std::endl;
    CHECK(f1 + 2.01, "3.28");

    // Prints out 3.28.
    std::cout << f1 + 2.011 << std::endl;
    CHECK(f1 + 2.011, "3.28");

    // Prints out 3.28.
    std::cout << 2.011 + f1 << std::endl;
    CHECK(2.011 + f1, "3.28");

    auto f2 = f1;

    // Prints out 2.54.
    std::cout << f1 + f2 << std::endl;
    CHECK(f1 + f2, "2.54");

    // Prints out 1.61.
    std::cout << f1*f2 << std::endl;
    CHECK(f1 * f2, "1.61");

    // Convert from int.
    f1 = 3;

    // Prints out 3.00.
    std::cout << f1 << std::endl;
    CHECK(f1, "3.00");
    f1 = 2.45;


    // Convert to int, just truncate.
    // Prints 2.
    std::cout << int(f1) << std::endl;
    assert(int(f1) == 2);

    // Convert to double.
    double x = (double)f1;
    // Prints 2.45.
    std::cout << x << std::endl;
    assert(fabs(x - 2.45) < .00001);

    Fixed f3{1.223};

    // Prints out 1.22.
    std::cout << f3 << std::endl;
    CHECK(f3, "1.22");

    // Division.
    Fixed f4(2);
    f4 = 2.01;

    // Prints out 1.00.
    std::cout << f4/2 << std::endl;
    CHECK(f4/2, "1.00");
}
