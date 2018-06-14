#ifndef _INTERPOLATE_HPP_
#define _INTERPOLATE_HPP_

#include <stdexcept>
#include <tuple>
#include <iostream>

namespace cs540{
	class WrongNumberOfArgs : public std::runtime_error{
		public:
			explicit WrongNumberOfArgs() : WrongNumberOfArgs("Error: Incorrect Args") {}
			explicit WrongNumberOfArgs(const std::string& error) : std::runtime_error(error) {}

	};

	template <typename... Ts>
	class Interpolate{
		public:
		friend std::ostream &operator<<(std::ostream &os, const Interpolate &i){
			std::string temp = i.s;
			i.apply<0>(i.args, temp, os);
			return os;
		}

		Interpolate(std::string s, const Ts &... as) : s(s), args{as...} {}
		
		std::tuple<const Ts &...> args;
		std::string s;

		template <std::size_t I, typename T>
		static void constexpr apply(const T &t, std::string &str, std::ostream &os){
			if constexpr (I < std::tuple_size_v<T>) {
				std::string temp = "";
				bool found = false;
				for(int i = 0; i < str.size(); i++){
					if(str[i] == '%' && !found) {
						if(i-1 >= 0 && str[i-1] == '\\'){
							temp += str[i];
						}
						else{
							os << temp << std::get<I>(t);
							temp = "";
							found = true;
						}
					}
					else{
						temp += str[i];
					}
				}
				if(!found) throw WrongNumberOfArgs("Wrong number of Args");
				apply<I+1>(t, temp, os);
			}
			else if(I == 0) {
				std::string temp = "";
				bool found = false;
				for(int i = 0; i < str.size(); i++){
					if(str[i] == '\\') {
						if(i+1 < str.size() && str[i+1] == '%'){
							temp += '%';
							i++;
						}
						else{
							temp += str[i];
						}
					}
					else{
						temp += str[i];
					}
				}
				os << temp;
			}
			else{
				for(int i = 0; i < str.size(); i++){
					if(str[i] == '%') throw WrongNumberOfArgs("Wrong number of Args");
				}
			}
		}
	};

	auto ffr(std::basic_ostream<char, std::char_traits<char>>& (*fp)(std::basic_ostream<char, std::char_traits<char>>&)){
		return fp;
	}

};


#endif